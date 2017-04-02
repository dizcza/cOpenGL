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

	#define DB_DEFINE_PRINTF_VECI(n,pref,b) \
	static void inline db_printf_vec##n##pref##int##b(vec##n##pref##int##b const v) { \
		uint8_t i; \
		db_printf("vec "); \
		for (i = 0; i < n; ++i) { \
			db_printf("%d ", v[i]); \
		} \
		db_printf("\n"); \
	}

	 extern void initialise_monitor_handles(void);
 #else
     #define DB_DEFINE_PRINTF_VECI(n,pref,b)
     #define db_puts(s)
     #define db_printf(szFormat, ...)
 #endif

	 DB_DEFINE_PRINTF_VECI(2,,16);
	 DB_DEFINE_PRINTF_VECI(3,,16);
	 DB_DEFINE_PRINTF_VECI(4,,16);

	 DB_DEFINE_PRINTF_VECI(2,u,32);
	 DB_DEFINE_PRINTF_VECI(3,u,32);
	 DB_DEFINE_PRINTF_VECI(4,u,32);

#endif /* DEBUG_PRINTF_H_ */
