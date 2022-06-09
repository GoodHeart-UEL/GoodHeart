/***********************************************************************************************************************
 * Copyright [2015-2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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

/**********************************************************************************************************************
 * File Name    : sf_touch_panel_i2c.c
 * Description  : Touch Panel Framework (SF_TOUCH_PANEL_I2C) Module for I2C Bus driver.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "sf_touch_panel_i2c.h"
#include "sf_touch_panel_i2c_cfg.h"
#include "sf_touch_panel_private_api.h"

/*******************************************************************************************************************//**
 * @addtogroup SF_TOUCH_PANEL_I2C
 * @{
 **********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SF_TOUCH_PANEL_ERROR_RETURN
/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define SF_TOUCH_PANEL_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), &g_module_name[0], &g_version)
#endif

#define SF_TOUCH_PANEL_PRV_RTOS_TICKS_SEC (100U)


/** "TI2C" in ASCII, used to identify I2C touch panel handle */
#define OPEN (0x54493243U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_sf_touch_panel_prv_flags
{
    SF_TOUCH_PANEL_PRV_FLAGS_STOP  = 1,
    SF_TOUCH_PANEL_PRV_FLAGS_START = 2
} sf_touch_panel_prv_flags_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if (1 == SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE)
static ssp_err_t sf_touch_panel_i2c_open_param_check(sf_touch_panel_i2c_instance_ctrl_t * const p_ctrl,
                                                     sf_touch_panel_cfg_t const * const p_cfg);
#endif

static void sf_touch_panel_i2c_thread  (ULONG thread_input);

static bool sf_touch_panel_i2c_check_event_to_be_sent (sf_touch_panel_i2c_instance_ctrl_t * p_ctrl,
                                                    sf_touch_panel_event_t event, ULONG last_updated_time);

