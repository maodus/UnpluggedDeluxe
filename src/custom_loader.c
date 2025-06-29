#include "custom/custom_loader.h"
#include "custom/custom_song.h"
#include "rbu/gem.h"
#include "rbu/bar.h"
#include "rbu/tempo.h"
#include "rbu/beat.h"
#include "rbu/measure.h"
#include "allocation/pspmalloc.h"

#include <stddef.h>
#include <pspsdk.h>
#include <pspuser.h>
#include <stdio.h>

static int custom_fid = -1;

static TempoInfoPoint *new_tempo_map;
static BeatInfo *new_beat_map;
static TimeSigChange *new_measure_map;
static BarInfo * new_bars[4];

int start_custom_load(char const *custom_path) {
  if (custom_fid >= 0){
    return -1;
  }

  // Free tempo/measure/beat maps prior to load to reduce fragmentation
  if (new_tempo_map != NULL){
    psp_free(new_tempo_map);
    new_tempo_map = NULL;
  }

  if (new_beat_map != NULL){
    psp_free(new_beat_map);
    new_beat_map = NULL;
  }

  if (new_measure_map != NULL){
    psp_free(new_measure_map);
    new_measure_map = NULL;
  }

  for (int i = 0; i < 4; i++){
    if (new_bars[i] != NULL){
      psp_free(new_bars[i]);
      new_bars[i] = NULL;
    }
  }

  custom_fid = sceIoOpen(custom_path, PSP_O_RDONLY, 0777);

  if (custom_fid < 0){
    return custom_fid;
  }

  sceIoLseek32(custom_fid, CUSTOM_LOAD_SIZE, SEEK_SET); // Skip header
  return custom_fid;
}

static void replace_tempos(int tempo_count) {
  MultiTempoTempoMap *tempo_map = *(MultiTempoTempoMap **)(0x096e9b94);
  tempo_map->points = new_tempo_map;
  tempo_map->capacity = tempo_count;
  tempo_map->size = tempo_count;
}

static void replace_beats(int beat_count) {
  BeatMap *beat_map = *(BeatMap **)(0x096e9b9c);
  beat_map->beats = new_beat_map;
  beat_map->capacity = beat_count;
  beat_map->size = beat_count;
}

static void replace_measures(int measure_count) {
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

static int read_map_section(void **map_ptr, size_t data_size) {
  size_t count;

  if (custom_fid < 0){
    return custom_fid;
  }

  sceIoRead(custom_fid, &count, sizeof(count));

  const size_t num_bytes = data_size * count;
  *map_ptr = psp_malloc(num_bytes);
  sceIoRead(custom_fid, *map_ptr, num_bytes);

  return (int)count;
}


// First in the load sequence
void load_tempo_map(){
  int tempo_count = read_map_section((void **)&new_tempo_map, sizeof(TempoInfoPoint));
  replace_tempos(tempo_count);
}

void load_beat_map(){
  int beat_count = read_map_section((void **)&new_beat_map, sizeof(BeatInfo));
  replace_beats(beat_count);
}

void load_measure_map(){
  int measure_count = read_map_section((void **)&new_measure_map, sizeof(TimeSigChange));
  replace_measures(measure_count);
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


void load_gems(STAllocator *allocator) {
  for (int i = 0; i < 4; i++){
    // Read the number of gems for the instrument
    int num_gems;
    sceIoRead(custom_fid, &num_gems, sizeof(int));
    const size_t gem_size = sizeof(GameGem) * num_gems;

    // Buffer and read gem data for instrument
    GameGem *gem_alloc = (GameGem *)(st_alloc(allocator, gem_size, 4));
    sceIoRead(custom_fid, gem_alloc, gem_size);
    replace_gems(gem_alloc, num_gems, i);
  }

}


void load_bars(){
  if (custom_fid == -1){
    return;
  }

  for (int i = 0; i < 4; i++){
    // Read the number of bars for the instrument
    int num_bars;    
    sceIoRead(custom_fid, &num_bars, sizeof(int));
    const size_t bar_size = sizeof(BarInfo) * num_bars;

    new_bars[i] = psp_malloc(bar_size);
    BarInfo *bar_alloc = new_bars[i];
    //BarInfo *bar_alloc = (BarInfo *)(st_alloc(allocator, bar_size, 4));
    sceIoRead(custom_fid, bar_alloc, bar_size);
    replace_bars(bar_alloc, num_bars, i);
  }
}

int end_custom_load() {
  sceIoClose(custom_fid);
  custom_fid = -1;
  return 0;
} 