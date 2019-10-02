/**
 * @file main.c
 * @author Gump 
 * @version V0.1
 * @date 2019-09-26
 * @brief LTDC版本的室内控制面板程序
 * 
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 * 
 * none
 */

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/gt9xx.h"
#include "panel.h"

 /**
  * @brief 主函数
  * 
  * @return int 
  */
int main(void)
{
	/* 调试串口初始化 */
	Debug_USART_Config();    

	/* 触摸屏初始化 */
	GTP_Init_Panel(); 
	
	/* 初始化液晶屏 */
	LCD_Init();
	LCD_LayerInit();
	LTDC_Cmd(ENABLE);
	
	/* 把背景层刷黑色 */
	LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
	/* 初始化后默认使用前景层 */
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/* 默认设置不透明，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明 */
	LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_WHITE);
	/* 经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层 */		
	


	/* 面板初始化 */
	Panel_Init();
	
	Delay(0xfff);
	
	while(1);

}

/*********************************************END OF FILE**********************/

