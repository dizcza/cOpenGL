/*
 * cube.c
 *
 *  Created on: Nov 11, 2016
 *      Author: dizcza
 */


#include "cube.h"

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

void Cube_Init(Cube* cube, float size) {
	Cube_InitIndices(cube);
	Cube_InitVertices(cube);
	mat4x4_identity(cube->model);
	mat4x4_scale_self(cube->model, size);
}


void Cube_Scale(Cube* cube, float scale) {
	mat4x4_scale_self(cube->model, scale);
}


void Cube_Translate(Cube* cube, float x, float y, float z) {
	mat4x4_translate_in_place(cube->model, x, y, z);
}

// in 3D space
void Cube_GetTrianlge(const Cube* cube, trian4 trian, uint8_t tr_id) {
	uint8_t index;
	for (index = 3 * tr_id; index < 3 * (tr_id + 1); ++index) {
		uint8_t vid = cube->indices[index];
		mat4x4_mul_vec4(trian[index % 3], cube->model, cube->vertices[vid]);
	}
}
