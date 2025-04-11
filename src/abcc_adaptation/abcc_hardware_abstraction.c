/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
********************************************************************************
*/

#include "abcc.h"

#include "abcc_hardware_abstraction_spi.h"
#include "abcc_hardware_abstraction_serial.h"

#include <stm32h7xx_hal.h>
#include "abcc_hardware_abstraction_aux.h"

#include "main.h"

/*
** The I2C addresses for the U8 + U9 I/O expanders are hardwired on the board.
*/
#define M00765_U8_I2C_ADDRESS                      ( 0x4e )
#define M00765_U9_I2C_ADDRESS                      ( 0x4c )

/*
** Structures for the I/O on the M00765 board. All of those are connected to
** the onboard I2C I/O expanders. The storage structured must be manually
** synchronised with the I/O expanders using the 'refresh' functions declared
** below.
**
** TODO - Is 'volatile' needed for any of those?
*/
typedef struct I2C_InputsTag
{
   UINT8    bS2;     /* S2 - "HEX 1"   U8, P0[3..0] */
   UINT8    bS3;     /* S3 - "HEX 2"   U8, P0[7..4] */
   UINT8    bS4;     /* S4 - "OpMode"  U9, P0[7..4] */
   UINT8    bS5;     /* S5 - "GPI"     U9, P0[3..0] */
   UINT8    bJP11;   /* JP11 - "GPIO"  U8, P1[5..0] */
   UINT8    bMI;     /* ABCC MI1 & MI0 U9, P1[5..4] */
   UINT8    bMD;     /* ABCC MD1 & MD0 U9, P1[7..6] */
} I2C_InputsType;
typedef struct I2C_OutputsTag
{
   UINT8    bJP11;   /* JP11 - "GPIO", U8, P0[5..0] */
   UINT8    bLEDs;   /* D3-D6 - "GPO", U9, P1[3..0] */
} I2C_OutputsType;

/*
** "SPI/UART transaction complete" callback function pointer.
*/
static ABCC_HAL_SpiDataReceivedCbfType    pnTransferCompleteCbf = NULL;

/*
** The CubeMX HAL handles for the IO interfaces that the M00765 PCB has been
** wired to. Needs to be assigned from "main()" before anything can be done
** here with those interfaces.
*/
static I2C_HandleTypeDef*     pxI2C_Handle = NULL;
#if( ABCC_CFG_DRV_SPI_ENABLED )
static SPI_HandleTypeDef*     pxSPI_Handle = NULL;
#endif
#if( ABCC_CFG_DRV_SERIAL_ENABLED )
static UART_HandleTypeDef*    pxUART_Handle = NULL;
#endif

/*
** Intermediate buffers for the SPI and UART transfers.
**
** The default linker settings in CubeIDE seems to assign all runtime variables
** to the DTCM-RAM, but the DMA controllers that services the I/O interfaces
** can't reach that memory directly.
**
** Adding appropriate 'section' attributes to the in/out buffer definitions in
** "abcc_spi_driver.c" and "abcc_serial_driver.c" would solve this, but will
** collide with how the present directory structure for the example code works.
** On the repository level the "abcc_drv" tree is common for all ports and
** therefore cannot have port-specific additions in them.
**
** As a workaround a second set of local buffers are defined here, which are
** explicitly placed in the corresponding SRAM banks which the DMA controllers
** can reach. One extra data synchronisation cycle is needed before/after each
** DMA-driven I/O transfer cycle to copy data between the DTCM-RAM and the
** SRAM, and this is presently made by "memcpy()" calls. This could also be
** made via the MDMA controller, but the benefit of this is probably too small
** to justify the extra complexity, and in a 'live' implementation one would
** rather solve this by placing the SPI/UART memory buffers in the appropriate
** SRAM area by editing those files.
**
** Also note that the default linker directives does not define the "sram1"
** or "sram4" sections by default.
**
** For more details, refer to chapter "2 - Memory and bus architecture" in
** "RM0433 - Reference manual, STM32H742, STM32H743/753 and STM32H750 Value
** line advanced Arm®-based 32-bit MCUs"
*/

#if( ABCC_CFG_DRV_SPI_ENABLED )

