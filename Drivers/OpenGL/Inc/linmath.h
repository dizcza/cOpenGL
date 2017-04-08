#ifndef LINMATH_H
#define LINMATH_H

#include <math.h>
#include "stdint.h"
#include "float.h"
#include "assert.h"

#define LINMATH_EPS 0.0001f

#define min(x,y) (x < y ? x : y)
#define max(x,y) (x > y ? x : y)
#define clamp(x,a,b) (x < a ? a : (x > b ? b : x))


#define LINMATH_H_DEFINE_VEC(n) \
typedef float vec##n[n]; \
static inline void vec##n##_add(vec##n r, vec##n const a, vec##n const b) \
{ \
	int i; \
	for(i=0; i<n; ++i) \
		r[i] = a[i] + b[i]; \
} \
static inline void vec##n##_add_self(vec##n r, vec##n const v) \
{ \
	vec##n##_add(r, r, v); \
} \
static inline void vec##n##_sub(vec##n r, vec##n const a, vec##n const b) \
{ \
	int i; \
	for(i=0; i<n; ++i) \
		r[i] = a[i] - b[i]; \
} \
static inline void vec##n##_sub_self(vec##n r, vec##n const v) \
{ \
	vec##n##_sub(r, r, v); \
} \
static inline void vec##n##_scale(vec##n r, vec##n const v, float const s) \
{ \
	int i; \
	for(i=0; i<n; ++i) \
		r[i] = v[i] * s; \
} \
static inline void vec##n##_scale_self(vec##n r, float const s) \
{ \
	vec##n##_scale(r, r, s); \
} \
static inline float vec##n##_mul_inner(vec##n const a, vec##n const b) \
{ \
	float p = 0.f; \
	int i; \
	for(i=0; i<n; ++i) \
		p += b[i]*a[i]; \
	return p; \
} \
static inline float vec##n##_len(vec##n const v) \
{ \
	return sqrtf(vec##n##_mul_inner(v,v)); \
} \
static inline void vec##n##_norm(vec##n r, vec##n const v) \
{ \
	float k = 1.f / vec##n##_len(v); \
	vec##n##_scale(r, v, k); \
} \
static inline void vec##n##_norm_self(vec##n r) \
{ \
	vec##n##_norm(r, r); \
} \
static inline void vec##n##_dup(vec##n r, vec##n const src) \
{ \
	int i; \
	for(i=0; i<n; ++i) \
		r[i] = src[i]; \
} \
static inline void vec##n##_min(vec##n r, vec##n const a, vec##n const b) \
{ \
	int i; \
	for(i=0; i<n; ++i) \
		r[i] = a[i]<b[i] ? a[i] : b[i]; \
} \
static inline void vec##n##_max(vec##n r, vec##n const a, vec##n const b) \
{ \
	int i; \
	for(i=0; i<n; ++i) \
		r[i] = a[i]>b[i] ? a[i] : b[i]; \
} \
static inline void vec##n##_interpolate(vec##n * buffer, uint32_t cnt, vec##n const from, vec##n const to) \
{ \
	vec##n inc; \
	vec##n##_sub(inc, to, from); \
	vec##n##_scale_self(inc, 1.f / (cnt - 1)); \
	vec##n##_dup(buffer[0], from); \
	uint32_t i; \
	for (i = 1; i < cnt; ++i) { \
		vec##n##_add(buffer[i], buffer[i - 1], inc); \
	} \
} \
static inline void vec##n##_pos_inf(vec##n v) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		v[i] = FLT_MAX; \
	} \
} \
static inline void vec##n##_neg_inf(vec##n v) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		v[i] = -FLT_MAX; \
	} \
} \
static inline uint8_t vec##n##_all_pos(vec##n v) { \
	uint8_t i; \
	uint8_t res = 1; \
	for (i = 0; i < n; ++i) { \
		res &= v[i] >= 0; \
	} \
	return res; \
} \
static inline uint8_t vec##n##_all_neg(vec##n v) { \
	uint8_t i; \
	uint8_t res = 1; \
	for (i = 0; i < n; ++i) { \
		res &= v[i] <= 0; \
	} \
	return res; \
}

