/*
 * debug_printf.h
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

#define DEBUG1
 #ifdef DEBUG
	 #include "linmath.h"
     #include <stdio.h>
     #include <stdlib.h>
     #define db_puts(s) puts(s)
     #define db_printf(szFormat, ...) printf(szFormat,##__VA_ARGS__)
	 extern void initialise_monitor_handles(void);

#define DB_DEFINE_PRINTF(n) \
static void inline db_printf_vec##n(vec##n const v) { \
	uint8_t i; \
	db_printf("vec%d { ", n); \
	for (i = 0; i < n; ++i) { \
		db_printf("%f ", v[i]); \
	} \
	db_printf(" }\n"); \
} \
static inline void db_printf_mat##n##x##n(mat##n##x##n const M) { \
	uint8_t i, j; \
	db_printf("mat%dx%d", n, n); \
	for (j = 0; j < n; ++j) { \
		db_printf("\t"); \
		for (i = 0; i < n; ++i) { \
			db_printf("%f ", M[i][j]); \
		} \
		db_printf("\n"); \
	} \
	db_printf("\n"); \
}

 #else
     #define db_puts(s)
     #define db_printf(szFormat, ...)
 #define DB_DEFINE_PRINTF(n)
 #endif

DB_DEFINE_PRINTF(3);
DB_DEFINE_PRINTF(4);

#endif /* DEBUG_PRINTF_H_ */
