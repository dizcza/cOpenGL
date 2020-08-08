/*
 * CubeRotationAnim.c
 *
 *  Created on: Apr 8, 2017
 *      Author: dizcza
 */

#include <stdlib.h>
#include "stm32f429i_discovery_lcd.h"

#include "oglcube.h"
#include "framehandler.h"
#include "depth_sdram.h"
#include "Demo.h"

#define ROTANIM_DURATION_MS 3000
#define ROTANIM_CUBE_CNT    3

static Cube m_Cubes[ROTANIM_CUBE_CNT];
static const Camera* m_Camera;
static const vec3 m_axis = {1, 1, 1};
static uint32_t m_lastTick;
static uint8_t m_isActive;

static inline float GetRandomFloat() {
	return ((float)rand()) / RAND_MAX;
}

static inline float GetRandomFloatRange(float a_min, float b_max) {
	return a_min + (GetRandomFloat()) * (b_max - a_min);
}

static void GetRandomTranslation(vec3 tr) {
	tr[0] = GetRandomFloatRange(-2.0f, 5.0f);
	tr[1] = GetRandomFloatRange(-1.5f, 0.5f);
	tr[2] = GetRandomFloatRange(-20.0f, 0.0f);
}

static void GetRandomRotation(quat rot) {
	vec3 axis;
	axis[0] = GetRandomFloatRange(-1, 1);
	axis[1] = GetRandomFloatRange(-1, 1);
	axis[2] = GetRandomFloatRange(-1, 1);
	float angle_rads = GetRandomFloat() * M_PI;
	quat_rotate(rot, angle_rads, axis);
}

void CubeRotationAnim_Init(const Camera* camera) {
	srand(1U);

	m_Camera = camera;
	m_isActive = 0;

	uint32_t i;
	for (i = 0; i < ROTANIM_CUBE_CNT; ++i) {
		float scale = GetRandomFloatRange(0.1f, 0.8f);
		OpenGL_Cube_Init(&m_Cubes[i], scale);

		quat rot;
		GetRandomRotation(rot);
		OpenGL_Cube_RotateLocal(&m_Cubes[i], rot);

		vec3 tr;
		GetRandomTranslation(tr);
		OpenGL_Cube_TranslateVec3(&m_Cubes[i], tr);
	}

	m_lastTick = HAL_GetTick();
}

void CubeRotationAnim_Pause() {
	m_isActive = 0;
}

void CubeRotationAnim_Resume() {
	if (!m_isActive) {
		FrameHandler_Reset();
		m_isActive = 1;
	}
	uint32_t i;
	quat q;
	uint32_t curr = HAL_GetTick();
	uint32_t ms_passed = (curr - m_lastTick) % ROTANIM_DURATION_MS;
	float time_local = ((float)ms_passed) / ROTANIM_DURATION_MS;
	float angle_rads = time_local * M_PI;
	quat_rotate(q, angle_rads, m_axis);
	for (i = 0; i < ROTANIM_CUBE_CNT; ++i) {
		OpenGL_Cube_RotateLocal(&m_Cubes[i], q);
		FrameHandler_DrawCube(m_Camera, &m_Cubes[i]);
	}
	FrameHandler_glFlush();
	m_lastTick = curr;
}
