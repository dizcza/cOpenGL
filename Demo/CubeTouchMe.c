/*
 * CubeTouchMe.c
 *
 *  Created on: Apr 14, 2017
 *      Author: dizcza
 */


#include "Demo.h"
#include "framehandler.h"
#include "linmath.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "debug_printf.h"

#define TOUCH_MAX_OFFSET_X       50
#define TOUCH_MAX_OFFSET_Y       50
#define TOUCH_FULL_ROTATION_PX   300
#define CONFIDENCE_SENSOR_PARAM  0.8f

typedef enum {
	TS_NONE,
	TS_MOVE
} TS_ActionTypeDef;

static uint8_t isTouchMove(vec2 const curr, vec2 const prev);
static void TS_Calibrate(vec2 curr, vec2 const prev);
static TS_ActionTypeDef onTouchMove(vec2 curr, vec2 prev);

static const Camera* m_Camera;
static Cube m_Cube;
static uint8_t m_isActive;
static vec2 m_LastTouch;

static uint8_t isTouchMove(vec2 const curr, vec2 const prev) {
	return (fabsf(curr[0] - prev[0]) < TOUCH_MAX_OFFSET_X) && (fabsf(curr[1] - prev[1]) < TOUCH_MAX_OFFSET_Y);
}

static void TS_Calibrate(vec2 curr, vec2 const prev) {
	uint8_t i;
	for (i = 0; i < 2; ++i) {
		curr[i] = curr[i] * CONFIDENCE_SENSOR_PARAM + prev[i] * (1.0f - CONFIDENCE_SENSOR_PARAM);
	}
}

static TS_ActionTypeDef onTouchMove(vec2 curr, vec2 prev) {
	TS_ActionTypeDef action;
	if (!isTouchMove(curr, prev)) {
		action = TS_NONE;
	} else {
		TS_Calibrate(curr, prev);
		vec3 vMove = {curr[1] - prev[1], curr[0] - prev[0], 0.f};
		vMove[0] += LINMATH_EPS;
		vMove[1] += LINMATH_EPS;
		float vMovePx = vec3_len(vMove);
//		LCD_Printf("%.1f, %.1f, %.1f", vMove[0], vMove[1], vMove[2]);
		quat q;
		float rads = LINMATH_DEGREES_TO_RADS(360.f * vMovePx / TOUCH_FULL_ROTATION_PX);
		quat_rotate(q, rads, vMove);
		Cube_RotateLocal(&m_Cube, q);
		action = TS_MOVE;
	}
	vec2_dup(prev, curr);
	return action;
}

void CubeTouchMe_Init(const Camera* camera) {
	m_Camera = camera;
	m_isActive = 0;
	m_LastTouch[0] = 0;
	m_LastTouch[1] = 0;

	Cube_Init(&m_Cube, 0.7f);

	vec3 axis = {1, 1, 1};
	quat q;
	quat_rotate(q, LINMATH_DEGREES_TO_RADS(30), axis);
	Cube_RotateLocal(&m_Cube, q);
}

void CubeTouchMe_Pause() {
	m_isActive = 0;
}

void CubeTouchMe_Resume() {
	if (!m_isActive) {
		FrameHandler_Reset();
		FrameHandler_DrawCube(m_Camera, &m_Cube);
		FrameHandler_glFlush();
		m_isActive = 1;
	}
	TS_StateTypeDef ts_state;
	BSP_TS_GetState(&ts_state);
	if (ts_state.TouchDetected) {
		vec2 currTouch = {ts_state.X, ts_state.Y};
		onTouchMove(currTouch, m_LastTouch);
		FrameHandler_DrawCube(m_Camera, &m_Cube);
		FrameHandler_glFlush();
	}
}

