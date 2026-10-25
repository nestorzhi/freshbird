/*
 * aht20.h
 *
 *  Created on: 2026年10月18日
 *      Author: nestor
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

#include "i2c.h"

void AHT20_Init();
void AHT20_Read(float *Temperature ,float *Humidity);
void AHT20_Meature();
void AHT20_Get();
void AHT20_Analysis(float *Temperature,float *Humidity);

#endif /* INC_AHT20_H_ */
