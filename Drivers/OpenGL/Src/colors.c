/*
 * colors.c
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */


#include "colors.h"

#define BASE_COLOR 0xFF000000

void Colors_RgbFromHex(vec3uint32 v_rgb, uint32_t const hex) {
	v_rgb[0] = (hex >> 16) & 0xFF;
	v_rgb[1] = (hex >> 8) & 0xFF;
	v_rgb[2] = (hex) & 0xFF;
}

void Colors_fRgbFromHex(vec3 fv_rgb, uint32_t const hex) {
	fv_rgb[0] = (float) ((hex >> 16) & 0xFF) / 0xFF;
	fv_rgb[1] = (float) ((hex >> 8) & 0xFF) / 0xFF;
	fv_rgb[2] = (float) ((hex) & 0xFF) / 0xFF;
}

uint32_t Colors_HexFromRgb(vec3uint32 const v_rgb) {
	return BASE_COLOR + (v_rgb[0] << 16) + (v_rgb[1] << 8) + v_rgb[2];
}

uint32_t Colors_HexFromRgbf(vec3 const fv_rgb) {
	vec3uint32 v_rgb;
	vec3 fv_rgb_scaled;
	vec3_scale(fv_rgb_scaled, fv_rgb, 0xFF);
	vec3uint32_from_vec3(v_rgb, fv_rgb_scaled);
	return BASE_COLOR + (v_rgb[0] << 16) + (v_rgb[1] << 8) + v_rgb[2];
}

void Colors_fRgbFromHexMat(mat3x3 m_rgb, vec3uint32 const v_hex) {
	uint8_t j;
	for (j = 0; j < 3; ++j) {
		vec3 fv_rgb;
		Colors_fRgbFromHex(fv_rgb, v_hex[j]);
		mat3x3_set_col(m_rgb, fv_rgb, j);
	}
}
