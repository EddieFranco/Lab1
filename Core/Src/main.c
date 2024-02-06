/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
HAL_Init(); // Reset of all peripherals, init the Flash and Systick
SystemClock_Config(); //Configure the system clock

//enable the GPIOC peripheral clock
RCC->AHBENR |= RCC_AHBENR_GPIOCEN ;

//Set the pins to general-purpose output mode in the MODER register.
GPIOC->MODER|=(GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0);
// Set the pins to push-pull output type in the OTYPER register
GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 |GPIO_OTYPER_OT_7 );
// Set the pins to low speed in the OSPEEDR register.	
GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6_Msk | GPIO_OSPEEDR_OSPEEDR7_Msk);
// Set to no pull-up/down resistors in the PUPDR register
GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6_Msk | GPIO_PUPDR_PUPDR7_Msk);	
//Initialize one pin logic high and the other to low.
GPIOC->BSRR  = GPIO_BSRR_BS_6 ;
GPIOC->BSRR = GPIO_BSRR_BR_7 ;
	

//while (1) {
// Toggle both pin output states within the endless loop.
		//GPIOC->ODR ^= (GPIO_ODR_6 | GPIO_ODR_7); // Toggle PC8 and PC9
		//HAL_Delay(200); // Delay 200ms
//}


//enable the clock to the appropriate GPIO peripheral
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

//Set the pins to input mode in the MODER register.
GPIOA->MODER &= ~ (GPIO_MODER_MODER0_Msk);
//Set the pins to low speed in the OSPEEDR register
GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0_Msk);
//Enable the pull-down resistor in the PUPDR register
GPIOA->OSPEEDR &= ~(GPIO_PUPDR_PUPDR0_Msk);


uint32_t debouncer = 0;
uint32_t LED = 0;

while (1) {
	
			int button = GPIOA->IDR & 1;
			debouncer = (debouncer << 1); // Always shift every loop iteration
	
			if (button) { // If input signal is set/high
			debouncer |= 0x01; // Set lowest bit of bit-vector
			}
			
		
			 if (debouncer == 0x7FFFFFFF) {
				 // This code triggers only once when transitioning to steady high!
					if (LED == 0){
					GPIOC->BSRR = GPIO_BSRR_BR_6; // Set PC6 low
					GPIOC->BSRR = GPIO_BSRR_BS_7; // Set PC7 high
					LED = 1;
					}
				else{
        GPIOC->BSRR = GPIO_BSRR_BS_6; // Set PC6 high
        GPIOC->BSRR = GPIO_BSRR_BR_7; // Set PC7 low
        LED = 0;
				}
    }
			
  
  HAL_Delay(1);
			// When button is bouncing the bit-vector value is random since bits are set when
			//the button is high and not when it bounces low.
				
}



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
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
