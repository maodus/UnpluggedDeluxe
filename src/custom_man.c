#include "custom/custom_man.h"
#include "common.h"
#include "utils.h"

#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <pspuser.h>

void init_custom_manager(CustomManager *custom_man) {
  if (!custom_man){
    return;
  }

  memset(custom_man, 0, sizeof(CustomManager));
}

void discover_customs(CustomManager *custom_man){
  if (!custom_man){
    return;
  }

  SceIoDirent dir_ent;
  char custom_path[MAX_FILE_PATH_LEN];

  int dir_id = sceIoDopen(DLC_DIRECTORY);
  if (dir_id < 0){
    //sceKernelPrintf("Could not open DLC directory\n");
    return;
  }
  memset(&dir_ent, 0, sizeof(SceIoDirent));
  while (sceIoDread(dir_id, &dir_ent ) > 0){
    // All custom file names should be prefixed with a Z
    if (toupper(dir_ent.d_name[0]) != 'Z'){
      continue;
    }

    CustomSong *custom = &custom_man->customs[custom_man->custom_count];

    // We only want MAX_CUSTOM_LEN - 1 characters
    strncpy(custom->name_internal, dir_ent.d_name, MAX_CUSTOM_LEN - 1);
    custom->name_internal[MAX_CUSTOM_LEN - 1] = '\0';

    sceKernelPrintf("Located custom song: %s\n", custom->name_internal);

    create_custom_path(custom->name_internal, custom_path);
    to_lower_str(custom->name_internal); // RBU wants this string to be lower case

    int custom_fid = sceIoOpen(custom_path, PSP_O_RDONLY, 0777);
    if (custom_fid < 0) {
      strcpy(custom->display_name, "CUSTOM");
      strcpy(custom->artist_name, "UNKNOWN");
    } else {
      sceIoRead(custom_fid, &custom->version, CUSTOM_LOAD_SIZE);
      sceIoClose(custom_fid);
    }

    custom_man->custom_count++;
  }

  sceIoDclose(dir_id);
}

CustomSong *get_custom(CustomManager *custom_man, const char *song_name) {
  if (!custom_man || !song_name){
    return NULL;
  }

  for (uint32_t i = 0; i < MAX_CUSTOMS; i++){
    CustomSong *cur_song = &custom_man->customs[i];

    if (!stricmp_fixed(cur_song->name_internal, song_name, MAX_CUSTOM_LEN)) {
      return cur_song;
    }
  }

  return NULL;

}
