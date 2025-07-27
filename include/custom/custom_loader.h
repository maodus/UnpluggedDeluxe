#ifndef CUSTOM_LOADER_H
#define CUSTOM_LOADER_H

#include <stdint.h>
#include "allocation/bump_alloc.h"


int start_custom_load(const char *custom_path);

void load_tempo_map(BumpAllocator *allocator);
void load_beat_map(BumpAllocator *allocator);
void load_measure_map(BumpAllocator *allocator);

void set_load_difficulty(uint32_t difficulty);

void load_gems(BumpAllocator *allocator, uint32_t difficulty);
void load_bars(BumpAllocator *allocator);
void load_solos(BumpAllocator *allocator);

int end_custom_load();

#endif // CUSTOM_LOADER_H
