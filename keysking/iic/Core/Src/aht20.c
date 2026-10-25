/*
 * aht20.c
 *
 *  Created on: 2026年10月18日
 *      Author: nestor
 */


#include "aht20.h"

#define AHT20_ADDRESS 0x70

uint8_t readBuffer[6] = {0};

void AHT20_Init()
{
	uint8_t readBuffer;
	HAL_Delay(40);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);
	if((readBuffer & 0x08) == 0x00)
	{
		uint8_t sendBuffer[3] = {0xBE,0x08,0x00};
		HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
	}
}

void AHT20_Read(float *Temperature ,float *Humidity)
{
	uint8_t sendBuffer[3] = {0xAC,0x33,0x00};
	uint8_t readBuffer[6] = {0};
	HAL_I2C_Master_Transmit_IT(&hi2c1, AHT20_ADDRESS, sendBuffer, 3);
	HAL_Delay(75);
	HAL_I2C_Master_Receive_IT(&hi2c1, AHT20_ADDRESS, readBuffer, 6);

	if((readBuffer[0] & 0x80) == 0x00)
	{
		uint32_t temp_raw,humi_raw;
		humi_raw = ((uint32_t)readBuffer[1] << 12) | ((uint32_t)readBuffer[2] << 4) | ((uint32_t)readBuffer[3] >> 4);
		temp_raw = (((uint32_t)readBuffer[3] & 0x0F) <<16) | ((uint32_t)readBuffer[4] <<8) | readBuffer[5];
		*Humidity = (float)humi_raw / (1 << 20) * 100.0f;
		*Temperature = (float)temp_raw / (1 << 20) * 200.0f -50.0f;
	}

}


void AHT20_Meature()
{
	static uint8_t sendBuffer[3] = {0xAC,0x33,0x00};
	HAL_I2C_Master_Transmit_DMA(&hi2c1, AHT20_ADDRESS, sendBuffer, 3);
}
void AHT20_Get()
{
	HAL_I2C_Master_Receive_DMA(&hi2c1, AHT20_ADDRESS, readBuffer, 6);
}
void AHT20_Analysis(float *Temperature,float *Humidity)
{
	if((readBuffer[0] & 0x80) == 0x00)
	{
		uint32_t temp_raw,humi_raw;
		humi_raw = ((uint32_t)readBuffer[1] << 12) | ((uint32_t)readBuffer[2] << 4) | ((uint32_t)readBuffer[3] >> 4);
		temp_raw = (((uint32_t)readBuffer[3] & 0x0F) <<16) | ((uint32_t)readBuffer[4] <<8) | readBuffer[5];
		*Humidity = (float)humi_raw / (1 << 20) * 100.0f;
		*Temperature = (float)temp_raw / (1 << 20) * 200.0f -50.0f;
	}
}