/*
** Size macros has been copied from "abcc_spi_driver.c"
*/
#define NUM_BYTES_2_WORDS(x) ( ( (x) + 1 ) >> 1 )
#define CRC_WORD_LEN_IN_WORDS 2
#define MAX_PAYLOAD_WORD_LEN ( ( NUM_BYTES_2_WORDS( ABCC_CFG_SPI_MSG_FRAG_LEN ) ) + ( NUM_BYTES_2_WORDS( ABCC_CFG_MAX_PROCESS_DATA_SIZE ) ) + ( CRC_WORD_LEN_IN_WORDS ) )

__attribute__((section(".sram1"))) static UINT8 abSpiMosiDmaBuff[ ( 5 + MAX_PAYLOAD_WORD_LEN ) * ABP_UINT16_SIZEOF ];
__attribute__((section(".sram1"))) static UINT8 abSpiMisoDmaBuff[ ( 5 + MAX_PAYLOAD_WORD_LEN ) * ABP_UINT16_SIZEOF ];

#endif

#if( ABCC_CFG_DRV_SERIAL_ENABLED )

/*
** Size macros has been copied from "abcc_serial_driver.c"
*/
#define SER_CMD_STAT_REG_LEN                       ( ABP_UINT8_SIZEOF )
#define SER_MSG_FRAG_LEN                           ( 16 * ABP_UINT8_SIZEOF )
#if( ABCC_CFG_MAX_PROCESS_DATA_SIZE > ABP_MAX_PROCESS_DATA )
   #define SER_MAX_PD_LEN                          ( ABP_MAX_PROCESS_DATA )
#else
   #define SER_MAX_PD_LEN                          ABCC_CFG_MAX_PROCESS_DATA_SIZE
#endif
#define SER_CRC_LEN                                ( ABP_UINT16_SIZEOF )

__attribute__((section(".sram4"))) static UINT8 abUartTxDmaBuff[ SER_CMD_STAT_REG_LEN + SER_MSG_FRAG_LEN + SER_MAX_PD_LEN + SER_CRC_LEN ];
__attribute__((section(".sram4"))) static UINT8 abUartRxDmaBuff[ SER_CMD_STAT_REG_LEN + SER_MSG_FRAG_LEN + SER_MAX_PD_LEN + SER_CRC_LEN ];

#endif

/*
** Copies of the destination and size for the received SPI/UART frame data,
** required to save because the 'copy RX data' work is made in a separate
** function compared to the 'send/receive data'.
**
** TODO - Is 'volatile' needed for any of those?
*/
static void*   pxInboundDest;
static UINT16  iInboundSize;

/*
** Buffer space for the I2C expander input/output data.
*/
static I2C_InputsType   sI2C_Inputs;
static I2C_OutputsType  sI2C_Outputs;

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_Set_I2C_Handle( I2C_HandleTypeDef* pxNewHandle )
{
   if( ( ( pxI2C_Handle == NULL ) && ( pxNewHandle != NULL ) ) ||
       ( ( pxI2C_Handle != NULL ) && ( pxNewHandle == NULL ) ) )
   {
      pxI2C_Handle = pxNewHandle;
      return( TRUE );
   }

   ABCC_LOG_WARNING( ABCC_EC_PARAMETER_NOT_VALID,
      (UINT32)pxNewHandle,
      "Provided I2C handle not supported in this state %p\n",
      (void*)pxNewHandle );
   return( FALSE );
}

#if( ABCC_CFG_DRV_SPI_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_Set_SPI_Handle( SPI_HandleTypeDef* pxNewHandle )
{
   if( ( ( pxSPI_Handle == NULL ) && ( pxNewHandle != NULL ) ) ||
       ( ( pxSPI_Handle != NULL ) && ( pxNewHandle == NULL ) ) )
   {
      pxSPI_Handle = pxNewHandle;
      return( TRUE );
   }

   ABCC_LOG_WARNING( ABCC_EC_PARAMETER_NOT_VALID,
      (UINT32)pxNewHandle,
      "Provided SPI handle not supported in this state %p\n",
      (void*)pxNewHandle );
   return( FALSE );
}
#endif

