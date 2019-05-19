/*******************************************************************************
* File Name: bob_OLED_ADDR_Select.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_bob_OLED_ADDR_Select_H) /* Pins bob_OLED_ADDR_Select_H */
#define CY_PINS_bob_OLED_ADDR_Select_H

#include "cytypes.h"
#include "cyfitter.h"
#include "bob_OLED_ADDR_Select_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} bob_OLED_ADDR_Select_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   bob_OLED_ADDR_Select_Read(void);
void    bob_OLED_ADDR_Select_Write(uint8 value);
uint8   bob_OLED_ADDR_Select_ReadDataReg(void);
#if defined(bob_OLED_ADDR_Select__PC) || (CY_PSOC4_4200L) 
    void    bob_OLED_ADDR_Select_SetDriveMode(uint8 mode);
#endif
void    bob_OLED_ADDR_Select_SetInterruptMode(uint16 position, uint16 mode);
uint8   bob_OLED_ADDR_Select_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void bob_OLED_ADDR_Select_Sleep(void); 
void bob_OLED_ADDR_Select_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(bob_OLED_ADDR_Select__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define bob_OLED_ADDR_Select_DRIVE_MODE_BITS        (3)
    #define bob_OLED_ADDR_Select_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - bob_OLED_ADDR_Select_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the bob_OLED_ADDR_Select_SetDriveMode() function.
         *  @{
         */
        #define bob_OLED_ADDR_Select_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define bob_OLED_ADDR_Select_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define bob_OLED_ADDR_Select_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define bob_OLED_ADDR_Select_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define bob_OLED_ADDR_Select_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define bob_OLED_ADDR_Select_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define bob_OLED_ADDR_Select_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define bob_OLED_ADDR_Select_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define bob_OLED_ADDR_Select_MASK               bob_OLED_ADDR_Select__MASK
#define bob_OLED_ADDR_Select_SHIFT              bob_OLED_ADDR_Select__SHIFT
#define bob_OLED_ADDR_Select_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in bob_OLED_ADDR_Select_SetInterruptMode() function.
     *  @{
     */
        #define bob_OLED_ADDR_Select_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define bob_OLED_ADDR_Select_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define bob_OLED_ADDR_Select_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define bob_OLED_ADDR_Select_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(bob_OLED_ADDR_Select__SIO)
    #define bob_OLED_ADDR_Select_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(bob_OLED_ADDR_Select__PC) && (CY_PSOC4_4200L)
    #define bob_OLED_ADDR_Select_USBIO_ENABLE               ((uint32)0x80000000u)
    #define bob_OLED_ADDR_Select_USBIO_DISABLE              ((uint32)(~bob_OLED_ADDR_Select_USBIO_ENABLE))
    #define bob_OLED_ADDR_Select_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define bob_OLED_ADDR_Select_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define bob_OLED_ADDR_Select_USBIO_ENTER_SLEEP          ((uint32)((1u << bob_OLED_ADDR_Select_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << bob_OLED_ADDR_Select_USBIO_SUSPEND_DEL_SHIFT)))
    #define bob_OLED_ADDR_Select_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << bob_OLED_ADDR_Select_USBIO_SUSPEND_SHIFT)))
    #define bob_OLED_ADDR_Select_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << bob_OLED_ADDR_Select_USBIO_SUSPEND_DEL_SHIFT)))
    #define bob_OLED_ADDR_Select_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(bob_OLED_ADDR_Select__PC)
    /* Port Configuration */
    #define bob_OLED_ADDR_Select_PC                 (* (reg32 *) bob_OLED_ADDR_Select__PC)
#endif
/* Pin State */
#define bob_OLED_ADDR_Select_PS                     (* (reg32 *) bob_OLED_ADDR_Select__PS)
/* Data Register */
#define bob_OLED_ADDR_Select_DR                     (* (reg32 *) bob_OLED_ADDR_Select__DR)
/* Input Buffer Disable Override */
#define bob_OLED_ADDR_Select_INP_DIS                (* (reg32 *) bob_OLED_ADDR_Select__PC2)

/* Interrupt configuration Registers */
#define bob_OLED_ADDR_Select_INTCFG                 (* (reg32 *) bob_OLED_ADDR_Select__INTCFG)
#define bob_OLED_ADDR_Select_INTSTAT                (* (reg32 *) bob_OLED_ADDR_Select__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define bob_OLED_ADDR_Select_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(bob_OLED_ADDR_Select__SIO)
    #define bob_OLED_ADDR_Select_SIO_REG            (* (reg32 *) bob_OLED_ADDR_Select__SIO)
#endif /* (bob_OLED_ADDR_Select__SIO_CFG) */

/* USBIO registers */
#if !defined(bob_OLED_ADDR_Select__PC) && (CY_PSOC4_4200L)
    #define bob_OLED_ADDR_Select_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define bob_OLED_ADDR_Select_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define bob_OLED_ADDR_Select_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define bob_OLED_ADDR_Select_DRIVE_MODE_SHIFT       (0x00u)
#define bob_OLED_ADDR_Select_DRIVE_MODE_MASK        (0x07u << bob_OLED_ADDR_Select_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins bob_OLED_ADDR_Select_H */


/* [] END OF FILE */
