/*
 *  Name:    usart.c
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
#include "main.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"


#define USART_TX_BUF_LEN    (128)

/*=====================================================================
 *                  local definitions
 *===================================================================*/
static struct {
    xQueueHandle txq;
} uart;

/*=====================================================================
 *                   local static interface
 *===================================================================*/

/* 
 * UART GPIO initialize,
 * config PA2 to USART1 TX
 */
static void uart_io_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /* PA2->TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
           
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
}

/* 
 * UART Hardware initialize,
 * config USART to BAUD 115200, 8-bit, 1 stop bit
 * no parity and no flow control
 */
static void uart_hw_init(uint32_t baud) {
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    
    USART_Init(USART2, &USART_InitStructure);   

    USART_Cmd(USART2, ENABLE);
}

/* 
 * UART IRQ initialize
 */
static void uart_irq_init(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*
 * The main task of UART
 */
static void uart_task(void *params) {
    int c;

    for (;;) {
        if (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET) {
            if (pdPASS == xQueueReceive(uart.txq, &c, portMAX_DELAY)) {
                USART_SendData(USART2, c);
            }
        } else {
            vTaskDelay(10);
        }
    }
}

static void uart_task_init(void) {
    xTaskCreate(uart_task, (signed char *)"UART", configMINIMAL_STACK_SIZE,
                (void *)NULL, tskIDLE_PRIORITY + 5, NULL);
}

/*=====================================================================
 *                    interfaces provided to others
 *===================================================================*/

void uart_init(uint32_t baud) {
    uart.txq = xQueueCreate(USART_TX_BUF_LEN, sizeof(int));

    uart_io_init();
    uart_hw_init(baud);

#if 0
    uart_irq_init();
#else
    uart_task_init();
#endif
}

/*
 * put a char to UART
 */
int uart_putchar(int c) {
    portBASE_TYPE res;
    
    res = xQueueSendToBack(uart.txq, (const void *)&c, 0);
    
    if (res == pdPASS) {
        return c;
    } else {
        return EOF;
    }

#if 0
    /* enable TX intrrupt */
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
#endif
}

/*
 * put a string to UART
 */
int uart_puts(const char *s) {
    int count = 0;

    do {
        if (uart_putchar(*s) == EOF) {
            return -EOF;
        } else {
            count++;
        }
        s++;
    } while (*s);

    return count;
}

#if 0
void USART2_IRQHandler(void) {
    int c;

    if (USART_GetITStatus(USART2, USART_IT_TXE) == SET) {
        if (pdPASS == xQueueReceiveFromISR(uart.txq, &c, portMAX_DELAY)) {
            USART_SendData(USART2, c);
        } else {
            /* disable TX intrrupt */
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }
}
#endif


