#ifndef TEMPO_H
#define TEMPO_H

typedef struct {
  float ms; // offset 0x0, size 0x4
  int tick; // offset 0x4, size 0x4
  int tempo; // offset 0x8, size 0x4
} __attribute__((__packed__)) TempoInfoPoint;

typedef struct {
  void *v_table; // 0x0

  // Vector
  TempoInfoPoint *points; // 0x4
  unsigned int capacity; // 0x8
  unsigned int size; // 0xC


  float mStartLoopTick; // 0x10
  float mEndLoopTick; // 0x14
  float mStartLoopTime; // 0x18
  float mEndLoopTime; // 0x1C
} __attribute__((__packed__)) MultiTempoTempoMap;
#endif