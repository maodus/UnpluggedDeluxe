#include "custom/custom_loader.h"
#include "custom/custom_song.h"
#include "rbu/gem.h"
#include "rbu/bar.h"
#include "rbu/tempo.h"
#include "rbu/beat.h"
#include "rbu/measure.h"
#include "rbu/solo.h"
#include "allocation/pspmalloc.h"

#include <stddef.h>
#include <pspsdk.h>
#include <pspuser.h>
#include <stdio.h>

static uint32_t load_version;
static int custom_fid = -1;

int start_custom_load(char const *custom_path) {
  if (custom_fid >= 0){
    return -1;
  }

  custom_fid = sceIoOpen(custom_path, PSP_O_RDONLY, 0777);

  if (custom_fid < 0){
    return custom_fid;
  }

  // Get the custom's version
  sceIoRead(custom_fid, &load_version, sizeof(load_version));

  // Skip header
  sceIoLseek32(custom_fid, CUSTOM_LOAD_SIZE, SEEK_SET); 
  return custom_fid;
}

static void replace_tempos(int tempo_count, TempoInfoPoint *new_tempo_map) {
  MultiTempoTempoMap *tempo_map = *(MultiTempoTempoMap **)(0x096e9b94);
  tempo_map->points = new_tempo_map;
  tempo_map->capacity = tempo_count;
  tempo_map->size = tempo_count;
}

static void replace_beats(int beat_count, BeatInfo *new_beat_map) {
  BeatMap *beat_map = *(BeatMap **)(0x096e9b9c);
  beat_map->beats = new_beat_map;
  beat_map->capacity = beat_count;
  beat_map->size = beat_count;
}

static void replace_measures(int measure_count, TimeSigChange *new_measure_map) {
  MeasureMap *measure_map = *(MeasureMap **)(0x096e9b98);
  measure_map->time_sigs = new_measure_map;
  measure_map->capacity = measure_count;
  measure_map->size = measure_count;
}

static void replace_gems(GameGem *gems, uint32_t count, uint32_t track) {
  uint32_t *root_ptr = (uint32_t *)(0x08b7cf14);
  uint32_t *track_watchers = (uint32_t *)(*root_ptr + 0x38);

  uint32_t *track_watcher = (uint32_t *)(track_watchers[track]);
  uint32_t *game_gem_vec = (uint32_t *)(*track_watcher + 0x8);

  // When we are in the menu, this will be 2 (which is not what we want)
  if (*game_gem_vec == 2) {
      return;
  }

  GameGem **gem_data = (GameGem **)((void *)(*game_gem_vec + 0x8));
  uint32_t *gem_capacity = (uint32_t *)(*game_gem_vec + 0xC); 
  uint32_t *gem_size = (uint32_t *)(*game_gem_vec + 0x10);
  
  // Update the internal vector
  *gem_data = gems;
  *gem_capacity = count;
  *gem_size = count;
}

static void replace_gem_infos(BumpAllocator *allocator, GameGem *gems, uint32_t count, uint32_t track, uint32_t difficulty) {
  uint32_t *root_ptr = (uint32_t *)(0x08b7d5a4);
  uint32_t *vec_ptr = (uint32_t *)(*root_ptr + 0x104);
  uint32_t *diff_vec = (uint32_t *)(*vec_ptr + 4 * difficulty);

  uint32_t *unk = (uint32_t *)*diff_vec;
  uint32_t *unk_2 = (uint32_t *)*unk;
  uint32_t *unk_3 = (uint32_t *)*unk_2;
  uint32_t *gem_info_list = (uint32_t *)(*unk_3 + 4 * track);

  char **gem_data = (char **)((void *)(*gem_info_list + 0x4));
  uint32_t *gem_capacity = (uint32_t *)(*gem_info_list + 0x8); 
  uint32_t *gem_size = (uint32_t *)(*gem_info_list + 0xC);

  *gem_capacity = count;
  *gem_size = count;

  char *new_gem_infos = (char *)bump_alloc(allocator, count, 4);
  *gem_data = new_gem_infos;

  for (uint32_t i = 0; i < count; ++i){
    new_gem_infos[i] = (char)((0xF - (i % 0x10)) << 4);
  }

} 

static void replace_bars(BarInfo *bars, uint32_t count, uint32_t track){
  uint32_t *root_ptr = (uint32_t *)(0x08b7d5a4); // Dont know what this is
  uint32_t *vec_ptr = (uint32_t *)(*root_ptr + 0x1E0); // This is a pointer to a vector of pointers (index by track id?)
  uint32_t *unk_ptr = (uint32_t *)(*vec_ptr + 4 * track); // Pointer to our object based on track

  uint32_t *vtable = (uint32_t *)(*unk_ptr + 0xC);
  uint32_t *song_data = (uint32_t *)(*vtable + 0xC);

  BarInfo **bar_vec = (BarInfo **)((void *)(*song_data + 0xC));
  uint32_t *bp_capacity = (uint32_t *)(*song_data + 0x10);
  uint32_t *bp_size = (uint32_t *)(*song_data + 0x14);

  // Update internal bar vec
  *bar_vec = bars;
  *bp_capacity = count;
  *bp_size = count;
}

