/***************************************************************************//**
* \file bob_I2C_I2C.c
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

#include "bob_I2C_PVT.h"
#include "bob_I2C_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 bob_I2C_state;  /* Current state of I2C FSM */

#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const bob_I2C_I2C_INIT_STRUCT bob_I2C_configI2C =
    {
        bob_I2C_I2C_MODE,
        bob_I2C_I2C_OVS_FACTOR_LOW,
        bob_I2C_I2C_OVS_FACTOR_HIGH,
        bob_I2C_I2C_MEDIAN_FILTER_ENABLE,
        bob_I2C_I2C_SLAVE_ADDRESS,
        bob_I2C_I2C_SLAVE_ADDRESS_MASK,
        bob_I2C_I2C_ACCEPT_ADDRESS,
        bob_I2C_I2C_WAKE_ENABLE,
        bob_I2C_I2C_BYTE_MODE_ENABLE,
        bob_I2C_I2C_DATA_RATE,
        bob_I2C_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: bob_I2C_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the bob_I2C for I2C operation.
    *
    *  This function is intended specifically to be used when the bob_I2C 
    *  configuration is set to “Unconfigured bob_I2C” in the customizer. 
    *  After initializing the bob_I2C in I2C mode using this function, 
    *  the component can be enabled using the bob_I2C_Start() or 
    * bob_I2C_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void bob_I2C_I2CInit(const bob_I2C_I2C_INIT_STRUCT *config)
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
            bob_I2C_SetPins(bob_I2C_SCB_MODE_I2C, bob_I2C_DUMMY_PARAM,
                                     bob_I2C_DUMMY_PARAM);

            /* Store internal configuration */
            bob_I2C_scbMode       = (uint8) bob_I2C_SCB_MODE_I2C;
            bob_I2C_scbEnableWake = (uint8) config->enableWake;
            bob_I2C_scbEnableIntr = (uint8) bob_I2C_SCB_IRQ_INTERNAL;

            bob_I2C_mode          = (uint8) config->mode;
            bob_I2C_acceptAddr    = (uint8) config->acceptAddr;

        #if (bob_I2C_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            bob_I2C_SET_I2C_CFG_SDA_FILT_TRIM(bob_I2C_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (bob_I2C_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((bob_I2C_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= bob_I2C_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (bob_I2C_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                bob_I2C_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = bob_I2C_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                bob_I2C_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = bob_I2C_DIGITAL_FILTER_ENABLE;
            }

        #if (!bob_I2C_CY_SCBIP_V0)
            locEnableWake = (bob_I2C_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!bob_I2C_CY_SCBIP_V0) */

            /* Configure I2C interface */
            bob_I2C_CTRL_REG     = bob_I2C_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            bob_I2C_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            bob_I2C_GET_CTRL_EC_AM_MODE (locEnableWake);

            bob_I2C_I2C_CTRL_REG = bob_I2C_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    bob_I2C_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    bob_I2C_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    bob_I2C_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            bob_I2C_RX_CTRL_REG      = bob_I2C_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                bob_I2C_I2C_RX_CTRL;
            bob_I2C_RX_FIFO_CTRL_REG = bob_I2C_CLEAR_REG;

            /* Set default address and mask */
            bob_I2C_RX_MATCH_REG    = ((bob_I2C_I2C_SLAVE) ?
                                                (bob_I2C_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 bob_I2C_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (bob_I2C_CLEAR_REG));


            /* Configure TX direction */
            bob_I2C_TX_CTRL_REG      = bob_I2C_I2C_TX_CTRL;
            bob_I2C_TX_FIFO_CTRL_REG = bob_I2C_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (bob_I2C_ISR_NUMBER);
            CyIntSetPriority(bob_I2C_ISR_NUMBER, bob_I2C_ISR_PRIORITY);
            (void) CyIntSetVector(bob_I2C_ISR_NUMBER, &bob_I2C_I2C_ISR);

            /* Configure interrupt sources */
        #if(!bob_I2C_CY_SCBIP_V1)
            bob_I2C_INTR_SPI_EC_MASK_REG = bob_I2C_NO_INTR_SOURCES;
        #endif /* (!bob_I2C_CY_SCBIP_V1) */

            bob_I2C_INTR_I2C_EC_MASK_REG = bob_I2C_NO_INTR_SOURCES;
            bob_I2C_INTR_RX_MASK_REG     = bob_I2C_NO_INTR_SOURCES;
            bob_I2C_INTR_TX_MASK_REG     = bob_I2C_NO_INTR_SOURCES;

            bob_I2C_INTR_SLAVE_MASK_REG  = ((bob_I2C_I2C_SLAVE) ?
                            (bob_I2C_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             bob_I2C_I2C_INTR_SLAVE_MASK) : (bob_I2C_CLEAR_REG));

            bob_I2C_INTR_MASTER_MASK_REG = bob_I2C_NO_INTR_SOURCES;

            /* Configure global variables */
            bob_I2C_state = bob_I2C_I2C_FSM_IDLE;

            /* Internal slave variables */
            bob_I2C_slStatus        = 0u;
            bob_I2C_slRdBufIndex    = 0u;
            bob_I2C_slWrBufIndex    = 0u;
            bob_I2C_slOverFlowCount = 0u;

            /* Internal master variables */
            bob_I2C_mstrStatus     = 0u;
            bob_I2C_mstrRdBufIndex = 0u;
            bob_I2C_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: bob_I2C_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void bob_I2C_I2CInit(void)
    {
    #if(bob_I2C_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        bob_I2C_SET_I2C_CFG_SDA_FILT_TRIM(bob_I2C_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (bob_I2C_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        bob_I2C_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        bob_I2C_CTRL_REG     = bob_I2C_I2C_DEFAULT_CTRL;
        bob_I2C_I2C_CTRL_REG = bob_I2C_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        bob_I2C_RX_CTRL_REG      = bob_I2C_I2C_DEFAULT_RX_CTRL;
        bob_I2C_RX_FIFO_CTRL_REG = bob_I2C_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        bob_I2C_RX_MATCH_REG     = bob_I2C_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        bob_I2C_TX_CTRL_REG      = bob_I2C_I2C_DEFAULT_TX_CTRL;
        bob_I2C_TX_FIFO_CTRL_REG = bob_I2C_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (bob_I2C_ISR_NUMBER);
        CyIntSetPriority(bob_I2C_ISR_NUMBER, bob_I2C_ISR_PRIORITY);
    #if(!bob_I2C_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(bob_I2C_ISR_NUMBER, &bob_I2C_I2C_ISR);
    #endif /* (bob_I2C_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!bob_I2C_CY_SCBIP_V1)
        bob_I2C_INTR_SPI_EC_MASK_REG = bob_I2C_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!bob_I2C_CY_SCBIP_V1) */

        bob_I2C_INTR_I2C_EC_MASK_REG = bob_I2C_I2C_DEFAULT_INTR_I2C_EC_MASK;
        bob_I2C_INTR_SLAVE_MASK_REG  = bob_I2C_I2C_DEFAULT_INTR_SLAVE_MASK;
        bob_I2C_INTR_MASTER_MASK_REG = bob_I2C_I2C_DEFAULT_INTR_MASTER_MASK;
        bob_I2C_INTR_RX_MASK_REG     = bob_I2C_I2C_DEFAULT_INTR_RX_MASK;
        bob_I2C_INTR_TX_MASK_REG     = bob_I2C_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        bob_I2C_state = bob_I2C_I2C_FSM_IDLE;

    #if(bob_I2C_I2C_SLAVE)
        /* Internal slave variable */
        bob_I2C_slStatus        = 0u;
        bob_I2C_slRdBufIndex    = 0u;
        bob_I2C_slWrBufIndex    = 0u;
        bob_I2C_slOverFlowCount = 0u;
    #endif /* (bob_I2C_I2C_SLAVE) */

    #if(bob_I2C_I2C_MASTER)
    /* Internal master variable */
        bob_I2C_mstrStatus     = 0u;
        bob_I2C_mstrRdBufIndex = 0u;
        bob_I2C_mstrWrBufIndex = 0u;
    #endif /* (bob_I2C_I2C_MASTER) */
    }
