#ifndef SONG_INFO_H
#define SONG_INFO_H

typedef struct {
  char field0_0x0[20];
  int id;
  char *internal_name_1;
  char *internal_name_2;
  int field4_0x20;
  int song_title_id;
  int artist_id;
  int field7_0x2c;
  int field8_0x30;
  int difficulty_id;
  int genre_id;
  int era_id;
  char field12_0x40[140];
  int drum_diff;
  int guitar_diff;
  int bass_diff;
  int vocal_diff;
  int band_diff;
  char field18_0xe0[52];
} __attribute__((__packed__)) SongInfo;
#endif