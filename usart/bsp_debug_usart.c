/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  AP_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void AP_USART_Config(void) {

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( AP_USART_RX_GPIO_CLK|AP_USART_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB2PeriphClockCmd(AP_USART_CLK, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(AP_USART_RX_GPIO_PORT,AP_USART_RX_SOURCE, AP_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(AP_USART_TX_GPIO_PORT,AP_USART_TX_SOURCE,AP_USART_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = AP_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(AP_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = AP_USART_RX_PIN;
  GPIO_Init(AP_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô�AP_USART ģʽ */
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
 * @brief DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
 * 
 */
void ST_USART_Config(void) {

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( ST_USART_RX_GPIO_CLK|ST_USART_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB2PeriphClockCmd(ST_USART_CLK, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(ST_USART_RX_GPIO_PORT,ST_USART_RX_SOURCE, ST_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(ST_USART_TX_GPIO_PORT,ST_USART_TX_SOURCE,ST_USART_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = ST_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ST_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = ST_USART_RX_PIN;
  GPIO_Init(ST_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô�ST_USART ģʽ */
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
 * @brief ��ʼ��AP��ST�Ĵ����ж�
 * @{
 */

/**
 * @brief AP��ST�Ĵ����жϳ�ʼ������
 * 
 */
void NVIC_Configuration(void) {

  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = ST_USART_IRQ;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
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
 * @brief �ض���c�������������
 * @{
 */

///�ض���c�⺯��printf������AP_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����AP_USART */
		USART_SendData(AP_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(AP_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������AP_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
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
