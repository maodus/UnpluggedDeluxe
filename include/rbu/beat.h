#ifndef BEAT_H
#define BEAT_H

typedef struct {
  int tick; // offset 0x0, size 0x4
  int level; // offset 0x8, size 0x4
} __attribute__((__packed__)) BeatInfo;

typedef struct {
  BeatInfo *beats; // 0x4
  unsigned int capacity; // 0x8
  unsigned int size; // 0xC
} __attribute__((__packed__)) BeatMap;
#endif