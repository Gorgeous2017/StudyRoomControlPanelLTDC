/**
 * @file main.c
 * @author Gump 
 * @version V0.1
 * @date 2019-09-26
 * @brief LTDC�汾�����ڿ���������
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
  * @brief ������
  * 
  * @return int 
  */
int main(void)
{
	/* ���Դ��ڳ�ʼ�� */
	Debug_USART_Config();    

	/* ��������ʼ�� */
	GTP_Init_Panel(); 
	
	/* ��ʼ��Һ���� */
	LCD_Init();
	LCD_LayerInit();
	LTDC_Cmd(ENABLE);
	
	/* �ѱ�����ˢ��ɫ */
	LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
	/* ��ʼ����Ĭ��ʹ��ǰ���� */
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/* Ĭ�����ò�͸�����ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸�� */
	LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_WHITE);
	/* ����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ����� */		
	


	/* ����ʼ�� */
	Panel_Init();
	
	Delay(0xfff);
	
	while(1);

}

/*********************************************END OF FILE**********************/

