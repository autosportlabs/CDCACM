/*******************************************************************************
 * @file    usbh_cdc_core.c
 * @author  Lipeng
 * @version V0.0.1
 * @date    2013-12-25
 * @brief   This file implements the CDC class driver functions
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "usbh_cdc_core.h"
#include "usbh_core.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined (__ICCARM__)                  /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN CDC_Machine_TypeDef           CDC_Machine __ALIGN_END;

uint8_t                                     CDC_port_num = 0;
uint8_t                                     set_line_code_done = 0;
USBH_Status									lineCodeStatus = 0;

/* 
 * USBH_CDC_CORE_Private_FunctionPrototypes
 */ 
static USBH_Status USBH_CDC_InterfaceInit(USB_OTG_CORE_HANDLE *pdev, void *phost);
static void USBH_CDC_InterfaceDeInit(USB_OTG_CORE_HANDLE *pdev, void *phost);
static USBH_Status USBH_CDC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , void *phost);
static USBH_Status USBH_CDC_Handle(USB_OTG_CORE_HANDLE *pdev, void *phost);


USBH_Class_cb_TypeDef USBH_CDC_cb = {
    USBH_CDC_InterfaceInit,
    USBH_CDC_InterfaceDeInit,
    USBH_CDC_ClassRequest,
    USBH_CDC_Handle,
};


/*========================================================
 * USBH_CDC_CORE_Private_Functions
 *======================================================*/ 

static USBH_Status USBH_SET_LINE_CODING(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost) {

    //uint8_t baud19200[7] = {0x00,0x4B,0x00,0x00,0x00,0x00,0x08};  /* 19200baud, 8bits, 1stop-bit, no parity-bit */
    uint8_t baud115200[7] = {0x04,0x51,0x13,0x01,0x00,0x00,0x08};  /* 19200baud, 8bits, 1stop-bit, no parity-bit */
   
    phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS |
                                           USB_REQ_RECIPIENT_INTERFACE;
     
    phost->Control.setup.b.bRequest = 0x20;
    phost->Control.setup.b.wValue.w = 0;
    phost->Control.setup.b.wIndex.w = 0;
    phost->Control.setup.b.wLength.w = 0x88;          
     
    return USBH_CtlReq(pdev, phost, baud115200, 7);
}

/**
  * @brief  USBH_CDC_InterfaceInit 
  *         Interface initialization for CDC class.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status : Status of class request handled.
  */
static USBH_Status USBH_CDC_InterfaceInit(USB_OTG_CORE_HANDLE *pdev, void *phost) {	 
    volatile uint8_t i_num = 0, ep_num = 0;  /* interface and endpoint number */
    USBH_HOST *pphost = phost;

    CDC_port_num = 0;
    for (i_num = 0; i_num < pphost->device_prop.Cfg_Desc.bNumInterfaces; i_num++) {
        if (pphost->device_prop.Itf_Desc[i_num].bInterfaceClass == CDC_DATA_CLASS
            || pphost->device_prop.Itf_Desc[i_num].bInterfaceClass == CUSTOM_CLASS) {
            /* CDC data class, or custom class */

            for (ep_num = 0; ep_num < pphost->device_prop.Itf_Desc[i_num].bNumEndpoints - 1;) {
                if (pphost->device_prop.Ep_Desc[i_num][ep_num].bmAttributes == USB_EP_TYPE_BULK
                    && pphost->device_prop.Ep_Desc[i_num][ep_num + 1].bmAttributes == USB_EP_TYPE_BULK
                    && (pphost->device_prop.Ep_Desc[i_num][ep_num].bEndpointAddress & 0x80)
                        != (pphost->device_prop.Ep_Desc[i_num][ep_num + 1].bEndpointAddress & 0x80)) {
                    /* the interface has 2 bulk endpoint, which has different direction */

                    if (pphost->device_prop.Ep_Desc[i_num][ep_num].bEndpointAddress & 0x80) {
                        /* in endpoint */
                        CDC_Machine.CDCBulkInEp[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num].bEndpointAddress;
                        CDC_Machine.CDCBulkInEpSize[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num].wMaxPacketSize;
                    } else {
                        /* out endpoint */
                        CDC_Machine.CDCBulkOutEp[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num].bEndpointAddress;
                        CDC_Machine.CDCBulkOutEpSize[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num].wMaxPacketSize;
                    }
        
                    if (pphost->device_prop.Ep_Desc[i_num][ep_num + 1].bEndpointAddress & 0x80) {
                        /* in endpoint */
                        CDC_Machine.CDCBulkInEp[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num + 1].bEndpointAddress;
                        CDC_Machine.CDCBulkInEpSize[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num + 1].wMaxPacketSize;
                    } else {
                        /* out endpoint */
                        CDC_Machine.CDCBulkOutEp[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num + 1].bEndpointAddress;
                        CDC_Machine.CDCBulkOutEpSize[CDC_port_num]
                            = pphost->device_prop.Ep_Desc[i_num][ep_num + 1].wMaxPacketSize;
                    }
 

                    CDC_Machine.hc_num_out[CDC_port_num]
                        = USBH_Alloc_Channel(pdev, CDC_Machine.CDCBulkOutEp[CDC_port_num]);
                    CDC_Machine.hc_num_in[CDC_port_num]
                        = USBH_Alloc_Channel(pdev, CDC_Machine.CDCBulkInEp[CDC_port_num]);  
        
                    /* Open the new channels */
                    USBH_Open_Channel(pdev,
                                      CDC_Machine.hc_num_out[CDC_port_num],
                                      pphost->device_prop.address,
                                      pphost->device_prop.speed,
                                      EP_TYPE_BULK,
                                      CDC_Machine.CDCBulkOutEpSize[CDC_port_num]);
        
                    USBH_Open_Channel(pdev,
                                      CDC_Machine.hc_num_in[CDC_port_num],
                                      pphost->device_prop.address,
                                      pphost->device_prop.speed,
                                      EP_TYPE_BULK,
                                      CDC_Machine.CDCBulkInEpSize[CDC_port_num]);    

                    CDC_port_num++;
                    ep_num += 2;
                } else {
                    ep_num++;
                }
            }
        }
    }

    if (CDC_port_num == 0) {
        pphost->usr_cb->DeviceNotSupported(); 
    } else {
        /* save pdev pointer */
        CDC_Machine.pdev = pdev;
    }
  
    return USBH_OK ;
}


