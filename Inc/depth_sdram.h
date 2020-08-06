/*
 * depth_sdram.h
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#ifndef OPENGL_INC_DEPTH_SDRAM_H_
#define OPENGL_INC_DEPTH_SDRAM_H_

#include "stm32f429i_discovery_sdram.h"
#include "stm32f4xx_hal.h"

#include "opengl.h"

#define DEPTH_DMA_WAIT_READY_MS    10


void  Depth_SDRAM_Init(SDRAM_HandleTypeDef* sdramHandle, uint32_t startAdrres, uint16_t lcd_width, uint16_t lcd_height);
HAL_DMA_StateTypeDef Depth_WaitReadyDma(uint32_t timeout_ms);
void  Depth_SDRAM_WriteDepth(uint16_t x, uint16_t y, float depth);
float Depth_SDRAM_ReadDepth(uint16_t x, uint16_t y);
void  Depth_SDRAM_ClearDepth();

#endif /* OPENGL_INC_DEPTH_SDRAM_H_ */
