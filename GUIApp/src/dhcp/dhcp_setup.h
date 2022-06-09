
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/*
 *      This file defines error status returns for the
 *      run_dhcp_client_session() function.  It defines a timeout
 *      WAIT_TO_BE_BOUND for the DHCP Client session, which can be adjusted longer
 *      for particular network environments or DHCP Client
 *      configurations. It also enables the application for
 *      debug output by defining SEMI_HOSTING.
 */

#ifndef DHCPV4_CLIENT_NETX_MG_H_
#define DHCPV4_CLIENT_NETX_MG_H_

#include "nx_dhcp.h"


/* Define the function to call for running a DHCP Client session. */
UINT run_dhcp_client_session(NX_DHCP *client_ptr, NX_IP *ip_ptr);

char* getIpText();


/* If define, debug output sent to Renesas Virtual Console */
#define SEMI_HOSTING

/* Define the wait limit for getting assigned an IP address.
 * NX_IP_PERIODIC_RATE is defined internally in NetX for converting
 * seconds to timer ticks. This timeout is defined for 2 seconds.
 */
#define WAIT_TO_BE_BOUND            (2*NX_IP_PERIODIC_RATE)

/* Define error status returns */
#define DHCP_RUN_ERRORS     0x1001
#define DHCP_START_ERROR    0x1002
#define MAX_DNS_SERVERS      (5)
#define MAX_GATEWAYS         (5)
#define IP_V4_SIZE           (4)
#endif /* DHCPV4_CLIENT_NETX_MG_H_ */