LINMATH_H_DEFINE_VEC(2)
LINMATH_H_DEFINE_VEC(3)
LINMATH_H_DEFINE_VEC(4)

# define LINMATH_H_DEFINE_VECI(n,pref,b) \
typedef pref##int##b##_t vec##n##pref##int##b[n]; \
static void inline vec##n##pref##int##b##_floor(vec##n##pref##int##b vint, vec##n const vf) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		vint[i] = (int##b##_t) floorf(vf[i]); \
	} \
} \
static void inline vec##n##pref##int##b##_ceil(vec##n##pref##int##b vint, vec##n const vf) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		vint[i] = (int##b##_t) ceilf(vf[i]); \
	} \
} \
static void inline vec##n##pref##int##b##_from_vec##n(vec##n##pref##int##b vint, vec##n const vf) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		vint[i] = (int##b##_t) vf[i]; \
	} \
} \
static void inline vec##n##_from_vec##n##pref##int##b(vec##n vf, vec##n##pref##int##b const vint) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		vf[i] = (float) vint[i]; \
	} \
}

LINMATH_H_DEFINE_VECI(2,,16);
LINMATH_H_DEFINE_VECI(3,,16);
LINMATH_H_DEFINE_VECI(4,,16);

LINMATH_H_DEFINE_VECI(2,u,32);
LINMATH_H_DEFINE_VECI(3,u,32);
LINMATH_H_DEFINE_VECI(4,u,32);


static inline float vec2_mul_cross(vec2 const a, vec2 const b) {
	return a[0] * b[1] - a[1] * b[0];
}

static inline void vec3_mul_cross(vec3 r, vec3 const a, vec3 const b) {
	vec3 r_temp;
	r_temp[0] = a[1] * b[2] - a[2] * b[1];
	r_temp[1] = a[2] * b[0] - a[0] * b[2];
	r_temp[2] = a[0] * b[1] - a[1] * b[0];
	vec3_dup(r, r_temp);
}

static inline void vec4_mul_cross(vec4 r, vec4 const a, vec4 const b) {
	vec4 r_temp;
	r_temp[0] = a[1] * b[2] - a[2] * b[1];
	r_temp[1] = a[2] * b[0] - a[0] * b[2];
	r_temp[2] = a[0] * b[1] - a[1] * b[0];
	r_temp[3] = 1.f;
	vec3_dup(r, r_temp);
}

static inline void vec3_reflect(vec3 r, vec3 const v, vec3 const n) {
	float p = 2.f * vec3_mul_inner(v, n);
	int i;
	for (i = 0; i < 3; ++i)
		r[i] = v[i] - p * n[i];
}

static inline void vec4_reflect(vec4 r, vec4 const v, vec4 const n) {
	float p = 2.f * vec4_mul_inner(v, n);
	int i;
	for (i = 0; i < 4; ++i)
		r[i] = v[i] - p * n[i];
}

static inline void vec3_set(vec3 r, float x, float y, float z) {
	r[0] = x;
	r[1] = y;
	r[2] = z;
}

static inline void vec4_set(vec4 r, float x, float y, float z, float w) {
	r[0] = x;
	r[1] = y;
	r[2] = z;
	r[3] = w;
}

static inline void vec4_from_vec3(vec4 r, vec3 const point) {
	r[0] = point[0];
	r[1] = point[1];
	r[2] = point[2];
	r[3] = 0.f;
}

/**
 * Matrices are stored in column order.
 */
