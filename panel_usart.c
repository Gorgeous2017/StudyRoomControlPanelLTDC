/**
 * @file panel_usart.c
 * @author Gump ()
 * @version V0.1
 * @date 2019-10-27
 * @brief
 *   - 初始化串口配置
 *   - 使能串口NVIC
 *   - 串口通讯通用功能函数
 *
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 *
 */

#include "panel_usart.h"
#include "panel.h"

void Update_StatusMsg(void* uartMsg ) {

	UartMsg *ptr = (UartMsg *)uartMsg;

	uint8_t i;
	for ( i = 0; i < ptr->MsgLenth; i++ ) {
		PANEL_DEBUG("MsgBuff[%d] = %#X", i, ptr->MsgBuff[i]);
	}

	/* 更新面板上的环境信息 */
	AP_DisplayStatus();
	/* 更新物联网公有云上的环境信息 */
	Usart_SendBuff(ST_USART, ptr->MsgBuff, ptr->MsgLenth);

}

/**
 * @brief ST uart 指令串处理函数
 * @details 
 * 	- 将对应用电器的状态更新
 *  - 若当前面板所处的页面是该指令串所指的用电器，重绘当前页面
 * 
 * @param uartMsg 
 */
void Update_DeviceMsg(void* uartMsg ) {

	UartMsg *ptr = (UartMsg *)uartMsg;

	uint8_t i;
	for ( i = 0; i < ptr->MsgLenth; i++ ) {
			PANEL_DEBUG("MsgBuff[%d] = %#X", i, ptr->MsgBuff[i]);
	}

	//PANEL_DEBUG("");

	/**
	 * @brief 更新串口指令串所指向的用电器的状态
	 * @param 
	 * 	@arg MsgBuff[1]: 用电器类型
	 * 	@arg MsgBuff[2]: 用电器编号
	 * 	@arg MsgBuff[3]: 用电器状态
	 * 	
	 */
	device[ ptr->MsgBuff[1] ][ ptr->MsgBuff[2] ].status = ptr->MsgBuff[3];
	
	/**
	 * @brief 判断当前面板所处的页面是该指令串所指的用电器
	 * @param last_widget: Touch_Icon 类型的指针 用于记录面板最后一次被触控的控件
	 * @details 当 Draw_Widget 函数被调用时，last_widget 会更新为调用该函数的图标数组指针，
	 * 以此来记录当前面板所处的页面。 
	 */
	if( last_widget == device[ ptr->MsgBuff[1] ] ) {

		Draw_Widget(device[ ptr->MsgBuff[1] ], 6);

	}

	/* 将控制用电器的指令串转发给AP */
	Usart_SendBuff(AP_USART, ptr->MsgBuff, 5);
	/* 将控制用电器的指令串发回给ST，用于更新云端用电器状态 */
	Usart_SendBuff(ST_USART, ptr->MsgBuff, 5);

}


/**
 * @addtogroup UART
 * @{
 */

/**
 * @defgroup Variables
 * @{
 */

UartMsg statusMsg = {
		.MsgLenth = 6,
		.MsgFlag = 0xFF,
		.MsgHandler = Update_StatusMsg
}; /*!< 从AP接收到的环境信息 */

UartMsg ctrlMsg = {
		.MsgLenth = 5,
		.MsgFlag = 0XFE,
		.MsgHandler = Update_DeviceMsg
};  /*!< 从ST接收到的云端下发的用电器控制信息  */

/**
 * @}
 */

/**
 * @defgroup Init
 * @{
 */

void PANEL_USART_Config(void)
{
	DEBUG_USART_Config(); /* 调试串口配置 */
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
 * @defgroup AP_Config
 * @{
 */

/**
  * @brief  AP_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void AP_USART_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(AP_USART_RX_GPIO_CLK | AP_USART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART 时钟 */
	RCC_APB2PeriphClockCmd(AP_USART_CLK, ENABLE);

	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(AP_USART_RX_GPIO_PORT, AP_USART_RX_SOURCE, AP_USART_RX_AF);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(AP_USART_TX_GPIO_PORT, AP_USART_TX_SOURCE, AP_USART_TX_AF);

	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = AP_USART_TX_PIN;
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
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_ITConfig(AP_USART,USART_IT_RXNE, ENABLE); /* !!! 使能串口中断 */
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
 * @defgroup ST_Config
 * @{
 */

/**
 * @brief DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
 *
 */
void ST_USART_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(ST_USART_RX_GPIO_CLK | ST_USART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART 时钟 */
	RCC_APB1PeriphClockCmd(ST_USART_CLK, ENABLE);

	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(ST_USART_RX_GPIO_PORT, ST_USART_RX_SOURCE, ST_USART_RX_AF);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(ST_USART_TX_GPIO_PORT, ST_USART_TX_SOURCE, ST_USART_TX_AF);

	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = ST_USART_TX_PIN;
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
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_ITConfig(ST_USART,USART_IT_RXNE, ENABLE); /* !!! 使能串口中断 */
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
 * @addtogroup DEBUG_UART
 * @{
 */

/**
 * @defgroup DEBUG_Config
 * @{
 */

/**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @note	调试串口，无需使能中断
  * @param  无
  * @retval 无
  */
void DEBUG_USART_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(DEBUG_USART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART 时钟 */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_TX_SOURCE, DEBUG_USART_TX_AF);

	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置串DEBUG_USART 模式 */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;

	USART_Init(DEBUG_USART, &USART_InitStructure);
	USART_Cmd(DEBUG_USART, ENABLE);
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
void NVIC_Configuration(void)
{

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

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	USART_SendData(DEBUG_USART, (uint8_t)ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
		;

	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET)
		;

	return (int)USART_ReceiveData(DEBUG_USART);
}

/**
 * @}
 */

/**
 * @defgroup Transfer
 * @brief 串口通讯通用功能函数
 * @{
 */

/**
 * @brief Usart_SendByte 发送单个字符
 *
 * @param pUSARTx 需要发送的串口
 * @param ch 需要发送的字符
 */
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{

	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx, ch);

	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/**
 * @brief Usart_SendBuff 发送一串字符
 *
 * @param buf 需要发送的字符数组
 * @param len 需要发送的字符数组长度
 */
void Usart_SendBuff(USART_TypeDef *pUSARTx, uint8_t *buf, uint8_t len)
{

	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, buf[k]);
		k++;
	} while (k < len);

	/* 等待发送完成 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/**
 * @}
 */

/**
 * @}
 */

/*********************************************END OF FILE**********************/
