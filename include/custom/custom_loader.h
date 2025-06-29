#ifndef CUSTOM_LOADER_H
#define CUSTOM_LOADER_H

#include <stdint.h>
#include "allocation/stalloc.h"


int start_custom_load(const char *custom_path);

void load_tempo_map();
void load_beat_map();
void load_measure_map();

void set_load_difficulty(uint32_t difficulty);

void load_gems(STAllocator *allocator);
void load_bars();

int end_custom_load();

#endif // CUSTOM_LOADER_H
