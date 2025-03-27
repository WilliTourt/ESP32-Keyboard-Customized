#ifndef __KEY_H__
#define __KEY_H__

#include <Arduino.h>
#include "def.h"

typedef struct {
    bool isPressed;
    bool shouldSend;
    bool isReleased;
} KeyState;

#define LONG_PRESS_TIME 2000

extern bool enableKey;

extern bool k4LongPressed;
extern bool k5LongPressed;
extern uint32_t k4PressStartTime;
extern uint32_t k5PressStartTime;

extern bool sendRelease;

extern char k1Buf[8];
extern char k2Buf[8];
extern char k3Buf[8];
extern char k4Buf[8];
extern char k5Buf[8];

extern char release[8];
extern KeyState keyState[5];

void KEY_Init();
bool KEY_Update();

#endif
