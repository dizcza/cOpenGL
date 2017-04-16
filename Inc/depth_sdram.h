/*
 * depth_sdram.h
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#ifndef OPENGL_INC_DEPTH_SDRAM_H_
#define OPENGL_INC_DEPTH_SDRAM_H_

#include "stm32f429i_discovery_sdram.h"

void  Depth_SDRAM_Init(SDRAM_HandleTypeDef* sdramHandle, uint32_t startAdrres, uint16_t lcd_width, uint16_t lcd_height);
void  Depth_SDRAM_WriteDepth(uint16_t x, uint16_t y, float depth);
float Depth_SDRAM_ReadDepth(uint16_t x, uint16_t y);
void  Depth_SDRAM_ClearDepth();

#endif /* OPENGL_INC_DEPTH_SDRAM_H_ */
