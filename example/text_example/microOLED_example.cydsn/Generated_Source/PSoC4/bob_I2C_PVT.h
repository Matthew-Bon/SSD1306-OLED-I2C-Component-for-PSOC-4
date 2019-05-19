/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_bob_I2C_H)
#define CY_SCB_PVT_bob_I2C_H

#include "bob_I2C.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define bob_I2C_SetI2CExtClkInterruptMode(interruptMask) bob_I2C_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define bob_I2C_ClearI2CExtClkInterruptSource(interruptMask) bob_I2C_CLEAR_INTR_I2C_EC(interruptMask)
#define bob_I2C_GetI2CExtClkInterruptSource()                (bob_I2C_INTR_I2C_EC_REG)
#define bob_I2C_GetI2CExtClkInterruptMode()                  (bob_I2C_INTR_I2C_EC_MASK_REG)
#define bob_I2C_GetI2CExtClkInterruptSourceMasked()          (bob_I2C_INTR_I2C_EC_MASKED_REG)

#if (!bob_I2C_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define bob_I2C_SetSpiExtClkInterruptMode(interruptMask) \
                                                                bob_I2C_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define bob_I2C_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                bob_I2C_CLEAR_INTR_SPI_EC(interruptMask)
    #define bob_I2C_GetExtSpiClkInterruptSource()                 (bob_I2C_INTR_SPI_EC_REG)
    #define bob_I2C_GetExtSpiClkInterruptMode()                   (bob_I2C_INTR_SPI_EC_MASK_REG)
    #define bob_I2C_GetExtSpiClkInterruptSourceMasked()           (bob_I2C_INTR_SPI_EC_MASKED_REG)
#endif /* (!bob_I2C_CY_SCBIP_V1) */

#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void bob_I2C_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (bob_I2C_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_bob_I2C_CUSTOM_INTR_HANDLER)
    extern cyisraddress bob_I2C_customIntrHandler;
#endif /* !defined (CY_REMOVE_bob_I2C_CUSTOM_INTR_HANDLER) */
#endif /* (bob_I2C_SCB_IRQ_INTERNAL) */

extern bob_I2C_BACKUP_STRUCT bob_I2C_backup;

#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 bob_I2C_scbMode;
    extern uint8 bob_I2C_scbEnableWake;
    extern uint8 bob_I2C_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 bob_I2C_mode;
    extern uint8 bob_I2C_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * bob_I2C_rxBuffer;
    extern uint8   bob_I2C_rxDataBits;
    extern uint32  bob_I2C_rxBufferSize;

    extern volatile uint8 * bob_I2C_txBuffer;
    extern uint8   bob_I2C_txDataBits;
    extern uint32  bob_I2C_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 bob_I2C_numberOfAddr;
    extern uint8 bob_I2C_subAddrSize;
#endif /* (bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (bob_I2C_SCB_MODE_I2C_CONST_CFG || \
        bob_I2C_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 bob_I2C_IntrTxMask;
#endif /* (! (bob_I2C_SCB_MODE_I2C_CONST_CFG || \
              bob_I2C_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define bob_I2C_SCB_MODE_I2C_RUNTM_CFG     (bob_I2C_SCB_MODE_I2C      == bob_I2C_scbMode)
    #define bob_I2C_SCB_MODE_SPI_RUNTM_CFG     (bob_I2C_SCB_MODE_SPI      == bob_I2C_scbMode)
    #define bob_I2C_SCB_MODE_UART_RUNTM_CFG    (bob_I2C_SCB_MODE_UART     == bob_I2C_scbMode)
    #define bob_I2C_SCB_MODE_EZI2C_RUNTM_CFG   (bob_I2C_SCB_MODE_EZI2C    == bob_I2C_scbMode)
    #define bob_I2C_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (bob_I2C_SCB_MODE_UNCONFIG == bob_I2C_scbMode)

    /* Defines wakeup enable */
    #define bob_I2C_SCB_WAKE_ENABLE_CHECK       (0u != bob_I2C_scbEnableWake)
#endif /* (bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!bob_I2C_CY_SCBIP_V1)
    #define bob_I2C_SCB_PINS_NUMBER    (7u)
#else
    #define bob_I2C_SCB_PINS_NUMBER    (2u)
#endif /* (!bob_I2C_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_bob_I2C_H) */


/* [] END OF FILE */