#define LINMATH_H_DEFINE_MAT(n) \
typedef vec##n mat##n##x##n[n]; \
static inline void mat##n##x##n##_mul_vec##n(vec##n r, mat##n##x##n const M, vec##n const v) { \
	vec##n r_temp; \
	uint8_t i, j; \
	for (j = 0; j < n; ++j) { \
		r_temp[j] = 0.f; \
		for (i = 0; i < n; ++i) \
			r_temp[j] += M[i][j] * v[i]; \
	} \
	vec##n##_dup(r, r_temp); \
} \
static inline void mat##n##x##n##_set_col(mat##n##x##n M, vec##n const v, uint8_t col_index) { \
	uint8_t i; \
	for (i = 0; i < n; ++i) { \
		M[col_index][i] = v[i]; \
	} \
} \
static inline void mat##n##x##n##_set_row(mat##n##x##n M, vec##n const v, uint8_t row_index) { \
	uint8_t j; \
	for (j = 0; j < n; ++j) { \
		M[j][row_index] = v[j]; \
	} \
}

LINMATH_H_DEFINE_MAT(3);
LINMATH_H_DEFINE_MAT(4);


static inline void mat4x4_identity(mat4x4 M) {
	int i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			M[i][j] = i == j ? 1.f : 0.f;
}
static inline void mat4x4_dup(mat4x4 M, mat4x4 const N) {
	int i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			M[i][j] = N[i][j];
}
static inline void mat4x4_row(vec4 r, mat4x4 const M, int i) {
	int k;
	for (k = 0; k < 4; ++k)
		r[k] = M[k][i];
}
static inline void mat4x4_col(vec4 r, mat4x4 const M, int i) {
	int k;
	for (k = 0; k < 4; ++k)
		r[k] = M[i][k];
}
static inline void mat4x4_transpose(mat4x4 M, mat4x4 const N) {
	mat4x4 M_temp;
	int i, j;
	for (j = 0; j < 4; ++j)
		for (i = 0; i < 4; ++i)
			M_temp[i][j] = N[j][i];
	mat4x4_dup(M, M_temp);
}
static inline void mat4x4_add(mat4x4 M, mat4x4 const a, mat4x4 const b) {
	int i;
	for (i = 0; i < 4; ++i)
		vec4_add(M[i], a[i], b[i]);
}
static inline void mat4x4_sub(mat4x4 M, mat4x4 const a, mat4x4 const b) {
	int i;
	for (i = 0; i < 4; ++i)
		vec4_sub(M[i], a[i], b[i]);
}
static inline void mat4x4_scale(mat4x4 M, mat4x4 const a, float k) {
	int i;
	for (i = 0; i < 4; ++i)
		vec4_scale(M[i], a[i], k);
}
static inline void mat4x4_scale_self(mat4x4 M, float k) {
	mat4x4_scale(M, M, k);
}
static inline void mat4x4_scale_aniso(mat4x4 M, mat4x4 const a, float x,
		float y, float z) {
	int i;
	vec4_scale(M[0], a[0], x);
	vec4_scale(M[1], a[1], y);
	vec4_scale(M[2], a[2], z);
	for (i = 0; i < 4; ++i) {
		M[3][i] = a[3][i];
	}
}
static inline void mat4x4_mul(mat4x4 M, mat4x4 const a, mat4x4 const b) {
	mat4x4 M_temp;
	int k, r, c;
	for (c = 0; c < 4; ++c)
		for (r = 0; r < 4; ++r) {
			M_temp[c][r] = 0.f;
			for (k = 0; k < 4; ++k)
				M_temp[c][r] += a[k][r] * b[c][k];
		}
	mat4x4_dup(M, M_temp);
}

