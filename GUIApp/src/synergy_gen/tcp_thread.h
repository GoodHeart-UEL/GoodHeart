/* generated thread header file - do not edit */
#ifndef TCP_THREAD_H_
#define TCP_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void tcp_thread_entry(void);
                #else
extern void tcp_thread_entry(void);
#endif
#include "nx_dhcp.h"
#ifdef __cplusplus
extern "C" {
#endif
extern NX_DHCP g_dhcp_client0;
void g_dhcp_client0_err_callback(void *p_instance, void *p_data);
void dhcp_client_init0(void);

#define DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0 (0)
#define DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0 (1)

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0
            UINT dhcp_user_option_add_client0(NX_DHCP *dhcp_ptr, UINT iface_index, UINT message_type, UCHAR *user_option_ptr, UINT *user_option_length);
            #if DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0
            extern UCHAR g_dhcp_client0_opt_num;
            extern CHAR *g_dhcp_client0_opt_val;
            #endif
            #endif
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* TCP_THREAD_H_ */
