#ifndef CUSTOM_SONG_H
#define CUSTOM_SONG_H

#include "common.h"

typedef struct {
  unsigned int drum_diff   : 4;
  unsigned int guitar_diff : 4;
  unsigned int bass_diff   : 4;
  unsigned int vocal_diff  : 4;
  unsigned int band_diff   : 4;
  unsigned int reserved    : 12;
} __attribute__((__packed__)) TrackDifficulties;

typedef struct {
  char name_internal[MAX_CUSTOM_LEN];
  int version;
  char display_name[MAX_CUSTOM_LEN];
  char artist_name[MAX_CUSTOM_LEN];
  int difficulty;
  int genre;
  int era;
  int initial_track;
  TrackDifficulties track_diffs;
  char reserved[124];
} __attribute__((__packed__)) CustomSong;

#define CUSTOM_LOAD_SIZE sizeof(CustomSong) - MAX_CUSTOM_LEN

#endif