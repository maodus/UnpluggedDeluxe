#ifndef CUSTOM_MANAGER_H
#define CUSTOM_MANAGER_H

#include "custom/custom_song.h"
#include "annotations.h"
#include "common.h"

#include <stdint.h>

typedef struct {
  CustomSong customs[MAX_CUSTOMS];
  uint32_t custom_count;
} CustomManager;

void init_custom_manager(_out_ CustomManager *custom_man);
void discover_customs(_out_ CustomManager *custom_man);

CustomSong *get_custom(CustomManager *custom_man, const char *song_name);

#endif