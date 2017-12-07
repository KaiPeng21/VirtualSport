/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM6_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
// User Defined Function Declarations
void Bluetooth_Send_Accelerometer(signed short int x, signed short int y, signed short int z);
void Bluetooth_Send(char * message, uint16_t length);
void Bluetooth_Pack_and_Send(void);
void DRV_Setup(void);
void DRV_SelectLibrary(uint8_t lib);
void DRV_SetWaveform(uint8_t slot, uint8_t lib);
void DRV_SetMode(uint8_t mode);
void DRV_Go(void);
void DRV_Stop(void);
void DRV_UseERM(void);
void DRV_UseLRA(void);
void Haptics_VibrateHigh(void);
void Haptics_VibrateMid(void);
void Haptics_VibrateLow(void);
void ServoUp(void);
void ServoDown(void);

// Electrical Component Constants
// ADXL345 Accelerometer
uint16_t ADXL_DEV_ADDR = 0x53 << 1; // STM requires device address to be shifted by 1 bit
uint8_t ADXL_POWCTL = 0x2D;
uint8_t ADXL_DATAPTR = 0x32;
// DRV2605L Haptic Driver
uint16_t DRV2605_DEVICE = 0x5A << 1;
uint8_t DRV2605_REG_STATUS = 0x00;
uint8_t DRV2605_REG_MODE = 0x01;
uint8_t DRV2605_MODE_INTTRIG = 0x00;
uint8_t DRV2605_MODE_EXTIRIGEDGE = 0x01;
uint8_t DRV2605_MODE_EXTIGLVL = 0x02;
uint8_t DRV2605_MODE_PWMANALOG = 0x03;
uint8_t DRV2605_MODE_AUDIOVIBE = 0x04;
uint8_t DRV2605_MODE_REALTIME = 0x05;
uint8_t DRV2605_MODE_DIAGNOS = 0x06;
uint8_t DRV2605_MODE_AUTOCAL = 0x07;

uint8_t DRV2605_REG_RTPIN = 0x02;
uint8_t DRV2605_REG_LIBRARY = 0x03;
uint8_t DRV2605_REG_WAVESEQ1 = 0x04;
uint8_t DRV2605_REG_WAVESEQ2 = 0x05;
uint8_t DRV2605_REG_WAVESEQ3 = 0x06;
uint8_t DRV2605_REG_WAVESEQ4 = 0x07;
uint8_t DRV2605_REG_WAVESEQ5 = 0x08;
uint8_t DRV2605_REG_WAVESEQ6 = 0x09;
uint8_t DRV2605_REG_WAVESEQ7 = 0x0A;
uint8_t DRV2605_REG_WAVESEQ8 = 0x0B;

uint8_t DRV2605_REG_GO = 0x0C;
uint8_t DRV2605_REG_OVERDRIVE = 0x0D;
uint8_t DRV2605_REG_SUSTAINPOS = 0x0E;
uint8_t DRV2605_REG_SUSTAINNEG = 0x0F;
uint8_t DRV2605_REG_BREAK = 0x10;
uint8_t DRV2605_REG_AUDIOCTRL = 0x11;
uint8_t DRV2605_REG_AUDIOLVL = 0x12;
uint8_t DRV2605_REG_AUDIOMAX = 0x13;
uint8_t DRV2605_REG_RATEDV = 0x16;
uint8_t DRV2605_REG_CLAMPV = 0x17;
uint8_t DRV2605_REG_AUTOCALCOMP = 0x18;
uint8_t DRV2605_REG_AUTOCALEMP = 0x19;
uint8_t DRV2605_REG_FEEDBACK = 0x1A;
uint8_t DRV2605_REG_CONTROL1 = 0x1B;
uint8_t DRV2605_REG_CONTROL2 = 0x1C;
uint8_t DRV2605_REG_CONTROL3 = 0x1D;
uint8_t DRV2605_REG_CONTROL4 = 0x1E;
uint8_t DRV2605_REG_VBAT = 0x21;
uint8_t DRV2605_REG_LRARESON = 0x22;

// Servo Parameters
const long int TIM4_CCR_DEG_0 = 16;
const long int TIM4_CCR_DEG_45 = 24; // average of degree 0 and degree 90, 1.5 ms 
const long int TIM4_CCR_DEG_90 = 32;
uint8_t ServoState = 0; // 0 - IDLE, 1 - Slide Up, 2 - Slide Down

// Vibration Motor Parameters
const uint8_t TIM4_CCR_VIB_5 = 144;
const uint8_t TIM4_CCR_VIB_4 = 112;
const uint8_t TIM4_CCR_VIB_3 = 80;
const uint8_t TIM4_CCR_VIB_2 = 64;
const uint8_t TIM4_CCR_VIB_1 = 32;
const uint8_t TIM4_CCR_VIB_0 = 0;

