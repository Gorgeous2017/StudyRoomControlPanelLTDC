/**
	******************************************************************************
	* @file    FMC_SDRAM/stm32f4xx_it.c 
	* @author  MCD Application Team
	* @version V1.0.1
	* @date    11-November-2013
	* @brief   Main Interrupt Service Routines.
	*         This file provides template for all exceptions handler and
	*         peripherals interrupt service routine.
	******************************************************************************
	* @attention
	*
	* <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
	*
	* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
	* You may not use this file except in compliance with the License.
	* You may obtain a copy of the License at:
	*
	*        http://www.st.com/software_license_agreement_liberty_v2
	*
	* Unless required by applicable law or agreed to in writing, software 
	* distributed under the License is distributed on an "AS IS" BASIS, 
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
	*
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

#include "panel_usart.h"
#include "panel.h"
#include "./touch/bsp_i2c_touch.h"

extern void GTP_TouchProcess(void);
extern UartMsg statusMsg;

/** @addtogroup STM32F429I_DISCOVERY_Examples
	* @{
	*/

/** @addtogroup FMC_SDRAM
	* @{
	*/ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
	* @brief  This function handles NMI exception.
	* @param  None
	* @retval None
	*/
void NMI_Handler(void)
{
}

/**
	* @brief  This function handles Hard Fault exception.
	* @param  None
	* @retval None
	*/
void HardFault_Handler(void)
{
	
	printf("hardfault err");
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{}
}

/**
	* @brief  This function handles Memory Manage exception.
	* @param  None
	* @retval None
	*/
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{}
}

/**
	* @brief  This function handles Bus Fault exception.
	* @param  None
	* @retval None
	*/
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{}
}

/**
	* @brief  This function handles Usage Fault exception.
	* @param  None
	* @retval None
	*/
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{}
}

/**
	* @brief  This function handles Debug Monitor exception.
	* @param  None
	* @retval None
	*/
void DebugMon_Handler(void)
{}

/**
	* @brief  This function handles SVCall exception.
	* @param  None
	* @retval None
	*/
void SVC_Handler(void)
{}

/**
	* @brief  This function handles PendSV_Handler exception.
	* @param  None
	* @retval None
	*/
void PendSV_Handler(void)
{}

/**
	* @brief  This function handles SysTick Handler.
	* @param  None
	* @retval None
	*/
void SysTick_Handler(void)
{}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
 * @brief GTP_IRQHandler 触控事件中断处理函数
 * 
 */
void GTP_IRQHandler(void)
{
	if(EXTI_GetITStatus(GTP_INT_EXTI_LINE) != RESET) //确保是否产生了EXTI Line中断
	{
		GTP_TouchProcess();    
		EXTI_ClearITPendingBit(GTP_INT_EXTI_LINE);     //清除中断标志位
	}  
}

void ST_USART_IRQHandler(void) {

	PANEL_DEBUG("Function: ST_USART_IRQHandler in ");


}	

void AP_USART_IRQHandler(void) {

	PANEL_DEBUG("Function: AP_USART_IRQHandler in ");

	uint8_t i;
	uint8_t ucTemp;
	static uint8_t uCount = 0;

	if (USART_GetITStatus(AP_USART, USART_IT_RXNE) != RESET) {

		USART_ClearFlag(AP_USART,USART_IT_RXNE); 

		ucTemp = USART_ReceiveData( AP_USART );

		PANEL_DEBUG("Receive char is %#X, count is %d", ucTemp, uCount);

		if ( (ucTemp == statusMsg.MsgFlag) && (uCount == 0) ) { /* 接收到开始标志位，且之前没有收到消息 */

			PANEL_DEBUG("Receive Start");

			statusMsg.MsgBuff[ uCount++ ] = ucTemp; /* uCount自增一，下一次进入中断处理函数会执行下一条if语句 */

		} else if ( (uCount > 0) && (uCount < statusMsg.MsgLenth - 1 ) ) { /* 接收数据位 */

			PANEL_DEBUG("Receiving");

			statusMsg.MsgBuff[ uCount++ ] = ucTemp;

		} else if ( ucTemp == statusMsg.MsgFlag) { /* 接收完全部串口信息，且收到了结束标志位 */
			
			PANEL_DEBUG("Receive over");

			statusMsg.MsgBuff[ uCount ] = ucTemp;
			statusMsg.MsgHandler(&statusMsg); /* 将接收到的信息发送到UART3 */

			uCount = 0; /* 重置串口信息长度计数 */
			/* 清空信息接收缓存区 */			
			for ( i = 0; i < statusMsg.MsgLenth; i++)	{
				statusMsg.MsgBuff[i] = 0;
			}

		} else { /* 非正常的出错情况 */
			
			PANEL_DEBUG("Receive error ");

			uCount = 0; /* 重置串口信息长度计数 */
			/* 清空信息接收缓存区 */
			for ( i = 0; i < statusMsg.MsgLenth; i++)	{
				statusMsg.MsgBuff[i] = 0;
			}

		}
	
	}

} 




/**
	* @}
	*/ 

/**
	* @}
	*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