static inline void mat4x4_translate_to(mat4x4 T, float x, float y, float z) {
	mat4x4_identity(T);
	T[3][0] = x;
	T[3][1] = y;
	T[3][2] = z;
}
static inline void mat4x4_translate_in_place(mat4x4 M, float x, float y,
		float z) {
	vec4 t = { x, y, z, 0 };
	vec4 r;
	int i;
	for (i = 0; i < 4; ++i) {
		mat4x4_row(r, M, i);
		M[3][i] += vec4_mul_inner(r, t);
	}
}
static inline void mat4x4_translate_gl(mat4x4 M, float x, float y,
		float z) {
	M[3][0] += x;
	M[3][1] += y;
	M[3][2] += z;
}
static inline void mat4x4_from_vec3_mul_outer(mat4x4 M, vec3 const a,
		vec3 const b) {
	int i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			M[i][j] = i < 3 && j < 3 ? a[i] * b[j] : 0.f;
}
static inline void mat4x4_rotate(mat4x4 R, mat4x4 const M, float x, float y,
		float z, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);
	vec3 u = { x, y, z };

	if (vec3_len(u) > 1e-4) {
		vec3_norm(u, u);
		mat4x4 T;
		mat4x4_from_vec3_mul_outer(T, u, u);

		mat4x4 S = {
			{    0,  u[2], -u[1], 0},
			{-u[2],     0,  u[0], 0},
			{ u[1], -u[0],     0, 0},
			{    0,     0,     0, 0}
		};
		mat4x4_scale(S, S, s);

		mat4x4 C;
		mat4x4_identity(C);
		mat4x4_sub(C, C, T);

		mat4x4_scale(C, C, c);

		mat4x4_add(T, T, C);
		mat4x4_add(T, T, S);

		T[3][3] = 1.f;
		mat4x4_mul(R, M, T);
	} else {
		mat4x4_dup(R, M);
	}
}
static inline void mat4x4_rotate_X(mat4x4 Q, mat4x4 const M, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);
	mat4x4 R = {
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, c,   s,   0.f },
			{ 0.f, -s,  c,   0.f },
			{ 0.f, 0.f, 0.f, 1.f }
	};
	mat4x4_mul(Q, M, R);
}
static inline void mat4x4_rotate_Y(mat4x4 Q, mat4x4 const M, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);
	mat4x4 R = {
			{ c,   0.f, s,   0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ -s,  0.f, c,   0.f },
			{ 0.f, 0.f, 0.f, 1.f }
	};
	mat4x4_mul(Q, M, R);
}
static inline void mat4x4_rotate_Z(mat4x4 Q, mat4x4 const M, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);
	mat4x4 R = {
			{ c,   s,   0.f, 0.f },
			{ -s,  c,   0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 0.f, 1.f }
	};
	mat4x4_mul(Q, M, R);
}
static inline void mat4x4_invert(mat4x4 T, mat4x4 const M) {
	mat4x4 T_temp;
	float s[6];
	float c[6];
	s[0] = M[0][0] * M[1][1] - M[1][0] * M[0][1];
	s[1] = M[0][0] * M[1][2] - M[1][0] * M[0][2];
	s[2] = M[0][0] * M[1][3] - M[1][0] * M[0][3];
	s[3] = M[0][1] * M[1][2] - M[1][1] * M[0][2];
	s[4] = M[0][1] * M[1][3] - M[1][1] * M[0][3];
	s[5] = M[0][2] * M[1][3] - M[1][2] * M[0][3];

	c[0] = M[2][0] * M[3][1] - M[3][0] * M[2][1];
	c[1] = M[2][0] * M[3][2] - M[3][0] * M[2][2];
	c[2] = M[2][0] * M[3][3] - M[3][0] * M[2][3];
	c[3] = M[2][1] * M[3][2] - M[3][1] * M[2][2];
	c[4] = M[2][1] * M[3][3] - M[3][1] * M[2][3];
	c[5] = M[2][2] * M[3][3] - M[3][2] * M[2][3];

	/* Assumes it is invertible */
	float idet = 1.f
			/ (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2]
					- s[4] * c[1] + s[5] * c[0]);

	T_temp[0][0] = (M[1][1] * c[5] - M[1][2] * c[4] + M[1][3] * c[3]) * idet;
	T_temp[0][1] = (-M[0][1] * c[5] + M[0][2] * c[4] - M[0][3] * c[3]) * idet;
	T_temp[0][2] = (M[3][1] * s[5] - M[3][2] * s[4] + M[3][3] * s[3]) * idet;
	T_temp[0][3] = (-M[2][1] * s[5] + M[2][2] * s[4] - M[2][3] * s[3]) * idet;

	T_temp[1][0] = (-M[1][0] * c[5] + M[1][2] * c[2] - M[1][3] * c[1]) * idet;
	T_temp[1][1] = (M[0][0] * c[5] - M[0][2] * c[2] + M[0][3] * c[1]) * idet;
	T_temp[1][2] = (-M[3][0] * s[5] + M[3][2] * s[2] - M[3][3] * s[1]) * idet;
	T_temp[1][3] = (M[2][0] * s[5] - M[2][2] * s[2] + M[2][3] * s[1]) * idet;

	T_temp[2][0] = (M[1][0] * c[4] - M[1][1] * c[2] + M[1][3] * c[0]) * idet;
	T_temp[2][1] = (-M[0][0] * c[4] + M[0][1] * c[2] - M[0][3] * c[0]) * idet;
	T_temp[2][2] = (M[3][0] * s[4] - M[3][1] * s[2] + M[3][3] * s[0]) * idet;
	T_temp[2][3] = (-M[2][0] * s[4] + M[2][1] * s[2] - M[2][3] * s[0]) * idet;

	T_temp[3][0] = (-M[1][0] * c[3] + M[1][1] * c[1] - M[1][2] * c[0]) * idet;
	T_temp[3][1] = (M[0][0] * c[3] - M[0][1] * c[1] + M[0][2] * c[0]) * idet;
	T_temp[3][2] = (-M[3][0] * s[3] + M[3][1] * s[1] - M[3][2] * s[0]) * idet;
	T_temp[3][3] = (M[2][0] * s[3] - M[2][1] * s[1] + M[2][2] * s[0]) * idet;

	mat4x4_dup(T, T_temp);
}
static inline void mat4x4_orthonormalize(mat4x4 R, mat4x4 const M) {
	mat4x4_dup(R, M);
	float s = 1.f;
	vec3 h;

	vec3_norm(R[2], R[2]);

	s = vec3_mul_inner(R[1], R[2]);
	vec3_scale(h, R[2], s);
	vec3_sub(R[1], R[1], h);
	vec3_norm(R[2], R[2]);

	s = vec3_mul_inner(R[1], R[2]);
	vec3_scale(h, R[2], s);
	vec3_sub(R[1], R[1], h);
	vec3_norm(R[1], R[1]);

	s = vec3_mul_inner(R[0], R[1]);
	vec3_scale(h, R[1], s);
	vec3_sub(R[0], R[0], h);
	vec3_norm(R[0], R[0]);
}

