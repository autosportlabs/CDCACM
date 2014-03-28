/*
 *  Name:    main.c
 *
 *  Purpose: the main function
 *
 *  Created By:         Lipeng<runangaozhong@163.com>
 *  Created Date:       2013-12-26
 *
 *  ChangeList:
 *  Created in 2013-12-26 by Lipeng;
 *
 */
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "usart.h"
#include "core_cm4.h"
#include "stm32f4xx_conf.h"
#include "led.h"

#include "debug.h"

#include <FreeRTOS.h>
#include <task.h>

#define TASK_RUNNING_LED    (LED3)
#define CDC_READY_LED       (LED6)

USB_OTG_CORE_HANDLE         USB_OTG_Core;
USBH_HOST                   USB_Host;
volatile int                enum_done = 0;

uint8_t                     inbuf[40];

#ifdef CONFIG_ENABLE_DEBUG
static int debug = DEBUG_LEVEL_DEBUG;
#endif

static void main_task(void *params) {
    static int flag = 0;

	(void)params;

	const portTickType heartbeat_delay = 2000 / portTICK_RATE_MS;   /* 2s */

    led_init();
    uart_init(115200);

    led_on(TASK_RUNNING_LED);
    led_off(CDC_READY_LED);

	/* Initialize USB Host Library */
    DEBUG("Before USBH_Init\n");
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_CDC_cb, &USR_Callbacks);

    DEBUG("waiting for enum...\n");
	while (1) {
		USBH_Process(&USB_OTG_Core, &USB_Host);

		if (enum_done >= 1) {
            DEBUG("enum_done\n");
            DEBUG("lineCode status %d\n", USBH_CDC_GetLastLineCodeStatus());
			enum_done = 0;

		}

        
        if (USBH_CDC_isReady() == 1) {
			DEBUG("Ports detected %d\n", USBH_CDC_GetPortCount());

            led_on(CDC_READY_LED);
            /* we can do the following things
               in here, now just blink the LED */

            uint8_t port = 0;

        #if 0   /* no need send AT while do usb serial testing */
            DEBUG("send 'AT' on port %d...\n", port);
            int sendRes = USBH_CDC_SendData("AT\r\n", 4, port);
            DEBUG("send result = %d\n", sendRes);
        #endif
            
            memset((void *)inbuf, 0, 40);
            int receiveRes = USBH_CDC_ReceiveData(inbuf, 40, port);
            DEBUG("receive result = %d\n", receiveRes);

            if (receiveRes > 0){
				inbuf[receiveRes] = 0;
				DEBUG("%d received data:\n%s\n", receiveRes, inbuf);
            }

            if (flag == 0) {
                flag = 1;
                led_on(TASK_RUNNING_LED);
            } else {
                flag = 0;
                led_off(TASK_RUNNING_LED);
            }
		    vTaskDelay(heartbeat_delay);

        } else {
            led_on(TASK_RUNNING_LED);
            led_off(CDC_READY_LED);
        }

	}
}

void vApplicationTickHook(void) { }

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName ) {
    (void)pxTask;
    (void)pcTaskName;
}

/*========================================================
 *                  public functions
 *======================================================*/

/*
 * Main function. Called when startup code is done with
 * copying memory and setting up clocks.
 */
int main(void) {

	xTaskCreate(main_task,
		    (signed portCHAR *) "main task",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 4),
		    NULL);

	vTaskStartScheduler();
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {
}