static void sf_touch_panel_i2c_touch_event_post (sf_touch_panel_i2c_instance_ctrl_t * p_ctrl);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char g_module_name[] = "sf_touch_panel_i2c";
#endif

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_version =
{
    .api_version_minor  = SF_TOUCH_PANEL_API_VERSION_MINOR,
    .api_version_major  = SF_TOUCH_PANEL_API_VERSION_MAJOR,
    .code_version_major = SF_TOUCH_PANEL_I2C_CODE_VERSION_MAJOR,
    .code_version_minor = SF_TOUCH_PANEL_I2C_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif


/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const sf_touch_panel_api_t g_sf_touch_panel_on_sf_touch_panel_i2c =
{
    .open      = SF_TOUCH_PANEL_I2C_Open,
    .calibrate = SF_TOUCH_PANEL_I2C_Calibrate,
    .start     = SF_TOUCH_PANEL_I2C_Start,
    .stop      = SF_TOUCH_PANEL_I2C_Stop,
    .reset     = SF_TOUCH_PANEL_I2C_Reset,
    .close     = SF_TOUCH_PANEL_I2C_Close,
    .versionGet = SF_TOUCH_PANEL_I2C_VersionGet,
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::open.
 *
 * @retval SSP_SUCCESS           Touch panel thread created and lower level drivers opened successfully.
 * @retval SSP_ERR_ASSERTION     A pointer parameter was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_INTERNAL      The touch panel thread or event flags could not be created, or a lower level driver
 *                               reported this error.
 * @retval SSP_ERR_IN_USE        Mutex was not available, or a lower level driver reported this error.
 * @return                       See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 *                               This function calls:
 *                                 * i2c_api_master_t::open
 *                                 * sf_touch_panel_i2c_chip_t::reset
 *                                 * sf_external_irq_api_t::open
 * @note This function is reentrant for any panel.
 **********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_Open (sf_touch_panel_ctrl_t      * const p_api_ctrl,
                                   sf_touch_panel_cfg_t const * const p_cfg)
{
    ssp_err_t                            err;
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl = (sf_touch_panel_i2c_instance_ctrl_t *) p_api_ctrl;

#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    err = sf_touch_panel_i2c_open_param_check(p_ctrl, p_cfg);
    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);
#endif /* if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE */

    sf_touch_panel_i2c_cfg_t const * const p_cfg_extend    = p_cfg->p_extend;

    /** Store API's. */
    p_ctrl->p_lower_lvl_i2c = p_cfg_extend->p_lower_lvl_i2c;
    p_ctrl->p_lower_lvl_irq = p_cfg_extend->p_lower_lvl_irq;

    /** Store user configurations in control block. */
    p_ctrl->hsize_pixels  = p_cfg->hsize_pixels;
    p_ctrl->vsize_pixels  = p_cfg->vsize_pixels;
    p_ctrl->update_hz     = p_cfg->update_hz;
    p_ctrl->pin    = p_cfg_extend->pin;
    p_ctrl->p_chip = p_cfg_extend->p_chip;
    p_ctrl->p_message     = p_cfg->p_message;
    p_ctrl->event_class_instance    = p_cfg->event_class_instance;
    p_ctrl->rotation_angle          = p_cfg->rotation_angle;

    /** Initialize previous event. */
    p_ctrl->last_payload.event_type = SF_TOUCH_PANEL_EVENT_INVALID;
    p_ctrl->last_payload.x          = -1;
    p_ctrl->last_payload.y          = -1;

    /** Get mutex before calling lower layer, which will access HW registers. */
    tx_mutex_create(&p_ctrl->mutex, NULL, TX_INHERIT);
    UINT tx_err;
    tx_err = tx_mutex_get(&p_ctrl->mutex, TX_NO_WAIT);
    SF_TOUCH_PANEL_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_IN_USE);

    /** Open the lower level I2C driver. */
    err = p_ctrl->p_lower_lvl_i2c->p_api->open(p_ctrl->p_lower_lvl_i2c->p_ctrl, p_cfg_extend->p_lower_lvl_i2c->p_cfg);
    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Reset the touch chip. */
    err = p_ctrl->p_chip->reset(p_ctrl);
    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Open the lower level external IRQ driver. */
    err = p_ctrl->p_lower_lvl_irq->p_api->open(p_ctrl->p_lower_lvl_irq->p_ctrl, p_cfg_extend->p_lower_lvl_irq->p_cfg);
    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Return mutex */
    tx_mutex_put(&p_ctrl->mutex);

    /** Create event flags for internal communication. */
    tx_err = tx_event_flags_create(&p_ctrl->flags, NULL);
    SF_TOUCH_PANEL_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_INTERNAL);

    /** Create main touch panel thread. */
    tx_err = tx_thread_create(&p_ctrl->thread,
                              (CHAR *) "Touch Panel Thread",
                              sf_touch_panel_i2c_thread,
                              (ULONG) p_ctrl,
                              &p_ctrl->stack,
                              SF_TOUCH_PANEL_I2C_STACK_SIZE,
                              p_cfg->priority,
                              p_cfg->priority,
                              TX_NO_TIME_SLICE,
                              TX_AUTO_START);
    SF_TOUCH_PANEL_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_INTERNAL);

    /** Mark control block open so other tasks know it is valid */
    p_ctrl->open = OPEN;

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::calibrate.
 *
 * @retval SSP_SUCCESS               Touch panel calibrated successfully.
 * @retval SSP_ERR_ASSERTION         A pointer parameter was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_CALIBRATE_FAILED  Actual touch value was not in expected range.
 * @retval SSP_ERR_NOT_OPEN          Touch panel is not configured.  Call SF_TOUCH_PANEL_I2C_Open.
 * @return See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 **********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_Calibrate (sf_touch_panel_ctrl_t             * const p_api_ctrl,
                                        sf_touch_panel_calibrate_t const * const  p_expected,
                                        sf_touch_panel_payload_t const * const    p_actual,
                                        ULONG                                     timeout)
{
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl = (sf_touch_panel_i2c_instance_ctrl_t *) p_api_ctrl;
    SSP_PARAMETER_NOT_USED(p_ctrl);

#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_expected);
    SSP_ASSERT(NULL != p_actual);
    SF_TOUCH_PANEL_ERROR_RETURN(OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE */

    /** Timeout not used in this implementation. */
    (void) timeout;

    /** Determine if touch message data matches expected value. */
    ssp_err_t err               = SSP_SUCCESS;
    int32_t   dx_squared        = ((p_expected->x - p_actual->x) * (p_expected->x - p_actual->x));
    int32_t   dy_squared        = ((p_expected->y - p_actual->y) * (p_expected->y - p_actual->y));
    int32_t   delta_squared     = dx_squared + dy_squared;
    int32_t   tolerance_squared = p_expected->tolerance_pixels * p_expected->tolerance_pixels;
    if (delta_squared > tolerance_squared)
    {
        /* Touched value is not in acceptable range. */
        err = SSP_ERR_CALIBRATE_FAILED;
    }

    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::start.
 *
 * @retval SSP_SUCCESS           Touch panel start flag posted to touch thread.
 * @retval SSP_ERR_NOT_OPEN      Touch panel is not configured.  Call SF_TOUCH_PANEL_I2C_Open.
 * @retval SSP_ERR_INTERNAL      An internal ThreadX error has occurred.
 * @return See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 **********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_Start (sf_touch_panel_ctrl_t * const p_api_ctrl)
{
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl = (sf_touch_panel_i2c_instance_ctrl_t *) p_api_ctrl;

#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_TOUCH_PANEL_ERROR_RETURN(OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    /** Set start flag. */
    UINT tx_err = tx_event_flags_set(&p_ctrl->flags, SF_TOUCH_PANEL_PRV_FLAGS_START, TX_OR);
    SF_TOUCH_PANEL_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_INTERNAL);

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::stop.
 *
 * @retval SSP_SUCCESS           Touch panel will not respond to touch events.
 * @retval SSP_ERR_ASSERTION     Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN      Touch panel is not configured.  Call SF_TOUCH_PANEL_I2C_Open.
 * @retval SSP_ERR_INTERNAL      An internal ThreadX error has occurred.
 * @return See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 **********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_Stop  (sf_touch_panel_ctrl_t * const p_api_ctrl)
{
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl = (sf_touch_panel_i2c_instance_ctrl_t *) p_api_ctrl;

#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_TOUCH_PANEL_ERROR_RETURN(OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    /** Set stop flag. */
    UINT tx_err = tx_event_flags_set(&p_ctrl->flags, SF_TOUCH_PANEL_PRV_FLAGS_STOP, TX_OR);
    SF_TOUCH_PANEL_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_INTERNAL);

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::reset.
 *
 * @retval SSP_SUCCESS           Touch panel and lower level I2C driver were reset successfully.
 * @retval SSP_ERR_ASSERTION     Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_IN_USE        Mutex was not available, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN      Touch panel is not configured. Use Open API to configure.
 * @return See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 *                               This function calls:
 *                               * sf_touch_panel_i2c_chip_t::reset
 * @note This function is reentrant for any panel.
 **********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_Reset (sf_touch_panel_ctrl_t * const p_api_ctrl)
{
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl = (sf_touch_panel_i2c_instance_ctrl_t *) p_api_ctrl;

#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_TOUCH_PANEL_ERROR_RETURN(OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    /** Obtain mutex since this accesses shared resources. */
    ULONG   wait_option = TX_NO_WAIT;
    UINT    tx_err = tx_mutex_get(&p_ctrl->mutex, wait_option);
    SF_TOUCH_PANEL_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_IN_USE);

    /** Call hardware specific reset function. */
    p_ctrl->p_chip->reset(p_ctrl);

    /** Release mutex. */
    tx_mutex_put(&p_ctrl->mutex);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::close.
 *
 * @retval SSP_SUCCESS           Touch panel instance successfully closed.
 * @retval SSP_ERR_ASSERTION     Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN      Touch panel is not configured.  Call SF_TOUCH_PANEL_I2C_Open.
 * @return See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 *                               This function calls:
 *                               * sf_external_irq_api_t::close
 *                               * i2c_api_master_t::close
 *                               * sf_message_api_t::bufferRelease
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_Close (sf_touch_panel_ctrl_t * const p_api_ctrl)
{
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl = (sf_touch_panel_i2c_instance_ctrl_t *) p_api_ctrl;

#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_TOUCH_PANEL_ERROR_RETURN(OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    i2c_api_master_t const * const p_i2c_api       = p_ctrl->p_lower_lvl_i2c->p_api;
    i2c_ctrl_t * const             p_i2c_ctrl      = p_ctrl->p_lower_lvl_i2c->p_ctrl;
    sf_external_irq_ctrl_t         * p_irq_ctrl    = p_ctrl->p_lower_lvl_irq->p_ctrl;
    sf_external_irq_api_t const    * p_irq_api     = p_ctrl->p_lower_lvl_irq->p_api;
    ssp_err_t                      err;

    /** Close the lower level external IRQ driver. */
    err = p_irq_api->close(p_irq_ctrl);
    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Close the lower level I2C driver. */
    err = p_i2c_api->close(p_i2c_ctrl);
    if (SSP_ERR_ABORTED == err)
    {
        /** If I2C Abort returned from previous close, one more try to close I2C driver. */
        err = p_i2c_api->close(p_i2c_ctrl);
    }
    SF_TOUCH_PANEL_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Suspend internal thread. */
    tx_thread_suspend(&p_ctrl->thread);

    if (NULL != p_ctrl->p_message)
    {
        /** If a messaging framework buffer is taken but a touch message is not yet posted to the subscribers, release it. */
        if (NULL != p_ctrl->p_payload)
        {
            p_ctrl->p_message->p_api->bufferRelease(p_ctrl->p_message->p_ctrl, (sf_message_header_t *) p_ctrl->p_payload,
                                     SF_MESSAGE_RELEASE_OPTION_ACK);
        }
    }

    /** Delete RTOS services used */
    tx_event_flags_delete(&p_ctrl->flags);
    tx_mutex_delete(&p_ctrl->mutex);
    tx_thread_terminate(&p_ctrl->thread);
    tx_thread_delete(&p_ctrl->thread);

    /** Mark control block close */
    p_ctrl->open = 0U;

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_touch_panel_api_t::versionGet.
 *
 * @retval SSP_SUCCESS           Version returned successfully.
 * @retval SSP_ERR_ASSERTION     Parameter p_version was null.
 *********************************************************************************************************************/
ssp_err_t SF_TOUCH_PANEL_I2C_VersionGet (ssp_version_t * const p_version)
{
#if SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif

    *p_version = g_version;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end defgroup SF_TOUCH_PANEL_I2C)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Publishes touch input messages.
 *
 * @param[in] thread_input  Pointer to touch panel control structure (sf_touch_panel_i2c_instance_ctrl_t).
 **********************************************************************************************************************/
static void sf_touch_panel_i2c_thread (ULONG thread_input)
{
    sf_touch_panel_i2c_instance_ctrl_t * p_ctrl             = (sf_touch_panel_i2c_instance_ctrl_t *) thread_input;
    ULONG                                last_updated_time  = tx_time_get();
    p_ctrl->p_payload = NULL;

    while (1)
    {
        ssp_err_t err;
        while (NULL == p_ctrl->p_payload)
        {
            /** Get a buffer from the messaging framework to store touch data. */
            sf_message_acquire_cfg_t acquire_cfg =
            {
                .buffer_keep = false,
            };
            err = p_ctrl->p_message->p_api->bufferAcquire(p_ctrl->p_message->p_ctrl,
                                           (sf_message_header_t **) &p_ctrl->p_payload,
                                           &acquire_cfg,
                                           10);
        }

        /** Set message payload event class and event. */
        p_ctrl->p_payload->header.event_b.class_code = SF_MESSAGE_EVENT_CLASS_TOUCH;
        p_ctrl->p_payload->header.event_b.code = SF_MESSAGE_EVENT_NEW_DATA;
        p_ctrl->p_payload->header.event_b.class_instance = p_ctrl->event_class_instance;

        /** Check for touch control stop flag.  If it is received, wait for touch control start flag.  After
         * start is received, reset touch chip to ensure old buffered data is flushed, then wait for
         * the next pin interrupt from the touch controller. */
        ULONG actual_flags = 0;
        tx_event_flags_get(&p_ctrl->flags, (ULONG)SF_TOUCH_PANEL_PRV_FLAGS_STOP | (ULONG)SF_TOUCH_PANEL_PRV_FLAGS_START,
                           TX_OR_CLEAR, &actual_flags, TX_NO_WAIT);
        if (actual_flags & (ULONG)SF_TOUCH_PANEL_PRV_FLAGS_STOP)
        {
            /* Wait for start flag. */
            tx_event_flags_get(&p_ctrl->flags, SF_TOUCH_PANEL_PRV_FLAGS_START,
                               TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

            /* Reset touch controller. */
            SF_TOUCH_PANEL_I2C_Reset(p_ctrl);
        }

        /** Get the touch event data from the touch driver */
        err = p_ctrl->p_chip->payloadGet(p_ctrl, p_ctrl->p_payload);
        if (SSP_SUCCESS == err)
        {
            /** Check the touch coordinate got from the touch driver */
            if ((p_ctrl->hsize_pixels < p_ctrl->p_payload->x) || (p_ctrl->vsize_pixels < p_ctrl->p_payload->y))
            {
                /* Touch panel returns the coordinate beyond the touch panel screen size, notify users as an invalid event */
                p_ctrl->p_payload->event_type = SF_TOUCH_PANEL_EVENT_INVALID;
            }
        }
        else if (SSP_ERR_WAIT_ABORTED == err)
        {
            /* Error might happen in the External IRQ module. Critical situation so that suspend this thread by itself. */
        	tx_thread_suspend(&p_ctrl->thread);
        }
        else
        {
            /* Error in touch chip.  Error handling should happen at the lower layer, so this is just a notification. */
            p_ctrl->p_payload->event_type = SF_TOUCH_PANEL_EVENT_INVALID;
        }

        /** Check if new touch event is to be sent. */
        if (true == sf_touch_panel_i2c_check_event_to_be_sent(p_ctrl, p_ctrl->p_payload->event_type, last_updated_time))
        {
            /** Store time, used to prevent too many events from being generated */
            last_updated_time = tx_time_get();

            /** Post a touch event to the message queue(s) of touch event subscribers. */
            sf_touch_panel_i2c_touch_event_post(p_ctrl);
        }
    }
}

/*******************************************************************************************************************//**
 * @brief   sf_touch_panel_i2c_check_event_to_be_sent : Check a touch event to be sent or not.
 *
 * This function checks if new touch event is to be sent or not. DOWN and UP will always be sent. HOLD, MOVE and
 * INVALID will be sent if user specified dormant time elapsed after previous event was sent.
 *
 * @param[in]  p_ctrl       : Pointer to SF_TOUCH_PANEL_I2C instance structure
 * @param[in]  event        : Touch event
 * @param[in]  last_updated_time : Last time when touch event was updated.
 *
 * @retval  true    - Send the event.
 * @retval  false   - Do not send the event.
***********************************************************************************************************************/
static bool sf_touch_panel_i2c_check_event_to_be_sent (sf_touch_panel_i2c_instance_ctrl_t * p_ctrl,
                                                    sf_touch_panel_event_t event, ULONG last_updated_time)
{
    bool  send_event = true;
    ULONG current_time;

    if (((SF_TOUCH_PANEL_EVENT_HOLD    == event)  ||
         (SF_TOUCH_PANEL_EVENT_MOVE    == event)) ||
         (SF_TOUCH_PANEL_EVENT_INVALID == event))
    {
        /** Only send repeat events as requested by user. */
        current_time = tx_time_get();
        if (current_time <= (last_updated_time + (SF_TOUCH_PANEL_PRV_RTOS_TICKS_SEC / (p_ctrl->update_hz))))
        {
            send_event = false;
        }
    }
    return send_event;
}

/*******************************************************************************************************************//**
 * @brief   sf_touch_panel_i2c_touch_event_post : Post new touch event message.
 *
 * This function posts a touch event message to the message subscribers.
 *
 * @param[in]  p_ctrl : Pointer to SF_TOUCH_PANEL_I2C instance structure
***********************************************************************************************************************/
static void sf_touch_panel_i2c_touch_event_post (sf_touch_panel_i2c_instance_ctrl_t * p_ctrl)
{
    ssp_err_t               err;
    int16_t                 temp_value = 0;
    sf_message_post_cfg_t   post_cfg;

    post_cfg.priority     = SF_MESSAGE_PRIORITY_NORMAL;
    post_cfg.p_callback   = NULL;
    post_cfg.p_context    = (void const *)NULL;

    /** Rotate touch coordinate clockwise. */
    switch (p_ctrl->rotation_angle)
    {
        case 90U:
            temp_value = p_ctrl->p_payload->y;
            p_ctrl->p_payload->y = p_ctrl->p_payload->x;
            p_ctrl->p_payload->x = (int16_t)(p_ctrl->vsize_pixels - temp_value);
            break;

        case 180U:
            p_ctrl->p_payload->x = (int16_t)(p_ctrl->hsize_pixels - p_ctrl->p_payload->x);
            p_ctrl->p_payload->y = (int16_t)(p_ctrl->vsize_pixels - p_ctrl->p_payload->y);
            break;

        case 270U:
            temp_value = p_ctrl->p_payload->x;
            p_ctrl->p_payload->x = p_ctrl->p_payload->y;
            p_ctrl->p_payload->y = (int16_t)(p_ctrl->hsize_pixels - temp_value);
            break;

        default:
            /* Do nothing */
            break;
    }

    /** Post an event to message queue. */
    err = p_ctrl->p_message->p_api->post(p_ctrl->p_message->p_ctrl,
                          (sf_message_header_t *) p_ctrl->p_payload,
                          &post_cfg,
                          NULL,
                          TX_NO_WAIT);
    if (err == SSP_SUCCESS)
    {
        /* Buffer belongs to receiver threads now. */
        p_ctrl->p_payload = NULL;
    }
}

#if (1 == SF_TOUCH_PANEL_I2C_CFG_PARAM_CHECKING_ENABLE)
/*******************************************************************************************************************//**
 * @brief   sf_touch_panel_i2c_open_param_check : SF_TOUCH_PANEL_I2C open parameter check
 *
 * This function validates the parameters given to open function.
 *
 * @param[in]  p_ctrl : Pointer to SF_TOUCH_PANEL_I2C instance structure
 * @param[in]  p_cfg  : Pointer to SF_TOUCH_PANEL_I2C configuration structure
 *
 * @retval  SSP_SUCCESS       - Successful
 * @retval  SSP_ERR_ASSERTION - NULL pointer error
***********************************************************************************************************************/
static ssp_err_t sf_touch_panel_i2c_open_param_check(sf_touch_panel_i2c_instance_ctrl_t * const p_ctrl,
                                                     sf_touch_panel_cfg_t const * const p_cfg)
{
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->p_extend);

    sf_touch_panel_i2c_cfg_t const * const p_cfg_extend     = p_cfg->p_extend;

    SSP_ASSERT(NULL != p_cfg_extend->p_lower_lvl_i2c);
    SSP_ASSERT(NULL != p_cfg_extend->p_lower_lvl_irq);
    SSP_ASSERT(NULL != p_cfg_extend->p_chip);

    return SSP_SUCCESS;
}
#endif
