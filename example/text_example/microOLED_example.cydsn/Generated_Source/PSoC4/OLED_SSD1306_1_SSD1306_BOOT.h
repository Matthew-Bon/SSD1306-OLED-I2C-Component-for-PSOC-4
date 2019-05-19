/***************************************************************************//**
* \file OLED_SSD1306_1_SSD1306_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_OLED_SSD1306_1_SSD1306_H)
#define CY_SCB_BOOT_OLED_SSD1306_1_SSD1306_H

#include "OLED_SSD1306_1_SSD1306_PVT.h"

#if (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_INC)
    #include "OLED_SSD1306_1_SSD1306_I2C.h"
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_INC) */

#if (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_INC)
    #include "OLED_SSD1306_1_SSD1306_EZI2C.h"
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_INC) */

#if (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_INC || OLED_SSD1306_1_SSD1306_SCB_MODE_UART_INC)
    #include "OLED_SSD1306_1_SSD1306_SPI_UART.h"
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_INC || OLED_SSD1306_1_SSD1306_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_OLED_SSD1306_1_SSD1306) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_INC)
    #define OLED_SSD1306_1_SSD1306_I2C_BTLDR_COMM_ENABLED     (OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED && \
                                                            (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             OLED_SSD1306_1_SSD1306_I2C_SLAVE_CONST))
#else
     #define OLED_SSD1306_1_SSD1306_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_INC)
    #define OLED_SSD1306_1_SSD1306_EZI2C_BTLDR_COMM_ENABLED   (OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED && \
                                                         OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define OLED_SSD1306_1_SSD1306_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (OLED_SSD1306_1_SSD1306_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_INC)
    #define OLED_SSD1306_1_SSD1306_SPI_BTLDR_COMM_ENABLED     (OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED && \
                                                            (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             OLED_SSD1306_1_SSD1306_SPI_SLAVE_CONST))
#else
        #define OLED_SSD1306_1_SSD1306_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (OLED_SSD1306_1_SSD1306_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (OLED_SSD1306_1_SSD1306_SCB_MODE_UART_INC)
       #define OLED_SSD1306_1_SSD1306_UART_BTLDR_COMM_ENABLED    (OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED && \
                                                            (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (OLED_SSD1306_1_SSD1306_UART_RX_DIRECTION && \
                                                              OLED_SSD1306_1_SSD1306_UART_TX_DIRECTION)))