#if( ABCC_CFG_DRV_SERIAL_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_Set_UART_Handle( UART_HandleTypeDef* pxNewHandle )
{
   if( ( ( pxUART_Handle == NULL ) && ( pxNewHandle != NULL ) ) ||
       ( ( pxUART_Handle != NULL ) && ( pxNewHandle == NULL ) ) )
   {
      pxUART_Handle = pxNewHandle;
      return( TRUE );
   }

   ABCC_LOG_WARNING( ABCC_EC_PARAMETER_NOT_VALID,
      (UINT32)pxNewHandle,
      "Provided UART handle not supported in this state %p\n",
      (void*)pxNewHandle );
   return( FALSE );
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_Refresh_I2C_In( void )
{
   HAL_StatusTypeDef    xHalStatus;
   UINT8                abBuffer[ 2 ];

   if( pxI2C_Handle == NULL )
   {
      /* No I2C interface handle registered, this should not happen! */
      ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No I2C interface handle registered\n" );
      return( FALSE );
   }

   /* Read both ports of U8 and copy the data to the correct variables. */
   xHalStatus = HAL_I2C_Master_Receive( pxI2C_Handle, M00765_U8_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)xHalStatus, "I2C read failed (%d)\n", xHalStatus );
      return( FALSE );
   }

   /* S2, P0[3..0] */
   sI2C_Inputs.bS2 = abBuffer[ 0 ] & 0xf;

   /* S3, P0[7..4] */
   sI2C_Inputs.bS3 = ( abBuffer[ 0 ] & 0xf0 ) >> 4;

   /* JP11, P1[5..0] */
   sI2C_Inputs.bJP11 = abBuffer[ 1 ] & 0x3f;

   /* Read both ports of U8 and copy the data to the correct variables. */
   xHalStatus = HAL_I2C_Master_Receive( pxI2C_Handle, M00765_U9_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)xHalStatus, "I2C read failed (%d)\n", xHalStatus );
      return( FALSE );
   }

   /* S5, P0[3..0] */
   sI2C_Inputs.bS5 = abBuffer[ 0 ] & 0xf;

   /* S4, P0[7..4], bit needs to be reversed for OM0 to end up at LSB. */
   sI2C_Inputs.bS4 = 0;
   if( ( abBuffer[ 0 ] & 0x10 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x8;
   }
   if( ( abBuffer[ 0 ] & 0x20 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x4;
   }
   if( ( abBuffer[ 0 ] & 0x40 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x2;
   }
   if( ( abBuffer[ 0 ] & 0x80 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x1;
   }

   /* ABCC MI1 & MI0, P1[5..4] */
   sI2C_Inputs.bMI = ( abBuffer[ 1 ] & 0x30 ) >> 4;

   /* ABCC MD1 & MD0, P1[7..6] */
   sI2C_Inputs.bMD = ( abBuffer[ 1 ] & 0xc0 ) >> 6;

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_Refresh_I2C_Out( void )
{
   HAL_StatusTypeDef    xHalStatus;
   UINT8                abBuffer[ 2 ];

   if( pxI2C_Handle == NULL )
   {
      /* No I2C interface handle registered, this should not happen! */
      ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No I2C interface handle registered\n" );
      return( FALSE );
   }

   /*
   ** NOTE: Bits that should be used as inputs must be set 'high'. Check the
   ** datasheet for the NXP PCF8575 I/O expander for more information.
   */

   /* U8, P0[7..0] are inputs. */
   abBuffer[ 0 ] = 0xff;

   /* U8, P1[7..6] are inputs. */
   abBuffer[ 1 ] = 0xc0;

   /* JP11, GPIO, only bits 5..0 can be used as outputs. */
   abBuffer[ 1 ] |= sI2C_Outputs.bJP11 & 0x3f;

   /* Write both ports of U8. */
   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U8_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)xHalStatus, "I2C write failed (%d)\n", xHalStatus );
      return( FALSE );
   }

   /* U9, P0[7..0] are inputs. */
   abBuffer[ 0 ] = 0xff;

   /* U9, P1[7..4] are inputs. */
   abBuffer[ 1 ] = 0xf0;

   /* LEDs D3-D6, active low. */
   abBuffer[ 1 ] |= ( ~(sI2C_Outputs.bLEDs) & 0x0f );

   /* Write both ports of U9. */
   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U9_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)xHalStatus, "I2C write failed (%d)\n", xHalStatus );
      return( FALSE );
   }

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_GetRestartButton( void )
{
   if( HAL_GPIO_ReadPin( M00765_BTN_S1_GPIO_Port, M00765_BTN_S1_Pin ) == GPIO_PIN_RESET )
   {
      return( TRUE );
   }

   return( FALSE );
}