#endif /* (bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: bob_I2C_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void bob_I2C_I2CStop(void)
{
    /* Clear command registers because they keep assigned value after IP block was disabled */
    bob_I2C_I2C_MASTER_CMD_REG = 0u;
    bob_I2C_I2C_SLAVE_CMD_REG  = 0u;
    
    bob_I2C_state = bob_I2C_I2C_FSM_IDLE;
}


/*******************************************************************************
* Function Name: bob_I2C_I2CFwBlockReset
****************************************************************************//**
*
* Resets the scb IP block and I2C into the known state.
*
*******************************************************************************/
void bob_I2C_I2CFwBlockReset(void)
{
    /* Disable scb IP: stop respond to I2C traffic */
    bob_I2C_CTRL_REG &= (uint32) ~bob_I2C_CTRL_ENABLED;

    /* Clear command registers they are not cleared after scb IP is disabled */
    bob_I2C_I2C_MASTER_CMD_REG = 0u;
    bob_I2C_I2C_SLAVE_CMD_REG  = 0u;

    bob_I2C_DISABLE_AUTO_DATA;

    bob_I2C_SetTxInterruptMode(bob_I2C_NO_INTR_SOURCES);
    bob_I2C_SetRxInterruptMode(bob_I2C_NO_INTR_SOURCES);
    
#if(bob_I2C_CY_SCBIP_V0)
    /* Clear interrupt sources as they are not cleared after scb IP is disabled */
    bob_I2C_ClearTxInterruptSource    (bob_I2C_INTR_TX_ALL);
    bob_I2C_ClearRxInterruptSource    (bob_I2C_INTR_RX_ALL);
    bob_I2C_ClearSlaveInterruptSource (bob_I2C_INTR_SLAVE_ALL);
    bob_I2C_ClearMasterInterruptSource(bob_I2C_INTR_MASTER_ALL);
#endif /* (bob_I2C_CY_SCBIP_V0) */

    bob_I2C_state = bob_I2C_I2C_FSM_IDLE;

    /* Enable scb IP: start respond to I2C traffic */
    bob_I2C_CTRL_REG |= (uint32) bob_I2C_CTRL_ENABLED;
}


