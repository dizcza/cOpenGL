/*
 * debug_printf.h
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

//#define DEBUG_CONSOLE
 #ifdef DEBUG_CONSOLE
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
 #endif /* DEBUG_CONSOLE */

DB_DEFINE_PRINTF(3);
DB_DEFINE_PRINTF(4);

#define DEBUG_LCD
#ifdef DEBUG_LCD
#include "stm32f429i_discovery_lcd.h"
#include <stdio.h>
#include <stdarg.h>
/**
 * \brief Print the specified Text
 *
 * \param fmt	Format text
 * \param
 *
 * \return void
 */
static uint16_t m_LcdLine = 0;
static void inline LCD_Printf(const char *fmt, ...) {
	static char buf[256];
	va_list lst;
	va_start(lst, fmt);
	vsprintf(buf, fmt, lst);
	va_end(lst);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAtLine(m_LcdLine, (uint8_t*) buf);
	m_LcdLine = (m_LcdLine + 1) % 10;
}
#else
#define LCD_Printf(...)
#endif /* DEBUG_LCD */

#endif /* DEBUG_PRINTF_H_ */
