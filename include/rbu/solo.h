#ifndef SOLO_H
#define SOLO_H

typedef struct {
  float ms;
  float ms_duration;
  int tick;
  int tick_duration;
  int num_solo_gems;
} __attribute__((__packed__)) SoloEvent;

#endif