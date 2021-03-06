/***************************************************************************//**
* \file OLED_SSD1306_1_SSD1306_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "OLED_SSD1306_1_SSD1306.h"
#include "OLED_SSD1306_1_SSD1306_PVT.h"

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_INC)
    #include "OLED_SSD1306_1_SSD1306_I2C_PVT.h"
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_INC) */

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_INC)
    #include "OLED_SSD1306_1_SSD1306_EZI2C_PVT.h"
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_INC) */

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_INC || OLED_SSD1306_1_SSD1306_SCB_MODE_UART_INC)
    #include "OLED_SSD1306_1_SSD1306_SPI_UART_PVT.h"
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_INC || OLED_SSD1306_1_SSD1306_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG || \
   (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG   && (!OLED_SSD1306_1_SSD1306_I2C_WAKE_ENABLE_CONST))   || \
   (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG && (!OLED_SSD1306_1_SSD1306_EZI2C_WAKE_ENABLE_CONST)) || \
   (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_CONST_CFG   && (!OLED_SSD1306_1_SSD1306_SPI_WAKE_ENABLE_CONST))   || \
   (OLED_SSD1306_1_SSD1306_SCB_MODE_UART_CONST_CFG  && (!OLED_SSD1306_1_SSD1306_UART_WAKE_ENABLE_CONST)))

    OLED_SSD1306_1_SSD1306_BACKUP_STRUCT OLED_SSD1306_1_SSD1306_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: OLED_SSD1306_1_SSD1306_Sleep
****************************************************************************//**
*
*  Prepares the OLED_SSD1306_1_SSD1306 component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the OLED_SSD1306_1_SSD1306_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void OLED_SSD1306_1_SSD1306_Sleep(void)
{
#if(OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)

    if(OLED_SSD1306_1_SSD1306_SCB_WAKE_ENABLE_CHECK)
    {
        if(OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_I2CSaveConfig();
        }
        else if(OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_EzI2CSaveConfig();
        }
    #if(!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1)
        else if(OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_SpiSaveConfig();
        }
        else if(OLED_SSD1306_1_SSD1306_SCB_MODE_UART_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_UartSaveConfig();
        }
    #endif /* (!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        OLED_SSD1306_1_SSD1306_backup.enableState = (uint8) OLED_SSD1306_1_SSD1306_GET_CTRL_ENABLED;

        if(0u != OLED_SSD1306_1_SSD1306_backup.enableState)
        {
            OLED_SSD1306_1_SSD1306_Stop();
        }
    }

#else

    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG && OLED_SSD1306_1_SSD1306_I2C_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_I2CSaveConfig();

    #elif (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG && OLED_SSD1306_1_SSD1306_EZI2C_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_EzI2CSaveConfig();

    #elif (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_CONST_CFG && OLED_SSD1306_1_SSD1306_SPI_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_SpiSaveConfig();

    #elif (OLED_SSD1306_1_SSD1306_SCB_MODE_UART_CONST_CFG && OLED_SSD1306_1_SSD1306_UART_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_UartSaveConfig();

    #else

        OLED_SSD1306_1_SSD1306_backup.enableState = (uint8) OLED_SSD1306_1_SSD1306_GET_CTRL_ENABLED;

        if(0u != OLED_SSD1306_1_SSD1306_backup.enableState)
        {
            OLED_SSD1306_1_SSD1306_Stop();
        }

    #endif /* defined (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG) && (OLED_SSD1306_1_SSD1306_I2C_WAKE_ENABLE_CONST) */

#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: OLED_SSD1306_1_SSD1306_Wakeup
****************************************************************************//**
*
*  Prepares the OLED_SSD1306_1_SSD1306 component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the OLED_SSD1306_1_SSD1306_Wakeup() function without first calling the 
*   OLED_SSD1306_1_SSD1306_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void OLED_SSD1306_1_SSD1306_Wakeup(void)
{
#if(OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)

    if(OLED_SSD1306_1_SSD1306_SCB_WAKE_ENABLE_CHECK)
    {
        if(OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_I2CRestoreConfig();
        }
        else if(OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_EzI2CRestoreConfig();
        }
    #if(!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1)
        else if(OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_SpiRestoreConfig();
        }
        else if(OLED_SSD1306_1_SSD1306_SCB_MODE_UART_RUNTM_CFG)
        {
            OLED_SSD1306_1_SSD1306_UartRestoreConfig();
        }
    #endif /* (!OLED_SSD1306_1_SSD1306_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != OLED_SSD1306_1_SSD1306_backup.enableState)
        {
            OLED_SSD1306_1_SSD1306_Enable();
        }
    }

#else

    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG  && OLED_SSD1306_1_SSD1306_I2C_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_I2CRestoreConfig();

    #elif (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG && OLED_SSD1306_1_SSD1306_EZI2C_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_EzI2CRestoreConfig();

    #elif (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_CONST_CFG && OLED_SSD1306_1_SSD1306_SPI_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_SpiRestoreConfig();

    #elif (OLED_SSD1306_1_SSD1306_SCB_MODE_UART_CONST_CFG && OLED_SSD1306_1_SSD1306_UART_WAKE_ENABLE_CONST)
        OLED_SSD1306_1_SSD1306_UartRestoreConfig();

    #else

        if(0u != OLED_SSD1306_1_SSD1306_backup.enableState)
        {
            OLED_SSD1306_1_SSD1306_Enable();
        }

    #endif /* (OLED_SSD1306_1_SSD1306_I2C_WAKE_ENABLE_CONST) */

#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
