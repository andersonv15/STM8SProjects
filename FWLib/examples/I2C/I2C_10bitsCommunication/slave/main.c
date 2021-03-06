/**
  ******************************************************************************
  * @file I2C_10bitsCommunication\slave\main.c
  * @brief This file contains the main function for I2C configured in slave 10 bit addressing mode.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"


/* Private defines*/
/* Evalboard I/Os configuration */

#define LEDS_PORT (GPIOH)
#define LED1_PIN  (GPIO_PIN_3)
#define LED2_PIN  (GPIO_PIN_2)
#define LED3_PIN  (GPIO_PIN_1)
#define LED4_PIN  (GPIO_PIN_0)

#define RX_BUFFERSIZE       ((u8)4)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 I2C_Buffer_Rx[RX_BUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @addtogroup I2C_10bit-slave
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief How to perform  a communication between 2 I2C in 10bit addressing mode, fast mode (slave receiver)
  * @par Examples description
  * - receive a buffer from a second 10bit addressed I2C device.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
  vu8 Rx_Idx = 0;
  u8 Input_Clock = 0x0;
  
  /* Initialize I/Os in Output Mode */
  GPIO_Init(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST);
  
  /* Get system clock frequency */
  Input_Clock = CLK_GetClockFreq()/1000000;
  
  /* Optional: put here as example only */
  I2C_DeInit();
  
  /* Initialize the I2C peripheral */
  I2C_Init(100000, 0x0102, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_10BIT, Input_Clock);
  
  /* All LEDs are ON per default */
  GPIO_WriteHigh(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));

  /* Test on EV1 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_SLAVE_ADDRESS_MATCHED));
  I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	
  /* Receive data */
   while (Rx_Idx < RX_BUFFERSIZE)
  {
    /* Test on EV2 and clear it */
    while(!I2C_CheckEvent(I2C_EVENT_SLAVE_BYTE_RECEIVED));
    /* Store received data  */
    I2C_Buffer_Rx[Rx_Idx++] = I2C_ReceiveData();
  }
  
  /* Test on EV4 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_SLAVE_STOP_DETECTED));
  
  /* Clear STOPF flag */
  I2C_ClearFlag(I2C_FLAG_STOPDETECTION);

	while(1)
	{
		GPIO_WriteReverse(LEDS_PORT,  (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));
		Delay((u16)60000);
	}
}

/**
  * @brief Delay.
  * @param[in] nCount
  * @retval 
  * None
  */
void Delay(u16 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval 
  * None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