#if( ABCC_CFG_DRV_SPI_ENABLED )
/*------------------------------------------------------------------------------
** See "stm32...hal_spi.h" and "stm32...hal_spi.c" for more information.
**------------------------------------------------------------------------------
*/
void HAL_SPI_TxRxCpltCallback( SPI_HandleTypeDef* pxHandle )
{
   if( pxHandle == pxSPI_Handle )
   {
      /* Deactivate SS, i.e. set it high. */
      HAL_GPIO_WritePin( M00765_SPI_SS_GPIO_Port, M00765_SPI_SS_Pin, GPIO_PIN_SET );

      /* Did the SPI transfer finish correctly? */
      if( pxHandle->ErrorCode != HAL_OK )
      {
         return;
      }

      /* Copy the MISO data from the I/O buffer (SRAMx) to the driver buffer (DTCM-RAM). */
      memcpy( pxInboundDest, abSpiMisoDmaBuff, iInboundSize );

      /* And notify the driver that we have a MISO frame to process. */
      if( pnTransferCompleteCbf != NULL )
      {
         pnTransferCompleteCbf();
         return;
      }
      else
      {
         /* No completion callback registered, this should not happen! */
         ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No SPI transfer completion callback registered\n" );
         return;
      }
   }
   else
   {
      /*
      ** Handle other SPI callbacks.
      **
      ** PORTING ALERT!
      **
      ** This function, and/or the contents of it, will probably have to be
      ** placed elsewhere if the DMAx controllers are used for other
      ** operations than the ABCC.
      */
   }
}
#endif