static inline void mat4x4_frustum(mat4x4 M, float l, float r, float b, float t,
		float n, float f) {
	M[0][0] = 2.f * n / (r - l);
	M[0][1] = M[0][2] = M[0][3] = 0.f;

	M[1][1] = 2.f * n / (t - b);
	M[1][0] = M[1][2] = M[1][3] = 0.f;

	M[2][0] = (r + l) / (r - l);
	M[2][1] = (t + b) / (t - b);
	M[2][2] = -(f + n) / (f - n);
	M[2][3] = -1.f;

	M[3][2] = -2.f * (f * n) / (f - n);
	M[3][0] = M[3][1] = M[3][3] = 0.f;
}
static inline void mat4x4_ortho(mat4x4 M, float l, float r, float b, float t,
		float n, float f) {
	M[0][0] = 2.f / (r - l);
	M[0][1] = M[0][2] = M[0][3] = 0.f;

	M[1][1] = 2.f / (t - b);
	M[1][0] = M[1][2] = M[1][3] = 0.f;

	M[2][2] = -2.f / (f - n);
	M[2][0] = M[2][1] = M[2][3] = 0.f;

	M[3][0] = -(r + l) / (r - l);
	M[3][1] = -(t + b) / (t - b);
	M[3][2] = -(f + n) / (f - n);
	M[3][3] = 1.f;
}
static inline void mat4x4_perspective(mat4x4 m, float y_fov, float aspect,
		float n, float f) {
	/* NOTE: Degrees are an unhandy unit to work with.
	 * linmath.h uses radians for everything! */
	float const a = 1.f / tanf(y_fov / 2.f);

	m[0][0] = a / aspect;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = a;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = -((f + n) / (f - n));
	m[2][3] = -1.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = -((2.f * f * n) / (f - n));
	m[3][3] = 0.f;
}
static inline void mat4x4_look_at(mat4x4 m, vec3 const eye, vec3 const center,
		vec3 const up) {
	/* Adapted from Android's OpenGL Matrix.java.                        */
	/* See the OpenGL GLUT documentation for gluLookAt for a description */
	/* of the algorithm. We implement it in a straightforward way:       */

	/* TODO: The negation of of can be spared by swapping the order of
	 *       operands in the following cross products in the right way. */
	vec3 f;
	vec3_sub(f, center, eye);
	vec3_norm(f, f);

	vec3 s;
	vec3_mul_cross(s, f, up);
	vec3_norm(s, s);

	vec3 t;
	vec3_mul_cross(t, s, f);

	m[0][0] = s[0];
	m[0][1] = t[0];
	m[0][2] = -f[0];
	m[0][3] = 0.f;

	m[1][0] = s[1];
	m[1][1] = t[1];
	m[1][2] = -f[1];
	m[1][3] = 0.f;

	m[2][0] = s[2];
	m[2][1] = t[2];
	m[2][2] = -f[2];
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;

	mat4x4_translate_in_place(m, -eye[0], -eye[1], -eye[2]);
}

