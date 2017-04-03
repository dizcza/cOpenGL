/*
 * cube.c
 *
 *  Created on: Nov 11, 2016
 *      Author: dizcza
 */


#include "cube.h"
#include "debug_printf.h"

static const uint8_t m_indices[CUBE_INDEX_COUNT] = {
		0, 1, 2,
		0, 2, 3,

		0, 1, 5,
		0, 5, 4,

		1, 5, 6,
		1, 6, 2,

		0, 4, 7,
		0, 7, 3,

		4, 5, 6,
		4, 6, 7,

		3, 2, 6,
		3, 6, 7
};

static const uint32_t m_colors[CUBE_VERTEX_COUNT] = {
		0xFFFF0000, // RED
		0xFF00FF00, // GREEN
		0xFF0000FF, // BLUE
		0xFF00FFFF, // CYAN
		0xFFFF00FF, // MAGENTA
		0xFFFFFF00, // YELLOW
		0xFFA52A2A, // BROWN
		0xFFFFA500  // ORANGE
};

static void Cube_InitIndices(Cube* cube) {
	uint8_t i;
	for (i = 0; i < CUBE_INDEX_COUNT; ++i) {
		cube->indices[i] = m_indices[i];
	}
}

static void Cube_InitVertices(Cube* cube) {
	const float half_sz = 0.5;
	vec4_set(cube->vertices[0],  - half_sz,  - half_sz,  + half_sz, 1.0);
	vec4_set(cube->vertices[1],  + half_sz,  - half_sz,  + half_sz, 1.0);
	vec4_set(cube->vertices[2],  + half_sz,  + half_sz,  + half_sz, 1.0);
	vec4_set(cube->vertices[3],  - half_sz,  + half_sz,  + half_sz, 1.0);

	vec4_set(cube->vertices[4],  - half_sz,  - half_sz,  - half_sz, 1.0);
	vec4_set(cube->vertices[5],  + half_sz,  - half_sz,  - half_sz, 1.0);
	vec4_set(cube->vertices[6],  + half_sz,  + half_sz,  - half_sz, 1.0);
	vec4_set(cube->vertices[7],  - half_sz,  + half_sz,  - half_sz, 1.0);
}

static void Cube_InitColors(Cube* cube) {
	uint8_t i;
	for (i = 0; i < CUBE_VERTEX_COUNT; ++i) {
		cube->colors[i] = m_colors[i];
	}
}

void Cube_Init(Cube* cube, float size) {
	Cube_InitIndices(cube);
	Cube_InitVertices(cube);
	Cube_InitColors(cube);
	mat4x4_identity(cube->model);
	Cube_Scale(cube, size);
}


void Cube_Scale(Cube* cube, float scale) {
	mat4x4_scale_aniso(cube->model, cube->model, scale, scale, scale);
}


void Cube_Translate(Cube* cube, float x, float y, float z) {
	mat4x4_translate_in_place(cube->model, x, y, z);
}

// in 3D space
void Cube_GetTriangle(const Cube* cube, trian4 trian, vec3uint32 colors, uint8_t tr_id) {
	uint8_t index;
	for (index = 3 * tr_id; index < 3 * (tr_id + 1); ++index) {
		const uint8_t tr_pid = index % 3;
		uint8_t vid = cube->indices[index];
		colors[tr_pid] = cube->colors[vid];
		db_printf("tr %d, vid %d, tr_pid %d, color 0x%08x\n", tr_id, vid, tr_pid, colors[tr_pid]);
		HAL_Delay(100);
		mat4x4_mul_vec4(trian[tr_pid], cube->model, cube->vertices[vid]);
	}
}