#if( ABCC_CFG_DRV_SERIAL_ENABLED )
/*------------------------------------------------------------------------------
** See "stm32...hal_uart.h" and "stm32...hal_uart.c"for more information.
**------------------------------------------------------------------------------
*/
void HAL_UART_RxCpltCallback( UART_HandleTypeDef* pxHandle )
{
   UINT32   lTemp;

   if( pxHandle == pxUART_Handle )
   {
      lTemp = pxHandle->ErrorCode;

      /*
      ** Terminate any ongoing UART DMA operations. Either this or something
      ** similar seems to be required in order to get the HAL 'gState' of the
      ** UART back to HAL_UART_STATE_READY, it does not seem to happen on its
      ** own when the DMA transfers has been finished.
      **
      ** TODO - Is there a better way to do this?
      */
      HAL_UART_Abort( pxUART_Handle );

      /* Did the UART transfer finish correctly? */
      if( lTemp != HAL_OK )
      {
         return;
      }

      /* Copy the RX data from the I/O buffer (SRAMx) to the driver buffer (DTCM-RAM). */
      memcpy( pxInboundDest, abUartRxDmaBuff, iInboundSize );

      /* And notify the driver that we have a RX frame to process. */
      if( pnTransferCompleteCbf != NULL )
      {
         pnTransferCompleteCbf();
         return;
      }
      else
      {
         /* No completion callback registered, this should not happen! */
         ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No UART transfer completion callback registered\n" );
         return;
      }
   }
   else
   {
      /*
      ** Handle other UART callbacks.
      **
      ** PORTING ALERT!
      **
      ** This function, and/or the contents of it, will probably have to be
      ** placed elsewhere if the DMAx controllers are used for other
      ** operations than the ABCC.
      */
   }
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_HwInit( void )
{
   HAL_StatusTypeDef    xHalStatus;
   UINT8                abBuffer[ 2 ];

   if( pxI2C_Handle == NULL )
   {
      /* No I2C interface handle registered, this should not happen! */
      ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No I2C interface handle registered\n" );
      return( FALSE );
   }

   /*
   ** Reset the I2C IO expanders to 'all inputs'. Check the datasheet for the
   ** NXP PCF8575 I/O expander for more information.
   */
   abBuffer[ 0 ] = 0xFF;
   abBuffer[ 1 ] = 0xFF;

   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U8_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)xHalStatus, "I2C write failed (%d)\n", xHalStatus );
      return( FALSE );
   }

   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U9_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, (UINT32)xHalStatus, "I2C write failed (%d)\n", xHalStatus );
      return( FALSE );
   }

   /*
   ** Refresh the I2C inputs so that MD and MI contains up-to-date values.
   */
   if( !ABCC_HAL_Refresh_I2C_In() )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, 0, "I2C refresh failed\n" );
      return( FALSE );
   }

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_Init( void )
{
   /* Pull the RESET signal low. */
   HAL_GPIO_WritePin( M00765_RESET_GPIO_Port, M00765_RESET_Pin, GPIO_PIN_RESET );

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_Close( void )
{
   /* Nothing to do. */
}

#if( ABCC_CFG_OP_MODE_SETTABLE )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SetOpmode( UINT8 bOpmode )
{
   HAL_StatusTypeDef    xHalStatus;

   /*
   ** Step 1, update the OM pins on the ABCC.
   */

   if( ( bOpmode & 0x1 ) != 0 )
   {
      HAL_GPIO_WritePin( M00765_OM0_GPIO_Port, M00765_OM0_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM0_GPIO_Port, M00765_OM0_Pin, GPIO_PIN_RESET );
   }

   if( ( bOpmode & 0x2 ) != 0 )
   {
      HAL_GPIO_WritePin( M00765_OM1_GPIO_Port, M00765_OM1_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM1_GPIO_Port, M00765_OM1_Pin, GPIO_PIN_RESET );
   }

   if( ( bOpmode & 0x4 ) != 0 )
   {
      HAL_GPIO_WritePin( M00765_OM2_GPIO_Port, M00765_OM2_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM2_GPIO_Port, M00765_OM2_Pin, GPIO_PIN_RESET );
   }

   if( ( bOpmode & 0x8 ) != 0 ) {
      HAL_GPIO_WritePin( M00765_OM3_GPIO_Port, M00765_OM3_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM3_GPIO_Port, M00765_OM3_Pin, GPIO_PIN_RESET );
   }

   /*
   ** Step 2, any other OpMode-dependent initialisations that are needed.
   */

   switch( bOpmode )
   {
#if( ABCC_CFG_DRV_SPI_ENABLED )
   case ABP_OP_MODE_SPI:

      /* Is any handle registered for the HW interface in question? */
      if( pxSPI_Handle == NULL )
      {
         /* No SPI interface handle registered, this should not happen! */
         ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No SPI interface handle registered\n" );
         return;
      }

      /* Update any interface-dependent settings that depends on the OpMode. */

      /* Re-initialise the interface HW with the new settings. */

   break;
#endif
#if( ABCC_CFG_DRV_SERIAL_ENABLED )
   case ABP_OP_MODE_SERIAL_19_2:
   case ABP_OP_MODE_SERIAL_57_6:
   case ABP_OP_MODE_SERIAL_115_2:
   case ABP_OP_MODE_SERIAL_625:

      /* Is any handle registered for the HW interface in question? */
      if( pxUART_Handle == NULL )
      {
         /* No UART interface handle registered, this should not happen! */
         ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES,
            bOpmode,
            "No UART interface handle registered\n" );
         return;
      }

      /* Update any interface-dependent settings that depends on the OpMode. */
      switch( bOpmode )
      {
      case ABP_OP_MODE_SERIAL_19_2:
         pxUART_Handle->Init.BaudRate = 19200;
         break;
      case ABP_OP_MODE_SERIAL_57_6:
         pxUART_Handle->Init.BaudRate = 57600;
         break;
      case ABP_OP_MODE_SERIAL_115_2:
         pxUART_Handle->Init.BaudRate = 115200;
         break;
      case ABP_OP_MODE_SERIAL_625:
         pxUART_Handle->Init.BaudRate = 625000;
         break;
      default:
         ABCC_LOG_FATAL( ABCC_EC_INCORRECT_OPERATING_MODE,
            bOpmode,
            "Incorrect operating mode %d\n",
            bOpmode );
         break;
      }

      /* Re-initialise the interface HW with the new settings. */
      xHalStatus = HAL_UART_Init( pxUART_Handle );
      if( xHalStatus != HAL_OK )
      {
         ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
            (UINT32)xHalStatus,
            "UART init failed (%d)\n",
            xHalStatus );
         return;
      }

      break;
#endif
   default:
      ABCC_LOG_FATAL( ABCC_EC_INCORRECT_OPERATING_MODE,
         bOpmode,
         "Incorrect operating mode %d\n",
         bOpmode );
      break;
   }

   return;
}
#endif

