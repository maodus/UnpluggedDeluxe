#ifndef GAME_MEM_H
#define GAME_MEM_H

#define JAL_READ_BEATMATCH 0x08ad6670 // Address of call to read beatmatch

#define JAL_OPEN_ASYNC 0x08A242CC

/*
 * Uncertain as to what to name this, but it has something to do with
 * game initialization. 
 */
#define J_INIT_GAME 0x088ef17c 

#define FILE_SIZE_CHECK_ADDR 0x089f1a18
#define FILE_SIZE_PATCH_INSTR 0x244C0000
#define HASH_CHECK_ADDR 0x08ab2aa8

#define VERIFY_FILES_FUNC 0x089f191c
#define JAL_VERIFY_FILES 0x08a2e7c4

#define INITIAL_TRACK1_ADDR 0x096e9d1c
#define INITIAL_TRACK2_ADDR 0x0919042c
#define CURRENT_TRACK_ADDR 0x096e9ca8

#define SONG_COUNT1_ADDR 0x08ed2b70 // Number of songs loaded
#define SONG_COUNT2_ADDR 0x08ed2b40 // Same as above, not sure if its used.

#define SONG_INFO_LAST_ADDR 0x08EDA30C // Pointer to beginning of last SongInfo

#define ENGLISH_DICT_ADDR 0x08b5d010 // Pointer to start of english dictionary (char ***)
#define ENGLISH_DICT_COUNT_ADDR 0x08b5d0a4 // Number of dictionary entries

#define ENGLISH_DIFF_OFFSET 0x167
#define DIFFICULTY_MAX 6

#define ENGLISH_ERA_OFFSET 0x16E
#define ERA_MAX 4

#define ENGLISH_GENRE_OFFSET 0x173
#define GENRE_MAX 19

#endif