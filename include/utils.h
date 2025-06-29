#ifndef UTILS_H
#define UTILS_H

#include "annotations.h"
#include <stddef.h>


extern const char DLC_DIRECTORY[];

int create_custom_path(char const *custom_name, _out_ char *output);
int extract_custom_name(char *filepath, _out_ char *custom_name);


void to_upper_str(_inout_ char *string);
void to_lower_str(_inout_ char *string);
int stricmp_fixed(const char *a, const char *b, size_t len);

#endif