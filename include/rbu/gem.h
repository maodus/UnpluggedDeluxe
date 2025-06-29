#ifndef GEM_H
#define GEM_H

typedef struct {
  float mMs;
  int mTick;
  short mDurationMs;
  short mDurationTicks;
  short unknown;

  char lane : 4; // 1 = red, 2 = yellow, 4 = green, 8 = red
  char unknown2 : 4;

  char unknown3;
  int unknown4;

}__attribute__((__packed__)) GameGem; // Size 0x14

#endif