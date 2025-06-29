#include "utils.h"
#include "common.h"
#include <string.h>
#include <ctype.h>

const char DLC_DIRECTORY[] = "ms0:/PSP/GAME/ULUS10418/MUSIC/";


int create_custom_path(char const *custom_name, char *output){
  const int name_len = strlen(custom_name);
  const int path_len = sizeof(DLC_DIRECTORY) + name_len * 2 + 6;

  if (path_len >= MAX_FILE_PATH_LEN){
    return 1;
  }

  strcpy(output, DLC_DIRECTORY);
  strcat(output, custom_name);
  strcat(output, "/");
  strcat(output, custom_name);
  strcat(output, ".rbu");

  return 0;
}

void to_lower_str(char *string){
  int str_len = strlen(string);
  for (int i = 0; i < str_len; ++i){
    string[i] = tolower(string[i]);
  }
}
void to_upper_str(char *string){
  int str_len = strlen(string);
  for (int i = 0; i < str_len; ++i){
    string[i] = toupper(string[i]);
  }
}

int extract_custom_name(char *filepath, char *custom_name){
    char *last_slash = strrchr(filepath, '/');
    
    if (!last_slash || last_slash == filepath){
        return 1;
    }

    char *search = last_slash - 1;

    while (*search != '/' && search > filepath){
        search--;
    }

    if (*search != '/'){
        return 1;
    }

    int copy_length = (last_slash) - (search) - 1;

    if (copy_length >= 48 || copy_length <= 0){
        return 1;
    }

    if (search[1] != 'Z'){
        return 1;
    }

    memcpy(custom_name, search + 1, copy_length);
    custom_name[copy_length] = '\0';

    return 0;
}

int stricmp_fixed(const char *a, const char *b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        char ca = tolower((unsigned char)a[i]);
        char cb = tolower((unsigned char)b[i]);

        if (ca != cb)
            return (unsigned char)ca - (unsigned char)cb;

        if (ca == '\0')
            break; // stop early if null terminator found
    }
    return 0;
}