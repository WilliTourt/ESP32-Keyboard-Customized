/*
This IIC library is transplanted from my STC89C52RC microcontroller library.
("//": Potential risks that may cause error. 5 lines)
*/

#ifndef __IIC_H__
#define __IIC_H__

#include <Arduino.h>
#include <def.h>

typedef struct {
	uint8_t SCL_Pin;
	uint8_t SDA_Pin;
	uint8_t Delay;
} IIC_BusCfg;

void IIC_Init(uint8_t SCL, uint8_t SDA, uint8_t spd, uint8_t bus);
void IIC_Start(uint8_t bus);
void IIC_Stop(uint8_t bus);
void IIC_SendByte(uint8_t bus, uint8_t byte);
uint8_t IIC_ReadByte(uint8_t bus);
uint8_t IIC_RcvACK(uint8_t bus);
void IIC_SendACK(uint8_t bus, uint8_t ack);

#endif