#if( ABCC_CFG_OP_MODE_GETTABLE )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_HAL_GetOpmode( void )
{
   if( !ABCC_HAL_Refresh_I2C_In() )
   {
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR, 0, "I2C refresh failed\n" );
      return( 0xf ); /* Service mode. */
   }

   return( sI2C_Inputs.bS4 );
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_HWReset( void )
{
   HAL_GPIO_WritePin( M00765_RESET_GPIO_Port, M00765_RESET_Pin, GPIO_PIN_RESET );
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_HWReleaseReset( void )
{
   HAL_GPIO_WritePin( M00765_RESET_GPIO_Port, M00765_RESET_Pin, GPIO_PIN_SET );
}

#if ABCC_CFG_MODULE_ID_PINS_CONN
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_HAL_ReadModuleId( void )
{
   return( sI2C_Inputs.bMI );
}
#endif

#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_ModuleDetect( void )
{
   if( sI2C_Inputs.bMD == 0 )
   {
      return( TRUE );
   }

   return( FALSE );
}
#endif

#if( ABCC_CFG_SYNC_ENABLED && ABCC_CFG_USE_ABCC_SYNC_SIGNAL_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SyncInterruptEnable( void )
{
   /*
   ** Implement according to abcc_hardware_abstraction.h
   */
}
#endif

#if( ABCC_CFG_SYNC_ENABLED && ABCC_CFG_USE_ABCC_SYNC_SIGNAL_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SyncInterruptDisable( void )
{
   /*
   ** Implement according to abcc_hardware_abstraction.h
   */
}
#endif

#if( ABCC_CFG_INT_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_AbccInterruptEnable( void )
{
   /*
   ** Implement according to abcc_hardware_abstraction.h
   */
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_AbccInterruptDisable( void )
{
   /*
   ** Implement according to abcc_hardware_abstraction.h
   */
}
#endif

#if( ABCC_CFG_POLL_ABCC_IRQ_PIN_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_HAL_IsAbccInterruptActive( void )
{
   if( HAL_GPIO_ReadPin( M00765_IRQ_GPIO_Port, M00765_IRQ_Pin ) == GPIO_PIN_RESET )
   {
      return( TRUE );
   }

   return( FALSE );
}
#endif

#if( ABCC_CFG_DRV_SPI_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SpiRegDataReceived( ABCC_HAL_SpiDataReceivedCbfType pnDataReceived  )
{
   if( pnDataReceived != NULL )
   {
      pnTransferCompleteCbf = pnDataReceived;
   }
   else
   {
      ABCC_LOG_ERROR( ABCC_EC_PARAMETER_NOT_VALID,
         (UINT32)pnDataReceived,
         "Provided SPI data received callback not valid %" PRIx32 "\n",
         (UINT32)pnDataReceived );
   }

   return;
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   HAL_StatusTypeDef    xHalStatus;

   if( pxSPI_Handle == NULL )
   {
      /* No SPI interface handle registered, this should not happen! */
      ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No SPI interface handle registered\n" );
      return;
   }

   /* Save the destination pointer and size for later. */
   pxInboundDest = pxReceiveDataBuffer;
   iInboundSize = iLength;

   /* Activate SS, i.e. set it low. */
   /* TODO - The SPI interface should have some kind of logic to directly    */
   /* TODO - control the SS signal, investigate if this can be used instead. */
   HAL_GPIO_WritePin( M00765_SPI_SS_GPIO_Port, M00765_SPI_SS_Pin, GPIO_PIN_RESET );

   /* Copy the MOSI data from the driver buffer (DTCM-RAM) to the I/O buffer (SRAMx). */
   memcpy( abSpiMosiDmaBuff, pxSendDataBuffer, iLength );

   /* Start the SPI transfer. */
   xHalStatus = HAL_SPI_TransmitReceive_DMA( pxSPI_Handle, abSpiMosiDmaBuff, abSpiMisoDmaBuff, iLength );
   if( xHalStatus != HAL_OK )
   {
      /* SPI transfer could not be started. */
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)xHalStatus,
         "SPI transfer failed (%d)\n",
         xHalStatus );

      /* Deactivate SS, i.e. set it high. */
      HAL_GPIO_WritePin( M00765_SPI_SS_GPIO_Port, M00765_SPI_SS_Pin, GPIO_PIN_SET );
   }

   return;
}
#endif

