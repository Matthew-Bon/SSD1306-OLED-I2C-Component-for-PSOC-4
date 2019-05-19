/***************************************************************************//**
* \file bob_I2C_PM.c
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

#include "bob_I2C.h"
#include "bob_I2C_PVT.h"

#if(bob_I2C_SCB_MODE_I2C_INC)
    #include "bob_I2C_I2C_PVT.h"
#endif /* (bob_I2C_SCB_MODE_I2C_INC) */

#if(bob_I2C_SCB_MODE_EZI2C_INC)
    #include "bob_I2C_EZI2C_PVT.h"
#endif /* (bob_I2C_SCB_MODE_EZI2C_INC) */

#if(bob_I2C_SCB_MODE_SPI_INC || bob_I2C_SCB_MODE_UART_INC)
    #include "bob_I2C_SPI_UART_PVT.h"
#endif /* (bob_I2C_SCB_MODE_SPI_INC || bob_I2C_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG || \
   (bob_I2C_SCB_MODE_I2C_CONST_CFG   && (!bob_I2C_I2C_WAKE_ENABLE_CONST))   || \
   (bob_I2C_SCB_MODE_EZI2C_CONST_CFG && (!bob_I2C_EZI2C_WAKE_ENABLE_CONST)) || \
   (bob_I2C_SCB_MODE_SPI_CONST_CFG   && (!bob_I2C_SPI_WAKE_ENABLE_CONST))   || \
   (bob_I2C_SCB_MODE_UART_CONST_CFG  && (!bob_I2C_UART_WAKE_ENABLE_CONST)))

    bob_I2C_BACKUP_STRUCT bob_I2C_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: bob_I2C_Sleep
****************************************************************************//**
*
*  Prepares the bob_I2C component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the bob_I2C_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void bob_I2C_Sleep(void)
{
#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG)

    if(bob_I2C_SCB_WAKE_ENABLE_CHECK)
    {
        if(bob_I2C_SCB_MODE_I2C_RUNTM_CFG)
        {
            bob_I2C_I2CSaveConfig();
        }
        else if(bob_I2C_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            bob_I2C_EzI2CSaveConfig();
        }
    #if(!bob_I2C_CY_SCBIP_V1)
        else if(bob_I2C_SCB_MODE_SPI_RUNTM_CFG)
        {
            bob_I2C_SpiSaveConfig();
        }
        else if(bob_I2C_SCB_MODE_UART_RUNTM_CFG)
        {
            bob_I2C_UartSaveConfig();
        }
    #endif /* (!bob_I2C_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        bob_I2C_backup.enableState = (uint8) bob_I2C_GET_CTRL_ENABLED;

        if(0u != bob_I2C_backup.enableState)
        {
            bob_I2C_Stop();
        }
    }

#else

    #if (bob_I2C_SCB_MODE_I2C_CONST_CFG && bob_I2C_I2C_WAKE_ENABLE_CONST)
        bob_I2C_I2CSaveConfig();

    #elif (bob_I2C_SCB_MODE_EZI2C_CONST_CFG && bob_I2C_EZI2C_WAKE_ENABLE_CONST)
        bob_I2C_EzI2CSaveConfig();

    #elif (bob_I2C_SCB_MODE_SPI_CONST_CFG && bob_I2C_SPI_WAKE_ENABLE_CONST)
        bob_I2C_SpiSaveConfig();

    #elif (bob_I2C_SCB_MODE_UART_CONST_CFG && bob_I2C_UART_WAKE_ENABLE_CONST)
        bob_I2C_UartSaveConfig();

    #else

        bob_I2C_backup.enableState = (uint8) bob_I2C_GET_CTRL_ENABLED;

        if(0u != bob_I2C_backup.enableState)
        {
            bob_I2C_Stop();
        }

    #endif /* defined (bob_I2C_SCB_MODE_I2C_CONST_CFG) && (bob_I2C_I2C_WAKE_ENABLE_CONST) */

#endif /* (bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: bob_I2C_Wakeup
****************************************************************************//**
*
*  Prepares the bob_I2C component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the bob_I2C_Wakeup() function without first calling the 
*   bob_I2C_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void bob_I2C_Wakeup(void)
{
#if(bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG)

    if(bob_I2C_SCB_WAKE_ENABLE_CHECK)
    {
        if(bob_I2C_SCB_MODE_I2C_RUNTM_CFG)
        {
            bob_I2C_I2CRestoreConfig();
        }
        else if(bob_I2C_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            bob_I2C_EzI2CRestoreConfig();
        }
    #if(!bob_I2C_CY_SCBIP_V1)
        else if(bob_I2C_SCB_MODE_SPI_RUNTM_CFG)
        {
            bob_I2C_SpiRestoreConfig();
        }
        else if(bob_I2C_SCB_MODE_UART_RUNTM_CFG)
        {
            bob_I2C_UartRestoreConfig();
        }
    #endif /* (!bob_I2C_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != bob_I2C_backup.enableState)
        {
            bob_I2C_Enable();
        }
    }

#else

    #if (bob_I2C_SCB_MODE_I2C_CONST_CFG  && bob_I2C_I2C_WAKE_ENABLE_CONST)
        bob_I2C_I2CRestoreConfig();

    #elif (bob_I2C_SCB_MODE_EZI2C_CONST_CFG && bob_I2C_EZI2C_WAKE_ENABLE_CONST)
        bob_I2C_EzI2CRestoreConfig();

    #elif (bob_I2C_SCB_MODE_SPI_CONST_CFG && bob_I2C_SPI_WAKE_ENABLE_CONST)
        bob_I2C_SpiRestoreConfig();

    #elif (bob_I2C_SCB_MODE_UART_CONST_CFG && bob_I2C_UART_WAKE_ENABLE_CONST)
        bob_I2C_UartRestoreConfig();

    #else

        if(0u != bob_I2C_backup.enableState)
        {
            bob_I2C_Enable();
        }

    #endif /* (bob_I2C_I2C_WAKE_ENABLE_CONST) */

#endif /* (bob_I2C_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
