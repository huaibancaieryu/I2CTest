/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
	
	
  /* USER CODE BEGIN 2 */
	
//	//确认OLED是否是SSD1306
//	//注释掉清屏代码，如果OLED亮了（雪花），则初始化成功，大概率是 SSD1306
//	// SSD1306 最小化必要初始化序列
//	uint8_t ssd1306_init[] = {
//		0x00,       // 命令模式
//		0xAE,       // Display OFF
//		0xD5, 0x80, // Clock div ratio/osc freq
//		0xA8, 0x3F, // MUX ratio = 64 (128x64屏)
//		0xD3, 0x00, // Display offset = 0
//		0x40,       // Start line = 0
//		0x8D, 0x14, // Charge pump enable ← 关键！不使能则不亮
//		0x20, 0x00, // Horizontal addressing mode
//		0xA1,       // Segment remap
//		0xC8,       // COM scan direction
//		0xDA, 0x12, // COM pins config
//		0x81, 0xCF, // Contrast
//		0xD9, 0xF1, // Pre-charge period
//		0xDB, 0x40, // VCOMH deselect level
//		0xA4,       // Output follows RAM
//		0xA6,       // Normal display (not inverted)
//		0xAF        // Display ON
//	};
//	HAL_I2C_Master_Transmit(&hi2c1, 0x78, ssd1306_init, sizeof(ssd1306_init), 100);
//	
//	//清屏代码（如果OLED全黑，确认 SSD1306，可以开始绘图）
//		// ====== 第一步：初始化（你已有的代码）======
//	HAL_I2C_Master_Transmit(&hi2c1, 0x78, ssd1306_init, sizeof(ssd1306_init), 100);

//	// ====== 第二步：设置显存写入起始位置 ======
//	uint8_t set_pos[] = {
//			0x00,       // 命令模式
//			0x21, 0, 127,   // 设置列地址范围 0~127
//			0x22, 0, 7      // 设置页地址范围 0~7 (64/8=8页)
//	};
//	HAL_I2C_Master_Transmit(&hi2c1, 0x78, set_pos, sizeof(set_pos), 100);

//	// ====== 第三步：清屏（向 GDDRAM 写入全 0）======
//	uint8_t clear_buf[17];       // 1字节控制 + 16字节数据
//	clear_buf[0] = 0x40;         // 数据模式（Co=0, D/C#=1）

//	for (uint8_t page = 0; page < 8; page++) {
//			// 每页开始前设置页地址和列起始
//			uint8_t page_addr[] = {
//					0x00,               // 命令模式
//					0xB0 | page,        // 设置页地址 (0xB0~0xB7)
//					0x00,               // 列低4位 = 0
//					0x10                // 列高4位 = 0
//			};
//			HAL_I2C_Master_Transmit(&hi2c1, 0x78, page_addr, sizeof(page_addr), 100);
//			
//			// 写入 128 字节的 0（分 8 次，每次 16 字节）
//			memset(&clear_buf[1], 0x00, 16);
//			for (uint8_t i = 0; i < 8; i++) {
//					HAL_I2C_Master_Transmit(&hi2c1, 0x78, clear_buf, 17, 100);
//			}
//	}
	
	//点亮OLED
	uint8_t commands[]={0x00, 0x8d, 0x14, 0xaf, 0xa5};

	HAL_I2C_Master_Transmit(&hi2c1,0x78,commands,sizeof(commands)/sizeof(commands[0]),HAL_MAX_DELAY);

	uint8_t dataRcvd;

	HAL_I2C_Master_Receive(&hi2c1,0x78,&dataRcvd,1,HAL_MAX_DELAY);

	if ((dataRcvd & (0x01 << 6)) == 0)
	{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else
	{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