#if(bob_I2C_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: bob_I2C_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables bob_I2C_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void bob_I2C_I2CSaveConfig(void)
    {
    #if (!bob_I2C_CY_SCBIP_V0)
        #if (bob_I2C_I2C_MULTI_MASTER_SLAVE_CONST && bob_I2C_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (bob_I2C_CTRL_REG & bob_I2C_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                bob_I2C_Stop();
                bob_I2C_CTRL_REG |= bob_I2C_CTRL_EC_AM_MODE;
                bob_I2C_Enable();
            }
        #endif /* (bob_I2C_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (bob_I2C_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            bob_I2C_SCBCLK_Stop();
        #endif /* (bob_I2C_SCB_CLK_INTERNAL) */
    #endif /* (!bob_I2C_CY_SCBIP_V0) */

        bob_I2C_SetI2CExtClkInterruptMode(bob_I2C_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: bob_I2C_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables bob_I2C_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void bob_I2C_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        bob_I2C_SetI2CExtClkInterruptMode(bob_I2C_NO_INTR_SOURCES);

    #if (!bob_I2C_CY_SCBIP_V0)
        #if (bob_I2C_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            bob_I2C_SCBCLK_Start();
        #endif /* (bob_I2C_SCB_CLK_INTERNAL) */
    #endif /* (!bob_I2C_CY_SCBIP_V0) */
    }
#endif /* (bob_I2C_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