typedef float quat[4];
#define quat_add vec4_add
#define quat_sub vec4_sub
#define quat_norm vec4_norm
#define quat_norm_self vec4_norm_self
#define quat_scale vec4_scale
#define quat_scale_self vec4_scale_self
#define quat_mul_inner vec4_mul_inner

static inline void quat_identity(quat q) {
	q[0] = q[1] = q[2] = 0.f;
	q[3] = 1.f;
}
static inline void quat_mul(quat r, quat const p, quat const q) {
	vec3 w;
	vec3_mul_cross(r, p, q);
	vec3_scale(w, p, q[3]);
	vec3_add(r, r, w);
	vec3_scale(w, q, p[3]);
	vec3_add(r, r, w);
	r[3] = p[3] * q[3] - vec3_mul_inner(p, q);
}
static inline void quat_conj(quat r, quat const q) {
	int i;
	for (i = 0; i < 3; ++i)
		r[i] = -q[i];
	r[3] = q[3];
}
static inline void quat_rotate(quat r, float angle, vec3 const axis) {
	vec3 v;
	vec3_scale(v, axis, sinf(angle / 2));
	int i;
	for (i = 0; i < 3; ++i)
		r[i] = v[i];
	r[3] = cosf(angle / 2);
}

static inline void quat_mul_vec3(vec3 r, quat const q, vec3 const v) {
	/*
	 * Method by Fabian 'ryg' Giessen (of Farbrausch)
	 t = 2 * cross(q.xyz, v)
	 v' = v + q.w * t + cross(q.xyz, t)
	 todo: check if in-place quat_mul_vec3(r, r, v) works
	 */
	vec3 t;
	vec3 q_xyz = { q[0], q[1], q[2] };
	vec3 u = { q[0], q[1], q[2] };

	vec3_mul_cross(t, q_xyz, v);
	vec3_scale(t, t, 2);

	vec3_mul_cross(u, q_xyz, t);
	vec3_scale(t, t, q[3]);

	vec3_add(r, v, t);
	vec3_add(r, r, u);
}

/*
 * Interpolates v0 & v1 by parameter 0 <= t <= 1.
 */
