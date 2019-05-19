/*******************************************************************************
* File Name: OLED_SSD1306_1_OLED_ADDR_Select.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "OLED_SSD1306_1_OLED_ADDR_Select.h"

static OLED_SSD1306_1_OLED_ADDR_Select_BACKUP_STRUCT  OLED_SSD1306_1_OLED_ADDR_Select_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: OLED_SSD1306_1_OLED_ADDR_Select_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet OLED_SSD1306_1_OLED_ADDR_Select_SUT.c usage_OLED_SSD1306_1_OLED_ADDR_Select_Sleep_Wakeup
*******************************************************************************/
void OLED_SSD1306_1_OLED_ADDR_Select_Sleep(void)
{
    #if defined(OLED_SSD1306_1_OLED_ADDR_Select__PC)
        OLED_SSD1306_1_OLED_ADDR_Select_backup.pcState = OLED_SSD1306_1_OLED_ADDR_Select_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            OLED_SSD1306_1_OLED_ADDR_Select_backup.usbState = OLED_SSD1306_1_OLED_ADDR_Select_CR1_REG;
            OLED_SSD1306_1_OLED_ADDR_Select_USB_POWER_REG |= OLED_SSD1306_1_OLED_ADDR_Select_USBIO_ENTER_SLEEP;
            OLED_SSD1306_1_OLED_ADDR_Select_CR1_REG &= OLED_SSD1306_1_OLED_ADDR_Select_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(OLED_SSD1306_1_OLED_ADDR_Select__SIO)
        OLED_SSD1306_1_OLED_ADDR_Select_backup.sioState = OLED_SSD1306_1_OLED_ADDR_Select_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        OLED_SSD1306_1_OLED_ADDR_Select_SIO_REG &= (uint32)(~OLED_SSD1306_1_OLED_ADDR_Select_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: OLED_SSD1306_1_OLED_ADDR_Select_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to OLED_SSD1306_1_OLED_ADDR_Select_Sleep() for an example usage.
*******************************************************************************/
void OLED_SSD1306_1_OLED_ADDR_Select_Wakeup(void)
{
    #if defined(OLED_SSD1306_1_OLED_ADDR_Select__PC)
        OLED_SSD1306_1_OLED_ADDR_Select_PC = OLED_SSD1306_1_OLED_ADDR_Select_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            OLED_SSD1306_1_OLED_ADDR_Select_USB_POWER_REG &= OLED_SSD1306_1_OLED_ADDR_Select_USBIO_EXIT_SLEEP_PH1;
            OLED_SSD1306_1_OLED_ADDR_Select_CR1_REG = OLED_SSD1306_1_OLED_ADDR_Select_backup.usbState;
            OLED_SSD1306_1_OLED_ADDR_Select_USB_POWER_REG &= OLED_SSD1306_1_OLED_ADDR_Select_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(OLED_SSD1306_1_OLED_ADDR_Select__SIO)
        OLED_SSD1306_1_OLED_ADDR_Select_SIO_REG = OLED_SSD1306_1_OLED_ADDR_Select_backup.sioState;
    #endif
}


/* [] END OF FILE */
