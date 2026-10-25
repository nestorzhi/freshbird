/*
 * oled.h
 *
 *  Created on: 2026年10月25日
 *      Author: nestor
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "i2c.h"

void OLED_Test();
void OLED_Init();
void OLED_NewFrame();
void OLED_ShowFrame0();
void OLED_ShowFrame1();
void OLED_SetPixel(uint8_t x,uint8_t y);

#endif /* INC_OLED_H_ */