#else
     #define OLED_SSD1306_1_SSD1306_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (OLED_SSD1306_1_SSD1306_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define OLED_SSD1306_1_SSD1306_BTLDR_COMM_MODE_ENABLED    (OLED_SSD1306_1_SSD1306_I2C_BTLDR_COMM_ENABLED   || \
                                                     OLED_SSD1306_1_SSD1306_SPI_BTLDR_COMM_ENABLED   || \
                                                     OLED_SSD1306_1_SSD1306_EZI2C_BTLDR_COMM_ENABLED || \
                                                     OLED_SSD1306_1_SSD1306_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void OLED_SSD1306_1_SSD1306_I2CCyBtldrCommStart(void);
    void OLED_SSD1306_1_SSD1306_I2CCyBtldrCommStop (void);
    void OLED_SSD1306_1_SSD1306_I2CCyBtldrCommReset(void);
    cystatus OLED_SSD1306_1_SSD1306_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus OLED_SSD1306_1_SSD1306_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG)
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStart   OLED_SSD1306_1_SSD1306_I2CCyBtldrCommStart
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStop    OLED_SSD1306_1_SSD1306_I2CCyBtldrCommStop
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommReset   OLED_SSD1306_1_SSD1306_I2CCyBtldrCommReset
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommRead    OLED_SSD1306_1_SSD1306_I2CCyBtldrCommRead
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommWrite   OLED_SSD1306_1_SSD1306_I2CCyBtldrCommWrite
    #endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommStart(void);
    void OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommStop (void);
    void OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommReset(void);
    cystatus OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG)
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStart   OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommStart
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStop    OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommStop
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommReset   OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommReset
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommRead    OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommRead
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommWrite   OLED_SSD1306_1_SSD1306_EzI2CCyBtldrCommWrite
    #endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void OLED_SSD1306_1_SSD1306_SpiCyBtldrCommStart(void);
    void OLED_SSD1306_1_SSD1306_SpiCyBtldrCommStop (void);
    void OLED_SSD1306_1_SSD1306_SpiCyBtldrCommReset(void);
    cystatus OLED_SSD1306_1_SSD1306_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus OLED_SSD1306_1_SSD1306_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_CONST_CFG)
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStart   OLED_SSD1306_1_SSD1306_SpiCyBtldrCommStart
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStop    OLED_SSD1306_1_SSD1306_SpiCyBtldrCommStop
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommReset   OLED_SSD1306_1_SSD1306_SpiCyBtldrCommReset
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommRead    OLED_SSD1306_1_SSD1306_SpiCyBtldrCommRead
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommWrite   OLED_SSD1306_1_SSD1306_SpiCyBtldrCommWrite
    #endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void OLED_SSD1306_1_SSD1306_UartCyBtldrCommStart(void);
    void OLED_SSD1306_1_SSD1306_UartCyBtldrCommStop (void);
    void OLED_SSD1306_1_SSD1306_UartCyBtldrCommReset(void);
    cystatus OLED_SSD1306_1_SSD1306_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus OLED_SSD1306_1_SSD1306_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_UART_CONST_CFG)
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStart   OLED_SSD1306_1_SSD1306_UartCyBtldrCommStart
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommStop    OLED_SSD1306_1_SSD1306_UartCyBtldrCommStop
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommReset   OLED_SSD1306_1_SSD1306_UartCyBtldrCommReset
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommRead    OLED_SSD1306_1_SSD1306_UartCyBtldrCommRead
        #define OLED_SSD1306_1_SSD1306_CyBtldrCommWrite   OLED_SSD1306_1_SSD1306_UartCyBtldrCommWrite
    #endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED)
    #if (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void OLED_SSD1306_1_SSD1306_CyBtldrCommStart(void);
        void OLED_SSD1306_1_SSD1306_CyBtldrCommStop (void);
        void OLED_SSD1306_1_SSD1306_CyBtldrCommReset(void);
        cystatus OLED_SSD1306_1_SSD1306_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus OLED_SSD1306_1_SSD1306_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (OLED_SSD1306_1_SSD1306_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_OLED_SSD1306_1_SSD1306)
        #define CyBtldrCommStart    OLED_SSD1306_1_SSD1306_CyBtldrCommStart
        #define CyBtldrCommStop     OLED_SSD1306_1_SSD1306_CyBtldrCommStop
        #define CyBtldrCommReset    OLED_SSD1306_1_SSD1306_CyBtldrCommReset
        #define CyBtldrCommWrite    OLED_SSD1306_1_SSD1306_CyBtldrCommWrite
        #define CyBtldrCommRead     OLED_SSD1306_1_SSD1306_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_OLED_SSD1306_1_SSD1306) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (OLED_SSD1306_1_SSD1306_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define OLED_SSD1306_1_SSD1306_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define OLED_SSD1306_1_SSD1306_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define OLED_SSD1306_1_SSD1306_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define OLED_SSD1306_1_SSD1306_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef OLED_SSD1306_1_SSD1306_SPI_BYTE_TO_BYTE
    #define OLED_SSD1306_1_SSD1306_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef OLED_SSD1306_1_SSD1306_UART_BYTE_TO_BYTE
    #define OLED_SSD1306_1_SSD1306_UART_BYTE_TO_BYTE  (2500u)
#endif /* OLED_SSD1306_1_SSD1306_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_OLED_SSD1306_1_SSD1306_H) */


/* [] END OF FILE */
