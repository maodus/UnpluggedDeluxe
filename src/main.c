#include <pspsdk.h>
#include <pspuser.h>
#include <pspctrl.h>
#include <systemctrl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "rbu/song_info.h"
#include "rbu/game_mem.h"
#include "custom/custom_song.h"
#include "custom/custom_man.h"
#include "custom/custom_loader.h"
#include "allocation/bump_alloc.h"
#include "allocation/pspmalloc.h"
#include "common.h"
#include "utils.h"

// Define the name of the game's main module here
#define MODULE_NAME_INTERNAL "\"rockband\""

// This is the name of this module that will be presented to the PSP OS.
#define MODULE_NAME "UnpluggedDeluxe"

#define MODULE_VERSION_MAJOR 1
#define MODULE_VERSION_MINOR 0

PSP_MODULE_INFO(MODULE_NAME, 0, MODULE_VERSION_MAJOR, MODULE_VERSION_MINOR);
PSP_HEAP_SIZE_MAX();

// Start of PPSSPP Boilerplate //

// Forward-declare initialization function
int main_init();

int bPPSSPP = 0;

// Imports
extern void sceKernelDcacheWritebackInvalidateAll(void);
extern void sceKernelIcacheInvalidateAll(void);
extern void h_game_init(void);


//
// CheckModules
// Executes only once on startup
// This works both on a real PSP and PPSSPP, but we limit this to PPSSPP because kuKernelFindModuleByName is better
//
static void CheckModules()
{
  SceUID modules[10];
  int count = 0;
  int bFoundMainModule = 0;
  int bFoundInternalModule = 0;
  if (sceKernelGetModuleIdList(modules, sizeof(modules), &count) >= 0)
  {
      int i;
      SceKernelModuleInfo info;
      for (i = 0; i < count; ++i)
      {
          info.size = sizeof(SceKernelModuleInfo);
          if (sceKernelQueryModuleInfo(modules[i], &info) < 0)
          {
              continue;
          }
          if (strcmp(info.name, MODULE_NAME_INTERNAL) == 0)
          {
              bFoundMainModule = 1;
          }
          else if (strcmp(info.name, MODULE_NAME) == 0)
          {
              bFoundInternalModule = 1;
          }
  }
}

  if (bFoundInternalModule)
  {
      if (bFoundMainModule)
      {
          main_init();
      }
  }
  return;
}

//
// CheckModulesPSP
// Executes only once on startup
// Works only on PSP CFW
//
void CheckModulesPSP() {
  main_init();
}


int module_start(SceSize argc, void* argp) {
  if (sceIoDevctl("kemulator:", 0x00000003, NULL, 0, NULL, 0) == 0) { 
    bPPSSPP = 1;
  }
  
  if (bPPSSPP) {
    CheckModules();
  } else {
    CheckModulesPSP();
  }

  return 0;
}

int module_stop (SceSize args, void *argp){
  return 0;
}

// End of PPSSPP Boilerplate code //

static char last_file[256] __attribute__((aligned(4))); // Path of last opened .ATM file
static void *fixed_heap __attribute__((aligned(4))); // Fixed heap for custom songs
static BumpAllocator allocator __attribute__((aligned(4)));
static CustomManager custom_man __attribute__((aligned(4)));

int inject_custom(int unk, int start_track){
  char custom_name[MAX_CUSTOM_LEN]; // Name of custom extracted from file path
  char custom_path[MAX_FILE_PATH_LEN]; // File path of custom

  // The last file should be the custom.ATM file path at this point
  if (extract_custom_name(last_file, custom_name)){
    return start_track;
  }

  // Find custom from name to get initial track
  CustomSong *custom = get_custom(&custom_man, custom_name);

  if (!custom){
    return start_track;
  }

  // Make sure custom name is uppercase
  to_upper_str(custom_name);

  if (create_custom_path(custom_name, custom_path)) {
    return start_track;
  }

  if (start_custom_load(custom_path) < 0){
    return start_track;
  }

  bump_alloc_init(&allocator, fixed_heap, 0x80000 - 8);

  // TODO: IO Error handling
  load_tempo_map(&allocator);
  load_beat_map(&allocator);
  load_measure_map(&allocator);


  uint32_t difficulty = **((uint32_t **)0x096e9b54);
  set_load_difficulty(difficulty);

  load_gems(&allocator, difficulty);
  load_bars(&allocator);
  load_solos(&allocator);
  end_custom_load();
  
  const int initial_track = custom->initial_track;
  _sw(initial_track, INITIAL_TRACK1_ADDR);
  _sw(initial_track, CURRENT_TRACK_ADDR);

  return initial_track;
}


/*
* np_loader_mod does not provide patches to this user module,
* so simply calling sceIoOpenAsync will not suffice here.
* Instead, we extract the game's patched version of sceIoOpenAsync
* and call that here.
*/
static int (*npdrm_sceIoOpenAsync)(const char *filename, int flags, int mode);
static int snoop_open_async(const char *filename, int flags, int mode){
  strncpy(last_file, filename, MAX_FILE_PATH_LEN);
  last_file[MAX_FILE_PATH_LEN - 1] = '\0';
  return npdrm_sceIoOpenAsync(filename, flags, mode);
}

