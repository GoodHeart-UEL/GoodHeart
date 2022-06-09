/* generated thread header file - do not edit */
#ifndef USB_THREAD1_H_
#define USB_THREAD1_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void usb_thread1_entry(void);
                #else
extern void usb_thread1_entry(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern TX_SEMAPHORE g_usb_media_sema;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* USB_THREAD1_H_ */