#if( ABCC_CFG_DRV_PARALLEL_ENABLED && !ABCC_CFG_MEMORY_MAPPED_ACCESS_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_hardware_abstraction_parallel.h
   */
}

#if( ABCC_CFG_DRV_PARALLEL_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_HAL_ParallelRead16( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_hardware_abstraction_parallel.h
   */
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_hardware_abstraction_parallel.h
   */
}

#if( ABCC_CFG_DRV_PARALLEL_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   /*
   ** Implement according to abcc_hardware_abstraction_parallel.h
   */
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void* ABCC_HAL_ParallelGetRdPdBuffer( void )
{
   /*
   ** Implement according to abcc_hardware_abstraction_parallel.h
   */
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void* ABCC_HAL_ParallelGetWrPdBuffer( void )
{
   /*
   ** Implement according to abcc_hardware_abstraction_parallel.h
   */
}
#endif

#if( ABCC_CFG_DRV_SERIAL_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SerRegDataReceived( ABCC_HAL_SpiDataReceivedCbfType pnDataReceived  )
{
   if( pnDataReceived != NULL )
   {
      pnTransferCompleteCbf = pnDataReceived;
      return;
   }

   ABCC_LOG_ERROR( ABCC_EC_PARAMETER_NOT_VALID,
      (UINT32)pnDataReceived,
      "Provided UART data received callback not valid %" PRIx32 "\n",
      (UINT32)pnDataReceived );
   return;
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
   HAL_StatusTypeDef    xHalStatus;

   if( pxUART_Handle == NULL )
   {
      /* No UART interface handle registered, this should not happen! */
      ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No UART interface handle registered\n" );
      return;
   }

   /* Save the destination pointer and size for later. */
   pxInboundDest = pxRxDataBuffer;
   iInboundSize = iRxSize;

   /* Copy the TX data from the driver buffer (DTCM-RAM) to the I/O buffer (SRAMx). */
   memcpy( abUartTxDmaBuff, pxTxDataBuffer, iTxSize );

   /* Start the UART transmission. */
   xHalStatus = HAL_UART_Transmit_DMA( pxUART_Handle, abUartTxDmaBuff, iTxSize );
   if( xHalStatus != HAL_OK )
   {
      /* UART transmit could not be executed. */
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)xHalStatus,
         "UART transmit failed (%d)\n",
         xHalStatus );
      return;
   }

   /* Start the UART reception. */
   xHalStatus = HAL_UART_Receive_DMA( pxUART_Handle, abUartRxDmaBuff, iRxSize );
   if( xHalStatus != HAL_OK )
   {
      /* UART transmit could not be executed. */
      ABCC_LOG_WARNING( ABCC_EC_HAL_ERR,
         (UINT32)xHalStatus,
         "UART receive failed (%d)\n",
         xHalStatus );
      return;
   }

   return;
}

/*------------------------------------------------------------------------------
** See "abcc_hardware_abstraction.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_HAL_SerRestart( void )
{
   if( pxUART_Handle == NULL )
   {
      /* No UART interface handle registered, this should not happen! */
      ABCC_LOG_FATAL( ABCC_EC_NO_RESOURCES, 0, "No UART interface handle registered\n" );
      return;
   }

   /* Terminate any ongoing UART operations. */
   HAL_UART_Abort( pxUART_Handle );

   return;
}
#endif