static inline void quat_slerp(quat res, quat const v0, quat const v1, float t) {
    // Only unit quaternions are valid rotations.
    // Normalize to avoid undefined behavior.
	assert(t <= 1.f && t >= 0.f);
	quat v0_norm, v1_norm;
	quat_norm(v0_norm, v0);
	quat_norm(v1_norm, v1);

    // Compute the cosine of the angle between the two vectors.
    float dot = quat_mul_inner(v0_norm, v1_norm);
    if (dot > 1.f - LINMATH_EPS) {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.
    	// Quaternion result = v0 + t*(v1 – v0);
    	quat v_diff;
    	quat_sub(v_diff, v1_norm, v0_norm);
    	quat_scale_self(v_diff, t);
    	quat_add(res, v0_norm, v_diff);
    	quat_norm_self(res);
        return;
    }

    // If the dot product is negative, the quaternions
    // have opposite handed-ness and slerp won't take
    // the shorter path. Fix by reversing one quaternion.
    if (dot < 0.f) {
    	quat_scale_self(v1_norm, -1);
        dot = -dot;
    }

    dot = clamp(dot, -1.f, 1.f);              // Robustness: Stay within domain of acos()
    float theta_0 = acosf(dot);            // theta_0 = angle between input vectors
    float theta = theta_0*t;              // theta = angle between v0 and result

    quat v2_norm;
    quat_scale_self(v0_norm, dot);
    quat_sub(v2_norm, v1_norm, v0_norm);  // Quaternion v2 = v1 – v0*dot;
    quat_norm_self(v2_norm);              // { v0, v2 } is now an orthonormal basis

    quat_scale_self(v0_norm, cosf(theta));
    quat_scale_self(v2_norm, sinf(theta));
    quat_add(res, v0_norm, v2_norm);      // v0*cos(theta) + v2_norm*sin(theta);
}


static inline void mat4x4_from_quat(mat4x4 M, quat const q) {
	float a = q[3];
	float b = q[0];
	float c = q[1];
	float d = q[2];
	float a2 = a * a;
	float b2 = b * b;
	float c2 = c * c;
	float d2 = d * d;

	M[0][0] = a2 + b2 - c2 - d2;
	M[0][1] = 2.f * (b * c + a * d);
	M[0][2] = 2.f * (b * d - a * c);
	M[0][3] = 0.f;

	M[1][0] = 2 * (b * c - a * d);
	M[1][1] = a2 - b2 + c2 - d2;
	M[1][2] = 2.f * (c * d + a * b);
	M[1][3] = 0.f;

	M[2][0] = 2.f * (b * d + a * c);
	M[2][1] = 2.f * (c * d - a * b);
	M[2][2] = a2 - b2 - c2 + d2;
	M[2][3] = 0.f;

	M[3][0] = M[3][1] = M[3][2] = 0.f;
	M[3][3] = 1.f;
}

static inline void mat4x4o_mul_quat(mat4x4 R, mat4x4 const M, quat const q) {
	/*  XXX: The way this is written only works for othogonal matrices. */
	/* TODO: Take care of non-orthogonal case. */
	quat_mul_vec3(R[0], q, M[0]);
	quat_mul_vec3(R[1], q, M[1]);
	quat_mul_vec3(R[2], q, M[2]);

	R[3][0] = R[3][1] = R[3][2] = 0.f;
	R[3][3] = 1.f;
}
static inline void quat_from_mat4x4(quat q, mat4x4 const M) {
	float r = 0.f;
	int i;

	int perm[] = { 0, 1, 2, 0, 1 };
	int *p = perm;

	for (i = 0; i < 3; i++) {
		float m = M[i][i];
		if (m < r)
			continue;
		m = r;
		p = &perm[i];
	}

	r = sqrtf(1.f + M[p[0]][p[0]] - M[p[1]][p[1]] - M[p[2]][p[2]]);

	if (r < 1e-6) {
		q[0] = 1.f;
		q[1] = q[2] = q[3] = 0.f;
		return;
	}

	q[0] = r / 2.f;
	q[1] = (M[p[0]][p[1]] - M[p[1]][p[0]]) / (2.f * r);
	q[2] = (M[p[2]][p[0]] - M[p[0]][p[2]]) / (2.f * r);
	q[3] = (M[p[2]][p[1]] - M[p[1]][p[2]]) / (2.f * r);
}

#endif  /* LINMATH_H */
