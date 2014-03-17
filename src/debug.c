/*
 *  debug system
 *
 *  Copyright (C) 2013 Lipeng<runangaozhong@163.com>
 *
 */

#include "debug.h"
#include <stdio.h>
#include <stdarg.h> 

#ifdef CONFIG_ENABLE_DEBUG


#ifdef CONFIG_DEBUG_USE_UART
/* UART is the primary debug method */
#include "usart.h"

#elif CONFIG_DEBUG_USE_ITM
#include "main.h"
#include "core_cm4.h"
#endif


#define LOG_BUFFER_LENGTH           1024 
static char                         log_buffer[LOG_BUFFER_LENGTH];


/*
 * put a string
 * I re-define the puts, and will cause printf
 * to the real debug method
 */
int puts(const char *s) {
    int count = 0;

    do {
    #ifdef CONFIG_DEBUG_USE_UART
        uart_putchar(*s);
        if (*s == '\n') {
            uart_putchar('\r');
        }
    #elif CONFIG_DEBUG_USE_ITM
        ITM_SendChar(*s);
    #endif

        count++;
        s++;
    } while (*s);

    return count;
}

void PRINT(const char *fmt, ...) {
    va_list args;
    
    va_start(args, fmt);
    vsnprintf(log_buffer, LOG_BUFFER_LENGTH, fmt, args);
    va_end(args);

    /* print to console */
    puts(log_buffer);
}
#else
void PRINT(const char *fmt, ...) { }
#endif
