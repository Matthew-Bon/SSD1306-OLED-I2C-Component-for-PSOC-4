/***************************************************************************//**
* \file OLED_SSD1306_1_I2C_I2C.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  I2C mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "OLED_SSD1306_1_I2C_PVT.h"
#include "OLED_SSD1306_1_I2C_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 OLED_SSD1306_1_I2C_state;  /* Current state of I2C FSM */

#if(OLED_SSD1306_1_I2C_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const OLED_SSD1306_1_I2C_I2C_INIT_STRUCT OLED_SSD1306_1_I2C_configI2C =
    {
        OLED_SSD1306_1_I2C_I2C_MODE,
        OLED_SSD1306_1_I2C_I2C_OVS_FACTOR_LOW,
        OLED_SSD1306_1_I2C_I2C_OVS_FACTOR_HIGH,
        OLED_SSD1306_1_I2C_I2C_MEDIAN_FILTER_ENABLE,
        OLED_SSD1306_1_I2C_I2C_SLAVE_ADDRESS,
        OLED_SSD1306_1_I2C_I2C_SLAVE_ADDRESS_MASK,
        OLED_SSD1306_1_I2C_I2C_ACCEPT_ADDRESS,
        OLED_SSD1306_1_I2C_I2C_WAKE_ENABLE,
        OLED_SSD1306_1_I2C_I2C_BYTE_MODE_ENABLE,
        OLED_SSD1306_1_I2C_I2C_DATA_RATE,
        OLED_SSD1306_1_I2C_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: OLED_SSD1306_1_I2C_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the OLED_SSD1306_1_I2C for I2C operation.
    *
    *  This function is intended specifically to be used when the OLED_SSD1306_1_I2C 
    *  configuration is set to “Unconfigured OLED_SSD1306_1_I2C” in the customizer. 
    *  After initializing the OLED_SSD1306_1_I2C in I2C mode using this function, 
    *  the component can be enabled using the OLED_SSD1306_1_I2C_Start() or 
    * OLED_SSD1306_1_I2C_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void OLED_SSD1306_1_I2C_I2CInit(const OLED_SSD1306_1_I2C_I2C_INIT_STRUCT *config)
    {
        uint32 medianFilter;
        uint32 locEnableWake;

        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            OLED_SSD1306_1_I2C_SetPins(OLED_SSD1306_1_I2C_SCB_MODE_I2C, OLED_SSD1306_1_I2C_DUMMY_PARAM,
                                     OLED_SSD1306_1_I2C_DUMMY_PARAM);

            /* Store internal configuration */
            OLED_SSD1306_1_I2C_scbMode       = (uint8) OLED_SSD1306_1_I2C_SCB_MODE_I2C;
            OLED_SSD1306_1_I2C_scbEnableWake = (uint8) config->enableWake;
            OLED_SSD1306_1_I2C_scbEnableIntr = (uint8) OLED_SSD1306_1_I2C_SCB_IRQ_INTERNAL;

            OLED_SSD1306_1_I2C_mode          = (uint8) config->mode;
            OLED_SSD1306_1_I2C_acceptAddr    = (uint8) config->acceptAddr;

        #if (OLED_SSD1306_1_I2C_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            OLED_SSD1306_1_I2C_SET_I2C_CFG_SDA_FILT_TRIM(OLED_SSD1306_1_I2C_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (OLED_SSD1306_1_I2C_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((OLED_SSD1306_1_I2C_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= OLED_SSD1306_1_I2C_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (OLED_SSD1306_1_I2C_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                OLED_SSD1306_1_I2C_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = OLED_SSD1306_1_I2C_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                OLED_SSD1306_1_I2C_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = OLED_SSD1306_1_I2C_DIGITAL_FILTER_ENABLE;
            }

        #if (!OLED_SSD1306_1_I2C_CY_SCBIP_V0)
            locEnableWake = (OLED_SSD1306_1_I2C_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!OLED_SSD1306_1_I2C_CY_SCBIP_V0) */

            /* Configure I2C interface */
            OLED_SSD1306_1_I2C_CTRL_REG     = OLED_SSD1306_1_I2C_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            OLED_SSD1306_1_I2C_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            OLED_SSD1306_1_I2C_GET_CTRL_EC_AM_MODE (locEnableWake);

            OLED_SSD1306_1_I2C_I2C_CTRL_REG = OLED_SSD1306_1_I2C_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    OLED_SSD1306_1_I2C_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    OLED_SSD1306_1_I2C_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    OLED_SSD1306_1_I2C_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            OLED_SSD1306_1_I2C_RX_CTRL_REG      = OLED_SSD1306_1_I2C_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                OLED_SSD1306_1_I2C_I2C_RX_CTRL;
            OLED_SSD1306_1_I2C_RX_FIFO_CTRL_REG = OLED_SSD1306_1_I2C_CLEAR_REG;

            /* Set default address and mask */
            OLED_SSD1306_1_I2C_RX_MATCH_REG    = ((OLED_SSD1306_1_I2C_I2C_SLAVE) ?
                                                (OLED_SSD1306_1_I2C_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 OLED_SSD1306_1_I2C_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (OLED_SSD1306_1_I2C_CLEAR_REG));


            /* Configure TX direction */
            OLED_SSD1306_1_I2C_TX_CTRL_REG      = OLED_SSD1306_1_I2C_I2C_TX_CTRL;
            OLED_SSD1306_1_I2C_TX_FIFO_CTRL_REG = OLED_SSD1306_1_I2C_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (OLED_SSD1306_1_I2C_ISR_NUMBER);
            CyIntSetPriority(OLED_SSD1306_1_I2C_ISR_NUMBER, OLED_SSD1306_1_I2C_ISR_PRIORITY);
            (void) CyIntSetVector(OLED_SSD1306_1_I2C_ISR_NUMBER, &OLED_SSD1306_1_I2C_I2C_ISR);

            /* Configure interrupt sources */
        #if(!OLED_SSD1306_1_I2C_CY_SCBIP_V1)
            OLED_SSD1306_1_I2C_INTR_SPI_EC_MASK_REG = OLED_SSD1306_1_I2C_NO_INTR_SOURCES;
        #endif /* (!OLED_SSD1306_1_I2C_CY_SCBIP_V1) */

            OLED_SSD1306_1_I2C_INTR_I2C_EC_MASK_REG = OLED_SSD1306_1_I2C_NO_INTR_SOURCES;
            OLED_SSD1306_1_I2C_INTR_RX_MASK_REG     = OLED_SSD1306_1_I2C_NO_INTR_SOURCES;
            OLED_SSD1306_1_I2C_INTR_TX_MASK_REG     = OLED_SSD1306_1_I2C_NO_INTR_SOURCES;

            OLED_SSD1306_1_I2C_INTR_SLAVE_MASK_REG  = ((OLED_SSD1306_1_I2C_I2C_SLAVE) ?
                            (OLED_SSD1306_1_I2C_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             OLED_SSD1306_1_I2C_I2C_INTR_SLAVE_MASK) : (OLED_SSD1306_1_I2C_CLEAR_REG));

            OLED_SSD1306_1_I2C_INTR_MASTER_MASK_REG = OLED_SSD1306_1_I2C_NO_INTR_SOURCES;

            /* Configure global variables */
            OLED_SSD1306_1_I2C_state = OLED_SSD1306_1_I2C_I2C_FSM_IDLE;

            /* Internal slave variables */
            OLED_SSD1306_1_I2C_slStatus        = 0u;
            OLED_SSD1306_1_I2C_slRdBufIndex    = 0u;
            OLED_SSD1306_1_I2C_slWrBufIndex    = 0u;
            OLED_SSD1306_1_I2C_slOverFlowCount = 0u;

            /* Internal master variables */
            OLED_SSD1306_1_I2C_mstrStatus     = 0u;
            OLED_SSD1306_1_I2C_mstrRdBufIndex = 0u;
            OLED_SSD1306_1_I2C_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: OLED_SSD1306_1_I2C_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void OLED_SSD1306_1_I2C_I2CInit(void)
    {
    #if(OLED_SSD1306_1_I2C_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        OLED_SSD1306_1_I2C_SET_I2C_CFG_SDA_FILT_TRIM(OLED_SSD1306_1_I2C_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (OLED_SSD1306_1_I2C_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        OLED_SSD1306_1_I2C_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        OLED_SSD1306_1_I2C_CTRL_REG     = OLED_SSD1306_1_I2C_I2C_DEFAULT_CTRL;
        OLED_SSD1306_1_I2C_I2C_CTRL_REG = OLED_SSD1306_1_I2C_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        OLED_SSD1306_1_I2C_RX_CTRL_REG      = OLED_SSD1306_1_I2C_I2C_DEFAULT_RX_CTRL;
        OLED_SSD1306_1_I2C_RX_FIFO_CTRL_REG = OLED_SSD1306_1_I2C_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        OLED_SSD1306_1_I2C_RX_MATCH_REG     = OLED_SSD1306_1_I2C_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        OLED_SSD1306_1_I2C_TX_CTRL_REG      = OLED_SSD1306_1_I2C_I2C_DEFAULT_TX_CTRL;
        OLED_SSD1306_1_I2C_TX_FIFO_CTRL_REG = OLED_SSD1306_1_I2C_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (OLED_SSD1306_1_I2C_ISR_NUMBER);
        CyIntSetPriority(OLED_SSD1306_1_I2C_ISR_NUMBER, OLED_SSD1306_1_I2C_ISR_PRIORITY);
    #if(!OLED_SSD1306_1_I2C_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(OLED_SSD1306_1_I2C_ISR_NUMBER, &OLED_SSD1306_1_I2C_I2C_ISR);
    #endif /* (OLED_SSD1306_1_I2C_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!OLED_SSD1306_1_I2C_CY_SCBIP_V1)
        OLED_SSD1306_1_I2C_INTR_SPI_EC_MASK_REG = OLED_SSD1306_1_I2C_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!OLED_SSD1306_1_I2C_CY_SCBIP_V1) */

        OLED_SSD1306_1_I2C_INTR_I2C_EC_MASK_REG = OLED_SSD1306_1_I2C_I2C_DEFAULT_INTR_I2C_EC_MASK;
        OLED_SSD1306_1_I2C_INTR_SLAVE_MASK_REG  = OLED_SSD1306_1_I2C_I2C_DEFAULT_INTR_SLAVE_MASK;
        OLED_SSD1306_1_I2C_INTR_MASTER_MASK_REG = OLED_SSD1306_1_I2C_I2C_DEFAULT_INTR_MASTER_MASK;
        OLED_SSD1306_1_I2C_INTR_RX_MASK_REG     = OLED_SSD1306_1_I2C_I2C_DEFAULT_INTR_RX_MASK;
        OLED_SSD1306_1_I2C_INTR_TX_MASK_REG     = OLED_SSD1306_1_I2C_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        OLED_SSD1306_1_I2C_state = OLED_SSD1306_1_I2C_I2C_FSM_IDLE;

    #if(OLED_SSD1306_1_I2C_I2C_SLAVE)
        /* Internal slave variable */
        OLED_SSD1306_1_I2C_slStatus        = 0u;
        OLED_SSD1306_1_I2C_slRdBufIndex    = 0u;
        OLED_SSD1306_1_I2C_slWrBufIndex    = 0u;
        OLED_SSD1306_1_I2C_slOverFlowCount = 0u;
    #endif /* (OLED_SSD1306_1_I2C_I2C_SLAVE) */

    #if(OLED_SSD1306_1_I2C_I2C_MASTER)
    /* Internal master variable */
        OLED_SSD1306_1_I2C_mstrStatus     = 0u;
        OLED_SSD1306_1_I2C_mstrRdBufIndex = 0u;
        OLED_SSD1306_1_I2C_mstrWrBufIndex = 0u;
    #endif /* (OLED_SSD1306_1_I2C_I2C_MASTER) */
    }
#endif /* (OLED_SSD1306_1_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: OLED_SSD1306_1_I2C_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void OLED_SSD1306_1_I2C_I2CStop(void)
{
    /* Clear command registers because they keep assigned value after IP block was disabled */
    OLED_SSD1306_1_I2C_I2C_MASTER_CMD_REG = 0u;
    OLED_SSD1306_1_I2C_I2C_SLAVE_CMD_REG  = 0u;
    
    OLED_SSD1306_1_I2C_state = OLED_SSD1306_1_I2C_I2C_FSM_IDLE;
}


/*******************************************************************************
* Function Name: OLED_SSD1306_1_I2C_I2CFwBlockReset
****************************************************************************//**
*
* Resets the scb IP block and I2C into the known state.
*
*******************************************************************************/
void OLED_SSD1306_1_I2C_I2CFwBlockReset(void)
{
    /* Disable scb IP: stop respond to I2C traffic */
    OLED_SSD1306_1_I2C_CTRL_REG &= (uint32) ~OLED_SSD1306_1_I2C_CTRL_ENABLED;

    /* Clear command registers they are not cleared after scb IP is disabled */
    OLED_SSD1306_1_I2C_I2C_MASTER_CMD_REG = 0u;
    OLED_SSD1306_1_I2C_I2C_SLAVE_CMD_REG  = 0u;

    OLED_SSD1306_1_I2C_DISABLE_AUTO_DATA;

    OLED_SSD1306_1_I2C_SetTxInterruptMode(OLED_SSD1306_1_I2C_NO_INTR_SOURCES);
    OLED_SSD1306_1_I2C_SetRxInterruptMode(OLED_SSD1306_1_I2C_NO_INTR_SOURCES);
    
#if(OLED_SSD1306_1_I2C_CY_SCBIP_V0)
    /* Clear interrupt sources as they are not cleared after scb IP is disabled */
    OLED_SSD1306_1_I2C_ClearTxInterruptSource    (OLED_SSD1306_1_I2C_INTR_TX_ALL);
    OLED_SSD1306_1_I2C_ClearRxInterruptSource    (OLED_SSD1306_1_I2C_INTR_RX_ALL);
    OLED_SSD1306_1_I2C_ClearSlaveInterruptSource (OLED_SSD1306_1_I2C_INTR_SLAVE_ALL);
    OLED_SSD1306_1_I2C_ClearMasterInterruptSource(OLED_SSD1306_1_I2C_INTR_MASTER_ALL);
#endif /* (OLED_SSD1306_1_I2C_CY_SCBIP_V0) */

    OLED_SSD1306_1_I2C_state = OLED_SSD1306_1_I2C_I2C_FSM_IDLE;

    /* Enable scb IP: start respond to I2C traffic */
    OLED_SSD1306_1_I2C_CTRL_REG |= (uint32) OLED_SSD1306_1_I2C_CTRL_ENABLED;
}


#if(OLED_SSD1306_1_I2C_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: OLED_SSD1306_1_I2C_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables OLED_SSD1306_1_I2C_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void OLED_SSD1306_1_I2C_I2CSaveConfig(void)
    {
    #if (!OLED_SSD1306_1_I2C_CY_SCBIP_V0)
        #if (OLED_SSD1306_1_I2C_I2C_MULTI_MASTER_SLAVE_CONST && OLED_SSD1306_1_I2C_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (OLED_SSD1306_1_I2C_CTRL_REG & OLED_SSD1306_1_I2C_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                OLED_SSD1306_1_I2C_Stop();
                OLED_SSD1306_1_I2C_CTRL_REG |= OLED_SSD1306_1_I2C_CTRL_EC_AM_MODE;
                OLED_SSD1306_1_I2C_Enable();
            }
        #endif /* (OLED_SSD1306_1_I2C_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (OLED_SSD1306_1_I2C_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            OLED_SSD1306_1_I2C_SCBCLK_Stop();
        #endif /* (OLED_SSD1306_1_I2C_SCB_CLK_INTERNAL) */
    #endif /* (!OLED_SSD1306_1_I2C_CY_SCBIP_V0) */

        OLED_SSD1306_1_I2C_SetI2CExtClkInterruptMode(OLED_SSD1306_1_I2C_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: OLED_SSD1306_1_I2C_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables OLED_SSD1306_1_I2C_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void OLED_SSD1306_1_I2C_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        OLED_SSD1306_1_I2C_SetI2CExtClkInterruptMode(OLED_SSD1306_1_I2C_NO_INTR_SOURCES);

    #if (!OLED_SSD1306_1_I2C_CY_SCBIP_V0)
        #if (OLED_SSD1306_1_I2C_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            OLED_SSD1306_1_I2C_SCBCLK_Start();
        #endif /* (OLED_SSD1306_1_I2C_SCB_CLK_INTERNAL) */
    #endif /* (!OLED_SSD1306_1_I2C_CY_SCBIP_V0) */
    }
#endif /* (OLED_SSD1306_1_I2C_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
