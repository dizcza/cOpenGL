/*
 * CubeTouchMe.c
 *
 *  Created on: Apr 14, 2017
 *      Author: dizcza
 */


#include "Demo.h"
#include "framehandler.h"

static const Camera* m_Camera;
static uint8_t m_isActive = 0;

void CubeTouchMe_Init(const Camera* camera) {
	m_Camera = camera;
}

void CubeTouchMe_Resume() {
	m_isActive = 1;
	BSP_LCD_DisplayStringAtLine(8, "resumed");
	FrameHandler_glFlush();
}

void CubeTouchMe_Pause() {
	m_isActive = 0;
}
