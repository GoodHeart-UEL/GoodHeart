/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2012 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               www.expresslogic.com          */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/

/***********************************************************************************************************************
 * Copyright [2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   SYNERGY Controller Driver                                           */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_hcd_synergy.h"
#include "ux_host_stack.h"
#include "ux_utility.h"

/*******************************************************************************************************************//**
 * @addtogroup sf_el_ux
 * @{
 **********************************************************************************************************************/

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    ux_hcd_synergy_request_control_transfer             PORTABLE C      */
/*                                                           5.6          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function performs a control transfer from a transfer request. */ 
/*     The USB control transfer is in 3 phases (setup, data, status).     */
/*     This function will chain all phases of the control sequence before */
/*     setting the Synergy endpoint as a candidate for transfer.          */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_synergy                           Pointer to Synergy controller */
/*    transfer_request                      Pointer to transfer request   */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    ux_hcd_synergy_regular_td_obtain        Obtain regular TD           */
/*    _ux_host_stack_transfer_request_abort Abort transfer request        */ 
/*    _ux_utility_memory_allocate           Allocate memory block         */ 
/*    _ux_utility_memory_free               Release memory block          */ 
/*    _ux_utility_semaphore_get             Get semaphore                 */ 
/*    _ux_utility_short_put                 Write 16-bit value            */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Synergy Controller Driver                                           */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2012     TCRG                     Initial Version 5.6           */ 
/*                                                                        */ 
/**************************************************************************/ 

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  This function performs a control transfer from a transfer request.
 *         The USB control transfer is in 3 phases (setup, data, status).
 *         This function will chain all phases of the control sequence before
 *         setting the Synergy endpoint as a candidate for transfer.
 *
 * @param[in,out]  hcd_synergy         : Pointer to a HCD control block
 * @param[in,out]  transfer_request    : Pointer to transfer request
 *
 * @retval UX_MEMORY_INSUFFICIENT               Insufficient memory to build the SETUP request.
 * @retval UX_NO_TD_AVAILABLE                   Unavailable New TD.
 * @retval ux_transfer_request_completion_code  Pointer to USBX transfer request structure(request completion code).
 **********************************************************************************************************************/
