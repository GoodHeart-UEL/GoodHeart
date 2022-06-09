/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "fx_api.h"
#include "ux_api.h"
#include "ux_host_class_storage.h"
#include "nx_api.h"
#include "sf_el_nx_cfg.h"
#include "../src/framework/sf_el_nx/nx_renesas_synergy.h"
#include "nx_api.h"

#include "sf_message.h"
#include "sf_message_payloads.h"

#include "r_jpeg_decode.h"
#include "sf_jpeg_decode.h"
#include "r_glcd.h"
#include "r_display_api.h"
#include "sf_el_gx.h"
#include "r_fmi.h"
#include "r_fmi_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "ux_api.h"
#include "ux_dcd_synergy.h"
#include "sf_el_ux_dcd_fs_cfg.h"
void g_sf_el_ux_dcd_fs_err_callback(void *p_instance, void *p_data);
void fx_common_init0(void);
#include "ux_api.h"

/* USBX Host Stack initialization error callback function. User can override the function if needed. */
void ux_v2_err_callback(void *p_instance, void *p_data);

#if !defined(ux_host_event_user)
/* User Callback for Host Event Notification (Only valid for USB Host). */
extern UINT ux_host_event_user(ULONG event, UX_HOST_CLASS *host_class, VOID *instance);
#endif

#if !defined(NULL)
/* User Callback for Device Event Notification (Only valid for USB Device). */
extern UINT NULL(ULONG event);
#endif

#ifdef UX_HOST_CLASS_STORAGE_H
            /* Utility function to get the pointer to a FileX Media Control Block for a USB Mass Storage device. */
            UINT ux_system_host_storage_fx_media_get(UX_HOST_CLASS_STORAGE * instance, UX_HOST_CLASS_STORAGE_MEDIA ** p_storage_media, FX_MEDIA ** p_fx_media);
#endif
void ux_common_init(void);
#include "ux_api.h"
#include "ux_dcd_synergy.h"

/* USBX Device Stack initialization error callback function. User can override the function if needed. */
void ux_device_err_callback(void *p_instance, void *p_data);
void ux_device_init(void);
void ux_device_remove_compiler_padding(unsigned char *p_device_framework, UINT length);
#include "ux_api.h"
#include "ux_device_class_storage.h"
/* USBX Mass Storage Class User Media Setup Callback Function */
extern void ux_msd_setup(UX_SLAVE_CLASS_STORAGE_PARAMETER *param);
/* USBX Mass Storage Class Media Read User Callback Function */
extern UINT ux_msd_read(VOID *storage, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba,
        ULONG *media_status);
/* USBX Mass Storage Class Media Write User Callback Function */
extern UINT ux_msd_write(VOID *storage, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba,
        ULONG *media_status);
/* USBX Mass Storage Class Media Status User Callback Function */
extern UINT ux_msd_status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status);
/* USBX Mass Storage Class Media Activate/Deactivate Callback Function */
#ifndef NULL
extern VOID NULL(VOID *storage);
#endif
#ifndef NULL
extern VOID NULL(VOID *storage);
#endif
/* USBX Mass Storage Class Parameter Setup Function */
void ux_msd_init(void);
void g_ux_msd_err_callback(void *p_instance, void *p_data);
#include "ux_api.h"
#include "ux_hcd_synergy.h"
#include "sf_el_ux_hcd_hs_cfg.h"
void g_ux_host_err_callback(void *p_instance, void *p_data);
void ux_host_init0(void);
#include "ux_api.h"
#include "ux_host_class_storage.h"

/* Pointer to a USBX Host Mass Storage Class Instance */
extern UX_HOST_CLASS_STORAGE *g_ux_host_class_storage;
/* A pointer to FileX Media Control Block for a USB flash device */
extern FX_MEDIA *g_fx_media_ptr;

/* Pointer to a USBX Host Mass Storage Class Media */
extern UX_HOST_CLASS_STORAGE_MEDIA *g_ux_host_class_storage_media;

/* Macro to dereference a pointer to FileX Media Control Block for a USB flash device */
#define  g_fx_media (*(FX_MEDIA*)g_fx_media_ptr)

/* The function prototype of getting g_fx_media for a USB Mass Storage device. */
void fx_media_init_function(void);

