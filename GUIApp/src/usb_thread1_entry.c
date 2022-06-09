#include "usb_thread1.h"

#define APP_PERFORM_DEMO (1)
#define APP_PROVIDE_MSD  (1)

#if (APP_PERFORM_DEMO)
/** USBX file control structure */
static FX_FILE g_file;
#endif

/* USB Thread entry function */
void usb_thread1_entry(void)
{
    UINT status;


    /* Signal that USBX is ready */
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_LOW);

    while (1)
    {
        status = tx_semaphore_get(&g_usb_media_sema, TX_WAIT_FOREVER);
        if (TX_SUCCESS != status)
        {
            __BKPT(0);
        }

        fx_media_init_function();

#if (APP_PROVIDE_MSD)
        ux_msd_init();
#endif

        /* Signal that USB device has been inserted */
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, IOPORT_LEVEL_LOW);

#if (APP_PERFORM_DEMO)

        status = fx_directory_name_test(g_fx_media_ptr, "arquivo.txt");
        if (FX_NOT_DIRECTORY == status)
        {
            status = fx_file_delete(g_fx_media_ptr, "arquivo.txt");
            if (FX_SUCCESS != status)
            {
                __BKPT(0);
            }

            status = fx_media_flush(g_fx_media_ptr);
            if (FX_SUCCESS != status)
            {
                __BKPT(0);
            }
        }
        else if (FX_NOT_FOUND != status)
        {
            if (FX_SUCCESS != status)
            {
                __BKPT(0);
            }
        }

        status = fx_file_create(g_fx_media_ptr, "arquivo.txt");
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }

        status = fx_file_open(g_fx_media_ptr, &g_file, "arquivo.txt", FX_OPEN_FOR_WRITE);
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }

        status = fx_file_write(&g_file, "Teste de escrita de arquivo na plaquinha\n", 100);
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }



        if(g_fx_media_ptr->fx_media_32_bit_FAT)
        {
            status = fx_file_write(&g_file, "File system is FAT32\r\n\r\n", 24);
            if (FX_SUCCESS != status)
            {
                __BKPT(0);
            }
        }
        else if(g_fx_media_ptr->fx_media_number_of_FATs - g_fx_media_ptr->fx_media_12_bit_FAT)
        {
            status = fx_file_write(&g_file, "File system is FAT16\r\n\r\n", 24);
            if (FX_SUCCESS != status)
            {
                __BKPT(0);
            }
        }
        else
        {
            status = fx_file_write(&g_file, "File System is FAT12\r\n\r\n", 24);
            if (FX_SUCCESS != status)
            {
                __BKPT(0);
            }
        }

        status = fx_file_write(&g_file, "Listing contents of root directory:\r\n", 37);
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }

        CHAR filename[FX_MAX_LONG_NAME_LEN];
        UINT attributes;

        status = fx_directory_first_full_entry_find(g_fx_media_ptr, filename, &attributes,
                                                        0, 0, 0, 0, 0, 0, 0);

        /* Filter out sub-directory dot entry */
        if (!strcmp((char *) filename, "."))
        {
            status = fx_directory_next_full_entry_find(g_fx_media_ptr, filename, &attributes,
                                                       0, 0, 0, 0, 0, 0, 0);
        }

        /* First pass: only directories are added to the list */
        while (!status)
        {
            if (attributes & FX_DIRECTORY)
            {
                status = fx_file_write(&g_file, "    ", 4);
                if (FX_SUCCESS != status)
                {
                    __BKPT(0);
                }

                status = fx_file_write(&g_file, filename, strlen(filename));
                if (FX_SUCCESS != status)
                {
                    __BKPT(0);
                }

                status = fx_file_write(&g_file, "\r\n", 2);
                if (FX_SUCCESS != status)
                {
                    __BKPT(0);
                }
            }

            status = fx_directory_next_full_entry_find(g_fx_media_ptr, filename, &attributes,
                                                       0, 0, 0, 0, 0, 0, 0);
        }

        status = fx_directory_first_full_entry_find(g_fx_media_ptr, filename, &attributes,
                                                    0, 0, 0, 0, 0, 0, 0);

        /* Second pass: volume name and directory entries are filtered out */
        while (!status)
        {
            if (!((attributes & FX_VOLUME) || (attributes & FX_DIRECTORY)))
            {
                status = fx_file_write(&g_file, "    ", 4);
                if (FX_SUCCESS != status)
                {
                    __BKPT(0);
                }

                status = fx_file_write(&g_file, filename, strlen(filename));
                if (FX_SUCCESS != status)
                {
                    __BKPT(0);
                }

                status = fx_file_write(&g_file, "\r\n", 2);
                if (FX_SUCCESS != status)
                {
                    __BKPT(0);
                }
            }

            status = fx_directory_next_full_entry_find(g_fx_media_ptr, filename, &attributes,
                                                       0, 0, 0, 0, 0, 0, 0);
        }

        status = fx_file_write(&g_file, "End of root directory", 21);
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }

        status = fx_file_close(&g_file);
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }

        status = fx_media_flush(g_fx_media_ptr);
        if (FX_SUCCESS != status)
        {
            __BKPT(0);
        }

