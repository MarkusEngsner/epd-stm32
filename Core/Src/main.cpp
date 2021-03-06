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

#include <cstring>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EPD_2in9.h"
#include "drawing-impl.cpp"
#include "drawing.h"
#include "marker-impl.cpp"
#include "marker.h"
#include "font24.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern "C" PUTCHAR_PROTOTYPE;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  const bool use_my_init = true;
  const bool use_my_clears = true;
  const bool skip_clears = false;

  //     EPD_2in9_test();
  printf("\r\n\r\nIn main of cppfromblinky!\r\n");

  const UWORD width = EPD_2IN9_WIDTH;
  const UWORD height = EPD_2IN9_HEIGHT;
  printf("EPD display size: w=%d, h=%d\r\n", width, height);
  const UWORD image_size = width * height / 8;
  printf("Total byte count of display: %d\r\n", image_size);
  printf("That is: %.2fKB\r\n", image_size / 1024.0);

  /* my implementation */
  auto epaper = emarker::EPaperScreen<width, height>(
      &hspi1, SPI_CS_GPIO_Port, SPI_CS_Pin, DC_GPIO_Port, DC_Pin, RST_GPIO_Port,
      RST_Pin, BUSY_GPIO_Port, BUSY_Pin);

  if (use_my_init) {
    printf("Using my inits...\r\n");
    epaper.InitializeDisplay();
  } else {
    printf("Using waveshare's inits...\r\n");
    DEV_Module_Init();
    EPD_2IN9_Init(EPD_2IN9_FULL);
  }
  HAL_Delay(500);

  if (!skip_clears) {
    if (use_my_clears) {
      printf("Using my clears... \r\n");
      epaper.ClearDisplay();
      HAL_Delay(1000);
      //      epaper.FillDisplay(0xF0);
      //      HAL_Delay(1000);

      //    epaper.FillDisplay(0b10101010);
      //    HAL_Delay(1000);
      printf("Creating canvas...\r\n");
      auto canvas = paintbrush::Canvas<width, height>();
      canvas.Fill(paintbrush::Color::Black);
      //      for (unsigned int y = 20; y < height - 20; y++){
      //        canvas.Set(10, y, paintbrush::Color::White);
      //        canvas.Set(30, y, paintbrush::Color::White);
      //      }
      //      for (unsigned int x = 0; x < width; x++){
      //        canvas.Set(x, 20, paintbrush::Color::White);
      //        canvas.Set(x, height - 20, paintbrush::Color::White);
      //      }
      //    for (unsigned x = 0; x < 10; x++){
      //      canvas.Set((x * 8) + 0, 50, paintbrush::Color::White);
      //    }
      paintbrush::DrawStraightLine(canvas, 0, 0, 50,
                                   paintbrush::Axis::Horizontal,
                                   paintbrush::Color::White);
      paintbrush::DrawStraightLine(canvas, 20, 30, 50,
                                   paintbrush::Axis::Horizontal,
                                   paintbrush::Color::White);
      paintbrush::DrawStraightLine(canvas, 20, 50, 50,
                                   paintbrush::Axis::Vertical,
                                   paintbrush::Color::White);
      paintbrush::DrawDiagonalLine(canvas, 20, 20, 50,
                                   paintbrush::Direction::DownRight,
                                   paintbrush::Color::White);
      std::array<uint8_t, 72> two = {
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
          0x07, 0xC0, 0x00,  //      #####
          0x1F, 0xF0, 0x00,  //    #########
          0x38, 0x30, 0x00,  //   ###     ##
          0x30, 0x18, 0x00,  //   ##       ##
          0x30, 0x18, 0x00,  //   ##       ##
          0x00, 0x18, 0x00,  //            ##
          0x00, 0x30, 0x00,  //           ##
          0x00, 0x60, 0x00,  //          ##
          0x01, 0xC0, 0x00,  //        ###
          0x03, 0x80, 0x00,  //       ###
          0x06, 0x00, 0x00,  //      ##
          0x0C, 0x00, 0x00,  //     ##
          0x18, 0x00, 0x00,  //    ##
          0x3F, 0xF8, 0x00,  //   ###########
          0x3F, 0xF8, 0x00,  //   ###########
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
          0x00, 0x00, 0x00,  //
      };
      paintbrush::Canvas<24, 24> symbol{two};

      for (int i = 0; i < 4; i++){
        paintbrush::DrawSymbol(canvas, 24 * i, 80, paintbrush::font24(i + '0'), paintbrush::Color::White);
      }

      printf("Printing canvas...\r\n");
      epaper.PrintFull(canvas);

    } else {
      printf("Using Waveshare's clear...\r\n");
      EPD_2IN9_Clear();
      HAL_Delay(2000);
      EPD_2IN9_Clear();
      HAL_Delay(2000);
    }
  }

  //  epaper.WakeUp();
  epaper.Sleep();
  printf("The screen is now asleep.\r\n");

  printf("Please input a digit\r\n");

  auto constexpr msg_size = 4;
  uint8_t buffer[msg_size] = {};
  uint32_t decoded_message = 0;
  memset(buffer, 0, msg_size);

  if (HAL_UART_Receive(&huart2, buffer, msg_size, 10000) != HAL_OK) {
    printf("Failed to receive message\r\n");
  } else {
    printf("Received message.\r\n");
    printf("%i \r\n", buffer[0]);
    for (auto i = 0; i < msg_size; ++i) {
      auto c = buffer[i];
      auto digit = c - '0';
      decoded_message = decoded_message * 10 + digit;
    }
    printf("As a uint32_t::\r\n");
    printf("%u \r\n", decoded_message);
  }
  //  epaper.WakeUp();
  //  epaper.ClearDisplay();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(10000);
  }
  /* USER CODE END 3 */
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE {
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of
   * transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {
  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 15;
  if (HAL_SPI_Init(&hspi1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {
  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_ODD;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DC_Pin */
  GPIO_InitStruct.Pin = DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUSY_Pin */
  GPIO_InitStruct.Pin = BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_Pin */
  GPIO_InitStruct.Pin = RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_CS_Pin */
  GPIO_InitStruct.Pin = SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
