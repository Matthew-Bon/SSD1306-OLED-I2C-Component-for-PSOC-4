/*******************************************************************************
* File Name: bob_OLED_ADDR_Select.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_bob_OLED_ADDR_Select_ALIASES_H) /* Pins bob_OLED_ADDR_Select_ALIASES_H */
#define CY_PINS_bob_OLED_ADDR_Select_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define bob_OLED_ADDR_Select_0			(bob_OLED_ADDR_Select__0__PC)
#define bob_OLED_ADDR_Select_0_PS		(bob_OLED_ADDR_Select__0__PS)
#define bob_OLED_ADDR_Select_0_PC		(bob_OLED_ADDR_Select__0__PC)
#define bob_OLED_ADDR_Select_0_DR		(bob_OLED_ADDR_Select__0__DR)
#define bob_OLED_ADDR_Select_0_SHIFT	(bob_OLED_ADDR_Select__0__SHIFT)
#define bob_OLED_ADDR_Select_0_INTR	((uint16)((uint16)0x0003u << (bob_OLED_ADDR_Select__0__SHIFT*2u)))

#define bob_OLED_ADDR_Select_INTR_ALL	 ((uint16)(bob_OLED_ADDR_Select_0_INTR))


#endif /* End Pins bob_OLED_ADDR_Select_ALIASES_H */


/* [] END OF FILE */
