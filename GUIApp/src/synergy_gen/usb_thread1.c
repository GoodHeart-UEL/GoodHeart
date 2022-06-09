/* generated thread source file - do not edit */
#include "usb_thread1.h"

TX_THREAD usb_thread1;
void usb_thread1_create(void);
static void usb_thread1_func(ULONG thread_input);
static uint8_t usb_thread1_stack[1536] BSP_PLACE_IN_SECTION_V2(".stack.usb_thread1") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_SEMAPHORE g_usb_media_sema;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void usb_thread1_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_usb_media_sema;
    err_g_usb_media_sema = tx_semaphore_create (&g_usb_media_sema, (CHAR*) "New Semaphore", 0);
    if (TX_SUCCESS != err_g_usb_media_sema)
    {
        tx_startup_err_callback (&g_usb_media_sema, 0);
    }

    UINT err;
    err = tx_thread_create (&usb_thread1, (CHAR*) "USB Thread", usb_thread1_func, (ULONG) NULL, &usb_thread1_stack,
                            1536, 5, 5, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&usb_thread1, 0);
    }
}

static void usb_thread1_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    usb_thread1_entry ();
}
