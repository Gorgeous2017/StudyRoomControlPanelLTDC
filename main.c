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

void Delay(__IO u32 nCount); 
void LCD_Test(void);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  

	
	Debug_USART_Config();    
  printf("\r\n ���ڿ������LTDC���� \r\n");
  /* ��������ʼ�� */
	GTP_Init_Panel(); 
	printf("��ʼ��������\n");
	
	/*��ʼ��Һ����*/
	LCD_Init();
	LCD_LayerInit();
	LTDC_Cmd(ENABLE);
	
	/*�ѱ�����ˢ��ɫ*/
	LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
	/*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
	LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_WHITE);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/		
	
	//Delay(0xfff);  

//	while(1)
//	{
//		LCD_DisplayPicture(0, 0,96,96,gImage_light_on);
//		Delay(0xFFFF);
//		LCD_DisplayPicture(96, 0,96,96,gImage_ac_on);
//		Delay(0xFFFF);
//		LCD_DisplayPicture(96*2, 0,96,96,gImage_curtain_on);
//		Delay(0xFFFF);
//		LCD_DisplayPicture(96*3, 0,96,96,gImage_fan_on);
//		Delay(0xFFFF);
//	}  

  Palette_Init();
  Delay(0xfff);
   

  while(1);


}



/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	/*��ʾ��ʾ����*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;
	
	/*ʹ�ò�͸��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER);  
  LCD_SetTransparency(0xff);
	
  LCD_Clear(LCD_COLOR_BLACK);	/* ��������ʾȫ�� */

	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
	/*ѡ������*/
  LCD_SetFont(&Font16x24);

  LCD_DisplayStringLine(LINE(1),(uint8_t* )"BH 5.0 inch LCD para:");
  LCD_DisplayStringLine(LINE(2),(uint8_t* )"Image resolution:800x480 px");
  LCD_DisplayStringLine(LINE(3),(uint8_t* )"Touch pad:5 point touch supported");
  LCD_DisplayStringLine(LINE(4),(uint8_t* )"Use STM32-LTDC directed driver,");
	LCD_DisplayStringLine(LINE(5),(uint8_t* )"no extern lcd driver needed,RGB888,24bits data bus");
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Touch pad use IIC to communicate");
  
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(7));
	
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine(LINE(7),(uint8_t* )dispBuff);


  /* ��ֱ�� */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  
	LCD_ClearLine(LINE(8));
  LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw line:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawUniLine(50,250,750,250);  
  LCD_DrawUniLine(50,300,750,300);
  
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawUniLine(300,250,400,400);  
  LCD_DrawUniLine(600,250,600,400);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  
  
  /*������*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
  LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw Rect:");
	
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawRect(200,350,50,200);
  
  Delay(0xFFFFFF);
  
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  

  /*������*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw Full Rect:");

 	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,350,50,200);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
  /* ��Բ */
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw circle:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
  LCD_DrawCircle(200,350,50);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
  LCD_DrawCircle(350,350,75);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


  /*���Բ*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw full circle:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullCircle(300,350,50);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullCircle(450,350,75);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
	LCD_ClearLine(LINE(8));
	
//���ֲ���Ч�����л������󣬲��Ƽ�ʹ�á�һ�㱳�����ʹ��ARGB��ʽ����ɫ�����	
	/*͸��Ч�� ǰ�������*/
	{
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_ClearLine(LINE(8));
		LCD_DisplayStringLine(LINE(8),(uint8_t* )"Transparency effect:");
		
		/*����ǰ���㲻͸����*/
		LCD_SetLayer(LCD_FOREGROUND_LAYER); 	
		LCD_SetTransparency(200);
		
		/*��ǰ����һ����ɫԲ*/
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
		LCD_DrawFullCircle(400,350,75);
	
	}
	
	/*͸��Ч�� ���������*/
	{		
		/*���ñ����㲻͸��*/
		LCD_SetLayer(LCD_BACKGROUND_LAYER); 	
		LCD_SetTransparency(0xff);
		LCD_Clear(LCD_COLOR_BLACK);

		/*�ڱ�����һ����ɫԲ*/
		LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
		LCD_DrawFullCircle(450,350,75);
		
		/*�ڱ�����һ����ɫԲ*/
		LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
		LCD_DrawFullCircle(350,350,75);
	}
  
  Delay(0xFFFFFF);
	
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

