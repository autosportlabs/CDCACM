/*
 *  Name:    led.c
 *
 *  Purpose: the LED driver for STM32F4
 *
 *  Created By:         Lipeng<runangaozhong@163.com>
 *  Created Date:       2013-10-15
 *
 *  ChangeList:
 *  Created in 2013-10-15 by Lipeng;
 *
 *  This document and the information contained in it is confidential and 
 *  proprietary to Unication Co., Ltd. The reproduction or disclosure, in 
 *  whole or in part, to anyone outside of Unication Co., Ltd. without the 
 *  written approval of the President of Unication Co., Ltd., under a 
 *  Non-Disclosure Agreement, or to any employee of Unication Co., Ltd. who 
 *  has not previously obtained written authorization for access from the 
 *  individual responsible for the document, will have a significant 
 *  detrimental effect on Unication Co., Ltd. and is expressly prohibited. 
 *  
 */

#include "main.h"
#include "stm32f4xx_conf.h"
#include "led.h"

void led_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOD Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* 
     * Configure the gpios used by LED,
     * PD12, PD13, PD14 and PD15 in output pushpull mode
     */
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* make sure all led is off */
    led_off(LED3);
    led_off(LED4);
    led_off(LED5);
    led_off(LED6);
}

void led_on(enum led_t led) {
    switch (led) {
        case LED3:
            GPIO_SetBits(GPIOD, GPIO_Pin_13);
            break;
        case LED4:
            GPIO_SetBits(GPIOD, GPIO_Pin_12);
            break;
        case LED5:
            GPIO_SetBits(GPIOD, GPIO_Pin_14);
            break;
        case LED6:
            GPIO_SetBits(GPIOD, GPIO_Pin_15);
            break;
        default:
            break;
    }
}

void led_off(enum led_t led) {
    switch (led) {
        case LED3:
            GPIO_ResetBits(GPIOD, GPIO_Pin_13);
            break;
        case LED4:
            GPIO_ResetBits(GPIOD, GPIO_Pin_12);
            break;
        case LED5:
            GPIO_ResetBits(GPIOD, GPIO_Pin_14);
            break;
        case LED6:
            GPIO_ResetBits(GPIOD, GPIO_Pin_15);
            break;
        default:
            break;
    }
}
