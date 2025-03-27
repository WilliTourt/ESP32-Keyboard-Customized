#ifndef __TIMER_METRONOME_H__
#define __TIMER_METRONOME_H__

#include <Arduino.h>
#include "music.h"
#include "key.h"
#include "def.h"
#include "sys.h"

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint32_t targetSec;
    uint32_t currentSec;
    bool enabled;
} Timer;

typedef struct {
    uint16_t bpm;
    uint8_t timeSig;
    bool isRunning;
    uint32_t nextBeatTime;
} Metronome;

extern Metronome metro;
extern Timer timer;
extern bool timerTriggered;

void TIMER_Set();
void TIMER_Display();
void TIMER_Handle();
void METRONOME_Set();
void METRONOME_Handle();

#endif
