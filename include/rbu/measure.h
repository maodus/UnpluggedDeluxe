#ifndef MEASURE_H
#define MEASURE_H

typedef struct {
  int measure; // offset 0x0, size 0x4
  int numerator; // offset 0x8, size 0x4
  int denominator; // offset 0xC, size 0x4
  int tick; // offset 0x10, size 0x4
} __attribute__((__packed__)) TimeSigChange;

typedef struct {
  TimeSigChange *time_sigs; // 0x4
  unsigned int capacity; // 0x8
  unsigned int size; // 0xC
} __attribute__((__packed__)) MeasureMap;
#endif