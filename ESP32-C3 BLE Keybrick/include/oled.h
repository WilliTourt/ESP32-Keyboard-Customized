#ifndef __OLED_H__
#define __OLED_H__

#include <Arduino.h>
#include <iic.h>
#include <char.h>
#include <stdio.h>
#include "def.h"

#define OLED_IIC_ADDR 0x78

void OLED_SetCursor(uint8_t x, uint8_t y);
void OLED_Clear();
void OLED_ClearPart(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void OLED_Init(uint8_t scl, uint8_t sda, uint8_t height, uint8_t bus);

void OLED_PrintText(uint8_t x, uint8_t y, const char* str, uint8_t size);
void OLED_PrintImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* image);
void OLED_PrintVar(uint8_t x, uint8_t y, float num, const char* type, uint8_t length);
void OLED_PrintHLine(uint8_t x, uint8_t y, uint8_t width);
void OLED_PrintVLine(uint8_t x, uint8_t y, uint8_t height);

void OLED_LowBrightness(bool lowBrightness);
void OLED_Power(bool state);

#endif
