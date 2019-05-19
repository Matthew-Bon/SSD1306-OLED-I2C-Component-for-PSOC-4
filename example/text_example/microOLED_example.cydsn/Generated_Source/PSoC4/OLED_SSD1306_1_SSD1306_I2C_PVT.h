/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
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

#if !defined(CY_SCB_I2C_PVT_OLED_SSD1306_1_SSD1306_H)
#define CY_SCB_I2C_PVT_OLED_SSD1306_1_SSD1306_H

#include "OLED_SSD1306_1_SSD1306_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 OLED_SSD1306_1_SSD1306_state; /* Current state of I2C FSM */

#if(OLED_SSD1306_1_SSD1306_I2C_SLAVE_CONST)
    extern volatile uint8 OLED_SSD1306_1_SSD1306_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * OLED_SSD1306_1_SSD1306_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * OLED_SSD1306_1_SSD1306_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   OLED_SSD1306_1_SSD1306_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (OLED_SSD1306_1_SSD1306_I2C_SLAVE_CONST) */

#if(OLED_SSD1306_1_SSD1306_I2C_MASTER_CONST)
    extern volatile uint16 OLED_SSD1306_1_SSD1306_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  OLED_SSD1306_1_SSD1306_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * OLED_SSD1306_1_SSD1306_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * OLED_SSD1306_1_SSD1306_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  OLED_SSD1306_1_SSD1306_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (OLED_SSD1306_1_SSD1306_I2C_MASTER_CONST) */

#if (OLED_SSD1306_1_SSD1306_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    extern uint32 (*OLED_SSD1306_1_SSD1306_customAddressHandler) (void);
#endif /* (OLED_SSD1306_1_SSD1306_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG)
    void OLED_SSD1306_1_SSD1306_I2CInit(void);
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG) */

void OLED_SSD1306_1_SSD1306_I2CStop(void);
void OLED_SSD1306_1_SSD1306_I2CFwBlockReset(void);

void OLED_SSD1306_1_SSD1306_I2CSaveConfig(void);
void OLED_SSD1306_1_SSD1306_I2CRestoreConfig(void);

#if(OLED_SSD1306_1_SSD1306_I2C_MASTER_CONST)
    void OLED_SSD1306_1_SSD1306_I2CReStartGeneration(void);
#endif /* (OLED_SSD1306_1_SSD1306_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_OLED_SSD1306_1_SSD1306_H) */


/* [] END OF FILE */
