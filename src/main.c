/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bdma.h"
#include "dma.h"
#include "eth.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "abcc.h"
#include "abcc_hardware_abstraction_aux.h"
#include "abcc_versions.h"
#include "abcc_api.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*------------------------------------------------------------------------------
** ABCC_API_CbfUserInit()
** Place to take action based on ABCC module network type and firmware version.
** Calls ABCC_API_UserInitComplete() to indicate to the abcc_driver to continue.
**------------------------------------------------------------------------------
*/
void ABCC_API_CbfUserInit( ABCC_API_NetworkType iNetworkType, ABCC_API_FwVersionType iFirmwareVersion )
{
   ABCC_API_UserInitComplete();
   return;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

   /*-------------------------------------------------------------------------
   **
   ** Port of the ABCC example code for the Nucleo-H743ZI2 board and the
   ** M00765 ABCC adapter board.
   **
   ** All HW/interface reservations and initializations has been made via the
   ** 'CubeMX' project, this includes the define:s for the I/O pins and the
   ** initial settings for the I2C, SPI, and UART communication interfaces.
   ** All additions or changes made in the CubeMX-generated files should be
   ** inside the "USER X BEGIN/END" markers so updates to that project file
   ** should be possible to apply without interfering with the ABCC-related
   ** parts.
   **
   ** The __io_putchar() and __io_getchar functions has been added to usart.c
   ** to allow for console I/O, but is not complete. FIFO/interrupt support
   ** remains to do.
   **
   ** Basic operation with SPI or UART shall work. Both implementations runs
   ** with polling operation. Interrupt-driven operation for SPI, and porting
   ** of the critical section macros, remains "to do", as does SYNC support.
   **
   **-------------------------------------------------------------------------
   */

   UINT8    bConsoleInput;

   /*
   ** Timekeeping variables for the ABCC driver. The ABCC_API_RunTimerSystem()
   ** requires a reasonably accurate reference, in this implementation the
   ** 1ms HAL_GetTick() is used as a time base.
   */
   UINT32   lTickThen;
   UINT32   lTickNow;
   UINT32   lTickDiff;

   /*
   ** The RESTART button on the M00765 PCB should be flank sensitive so we
   ** need variables to keep track of its state.
   */
   BOOL  fAbccRestartButtonThen;
   BOOL  fAbccRestartButtonNow;

   ABCC_ErrorCodeType eErrorCode = ABCC_EC_NO_ERROR;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ETH_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  MX_BDMA_Init();
  MX_LPUART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

   printf( "%c", 12 ); /* 12 = 'form feed' */
   printf( "HMS Industrial Networks\n" );
   printf( "Embedded ABCC Demo\n" );
   printf( "NUCLEO-H743ZI2 with M00765 option card\n" );
   printf( ABCC_STARTER_KIT_VER_STRING );
   printf( "\n\n" );

   /*
   ** Inform the adaptation functions in "abcc_hardware_abstraction.c" which I2C,
   ** SPI and UART interfaces to use for the M00765 PCB and the ABCC. It is
   ** assumed that those interfaces already has been claimed/reserved via the
   ** CubeMX configuration tool, and that its code generator has defined the
   ** corresponding structures here in the "main.c" so that the only
   ** remaining task is to pass on pointers to those existing structures.
   */
   if( !ABCC_HAL_Set_I2C_Handle( &hi2c1 ) )
   {
      Error_Handler();
   }
   if( !ABCC_HAL_Set_SPI_Handle( &hspi1 ) )
   {
      Error_Handler();
   }
   if( !ABCC_HAL_Set_UART_Handle( &hlpuart1 ) )
   {
      Error_Handler();
   }

   if( ABCC_API_Init() != ABCC_EC_NO_ERROR )
   {
      Error_Handler();
   }

   lTickThen = HAL_GetTick();
   fAbccRestartButtonThen = ABCC_HAL_GetRestartButton();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      /*
      ** Handle console input.
      */
      if( HAL_UART_Receive( &huart3, &bConsoleInput, 1, 0 ) == HAL_OK )
      {
      /* TODO - Restart the ABCC when someone presses 'R' ? */
      /* TODO - Dump some statistics data ? */
      }

      /*
      ** Run the ABCC driver and handle any state-dependent actions.
      */
      eErrorCode = ABCC_API_Run();

      if( eErrorCode != ABCC_EC_NO_ERROR )
      {
         printf( "ABCC_API_Run() returned error code %d\n", eErrorCode );
         break;
      }

      /*
      ** Poll the system timer and inform ABCC_API_RunTimerSystem() about how
      ** much time that has elapsed.
      */
      lTickNow = HAL_GetTick();
      if( lTickNow != lTickThen )
      {
         lTickDiff = lTickNow - lTickThen;
         lTickThen = lTickNow;

         /*
         ** Make sure that *all* elapsed time is accounted for...
         */
         while( lTickDiff > 0 )
         {
            if( lTickDiff > ABP_SINT16_MAX )
            {
               ABCC_API_RunTimerSystem( ABP_SINT16_MAX );
               lTickDiff -= ABP_SINT16_MAX;
            }
            else
            {
               ABCC_API_RunTimerSystem( (INT16)lTickDiff );
               lTickDiff = 0;
            }
         }
      }

      /*
      ** Restart the ABCC when the S1/RESTART is pushed down. Debouncing is not
      ** needed since the button is wired via a voltage WD chip (U4).
      */
      fAbccRestartButtonNow = ABCC_HAL_GetRestartButton();
      if( fAbccRestartButtonNow != fAbccRestartButtonThen )
      {
         fAbccRestartButtonThen = fAbccRestartButtonNow;
         if( !fAbccRestartButtonThen )
         {
            ABCC_API_Restart();
         }
      }
  }

  ABCC_API_Shutdown();
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 24;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
