/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdint.h>
#include "stm32f0xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define MAX_ITER 100
/* USER CODE END PTD */
#define SCALE_BITS 16
#define SCALE (1 << SCALE_BITS)  // 65536
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//TODO: Define and initialise the global varibales required
/*
  start_time
  end_time
  execution_time 
  checksum: should be uint64_t
  initial width and height maybe or you might opt for an array??
*/
uint16_t test_image_sizes[] = { 128, 160, 192, 224, 256 };
//variables profiling for execution time
uint32_t start_time = 0;
uint32_t end_time   = 0;

uint32_t mandelbrot_checksum = 0;//to hole checksum returned
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
uint64_t calculate_mandelbrot_fixed_point_arithmetic(int width, int height, int max_iterations);
uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations);


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
  /* USER CODE BEGIN 2 */
  //TODO: Turn on LED 0 to signify the start of the operation
  LL_GPIO_ResetOutputPin(GPIOB, 0b11111111);  // Clear all pins on GPIOB
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  // Set just pin 0 high

  //TODO: Record the start time
  start_time = HAL_GetTick(); // time in ms before
  
  //TODO: Call the Mandelbrot Function and store the output in the checksum variable defined initially
  for(int i =0; i<5; i++){
  mandelbrot_checksum = calculate_mandelbrot_fixed_point_arithmetic(test_image_sizes[i], test_image_sizes[i],MAX_ITER);
  }
  //TODO: Record the end time
  
   end_time = HAL_GetTick(); // time in ms before
  //TODO: Calculate the execution time
  
   uint32_t execution_time = end_time - start_time;
  //TODO: Turn on LED 1 to signify the end of the operation
   LL_GPIO_ResetOutputPin(GPIOB, 0b11111111);  // Clear all pins on GPIOB
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);  // Set just pin 0 high

  //TODO: Hold the LEDs on for a 1s delay
   HAL_Delay(1000);

  //TODO: Turn off the LEDs
  

   HAL_GPIO_WritePin(GPIOB, 0b11111111, GPIO_PIN_RESET);  // Set just pin 0 high
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
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

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
  _HAL_RCC_GPIOB_CLK_ENABLE();
  _HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//TODO: Mandelbroat using variable type integers and fixed point arithmetic
uint64_t calculate_mandelbrot_fixed_point_arithmetic(int width, int height, int max_iterations){
  uint64_t mandelbrot_sum = 0;
    //TODO: Complete the function implementation
  for (int y = 0; y < height; y++) {
          for (int x = 0; x < width; x++) {
              // Scale x0 to [-2.5, 1.0] in fixed-point(converting float to fixed point)
              int64_t x0 = ((x * 3 * SCALE) / width) - (5 * SCALE / 2);  // x0 = x/width * 3.5 - 2.5
              // Scale y0 to [-1.0, 1.0] in fixed-point
              int64_t y0 = ((y * 2 * SCALE) / height) - (1 * SCALE);     // y0 = y/height * 2.0 - 1.0

              int64_t xi = 0, yi = 0;
              int iteration = 0;

              while (iteration < max_iterations) {
                  // Compute xi² and yi² (fixed-point multiplication)
                  int64_t xi_sq = (xi * xi) >> SCALE_BITS;
                  int64_t yi_sq = (yi * yi) >> SCALE_BITS;

                  // Check escape condition (xi² + yi² > 4.0 in fixed-point)
                  if (xi_sq + yi_sq > (4 * SCALE)) {
                      break;
                  }

                  // Compute new xi and yi (fixed-point arithmetic)
                  int64_t temp = xi_sq - yi_sq;
                  yi = ((2 * xi * yi) >> SCALE_BITS) + y0;
                  xi = temp + x0;

                  iteration++;
              }

              mandelbrot_sum += iteration;
          }
      }

    return mandelbrot_sum;

}

//TODO: Mandelbroat using variable type double
uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations){
    uint64_t mandelbrot_sum = 0;
    //TODO: Complete the function implementation
    
    for (int y = 0; y < height; y++) {
           for (int x = 0; x < width; x++) {
               // Convert pixel coordinates to Mandelbrot coordinates (floating-point)
               double x0 = (double)x / width * 3.5 - 2.5;  // x0 ∈ [-2.5, 1.0]
               double y0 = (double)y / height * 2.0 - 1.0;  // y0 ∈ [-1.0, 1.0]

               double xi = 0.0, yi = 0.0;
               int iteration = 0;

               while (iteration < max_iterations) {
                   double xi_sq = xi * xi;
                   double yi_sq = yi * yi;

                   // Escape condition: xi² + yi² > 4.0
                   if (xi_sq + yi_sq > 4.0) {
                       break;
                   }

                   // Update zi = zi² + c (Mandelbrot formula)
                   double temp = xi_sq - yi_sq;
                   yi = 2 * xi * yi + y0;
                   xi = temp + x0;

                   iteration++;
               }

               mandelbrot_sum += iteration;
           }
       }

    return mandelbrot_sum;
}

uint64_t calculate_mandelbrot_float(int width, int height, int max_iterations){
    uint64_t mandelbrot_sum = 0;
    //TODO: Complete the function implementation

    for (int y = 0; y < height; y++) {
           for (int x = 0; x < width; x++) {
               // Convert pixel coordinates to Mandelbrot coordinates (floating-point)
               float x0 = (float)x / width * 3.5 - 2.5;  // x0 ∈ [-2.5, 1.0]
               float y0 = (float)y / height * 2.0 - 1.0;  // y0 ∈ [-1.0, 1.0]

               float xi = 0.0, yi = 0.0;
               int iteration = 0;

               while (iteration < max_iterations) {
                   float xi_sq = xi * xi;
                   float yi_sq = yi * yi;

                   // Escape condition: xi² + yi² > 4.0
                   if (xi_sq + yi_sq > 4.0) {
                       break;
                   }

                   // Update zi = zi² + c (Mandelbrot formula)
                   float temp = xi_sq - yi_sq;
                   yi = 2 * xi * yi + y0;
                   xi = temp + x0;

                   iteration++;
               }

               mandelbrot_sum += iteration;
           }
       }

    return mandelbrot_sum;
}

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