static int (*verify_files)(uint32_t *param_1,uint32_t *param_2);
static int h_verify_files(uint32_t *param_1,uint32_t *param_2){
  static int orig_count = -1;
  static int orig_text_count = -1;

  // Save original song and dictionary counts once
  if (orig_count == -1){
    orig_count = _lw(SONG_COUNT1_ADDR);
    orig_text_count = _lw(ENGLISH_DICT_COUNT_ADDR);
  }

  SongInfo *song_info = (SongInfo *)(SONG_INFO_LAST_ADDR);
  uint8_t ***dictionary = (uint8_t ***)(ENGLISH_DICT_ADDR);
  const uint32_t custom_count = custom_man.custom_count;
  for (uint32_t i = 0; i < custom_count; i++){
    // Copy previous DLC song into next SongInfo slot
    memcpy(song_info + 1, song_info, 0x114);
    song_info += 1;

    const uint32_t dict_offset = 2 * i; 
    const uint32_t dict_idx = orig_text_count + dict_offset;
    CustomSong *custom = &custom_man.customs[i];

    _sw(orig_text_count + custom_count * 2 + 1, ENGLISH_DICT_COUNT_ADDR);
    *((uint8_t**)*dictionary + dict_idx) = custom->display_name;
    *((uint8_t**)*dictionary + dict_idx + 1) = custom->artist_name; 

    // No clue how the IDs work, some values will not let the song be displayed.
    // This value range seems to suffice.
    song_info->id = 0x0000ffff | (i << 16);
    song_info->internal_name_1 = custom->name_internal;
    song_info->internal_name_2 = custom->name_internal;
    song_info->song_title_id = dict_idx;
    song_info->artist_id = dict_idx + 1;
    song_info->difficulty_id = ENGLISH_DIFF_OFFSET + custom->difficulty;
    song_info->genre_id = ENGLISH_GENRE_OFFSET + custom->genre;
    song_info->era_id = ENGLISH_ERA_OFFSET + custom->era;

    TrackDifficulties track_diffs = custom->track_diffs;

    song_info->drum_diff = track_diffs.drum_diff;
    song_info->guitar_diff = track_diffs.guitar_diff;
    song_info->bass_diff = track_diffs.bass_diff;
    song_info->vocal_diff = track_diffs.vocal_diff;
    song_info->band_diff = track_diffs.band_diff;
  
    // We add an extra 1 here so the game reads an invalid song into memory.
    // Reading the invalid song allows for custom songs to be indexed properly.
    // I dont know why this is the case, but customs dont work otherwise.
    _sw(orig_count + custom_count + 1, SONG_COUNT1_ADDR);
    _sw(orig_count + custom_count + 1, SONG_COUNT2_ADDR);

    //08b5d010 Ptr to english strings
  }
  return verify_files(param_1, param_2);
}

static uint32_t last_tick;
static void check_song_end(int argc, void *argv){
  uint32_t *game_obj_ptr = (uint32_t *)(0x08b7d5a8);

  while (1){
    sceKernelDelayThread(5000000);

    if (!(*game_obj_ptr)){
      continue;
    }

    float * ticks_ptr = (float *)(*game_obj_ptr + 0x658);
    int tick = (int)(*ticks_ptr);
    
    if (tick <= 0 ){
      last_tick = tick;
      continue;
    }

    if (!(*(uint32_t *)(0x08B4E3B8))){
      continue;
    }

    // We have finished our song (hopefully), now we have to force end the track
    if (last_tick == tick){
      *(uint32_t *)(0x08B7D58C) = 1;
    }

    last_tick = tick;
  }

}

int main_init() {
  sceKernelPrintf("Unplugged Deluxe started\n");

  sceKernelPrintf("Patching functions\n");
  // Get address of np_loader_mod patched sceIoOpenAsync
  npdrm_sceIoOpenAsync = (int (*)(const char *, int, int))(
      JUMP_TARGET(*(uint32_t *)(JAL_OPEN_ASYNC))
  );

  _sw(MAKE_CALL(&snoop_open_async), JAL_OPEN_ASYNC);
  _sw(MAKE_JUMP(&h_game_init), J_INIT_GAME);

  // Disable file-size validation for ATM files
  _sw(FILE_SIZE_PATCH_INSTR, FILE_SIZE_CHECK_ADDR);
  
  // Disable hash check for ATM on song load
  _sw(0, HASH_CHECK_ADDR);

  verify_files = (int (*)(uint32_t *,uint32_t *))(VERIFY_FILES_FUNC);
  _sw(MAKE_CALL(&h_verify_files), JAL_VERIFY_FILES);

  sceKernelDcacheWritebackAll();
  sceKernelIcacheInvalidateAll();

  sceKernelPrintf("Discovering custom songs\n");
  init_custom_manager(&custom_man);
  discover_customs(&custom_man);
  
  int thid = sceKernelCreateThread("end_song_thrd", check_song_end, 64, 0x400, 0, NULL);

  if (thid >= 0){
    sceKernelStartThread(thid, 0, NULL);
  }

  fixed_heap = psp_malloc(0x80000);
  sceKernelPrintf("Unplugged Deluxe fixed heap init at: %08X\n", fixed_heap);
  return 0;
}