#endif

        /* Signal that USB device is ready to be removed */
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, IOPORT_LEVEL_HIGH);
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, IOPORT_LEVEL_LOW);

        status = tx_semaphore_get(&g_usb_media_sema, TX_WAIT_FOREVER);
        if (TX_SUCCESS != status)
        {
            __BKPT(0);
        }

        g_fx_media_ptr = FX_NULL;

#if (APP_PROVIDE_MSD)
        ux_device_stack_class_unregister(_ux_system_slave_class_storage_name,
                                         ux_device_class_storage_entry);
#endif

        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, IOPORT_LEVEL_HIGH);
    }
}

UINT ux_host_event_user(ULONG event, UX_HOST_CLASS * p_class, VOID * p_instance)
{
    SSP_PARAMETER_NOT_USED(p_instance);

    UINT status = ux_utility_memory_compare(_ux_system_host_class_storage_name,
                                            p_class->ux_host_class_name,
                                            ux_utility_string_length_get(_ux_system_host_class_storage_name));
    if (UX_SUCCESS == status)
    {
        if (UX_DEVICE_INSERTION == event)
        {
            tx_semaphore_put(&g_usb_media_sema);
        }

        else if (UX_DEVICE_REMOVAL == event)
        {
            tx_semaphore_put(&g_usb_media_sema);
        }

        else
        {
            /* Coding stds */
        }
    }

    return (UX_SUCCESS);
}

#if (APP_PROVIDE_MSD)
void ux_msd_setup(UX_SLAVE_CLASS_STORAGE_PARAMETER * p_param)
{


    UX_SLAVE_CLASS_STORAGE_LUN * p_lun = p_param->ux_slave_class_storage_parameter_lun;


    p_param->ux_slave_class_storage_parameter_number_lun = 1;
    p_lun[0].ux_slave_class_storage_media_last_lba       = (ULONG) g_fx_media_ptr->fx_media_total_sectors;
    p_lun[0].ux_slave_class_storage_media_block_length   = g_fx_media_ptr->fx_media_bytes_per_sector;
    p_lun[0].ux_slave_class_storage_media_type           = 0;
    p_lun[0].ux_slave_class_storage_media_removable_flag = 0x80;
    p_lun[0].ux_slave_class_storage_media_read           = ux_msd_read;
    p_lun[0].ux_slave_class_storage_media_write          = ux_msd_write;
    p_lun[0].ux_slave_class_storage_media_status         = ux_msd_status;
}

UINT ux_msd_read(VOID * p_storage, ULONG lun, UCHAR * p_dest, ULONG blocks,
                 ULONG lba, ULONG *media_status)
{
    SSP_PARAMETER_NOT_USED(lun);
    SSP_PARAMETER_NOT_USED(media_status);
    SSP_PARAMETER_NOT_USED(p_storage);
    ux_utility_semaphore_get(&g_ux_host_class_storage->ux_host_class_storage_semaphore,
                             UX_WAIT_FOREVER);

    UINT status;
    status = ux_host_class_storage_media_read(g_ux_host_class_storage, lba,
                                              blocks, p_dest);

    ux_utility_semaphore_put(&g_ux_host_class_storage->ux_host_class_storage_semaphore);

    return (status);
}

UINT ux_msd_write(VOID * p_storage, ULONG lun, UCHAR * p_src, ULONG blocks,
                  ULONG lba, ULONG *media_status)
{
    SSP_PARAMETER_NOT_USED(lun);
    SSP_PARAMETER_NOT_USED(media_status);
    SSP_PARAMETER_NOT_USED(p_storage);
    ux_utility_semaphore_get(&g_ux_host_class_storage->ux_host_class_storage_semaphore,
                             UX_WAIT_FOREVER);

    UINT status;
    status = ux_host_class_storage_media_write(g_ux_host_class_storage, lba,
                                               blocks, p_src);

    ux_utility_semaphore_put(&g_ux_host_class_storage->ux_host_class_storage_semaphore);

    return (status);
}

UINT ux_msd_status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status)
{
    SSP_PARAMETER_NOT_USED(storage);
    SSP_PARAMETER_NOT_USED(lun);
    SSP_PARAMETER_NOT_USED(media_id);
    SSP_PARAMETER_NOT_USED(media_status);
    return (UX_SUCCESS);
}
#endif











