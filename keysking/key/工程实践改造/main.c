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
typedef struct {
    GPIO_TypeDef* Port;      // 按键接在哪个端口（GPIOA / GPIOB...）
    uint16_t Pin;            // 按键接在哪个引脚（GPIO_PIN_0 / GPIO_PIN_1...）
    uint8_t  State;          // 当前状态
    uint32_t PressTick;      // 上次按下时的时间戳
    uint8_t  Event;          // 发生了什么事件（短按/长按）
} Key_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEY_IDLE      0   // 空闲：没按任何东西
#define KEY_DEBOUNCE  1   // 消抖中：刚检测到按下，等 10ms 确认
#define KEY_PRESSED   2   // 已按下：确认按键真的按下了
#define KEY_RELEASE   3   // 已释放：检测到松开了
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Key_t Key1 = {KEY1_GPIO_Port, KEY1_Pin, KEY_IDLE, 0, 0};
Key_t Key2 = {KEY2_GPIO_Port, KEY2_Pin, KEY_IDLE, 0, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void Key_Scan(Key_t* key);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Key_Scan(Key_t* key) {
    // 1. 先读取当前引脚的电平
    uint8_t level = HAL_GPIO_ReadPin(key->Port, key->Pin);
    
    // 2. 根据当前状态，决定怎么做
    switch (key->State) {
        
        // ---------- 状态 0：空闲 ----------
        case KEY_IDLE:
            if (level == GPIO_PIN_RESET) {          // 检测到按键被按下了
                key->PressTick = HAL_GetTick();      // 记下当前时间
                key->State = KEY_DEBOUNCE;           // 进入"消抖"状态
            }
            break;
        
        // ---------- 状态 1：消抖中 ----------
        case KEY_DEBOUNCE:
            if (HAL_GetTick() - key->PressTick >= 10) {  // 距离上次记录已经过了10ms
                if (level == GPIO_PIN_RESET) {            // 10ms后还是按下状态，说明是真按
                    key->State = KEY_PRESSED;             // 进入"已按下"
                    key->Event = 1;                       // 标记：发生了一次短按事件
                    key->PressTick = HAL_GetTick();       // 重新记录时间，用于长按判断
                } else {
                    key->State = KEY_IDLE;                // 10ms后已经松开了，说明是抖动，回去
                }
            }
            break;
        
        // ---------- 状态 2：已按下 ----------
        case KEY_PRESSED:
            if (HAL_GetTick() - key->PressTick >= 1000) {  // 按住超过 1 秒
                key->Event = 2;                             // 标记：长按事件
                //key->State = KEY_RELEASE;                   // 进入"等松开"
            }
            if (level != GPIO_PIN_RESET) {                  // 如果松开了
                key->State = KEY_RELEASE;                   // 也进入"等松开"
            }
            break;
        
        // ---------- 状态 3：等松开 ----------
        case KEY_RELEASE:
            if (level != GPIO_PIN_RESET) {                  // 确认完全松开了
                key->State = KEY_IDLE;                      // 回到空闲，等待下一次按键
            }
            break;
    }
}
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
  /* USER CODE BEGIN 2 */
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      Key_Scan(&Key1);   // 快速扫一眼按键1
      Key_Scan(&Key2);   // 快速扫一眼按键2
        
        // ---- 处理 Key1 的事件 ----
        if (Key1.State == KEY_PRESSED) {
            // 按住的时候亮绿灯
            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
        } else {
            // 松开的时候灭绿灯
            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
        }
       //if(Key1.Event)
        // ---- 处理 Key2 的事件 ----
        if (Key2.Event == 1) {              // 短按事件
            HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
            Key2.Event = 0;                 // 处理完记得清零，否则会一直触发
        }
        if (Key2.Event == 2) {              // 长按事件
            // 比如：长按进入某个模式
            // 你自己写逻辑
            while(Key2.State == KEY_PRESSED) {
                // 这里可以做一些事情，比如闪烁红灯
                HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
                HAL_Delay(500);
                Key_Scan(&Key2);   // 快速扫一眼按键2
            }
            Key2.Event = 0;                 // 处理完记得清零
        }
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY1_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY2_Pin */
  GPIO_InitStruct.Pin = KEY2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStruct);

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
