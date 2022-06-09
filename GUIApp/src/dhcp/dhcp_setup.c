#include <dhcp/dhcp_setup.h>
#include "main_thread.h"
#include "guiapp_event_handlers.h"

/* If semi hosting is defined, define the debug output
 * method using printf.
 */
#ifdef SEMI_HOSTING
#include "stdio.h"
#ifdef __GNUC__
extern void initialise_monitor_handles(void);
#endif
#endif


/* Define some global variables. */
static UINT error_counter = 0;
static UINT is_bound = NX_FALSE;

/* Declare the DHCP Client state change callback. */
static  VOID   my_notify_callback(NX_DHCP *dhcp_ptr, UCHAR new_state);

char ip[20];


static void update_text(GX_WIDGET * p_widget, GX_RESOURCE_ID id, char* text)
{
    GX_PROMPT * p_prompt = NULL;

    ssp_err_t err = (ssp_err_t)gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET**)&p_prompt);
    if (TX_SUCCESS == err)
    {
        gx_prompt_text_set(p_prompt, text);
    }
}

/*
 * This function runs a DHCP Client session.
 * client_ptr; pointer to an NX_DHCP instance, an already created DHCP Client instance
 * ip_ptr; pointer to an NX_IP instance, an already created IP instance
 * If successful return NX_SUCCESS (0x0); else return DHCP_RUN_ERRORS error status.
 */
UINT run_dhcp_client_session(NX_DHCP *client_ptr, NX_IP *ip_ptr)
{

    UINT        status;
    NX_PACKET   *my_packet;
    ULONG       server_address;
    ULONG       client_address;
    ULONG       network_mask;
    UINT        wait = 0;

          /* Initialize the debug output utility. */
    #ifdef SEMI_HOSTING
    #ifdef __GNUC__
        initialise_monitor_handles();
    #endif
    #endif

    /* Register a DHCP state change callback function. */
    status = nx_dhcp_state_change_notify(client_ptr, my_notify_callback);

    if (status)
    {
        #ifdef SEMI_HOSTING
                if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                {
                    printf("Failed to register DHCP chance callback notification %x\n", status);
                }
        #endif
        error_counter++;
    }
    /* Now we're ready to start the DHCP Client.  */
    status =  nx_dhcp_start(client_ptr);

     /* Check the status. */
    if (status)
    {

        error_counter++;

        #ifdef SEMI_HOSTING
                if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                {
                    /* Debugger is connected */
                    printf("Aborting DHCP Client. Failed DHCP start 0x%x\n", status);
                }
        #endif
        /* Internal DHCP error or NetX internal error. We cannot continue this test. */
        nx_dhcp_delete(client_ptr);

        return DHCP_START_ERROR;
    }

    #ifdef SEMI_HOSTING
            if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
            {
                /* Debugger is connected */
                printf("The DHCPv4 Client is running\n");
            }
    #endif
    /* Wait for the flag that the Client is Bound is set. */
    wait = 0;

    while(wait < WAIT_TO_BE_BOUND)
    {

        /* If the is_bound flag is set, we have a valid IP address */
        if (is_bound == NX_TRUE)
            break;

        /* Not bound yet. Let other application threads run. */
        tx_thread_sleep(100);

        /* Update how long we've waited */
        wait += 100;
    }

    /* Check if Client is bound to an IP address. */
    if (is_bound)
    {

        #ifdef SEMI_HOSTING
                if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                {
                    /* Debugger is connected */
                    printf("DHCP Client is assigned an IP address lease.\n");
                }
        #endif

        /* It is. Get the client IP address from this NetX service. */
        status = nx_ip_address_get(ip_ptr, &client_address, &network_mask);

        /* Check for errors. */
        if (status)
        {    error_counter++;
            #ifdef SEMI_HOSTING
                        if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                        {
                            /* Debugger is connected */
                            printf("failed to get client address \n");
                        }
            #endif
        }
        else
        {
            #ifdef SEMI_HOSTING
                        if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                        {
                            unsigned int a = ((unsigned int)client_address>>24);
                            unsigned int b = (((unsigned int)client_address>>16)&0xff);
                            unsigned int c = ((( unsigned int)client_address>>8)&0xff);
                            unsigned int d = (((unsigned int)client_address)&0xff);
                            sprintf(ip, "IP: %d.%d.%d.%d", a, b, c, d);
                            setIP(ip);
                            /* Debugger is connected */
                            printf("DHCP Client address is %d.%d.%d.%d \n", a, b, c, d);
                        }
            #endif
        }

        /* Get the DHCP Server IP address.  */
        status = nx_dhcp_server_address_get(client_ptr, &server_address);

        /* Check for errors. */
        if (status)
        {
            error_counter++;
            #ifdef SEMI_HOSTING
                        if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                        {
                            /* Debugger is connected */
                            printf("failed to get server address \n");
                        }
            #endif
        }
        else
        {
            #ifdef SEMI_HOSTING
                if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                {
                    /* Debugger is connected */
                    printf("DHCP server's address is %d.%d.%d.%d \n", ((unsigned int)server_address>>24),(((unsigned int)server_address>>16)&0xff),
                           ((( unsigned int)server_address>>8)&0xff),(((unsigned int)server_address)&0xff));
                }
            #endif
            /* Check that the device is able to send and receive packets with this IP address. */
            status =  nx_icmp_ping(ip_ptr, server_address, "Hello World", sizeof("Hello World"), &my_packet, 3* NX_IP_PERIODIC_RATE);
            /* Check status. */
            if (status)
            {
                /* No valid ICMP packet received (no packet to release). Update the error counter. */
                error_counter++;
                #ifdef SEMI_HOSTING
                    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                    {
                        /* Debugger is connected */
                        printf("Failed to ping Server.\n");
                    }
                #endif
            }
            else
            {
#ifdef SEMI_HOSTING
                if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                {
                    /* Debugger is connected */
                    printf("Successfully pinged Server. \n");
                }
#endif
                /* Release the echo response packet when we are done with it. */
                nx_packet_release(my_packet);
            }
        }
    }
}

/*
 * This function defines a user callback for DHCP Client to notify
 * when there is a DHCP state change.
 * NX_DHCP *client_ptr; previously created DHCP Client instance
 * UCHAR state; 2 byte numeric representation of DHCP state
 *
 * In this callback, we only check if the DHCP
 * Client has changed to the bound state (has a valid IP
 * address) and we set a flag for the application to check.
 */
VOID my_notify_callback(NX_DHCP *dhcp_ptr, UCHAR state)
{

    UINT new_state = (UINT)state;

    NX_PARAMETER_NOT_USED(dhcp_ptr);

    /* Check if we have transitioned to the bound state
       (have a valid IP address). */
    if (new_state == NX_DHCP_STATE_BOUND)
    {
        /* We have. Set the flag for the application. */
        is_bound = NX_TRUE;
    }
}

char* getIpText(){
    return ip;
}




