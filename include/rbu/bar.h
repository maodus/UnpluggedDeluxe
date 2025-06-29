#ifndef BAR_H
#define BAR_H

typedef struct {
  int start_gem_id;
  int end_gem_id;
  float start_ms;
  float end_ms; 
} __attribute__((__packed__)) BarInfo;

#endif