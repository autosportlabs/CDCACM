/*
 *  Name:    usart.h
 *
 *  Purpose: the USART driver for STM32F4
 *
 *  Created By:         Lipeng<runangaozhong@163.com>
 *  Created Date:       2013-12-27
 *
 *  ChangeList:
 *  Created in 2013-12-27 by Lipeng;
 *
 */

#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>

void uart_init(uint32_t baud);
int uart_putchar(int c);
int uart_getchar(void);
int uart_puts(const char *s);

#endif /* __USART_H__ */
