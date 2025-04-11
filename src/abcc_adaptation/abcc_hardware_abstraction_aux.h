/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Defines data structures and service functions for the M00765 adapter board.
********************************************************************************
*/
#ifndef ABCC_HAL_AUX
#define ABCC_HAL_AUX

/*------------------------------------------------------------------------------
** Register the CubeMX HAL handle to use for each communication interface to
** use with the M00765 board.
**------------------------------------------------------------------------------
** Arguments:
**    pxNewHandle - Pointer to the struct/handle that represents the selected
**                    interface hardware.
**
** Returns:
**    TRUE if the handle could be changed, FALSE otherwise.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_HAL_Set_I2C_Handle( I2C_HandleTypeDef* pxNewHandle );
#if( ABCC_CFG_DRV_SPI_ENABLED )
EXTFUNC BOOL ABCC_HAL_Set_SPI_Handle( SPI_HandleTypeDef* pxNewHandle );
#endif
#if( ABCC_CFG_DRV_SERIAL_ENABLED )
EXTFUNC BOOL ABCC_HAL_Set_UART_Handle( UART_HandleTypeDef* pxNewHandle );
#endif

/*------------------------------------------------------------------------------
** Synchronise in/out data with the I2C I/O expanders on the M00765 board.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE if the data could be read/written, FALSE otherwise.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_HAL_Refresh_I2C_In( void );
EXTFUNC BOOL ABCC_HAL_Refresh_I2C_Out( void );

/*------------------------------------------------------------------------------
** Returns the state of the S1 / RESTART button on the M00765 board.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE when the button is down, FALSE when it is up.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_HAL_GetRestartButton( void );

#endif  /* inclusion lock */
