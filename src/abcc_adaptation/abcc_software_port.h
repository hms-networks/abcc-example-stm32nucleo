/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Platform dependent macros and functions required by the ABCC driver and
** Anybus objects implementation to be platform independent.
** The description of the macros are found in abcc_port.h. Abcc_port.h is found
** in the public ABCC40 driver interface.
********************************************************************************
*/

#ifndef ABCC_SW_PORT_H_
#define ABCC_SW_PORT_H_

#include <stdio.h>
#include <stdarg.h>
#include "abcc_types.h"
#include "abcc_config.h"

#define ABCC_PORT_printf( ... )          printf( __VA_ARGS__ )

#define ABCC_PORT_vprintf( ... )         vprintf( __VA_ARGS__ )

#define ABCC_PORT_UseCritical()

#define ABCC_PORT_EnterCritical()

#define ABCC_PORT_ExitCritical()

#endif  /* inclusion lock */