static void replace_ui_solos(SoloEvent *solos, uint32_t count, uint32_t track){
  uint32_t *root_ptr = (uint32_t *)(0x08b7d5a4); // Dont know what this is
  const uint32_t ui_track_offset = 0x28 * track;

  // How many solo UI events the custom track has
  uint32_t *solo_count = (uint32_t *)(*root_ptr + ui_track_offset + 0x338);
  *solo_count = count;

  float *solo_start_ms = (float *)(*root_ptr + ui_track_offset + 0x33C);
  float *solo_end_ms = (float *)(*root_ptr + ui_track_offset + 0x328);

  for (uint32_t i = 0; i < count; ++i){
    float start_ms = solos[i].ms;

    // Note: We should probably round down the the ms of the nearest bar
    // because that is what the game seems to do. But this will work for now
    float end_ms = start_ms + solos[i].ms_duration;

    solo_start_ms[i] = start_ms;
    solo_end_ms[i] = end_ms;
  }
}

static void replace_solos(SoloEvent *solos, uint32_t count, uint32_t track){
  uint32_t *root_ptr = (uint32_t *)(0x08b7d5a4); // Dont know what this is
  uint32_t *vec_ptr = (uint32_t *)(*root_ptr + 0x58 + track * 0xC); // This is a pointer to a vector of pointers (index by track id?)

  if (count == 0){
    solos = NULL;
  }

  *(vec_ptr) = (uint32_t)solos;
  *(vec_ptr + 1) = count;
  *(vec_ptr + 2) = count;

  replace_ui_solos(solos, count, track);
}


static int read_map_section(BumpAllocator *allocator, void **map_ptr, size_t data_size) {
  size_t count = 0;

  if (custom_fid < 0){
    return custom_fid;
  }

  sceIoRead(custom_fid, &count, sizeof(count));

  const size_t num_bytes = data_size * count;
  *map_ptr = bump_alloc(allocator, num_bytes, 4);
  sceIoRead(custom_fid, *map_ptr, num_bytes);

  return (int)count;
}


// First in the load sequence
void load_tempo_map(BumpAllocator *allocator){
  TempoInfoPoint *new_tempo_map;
  int tempo_count = read_map_section(allocator, (void **)&new_tempo_map, sizeof(TempoInfoPoint));
  replace_tempos(tempo_count, new_tempo_map);
}

void load_beat_map(BumpAllocator *allocator){
  BeatInfo *new_beat_map;
  int beat_count = read_map_section(allocator, (void **)&new_beat_map, sizeof(BeatInfo));
  replace_beats(beat_count, new_beat_map);
}

void load_measure_map(BumpAllocator *allocator){
  TimeSigChange *new_measure_map;
  int measure_count = read_map_section(allocator, (void **)&new_measure_map, sizeof(TimeSigChange));
  replace_measures(measure_count, new_measure_map);
}

void set_load_difficulty(uint32_t difficulty) {
  uint32_t curr_diff = 0; // Current difficulty

  while (curr_diff < difficulty){
    int section_size;
    sceIoRead(custom_fid, &section_size, sizeof(int));
    sceIoLseek32(custom_fid, section_size, SEEK_CUR);
    ++curr_diff;
  }

  sceIoLseek32(custom_fid, 4, SEEK_CUR);
}


void load_gems(BumpAllocator *allocator, uint32_t difficulty) {
  if (custom_fid < 0) {
    return;
  }

  for (int i = 0; i < 4; i++){
    // Read the number of gems for the instrument
    int num_gems;
    sceIoRead(custom_fid, &num_gems, sizeof(int));
    const size_t gem_size = sizeof(GameGem) * num_gems;

    // Buffer and read gem data for instrument
    GameGem *gem_alloc = (GameGem *)(bump_alloc(allocator, gem_size, 4));
    sceIoRead(custom_fid, gem_alloc, gem_size);
    replace_gems(gem_alloc, num_gems, i);
    replace_gem_infos(allocator, gem_alloc, num_gems, i, difficulty);
  }

}

void load_bars(BumpAllocator *allocator){
  if (custom_fid < 0){
    return;
  }

  for (int i = 0; i < 4; i++){
    // Read the number of bars for the instrument
    int num_bars;    
    sceIoRead(custom_fid, &num_bars, sizeof(int));
    const size_t bar_size = sizeof(BarInfo) * num_bars;

    BarInfo *bar_alloc = (BarInfo *)(bump_alloc(allocator, bar_size, 4));
    sceIoRead(custom_fid, bar_alloc, bar_size);
    replace_bars(bar_alloc, num_bars, i);
  }
}

void load_solos(BumpAllocator *allocator){
  if (custom_fid < 0){
    return;
  }

  for (int i = 0; i < 4; i++){
    // Only parser v2 supports track solos
    if (load_version >= 2){
      int num_solos;    
      sceIoRead(custom_fid, &num_solos, sizeof(int));
      const size_t solo_size = sizeof(SoloEvent) * num_solos;

      if (num_solos == 0){
        replace_solos(NULL, 0, i);
      } else {
        SoloEvent *solo_alloc = (SoloEvent *)(bump_alloc(allocator, solo_size, 4));
        sceIoRead(custom_fid, solo_alloc, solo_size);
        replace_solos(solo_alloc, num_solos, i);
      }
    } else {
      replace_solos(NULL, 0, i);
    }
  }
}

int end_custom_load() {
  sceIoClose(custom_fid);
  custom_fid = -1;
  return 0;
} 