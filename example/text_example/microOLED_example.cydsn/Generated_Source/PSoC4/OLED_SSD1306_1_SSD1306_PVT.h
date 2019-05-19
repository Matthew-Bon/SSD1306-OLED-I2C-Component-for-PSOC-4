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

#if !defined(CY_SCB_PVT_OLED_SSD1306_1_SSD1306_H)
#define CY_SCB_PVT_OLED_SSD1306_1_SSD1306_H

#include "OLED_SSD1306_1_SSD1306.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define OLED_SSD1306_1_SSD1306_SetI2CExtClkInterruptMode(interruptMask) OLED_SSD1306_1_SSD1306_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define OLED_SSD1306_1_SSD1306_ClearI2CExtClkInterruptSource(interruptMask) OLED_SSD1306_1_SSD1306_CLEAR_INTR_I2C_EC(interruptMask)
#define OLED_SSD1306_1_SSD1306_GetI2CExtClkInterruptSource()                (OLED_SSD1306_1_SSD1306_INTR_I2C_EC_REG)
#define OLED_SSD1306_1_SSD1306_GetI2CExtClkInterruptMode()                  (OLED_SSD1306_1_SSD1306_INTR_I2C_EC_MASK_REG)
#define OLED_SSD1306_1_SSD1306_GetI2CExtClkInterruptSourceMasked()          (OLED_SSD1306_1_SSD1306_INTR_I2C_EC_MASKED_REG)

#if (!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define OLED_SSD1306_1_SSD1306_SetSpiExtClkInterruptMode(interruptMask) \
                                                                OLED_SSD1306_1_SSD1306_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define OLED_SSD1306_1_SSD1306_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                OLED_SSD1306_1_SSD1306_CLEAR_INTR_SPI_EC(interruptMask)
    #define OLED_SSD1306_1_SSD1306_GetExtSpiClkInterruptSource()                 (OLED_SSD1306_1_SSD1306_INTR_SPI_EC_REG)
    #define OLED_SSD1306_1_SSD1306_GetExtSpiClkInterruptMode()                   (OLED_SSD1306_1_SSD1306_INTR_SPI_EC_MASK_REG)
    #define OLED_SSD1306_1_SSD1306_GetExtSpiClkInterruptSourceMasked()           (OLED_SSD1306_1_SSD1306_INTR_SPI_EC_MASKED_REG)
#endif /* (!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1) */

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void OLED_SSD1306_1_SSD1306_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (OLED_SSD1306_1_SSD1306_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_OLED_SSD1306_1_SSD1306_CUSTOM_INTR_HANDLER)
    extern cyisraddress OLED_SSD1306_1_SSD1306_customIntrHandler;
#endif /* !defined (CY_REMOVE_OLED_SSD1306_1_SSD1306_CUSTOM_INTR_HANDLER) */
#endif /* (OLED_SSD1306_1_SSD1306_SCB_IRQ_INTERNAL) */

extern OLED_SSD1306_1_SSD1306_BACKUP_STRUCT OLED_SSD1306_1_SSD1306_backup;

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 OLED_SSD1306_1_SSD1306_scbMode;
    extern uint8 OLED_SSD1306_1_SSD1306_scbEnableWake;
    extern uint8 OLED_SSD1306_1_SSD1306_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 OLED_SSD1306_1_SSD1306_mode;
    extern uint8 OLED_SSD1306_1_SSD1306_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * OLED_SSD1306_1_SSD1306_rxBuffer;
    extern uint8   OLED_SSD1306_1_SSD1306_rxDataBits;
    extern uint32  OLED_SSD1306_1_SSD1306_rxBufferSize;

    extern volatile uint8 * OLED_SSD1306_1_SSD1306_txBuffer;
    extern uint8   OLED_SSD1306_1_SSD1306_txDataBits;
    extern uint32  OLED_SSD1306_1_SSD1306_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 OLED_SSD1306_1_SSD1306_numberOfAddr;
    extern uint8 OLED_SSD1306_1_SSD1306_subAddrSize;
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG || \
        OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 OLED_SSD1306_1_SSD1306_IntrTxMask;
#endif /* (! (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG || \
              OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_RUNTM_CFG     (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C      == OLED_SSD1306_1_SSD1306_scbMode)
    #define OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_RUNTM_CFG     (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI      == OLED_SSD1306_1_SSD1306_scbMode)
    #define OLED_SSD1306_1_SSD1306_SCB_MODE_UART_RUNTM_CFG    (OLED_SSD1306_1_SSD1306_SCB_MODE_UART     == OLED_SSD1306_1_SSD1306_scbMode)
    #define OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_RUNTM_CFG   (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C    == OLED_SSD1306_1_SSD1306_scbMode)
    #define OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG == OLED_SSD1306_1_SSD1306_scbMode)

    /* Defines wakeup enable */
    #define OLED_SSD1306_1_SSD1306_SCB_WAKE_ENABLE_CHECK       (0u != OLED_SSD1306_1_SSD1306_scbEnableWake)
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1)
    #define OLED_SSD1306_1_SSD1306_SCB_PINS_NUMBER    (7u)
#else
    #define OLED_SSD1306_1_SSD1306_SCB_PINS_NUMBER    (2u)
#endif /* (!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_OLED_SSD1306_1_SSD1306_H) */


/* [] END OF FILE */