UINT  ux_hcd_synergy_request_control_transfer(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request)
{
    UX_ENDPOINT       * endpoint;
    UCHAR             * setup_request;
    UX_SYNERGY_ED     * ed;
    UX_SYNERGY_TD     * setup_td;
    UX_SYNERGY_TD     * chain_td;
    UX_SYNERGY_TD     * data_td;
    UX_SYNERGY_TD     * tail_td;
    UX_SYNERGY_TD     * status_td;
    UX_SYNERGY_TD     * start_data_td;
    UX_SYNERGY_TD     * next_data_td;
    UINT                status;
    ULONG               transfer_request_payload_length;
    ULONG               control_packet_payload_length;
    UCHAR             * data_pointer;
    ULONG               toggle;
    ULONG               max_packet_size;
    
    /* Get the pointer to the endpoint.  */
    endpoint =  (UX_ENDPOINT *) transfer_request -> ux_transfer_request_endpoint;

    /* Now get the physical ED attached to this endpoint.  */
    ed =  endpoint -> ux_endpoint_ed;

    /* Build the SETUP packet (phase 1 of the control transfer).  */
    setup_request =  _ux_utility_memory_allocate((ULONG)UX_NO_ALIGN, (ULONG)UX_REGULAR_MEMORY, (ULONG)UX_SETUP_SIZE);
    if (setup_request == UX_NULL)
    {
        return (UINT)UX_MEMORY_INSUFFICIENT;
    }

    *setup_request =                            (UCHAR) transfer_request -> ux_transfer_request_function;
    *(setup_request + UX_SETUP_REQUEST_TYPE) =  (UCHAR) transfer_request -> ux_transfer_request_type;
    *(setup_request + UX_SETUP_REQUEST) =       (UCHAR) transfer_request -> ux_transfer_request_function;
    _ux_utility_short_put(setup_request + UX_SETUP_VALUE, (USHORT) transfer_request -> ux_transfer_request_value);
    _ux_utility_short_put(setup_request + UX_SETUP_INDEX, (USHORT) transfer_request -> ux_transfer_request_index);
    _ux_utility_short_put(setup_request + UX_SETUP_LENGTH, (USHORT) transfer_request -> ux_transfer_request_requested_length);

    /* Use the TD pointer by ed -> tail for our setup TD and chain from this one on.  */
    setup_td =  ed -> ux_synergy_ed_tail_td;
    setup_td -> ux_synergy_td_buffer =  setup_request;
    setup_td -> ux_synergy_td_length =  (ULONG)UX_SETUP_SIZE;
    chain_td =  setup_td;

    /* Attach the endpoint and transfer_request to the TD.  */
    setup_td -> ux_synergy_td_transfer_request =  transfer_request;
    setup_td -> ux_synergy_td_ed =  ed;

    /* Setup is OUT.  */
    setup_td -> ux_synergy_td_direction =  UX_SYNERGY_TD_OUT;

    /* Mark TD toggle as being DATA0.  */
    setup_td -> ux_synergy_td_toggle =  0UL;

    /* Mark the TD with the SETUP phase.  */
    setup_td -> ux_synergy_td_status |=  UX_SYNERGY_TD_SETUP_PHASE;

    /* Check if there is a data phase, if not jump to status phase.  */
    data_td =  UX_NULL;    
    start_data_td =  UX_NULL;

    /* Use local variables to manipulate data pointer and length.  */
    transfer_request_payload_length =  transfer_request -> ux_transfer_request_requested_length;
    data_pointer =  transfer_request -> ux_transfer_request_data_pointer;

    /* Data starts with DATA1. For the data phase, we use the ED to contain the toggle.  */
    toggle = 1UL;

    /* The Control data payload may be split into several smaller blocks.  */
    while (transfer_request_payload_length != 0UL)
    {
        /* Get a new TD to hook this payload.  */
        data_td =  ux_hcd_synergy_regular_td_obtain(hcd_synergy);
        if (data_td == UX_NULL)
        {
            /* Free the Setup packet resources.  */
            _ux_utility_memory_free(setup_request);

            /* If there was already a TD chain in progress, free it.  */
            if (start_data_td != UX_NULL)
            {
                data_td =  start_data_td;
                while (data_td)
                {
                    next_data_td =  data_td -> ux_synergy_td_next_td;
                    data_td -> ux_synergy_td_status = (ULONG)UX_UNUSED;
                    data_td =  next_data_td;
                }
            }

            return (UINT)UX_NO_TD_AVAILABLE;
        }

        /* Get the max packet size for this endpoint.  */
        max_packet_size = ed -> ux_synergy_ed_endpoint -> ux_endpoint_descriptor.wMaxPacketSize;

        /* Check the current payload requirement for the max size.  */
        if (transfer_request_payload_length > max_packet_size)
        {
            control_packet_payload_length =  max_packet_size;
        }
        else
        {
            control_packet_payload_length =  transfer_request_payload_length;
        }

        /* Store the beginning of the buffer address in the TD.  */
        data_td -> ux_synergy_td_buffer =  data_pointer;

        /* Update the length of the transfer for this TD.  */
        data_td -> ux_synergy_td_length =  control_packet_payload_length;

        /* Attach the endpoint and transfer request to the TD.  */
        data_td -> ux_synergy_td_transfer_request =  transfer_request;
        data_td -> ux_synergy_td_ed =  ed;

        /* Adjust the data payload length and the data payload pointer.  */
        transfer_request_payload_length -=  control_packet_payload_length;
        data_pointer +=  control_packet_payload_length;

        /* The direction of the transaction is set in the TD.  */
        if ((transfer_request -> ux_transfer_request_type & (UINT)UX_REQUEST_DIRECTION) == (UINT)UX_REQUEST_IN)
        {
            data_td -> ux_synergy_td_direction =  UX_SYNERGY_TD_IN;
        }
        else
        {
            data_td -> ux_synergy_td_direction =  UX_SYNERGY_TD_OUT;
        }

        /* Mark the TD with the DATA phase.  */
        data_td -> ux_synergy_td_status |=  UX_SYNERGY_TD_DATA_PHASE;

        /* The Toggle value.  */
        data_td -> ux_synergy_td_toggle =  toggle;

        /* Invert toggle now. */
        if (toggle)
        {
            toggle = 0UL;
        }
        else
        {
            toggle = 1UL;
        }

        /* The first obtained TD in the chain has to be remembered.  */
        if (start_data_td == UX_NULL)
        {
            start_data_td =  data_td;
        }

        /* Attach this new TD to the previous one.  */                                
        chain_td -> ux_synergy_td_next_td =  data_td;
        chain_td =  data_td;
    }

    /* Now, program the status phase.  */
    status_td =  ux_hcd_synergy_regular_td_obtain(hcd_synergy);

    if (status_td == UX_NULL)
    {
        _ux_utility_memory_free(setup_request);
        if (data_td != UX_NULL)
        {
            data_td =  start_data_td;
            while (data_td)
            {
                next_data_td =  data_td -> ux_synergy_td_next_td;
                data_td -> ux_synergy_td_status = (ULONG)UX_UNUSED;
                data_td =  next_data_td;
            }
        }

        return (UINT)UX_NO_TD_AVAILABLE;
    }

    /* Attach the endpoint and transfer request to the TD.  */
    status_td -> ux_synergy_td_transfer_request =  transfer_request;
    status_td -> ux_synergy_td_ed =  ed;

    /* Mark the TD with the STATUS phase.  */
    status_td -> ux_synergy_td_status |=  UX_SYNERGY_TD_STATUS_PHASE;

    /* The direction of the status phase is IN if data phase is OUT and
       vice versa.  */
    if ((transfer_request -> ux_transfer_request_type & (UINT)UX_REQUEST_DIRECTION) == (UINT)UX_REQUEST_IN)
    {
        status_td -> ux_synergy_td_direction =  UX_SYNERGY_TD_OUT;
    }
    else
    {
        status_td -> ux_synergy_td_direction =  UX_SYNERGY_TD_IN;
    }

    /* No data payload for the status phase.  */
    status_td -> ux_synergy_td_buffer =  NULL;
    status_td -> ux_synergy_td_length =  0UL;

    /* Status Phase toggle is ALWAYS 1.  */
    status_td -> ux_synergy_td_toggle =  1UL;

    /* Hook the status phase to the previous TD.  */
    chain_td -> ux_synergy_td_next_td =  status_td;

    /* Since we have consumed out tail TD for the setup packet, we must get another one 
       and hook it to the ED's tail.  */
    tail_td =  ux_hcd_synergy_regular_td_obtain(hcd_synergy);
    if (tail_td == UX_NULL)
    {
        _ux_utility_memory_free(setup_request);

        if (data_td != UX_NULL)
        {
            data_td -> ux_synergy_td_status = (ULONG)UX_UNUSED;
        }

        status_td -> ux_synergy_td_status = (ULONG)UX_UNUSED;

        return (UINT)UX_NO_TD_AVAILABLE;
    }

    /* Hook the new TD to the status TD.  */
    status_td -> ux_synergy_td_next_td =  tail_td;
            
    /* At this stage, the Head and Tail in the ED are still the same and
       the Synergy controller will skip this ED until we have hooked the new
       tail TD.  */
    ed -> ux_synergy_ed_tail_td =  tail_td;

    /* Wait for the completion of the transfer request.  */
    status =  _ux_utility_semaphore_get(&transfer_request -> ux_transfer_request_semaphore,
                                    ((ULONG)UX_CONTROL_TRANSFER_TIMEOUT_IN_MS * (ULONG)UX_PERIODIC_RATE) / 1000UL);

    /* If the semaphore did not succeed we probably have a time out.  */
    if (status != (UINT)UX_SUCCESS)
    {
        /* All transfers pending need to abort. There may have been a partial transfer.  */
        _ux_host_stack_transfer_request_abort(transfer_request);
        
        /* There was an error, return to the caller.  */
        transfer_request -> ux_transfer_request_completion_code = (UINT)UX_TRANSFER_TIMEOUT;

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_TRANSFER_TIMEOUT, transfer_request, 0, 0, UX_TRACE_ERRORS, 0, 0)
    }            

    /* Free the resources.  */
    _ux_utility_memory_free(setup_request);

    /* Return completion to caller.  */
    return (transfer_request -> ux_transfer_request_completion_code);
}
/*******************************************************************************************************************//**
 * @} (end addtogroup sf_el_ux)
 **********************************************************************************************************************/

