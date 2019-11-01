#ifndef __PANEL_USART_H
#define __PANEL_USART_H

#include "stm32f4xx.h"
#include <stdio.h>

/**
 * @defgroup UART
 * @{
 */

/**
 * @brief Data Structure
 * @{
 */

/**
 * @brief 串口信息结构体
 * 
 */
typedef struct
{
    uint8_t MsgBuff[10];    /*!< 串口信息缓存区 */
    const uint8_t MsgLenth; /*!< 串口信息的长度 */
    const uint8_t MsgFlag;  /*!< 串口信息的首部和尾部标志 */

    void (*MsgHandler)(void *uartMsg); /*!< 串口信息处理函数 */

} UartMsg;

extern UartMsg statusMsg;
extern UartMsg ctrlMsg;

/**
 * @} 
 */

/**
 * @defgroup AP_UART
 * @brief AP与面板进行通讯的串口
 * @{
 */

/**
 * @defgroup AP_Macro
 * @{
 */
#define AP_USART USART1
#define AP_USART_CLK RCC_APB2Periph_USART1
#define AP_USART_BAUDRATE 115200

#define AP_USART_RX_GPIO_PORT GPIOA
#define AP_USART_RX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define AP_USART_RX_PIN GPIO_Pin_10
#define AP_USART_RX_AF GPIO_AF_USART1
#define AP_USART_RX_SOURCE GPIO_PinSource10

#define AP_USART_TX_GPIO_PORT GPIOA
#define AP_USART_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define AP_USART_TX_PIN GPIO_Pin_9
#define AP_USART_TX_AF GPIO_AF_USART1
#define AP_USART_TX_SOURCE GPIO_PinSource9

#define AP_USART_IRQHandler USART1_IRQHandler
#define AP_USART_IRQ USART1_IRQn

/**
 * @} 
 */

/**
 * @} 
 */

/**
 * @defgroup ST_UART
 * @brief ST与面板进行通讯的串口
 * @{
 */

/**
 * @defgroup ST_Macro
 * @{
 */
#define ST_USART USART3
#define ST_USART_CLK RCC_APB1Periph_USART3
#define ST_USART_BAUDRATE 115200

#define ST_USART_RX_GPIO_PORT GPIOC
#define ST_USART_RX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define ST_USART_RX_PIN GPIO_Pin_11
#define ST_USART_RX_AF GPIO_AF_USART3
#define ST_USART_RX_SOURCE GPIO_PinSource11

#define ST_USART_TX_GPIO_PORT GPIOC
#define ST_USART_TX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define ST_USART_TX_PIN GPIO_Pin_10
#define ST_USART_TX_AF GPIO_AF_USART3
#define ST_USART_TX_SOURCE GPIO_PinSource10

#define ST_USART_IRQHandler USART3_IRQHandler
#define ST_USART_IRQ USART3_IRQn

/**
 * @} 
 */

/**
 * @} 
 */


/**
 * @defgroup DEBUG_UART
 * @brief 用于输出调试信息的串口
 * @{
 */

/**
 * @defgroup DEBUG_Macro
 * @{
 */
#define DEBUG_USART USART6
#define DEBUG_USART_CLK RCC_APB2Periph_USART6
#define DEBUG_USART_BAUDRATE 115200

#define DEBUG_USART_TX_GPIO_PORT GPIOC
#define DEBUG_USART_TX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define DEBUG_USART_TX_PIN GPIO_Pin_6
#define DEBUG_USART_TX_AF GPIO_AF_USART6
#define DEBUG_USART_TX_SOURCE GPIO_PinSource6

#define DEBUG_USART_RX_GPIO_PORT GPIOC
#define DEBUG_USART_RX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define DEBUG_USART_RX_PIN GPIO_Pin_7
#define DEBUG_USART_RX_AF GPIO_AF_USART6
#define DEBUG_USART_RX_SOURCE GPIO_PinSource7

/**
 * @} 
 */

/**
 * @} 
 */


/**
 * @defgroup Function
 * @{
 */
void PANEL_USART_Config(void);
void DEBUG_USART_Config(void);
void AP_USART_Config(void);
void ST_USART_Config(void);
void NVIC_Configuration(void);

void Usart_SendBuff(USART_TypeDef *pUSARTx, uint8_t *buf, uint8_t len);

/**
 * @} 
 */

/**
 * @} 
 */

#endif /* __USART1_H */