/* FileX media initialization error callback function for USBX Host Mass Storage. User can override the function if needed. */
void g_fx_media_err_callback_failed_to_get_fx_media(void *p_instance, void *p_data);
#ifndef NULL
void NULL(nx_mac_address_t *p_mac_config);
#endif
#ifndef NULL
void NULL(NX_PACKET *packet_ptr, USHORT packet_type);
#endif
VOID nx_ether_driver_eth1(NX_IP_DRIVER *driver_req_ptr);
extern VOID (*g_sf_el_nx)(NX_IP_DRIVER *driver_req_ptr);
void nx_common_init0(void);
extern NX_PACKET_POOL g_packet_pool0;
void g_packet_pool0_err_callback(void *p_instance, void *p_data);
void packet_pool_init0(void);
#define g_ip0_PACKET_POOL g_packet_pool0
extern NX_IP g_ip0;
void g_ip0_err_callback(void *p_instance, void *p_data);
void ip_init0(void);
extern void g_message_init(void);

/* SF Message on SF Message Instance. */
extern const sf_message_instance_t g_sf_message0;
void g_sf_message0_err_callback(void *p_instance, void *p_data);
void sf_message_init0(void);

extern const jpeg_decode_instance_t g_jpeg_decode0;
extern const jpeg_decode_cfg_t g_jpeg_decode0_cfg;
#ifndef NULL
void NULL(jpeg_decode_callback_args_t *p_args);
#endif
extern const sf_jpeg_decode_instance_t g_sf_jpeg_decode0;
/* Display on GLCD Instance. */
extern const display_instance_t g_display;
extern display_runtime_cfg_t g_display_runtime_cfg_fg;
extern display_runtime_cfg_t g_display_runtime_cfg_bg;
#if (false)
            extern display_clut_cfg_t g_display_clut_cfg_glcd;
            #endif
#ifndef NULL
void NULL(display_callback_args_t *p_args);
#endif
#if (true)
            #define DISPLAY_IN_FORMAT_16BITS_RGB565_0
            #if defined (DISPLAY_IN_FORMAT_32BITS_RGB888_0) || defined (DISPLAY_IN_FORMAT_32BITS_ARGB8888_0)
            #define DISPLAY_BITS_PER_PIXEL_INPUT0 (32)
            #elif defined (DISPLAY_IN_FORMAT_16BITS_RGB565_0) || defined (DISPLAY_IN_FORMAT_16BITS_ARGB1555_0) || defined (DISPLAY_IN_FORMAT_16BITS_ARGB4444_0)
            #define DISPLAY_BITS_PER_PIXEL_INPUT0 (16)
            #elif defined (DISPLAY_IN_FORMAT_CLUT8_0)
            #define DISPLAY_BITS_PER_PIXEL_INPUT0 (8)
            #elif defined (DISPLAY_IN_FORMAT_CLUT4_0)
            #define DISPLAY_BITS_PER_PIXEL_INPUT0 (4)
            #else
            #define DISPLAY_BITS_PER_PIXEL_INPUT0 (1)
            #endif
            extern uint8_t g_display_fb_background[2][((256 * 320) * DISPLAY_BITS_PER_PIXEL_INPUT0) >> 3];
            #endif
#if (false)
            #define DISPLAY_IN_FORMAT_16BITS_RGB565_1
            #if defined (DISPLAY_IN_FORMAT_32BITS_RGB888_1) || defined (DISPLAY_IN_FORMAT_32BITS_ARGB8888_1)
            #define DISPLAY_BITS_PER_PIXEL_INPUT1 (32)
            #elif defined (DISPLAY_IN_FORMAT_16BITS_RGB565_1) || defined (DISPLAY_IN_FORMAT_16BITS_ARGB1555_1) || defined (DISPLAY_IN_FORMAT_16BITS_ARGB4444_1)
            #define DISPLAY_BITS_PER_PIXEL_INPUT1 (16)
            #elif defined (DISPLAY_IN_FORMAT_CLUT8_1)
            #define DISPLAY_BITS_PER_PIXEL_INPUT1 (8)
            #elif defined (DISPLAY_IN_FORMAT_CLUT4_1)
            #define DISPLAY_BITS_PER_PIXEL_INPUT1 (4)
            #else
            #define DISPLAY_BITS_PER_PIXEL_INPUT1 (1)
            #endif
            extern uint8_t g_display_fb_foreground[2][((800 * 480) * DISPLAY_BITS_PER_PIXEL_INPUT1) >> 3];
            #endif
#ifndef NULL
extern void NULL(sf_el_gx_callback_args_t *p_arg);
#endif
extern sf_el_gx_instance_t g_sf_el_gx;
/** FMI on FMI Instance. */
extern const fmi_instance_t g_fmi;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
