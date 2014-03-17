/*******************************************************************************
 * @file    usbh_cdc_core.c
 * @author  Lipeng
 * @version V0.0.1
 * @date    2013-12-25
 * @brief   This file implements the CDC class driver functions
 *
 ******************************************************************************/

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_CDC_CORE_H
#define __USBH_CDC_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_stdreq.h"
#include "usb_bsp.h"
#include "usbh_ioreq.h"
#include "usbh_hcs.h"

/* Structure for CDC process */
typedef struct _CDC_Process {
    USB_OTG_CORE_HANDLE         *pdev;

    uint8_t                     CDCBulkOutEp[USBH_MAX_NUM_INTERFACES];
    uint8_t                     CDCBulkOutEpSize[USBH_MAX_NUM_INTERFACES];

    uint8_t                     CDCBulkInEp[USBH_MAX_NUM_INTERFACES];
    uint8_t                     CDCBulkInEpSize[USBH_MAX_NUM_INTERFACES];

    uint8_t                     hc_num_in[USBH_MAX_NUM_INTERFACES]; 
    uint8_t                     hc_num_out[USBH_MAX_NUM_INTERFACES]; 
} CDC_Machine_TypeDef; 


/* USBH_MSC_CORE_Exported_Variables */ 
extern USBH_Class_cb_TypeDef    USBH_CDC_cb;
extern CDC_Machine_TypeDef      CDC_Machine;
extern uint8_t                  CDC_port_num;

uint8_t USBH_CDC_isReady(void);
int USBH_CDC_SendData(uint8_t *data, int len, uint8_t port);
int USBH_CDC_ReceiveData(uint8_t *data, int len, uint8_t port);
int USBH_CDC_GetPortCount();
USBH_Status USBH_CDC_GetLastLineCodeStatus();

#endif  /* __USBH_CDC_CORE_H */
