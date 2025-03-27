#ifndef __BATTERY_H__
#define __BATTERY_H__

#include <Arduino.h>
#include "def.h"

#define ADC_VREF 2.8779789
#define BAT_VOLT_DIVIDER 0.6357

#define BAT_READ_TIME_GAP 10000 // 10s

#define BAT_LOW 3.30
#define BAT_FULL 4.20
#define BAT_EMPTY 2.64

extern bool BAT_IS_LOW;
extern float BAT_Voltage;

void BAT_Read();
uint8_t BAT_GetPercentage();

#endif
