#ifndef __SYS_H__
#define __SYS_H__

#include <Arduino.h>
#include <Preferences.h>
#include "key.h"
#include "battery.h"
#include "def.h"

typedef enum {
    MODE_NORMAL,
    MODE_TIMER_SET,
    MODE_METRONOME,
    MODE_KEY_CONFIG
} SystemMode;

struct SystemStatus {
    bool bleConnected;
    unsigned long lastLedUpdate;
};

#define PRESET_COUNT 6 // Add extra preset number here

typedef struct {
    uint8_t keymap[5][8];
    uint8_t name[20];
    uint8_t keyDescription[5][16];
} KeyPreset;

extern KeyPreset presets[PRESET_COUNT]; // Add preset in this array
extern char media[5][2];
extern uint8_t currentPreset;
extern uint8_t scrollPos;

extern SystemMode currentMode;
extern struct SystemStatus sysStatus;
extern bool changeName;

extern uint32_t lastActivityTime;
extern bool screenOn;
extern bool active;

void SYS_ModeSwitch();
void SYS_KeyConfig();
void SYS_SavePreset();
void SYS_LoadPreset();
void SYS_ConfirmPreset(uint8_t preset);
void SYS_ApplyPreset(uint8_t presetIndex);
void SYS_StatusLEDCtrl();

#endif
