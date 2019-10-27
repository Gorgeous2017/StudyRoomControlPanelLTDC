/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

/**
 * @addtogroup UART
 * @{
 */

/**
 * @defgroup Init
 * @{
 */

void PANEL_USART_Config(void){

  AP_USART_Config();
  ST_USART_Config();
  NVIC_Configuration();

}

/**
 * @} 
 */


/**
 * @addtogroup AP_UART
 * @{
 */

/**
 * @defgroup Config
 * @{
 */

 /**
  * @brief  AP_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void AP_USART_Config(void) {

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( AP_USART_RX_GPIO_CLK|AP_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB2PeriphClockCmd(AP_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(AP_USART_RX_GPIO_PORT,AP_USART_RX_SOURCE, AP_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(AP_USART_TX_GPIO_PORT,AP_USART_TX_SOURCE,AP_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = AP_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(AP_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = AP_USART_RX_PIN;
  GPIO_Init(AP_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串AP_USART 模式 */
  USART_InitStructure.USART_BaudRate = AP_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(AP_USART, &USART_InitStructure); 
  USART_Cmd(AP_USART, ENABLE);
}

/**
 * @} 
 */

/**
 * @} 
 */

/**
 * @addtogroup ST_UART
 * @{
 */

/**
 * @defgroup Config
 * @{
 */

/**
 * @brief DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
 * 
 */
void ST_USART_Config(void) {

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( ST_USART_RX_GPIO_CLK|ST_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB2PeriphClockCmd(ST_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(ST_USART_RX_GPIO_PORT,ST_USART_RX_SOURCE, ST_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(ST_USART_TX_GPIO_PORT,ST_USART_TX_SOURCE,ST_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = ST_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ST_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = ST_USART_RX_PIN;
  GPIO_Init(ST_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串ST_USART 模式 */
  USART_InitStructure.USART_BaudRate = ST_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(ST_USART, &USART_InitStructure); 
  USART_Cmd(ST_USART, ENABLE);
}
/**
 * @} 
 */

/**
 * @} 
 */

/**
 * @defgroup NVIC
 * @brief 初始化AP和ST的串口中断
 * @{
 */

/**
 * @brief AP和ST的串口中断初始化函数
 * 
 */
void NVIC_Configuration(void) {

  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = ST_USART_IRQ;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = AP_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @} 
 */

/**
 * @defgroup Redirect
 * @brief 重定向c库输入输出函数
 * @{
 */

///重定向c库函数printf到串口AP_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口AP_USART */
		USART_SendData(AP_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(AP_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口AP_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(AP_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(AP_USART);
}

/**
 * @} 
 */

/**
 * @} 
 */



/*********************************************END OF FILE**********************/