/**
  * @brief  USBH_CDC_InterfaceDeInit 
  *         De-Initialize interface by freeing host channels allocated to interface
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval None
  */
static void USBH_CDC_InterfaceDeInit(USB_OTG_CORE_HANDLE *pdev, void *phost) {	
    int port;

    for (port = 0; port < CDC_port_num; port++) {
        if (CDC_Machine.hc_num_out[port]) {
            USB_OTG_HC_Halt(pdev, CDC_Machine.hc_num_out[port]);
            USBH_Free_Channel(pdev, CDC_Machine.hc_num_out[port]);
            CDC_Machine.hc_num_out[port] = 0;   /* Reset the Channel as Free */
        }
   
        if (CDC_Machine.hc_num_in[port]) {
            USB_OTG_HC_Halt(pdev, CDC_Machine.hc_num_in[port]);
            USBH_Free_Channel(pdev, CDC_Machine.hc_num_in[port]);
            CDC_Machine.hc_num_in[port] = 0;    /* Reset the Channel as Free */
        } 
    }

    CDC_port_num = 0;
    set_line_code_done = 0;
    CDC_Machine.pdev = 0;    
}

/**
  * @brief  USBH_CDC_ClassRequest 
  *         This function will only initialize the CDC state machine
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status : Status of class request handled.
  */
static USBH_Status USBH_CDC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , void *phost) {   
    return USBH_OK; 
}


/**
  * @brief  USBH_CDC_Handle 
  *         CDC state machine handler 
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */

static USBH_Status USBH_CDC_Handle(USB_OTG_CORE_HANDLE *pdev , void *phost) {
    if (set_line_code_done == 0) {
        if (USBH_SET_LINE_CODING(pdev, phost) == USBH_OK) {
            set_line_code_done = 1;
        }
    }

    return USBH_OK; 
}


/*========================================================
 * USBH_CDC_CORE_external_Functions
 *======================================================*/ 
uint8_t USBH_CDC_isReady(void) {
    if (CDC_port_num > 0 && set_line_code_done == 1) {
        return 1;
    } else {
        return 0;
    }
}

int USBH_CDC_SendData(uint8_t *data, int len, uint8_t port) {
    USB_OTG_CORE_HANDLE *pdev = CDC_Machine.pdev;

    if (port >= CDC_port_num) return -1;
    if ( pdev == 0 ) return -2;
    if (set_line_code_done == 0) return -3;
    if (data == 0) return -4;
    if (len == 0) return -5;
    if (!HCD_IsDeviceConnected(pdev)) return -10;         /* device is not connected */

    /* wait for URB Ready */
    while (HCD_GetURB_State(pdev, CDC_Machine.hc_num_out[port]) == URB_NOTREADY
           && HCD_IsDeviceConnected(pdev));

    USBH_BulkSendData(pdev, data, len, CDC_Machine.hc_num_out[port]);

    /* wait for URB done */
    while (HCD_GetURB_State(pdev, CDC_Machine.hc_num_out[port]) != URB_DONE
           && HCD_IsDeviceConnected(pdev));

    return HCD_GetXferCnt(pdev, CDC_Machine.hc_num_out[port]); 
}

int USBH_CDC_ReceiveData(uint8_t *data, int len, uint8_t port) {
    USB_OTG_CORE_HANDLE *pdev = CDC_Machine.pdev;

    if (port >= CDC_port_num) return -1;
    if (pdev == 0) return -2;
    if (set_line_code_done == 0) return -3;
    if (data == 0) return -4;
    if (len == 0) return -5;
    if (!HCD_IsDeviceConnected(pdev)) return -10;

    /* wait for URB Ready */
    while (HCD_GetURB_State(pdev, CDC_Machine.hc_num_in[port]) == URB_NOTREADY
           && HCD_IsDeviceConnected(pdev));

    USBH_BulkReceiveData(pdev, data, len, CDC_Machine.hc_num_in[port]);

    /* wait for URB Done */
    while (HCD_GetURB_State(pdev, CDC_Machine.hc_num_in[port]) != URB_DONE
           && HCD_IsDeviceConnected(pdev));

    return HCD_GetXferCnt(pdev, CDC_Machine.hc_num_in[port]); 
}

int USBH_CDC_GetPortCount(){
	return CDC_port_num;
}


