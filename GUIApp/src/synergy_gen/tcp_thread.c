/* generated thread source file - do not edit */
#include "tcp_thread.h"

TX_THREAD tcp_thread;
void tcp_thread_create(void);
static void tcp_thread_func(ULONG thread_input);
static uint8_t tcp_thread_stack[2048] BSP_PLACE_IN_SECTION_V2(".stack.tcp_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if defined(__ICCARM__)
            #define g_dhcp_client0_err_callback_WEAK_ATTRIBUTE
            #pragma weak g_dhcp_client0_err_callback  = g_dhcp_client0_err_callback_internal
            #elif defined(__GNUC__)
            #define g_dhcp_client0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_dhcp_client0_err_callback_internal")))
            #endif
void g_dhcp_client0_err_callback(void *p_instance, void *p_data)
g_dhcp_client0_err_callback_WEAK_ATTRIBUTE;
NX_DHCP g_dhcp_client0;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_dhcp_client0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_dhcp_client0_err_callback_internal(void *p_instance, void *p_data);
void g_dhcp_client0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.

 *            - void dhcp_client_init0(void)
 **********************************************************************************************************************/
void dhcp_client_init0(void)
{
    UINT g_dhcp_client0_err;
    /* Create DHCP client. */
    g_dhcp_client0_err = nx_dhcp_create (&g_dhcp_client0, &g_ip0, "g_dhcp_client0_DHCP");
    if (NX_SUCCESS != g_dhcp_client0_err)
    {
        g_dhcp_client0_err_callback ((void*) &g_dhcp_client0, &g_dhcp_client0_err);
    }

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0
                /* Set callback function to add user options.  */
                g_dhcp_client0_err =  nx_dhcp_user_option_add_callback_set(&g_dhcp_client0,
                                                                                             dhcp_user_option_add_client0);

                if (NX_SUCCESS != g_dhcp_client0_err)
                {
                    g_dhcp_client0_err_callback((void *)&g_dhcp_client0,&g_dhcp_client0_err);
                }
                #endif
}

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0 && DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0
            UCHAR g_dhcp_client0_opt_num = 60;
            CHAR *g_dhcp_client0_opt_val = "REA";

            /*******************************************************************************************************************//**
             * @brief     This DHCP user options add function adds Vendor Class ID. User can change the option number and/or option
             *            value in the user defined code by simply overriding the values. This function work fine to add any option
             *            which takes string values for option. Like below
             *            g_dhcp_client0_opt_num = 43;
             *            g_dhcp_client0_opt_val = "OPT43VAL";
             *            If user wants to chain the options, should disable this function generation and provide their own definition.
             **********************************************************************************************************************/
            UINT dhcp_user_option_add_client0(NX_DHCP *dhcp_ptr, UINT iface_index, UINT message_type, UCHAR *user_option_ptr, UINT *user_option_length)
            {
                NX_PARAMETER_NOT_USED(dhcp_ptr);
                NX_PARAMETER_NOT_USED(iface_index);
                NX_PARAMETER_NOT_USED(message_type);

                /* Application can check if add options into the packet by iface_index and message_type.
                   message_type are defined in header file, such as: NX_DHCP_TYPE_DHCPDISCOVER.  */
                /* Add Vendor Class ID option refer to RFC2132.  */

                /* Check if have enough space for this option.  */
                if (*user_option_length < (strlen(g_dhcp_client0_opt_val) + 2))
                {
                    return(NX_FALSE);
                }

                /* Set the option code.  */
                *user_option_ptr = g_dhcp_client0_opt_num;

                /* Set the option length.  */
                *(user_option_ptr + 1) = (UCHAR)strlen(g_dhcp_client0_opt_val);

                /* Set the option value (Vendor class id).  */
                memcpy((user_option_ptr + 2), g_dhcp_client0_opt_val, strlen(g_dhcp_client0_opt_val));

                /* Update the option length.  */
                *user_option_length = (strlen(g_dhcp_client0_opt_val) + 2);

                return(NX_TRUE);
            }
            #endif
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void tcp_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&tcp_thread, (CHAR*) "TCP Thread", tcp_thread_func, (ULONG) NULL, &tcp_thread_stack, 2048,
                            1, 1, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&tcp_thread, 0);
    }
}

static void tcp_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */
    /** Call initialization function if user has selected to do so. */
#if (1)
    dhcp_client_init0 ();
#endif

    /* Enter user code for this thread. */
    tcp_thread_entry ();
}