uint8_t vib_slot_index = 0;
uint8_t vib_slot_size = 5;
uint8_t vib_slot[5];
uint8_t vib_flag = 0;
uint8_t torque_flag = 0;
uint8_t torque_ct = 0;

// Buffer Data Structure
uint8_t adxl_buffer[6];
signed short int x;
signed short int y;
signed short int z;

// I2C Buffer
uint8_t i2c_data[2];

// bluetooth receive buffer
uint8_t buffrec[5];
uint8_t prev_buffrec;

// haptic driver buffer
uint8_t buff_haptic[1];

// timer flags
uint8_t onems_ct;
uint8_t tenms_ct;
uint8_t hunms_ct;
uint8_t tenms;
uint8_t hunms;
uint8_t onesec;

// i2c transmit/ receive flag
uint8_t is_i2c_transmitted;

// pushbutton status
uint8_t pushNONE;
uint8_t pushSEL;
uint8_t pushINC;
uint8_t pushDEC;
uint8_t pushBEF;
uint8_t pushNXT;

int i = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM6) {
		onems_ct++;
	  if (onems_ct >= 10){
		  onems_ct = 0;
		  tenms = 1;
		  tenms_ct++;
			if (tenms_ct >= 10){
				tenms_ct = 0;
				hunms = 1;
				hunms_ct++;
				if (hunms_ct >= 10){
					hunms_ct = 0;
					onesec = 1;
				}
			}
	  }
	}
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

	onems_ct = 0;
	tenms_ct = 0;
  hunms_ct = 0;
  tenms = 0;
  hunms = 0;
  onesec = 0;
	
	is_i2c_transmitted = 0;
	
	vib_flag = 0;
	torque_flag = 0;
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */
	
	// USART and Bluetooth Interrupt Initializations
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	
	// I2C and ADXL345 Accelerometer initializations 
	i2c_data[0] = ADXL_POWCTL;
	i2c_data[1] = 0;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_DEV_ADDR, i2c_data, 2, 100);
	i2c_data[0] = ADXL_POWCTL;
	i2c_data[1] = 16;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_DEV_ADDR, i2c_data, 2, 100);
	i2c_data[0] = ADXL_POWCTL;
	i2c_data[1] = 8;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_DEV_ADDR, i2c_data, 2, 100);
	
	//DRV_Setup();
	
	// Start Timer
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim6);
	
	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	TIM4->CCR1 = 0;
	TIM4->CCR2 = 0;
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;
	
	HAL_Delay(1000);			


	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
		// retrieve the membrane pushbutton status
		pushNONE = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		pushSEL = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
		pushDEC = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
		pushINC = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
		pushBEF = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
		pushNXT = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
		
		
		if (tenms){
			tenms = 0;
			
			if (!is_i2c_transmitted){
				// Receive a haptic command from Bluetooth
				prev_buffrec = buffrec[0];
				HAL_UART_Receive(&huart1, buffrec, 5, 1);
				
				// If haptic command is received, decode command
				if (prev_buffrec != buffrec[0]){
					
					// Decode and respond to command
					if (buffrec[0] == 0x56)
					{
						Haptics_VibrateHigh();
						ServoUp();
					} else if (buffrec[0] == 0x57){
						Haptics_VibrateHigh();
						ServoDown();
					} else if (buffrec[0] == 0x66){
						Haptics_VibrateMid();
						ServoUp();
					} else if (buffrec[0] == 0x67){
						Haptics_VibrateMid();
						ServoDown();
					} else if (buffrec[0] == 0x76){
						Haptics_VibrateLow();
						ServoUp();
					} else if (buffrec[0] == 0x77) {
						Haptics_VibrateLow();
						ServoDown();
					}
					
					prev_buffrec = 0;
					buffrec[0] = 0;
				}
				
				// reset sliding handle to center after 0.2 sec
				if (torque_flag) {
					torque_ct++;
					if (torque_ct >= 10) {
						torque_ct = 0;
						torque_flag = 0;
						TIM4->CCR1 = TIM4_CCR_DEG_45;
					}
				} else {
						TIM4->CCR1 = TIM4_CCR_DEG_45;
				}
				
				// ping the accelerometer data from I2C line
				i2c_data[0] = ADXL_DATAPTR;
		    HAL_I2C_Master_Transmit(&hi2c1, ADXL_DEV_ADDR, i2c_data, 1, 100);
				is_i2c_transmitted = 1;
			}else{
				// Receive the accelerometer data from I2C line
				HAL_I2C_Master_Receive(&hi2c1, ADXL_DEV_ADDR, adxl_buffer, 6, 100);
				is_i2c_transmitted = 0;
				
				// parse the accelerometer data
				x = (((signed short int) adxl_buffer[1]) << 8) | adxl_buffer[0];
				y = (((signed short int) adxl_buffer[3]) << 8) | adxl_buffer[2];
				z = (((signed short int) adxl_buffer[5]) << 8) | adxl_buffer[4];
				
				// Transmit packet data to bluetooth
				//Bluetooth_Send_Accelerometer(x, y, z);
				Bluetooth_Pack_and_Send();
			}
			
		}
		
		if (hunms){
		  hunms = 0;	
			
			// Enable vibration when receiving vibration command
			if (vib_flag){
				TIM4->CCR2 = vib_slot[vib_slot_index];
				TIM4->CCR3 = vib_slot[vib_slot_index];
				TIM4->CCR4 = vib_slot[vib_slot_index];
				// Disable vibration when timeout
				if (vib_slot_index >= vib_slot_size){
					vib_flag = 0;
					vib_slot_index = 0;
				}
				vib_slot_index++;
			} else {
				TIM4->CCR2 = TIM4_CCR_VIB_0;
				TIM4->CCR3 = TIM4_CCR_VIB_0;
				TIM4->CCR4 = TIM4_CCR_VIB_0;
				vib_slot_index = 0;
			}
			
		}
		
		// debugging counter
		if (onesec){
			onesec = 0;
			i++;
			
		}
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 319;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 319;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim4);

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 799;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 19;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pins : PA0 PA1 PA4 PA5 
                           PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void Bluetooth_Send_Accelerometer(signed short int x, signed short int y, signed short int z)
{
	char * bluetooth_buff;
	uint8_t tmp;
	
	if (x >= 100){
		
		tmp = (x / 100) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (x / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (x % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
	
	} else if (x >= 10){
		
		tmp = (x / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (x % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (x >= 0){
		
		tmp = (x % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (x >= -9){
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-x % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (x >= -99){
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-x / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-x % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else{
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-x / 100) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-x / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-x % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	}
	
	bluetooth_buff = ",";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
	
	if (y >= 100){
		
		tmp = (y / 100) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (y / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (y % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
	
	} else if (y >= 10){
		
		tmp = (y / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (y % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (y >= 0){
		
		tmp = (y % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (y >= -9){
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-y % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (y >= -99){
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-y / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-y % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else{
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-y / 100) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-y / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-y % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	}
	
	bluetooth_buff = ",";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
	
	if (z >= 100){
		
		tmp = (z / 100) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (z / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (z % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
	
	} else if (z >= 10){
		
		tmp = (z / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (z % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (z >= 0){
		
		tmp = (z % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (z >= -9){
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-z % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else if (z >= -99){
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-z / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-z % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	} else{
		
		bluetooth_buff = "-";
		HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
		tmp = (-z / 100) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-z / 10) % 10 + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		tmp = (-z % 10) + 48;
		HAL_UART_Transmit(&huart1, &tmp, 1, 20);
		
	}
	
	bluetooth_buff = "\n\r";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 2, 20);
}

void Bluetooth_Send(char * message, uint16_t length)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)message, length, 20);
}

void Bluetooth_Pack_and_Send()
{
	uint8_t trans_byte;
	char * bluetooth_buff;
	
	// data structure:
	// <LX>, <HX>, <,>, <LY>, <HY>, <,>, <LZ>, <HZ>, <,>, <0,0,NONE,SEL,INC,DEC,BEF,NXT>, <\n>, <\r>
	
	
	trans_byte = adxl_buffer[0]; // lower byte of adxl x
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	trans_byte = adxl_buffer[1]; // higher byte of adxl x
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	
	bluetooth_buff = ",";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
	
	trans_byte = adxl_buffer[2]; // lower byte of adxl y
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	trans_byte = adxl_buffer[3]; // higher byte of adxl y
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	
  bluetooth_buff = ",";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
	
	trans_byte = adxl_buffer[4]; // lower byte of adxl z
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	trans_byte = adxl_buffer[5]; // higher byte of adxl z
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	
  bluetooth_buff = ",";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 1, 20);
	
	// pushbutton bits
	trans_byte =  (pushNONE << 5) | (pushSEL << 4) | (pushINC << 3) | (pushDEC << 2) | (pushBEF << 1) | pushNXT;
	HAL_UART_Transmit(&huart1, &trans_byte, 1, 20);
	
	bluetooth_buff = "\n\r";
	HAL_UART_Transmit(&huart1, (uint8_t *)bluetooth_buff, 2, 20);
}

void DRV_Setup()
{
	i2c_data[0] = DRV2605_REG_MODE;
	i2c_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);	
  HAL_Delay(10);
	i2c_data[0] = DRV2605_REG_RTPIN;
	i2c_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
  HAL_Delay(10);	
  i2c_data[0] = DRV2605_REG_OVERDRIVE;
	i2c_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
	i2c_data[0] = DRV2605_REG_SUSTAINPOS;
	i2c_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
	i2c_data[0] = DRV2605_REG_SUSTAINNEG;
	i2c_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
	i2c_data[0] = DRV2605_REG_BREAK;
	i2c_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
	i2c_data[0] = DRV2605_REG_AUDIOMAX;
	i2c_data[1] = 0x64;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
	
	DRV_UseLRA();
	
	// turn on ERM_LRA_LOOP
	i2c_data[0] = DRV2605_REG_CONTROL3;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 1, 100);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(&hi2c1, DRV2605_DEVICE, buff_haptic, 1, 100);
	i2c_data[0] = DRV2605_REG_CONTROL3;
	i2c_data[1] = buff_haptic[0] | 0x20;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
	
	DRV_SelectLibrary(1);
	DRV_SetMode(DRV2605_MODE_INTTRIG);
}

void DRV_SelectLibrary(uint8_t lib)
{
	i2c_data[0] = DRV2605_REG_LIBRARY;
	i2c_data[1] = lib;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
}
void DRV_SetWaveform(uint8_t slot, uint8_t lib)
{
	i2c_data[0] = DRV2605_REG_WAVESEQ1 + slot;
	i2c_data[1] = lib;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);	
}
void DRV_SetMode(uint8_t mode)
{
  i2c_data[0] = DRV2605_REG_MODE;
	i2c_data[1] = mode;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
}
void DRV_Go(void)
{
  i2c_data[0] = DRV2605_REG_GO;
	i2c_data[1] = 1;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
}
void DRV_Stop(void)
{
  i2c_data[0] = DRV2605_REG_GO;
	i2c_data[1] = 0;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);	
}
void DRV_UseERM(void)
{
  i2c_data[0] = DRV2605_REG_FEEDBACK;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 1, 100);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(&hi2c1, DRV2605_DEVICE, buff_haptic, 1, 100);
	i2c_data[0] = DRV2605_REG_FEEDBACK;
	i2c_data[1] = buff_haptic[0] & 0x7F;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
}
void DRV_UseLRA(void)
{
  i2c_data[0] = DRV2605_REG_FEEDBACK;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 1, 100);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(&hi2c1, DRV2605_DEVICE, buff_haptic, 1, 100);
	i2c_data[0] = DRV2605_REG_FEEDBACK;
	i2c_data[1] = buff_haptic[0] | 0x80;
	HAL_I2C_Master_Transmit(&hi2c1, DRV2605_DEVICE, i2c_data, 2, 100);
	HAL_Delay(10);
}

void Haptics_VibrateHigh(void)
{
	// set DRV registers
	DRV_SetWaveform(0, 7);
	DRV_SetWaveform(1, 1);
	DRV_SetWaveform(2, 1);
	DRV_SetWaveform(3, 0);
	DRV_Go();
	
	// set PWM parameters
	vib_slot[0] = TIM4_CCR_VIB_4;
	vib_slot[1] = TIM4_CCR_VIB_5;
	vib_slot[2] = TIM4_CCR_VIB_5;
	vib_slot[3] = TIM4_CCR_VIB_5;
	vib_slot[4] = TIM4_CCR_VIB_2;
	vib_flag = 1;
}
void Haptics_VibrateMid(void)
{
	// set DRV registers
	DRV_SetWaveform(0, 7);
	DRV_SetWaveform(1, 1);
	DRV_SetWaveform(2, 0);
	DRV_SetWaveform(3, 0);
	DRV_Go();	
	
	// set PWM parameters
	vib_slot[0] = TIM4_CCR_VIB_3;
	vib_slot[1] = TIM4_CCR_VIB_4;
	vib_slot[2] = TIM4_CCR_VIB_4;
	vib_slot[3] = TIM4_CCR_VIB_3;
	vib_slot[4] = TIM4_CCR_VIB_1;
	vib_flag = 1;
}
void Haptics_VibrateLow(void)
{
	DRV_SetWaveform(0, 9);
	DRV_SetWaveform(1, 9);
	DRV_SetWaveform(2, 0);
	DRV_SetWaveform(3, 0);
	DRV_Go();	
	
	// set PWM parameters
	vib_slot[0] = TIM4_CCR_VIB_2;
	vib_slot[1] = TIM4_CCR_VIB_3;
	vib_slot[2] = TIM4_CCR_VIB_2;
	vib_slot[3] = TIM4_CCR_VIB_1;
	vib_slot[4] = TIM4_CCR_VIB_0;
	vib_flag = 1;
}

void ServoUp(void)
{
	TIM4->CCR1 = TIM4_CCR_DEG_90;
	torque_flag = 1;

}
void ServoDown(void)
{
	TIM4->CCR1 = TIM4_CCR_DEG_0;
	torque_flag = 1;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
