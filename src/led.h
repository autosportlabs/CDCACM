/*
 *  Name:    led.h
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

#ifndef _LED_H_
#define _LED_H_

enum led_t {
    LED3,
    LED4,
    LED5,
    LED6,
};

void led_init(void);
void led_on(enum led_t led);
void led_off(enum led_t led);

#endif
