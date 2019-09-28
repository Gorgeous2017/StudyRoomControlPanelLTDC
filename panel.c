/**
 * @file panel.c
 * @author Gump ()
 * @version V0.1
 * @date 2019-09-28
 * @brief 室内控制面板UI界面主程序
 * 
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 * 
 * none
 */

#include "panel.h"

#include "./touch/gt9xx.h"
#include "./lcd/bsp_lcd.h"
 
/*按钮结构体数组*/
Touch_Icon icon[ICON_NUM];
static void Draw_Icon(void *icon);

void Palette_Init(void)
{
  
  uint8_t i;

  /* 整屏清为白色 */
  LCD_Clear(LCD_COLOR_WHITE);	/* 清屏，显示全黑 */

  /* 初始化按钮 */
  Touch_Icon_Init();
  
  /* 描绘按钮 */
  for(i=0;i<ICON_NUM;i++)
  {
    icon[i].draw_icon(&icon[i]);
  }
  
}

/**
 * @brief Touch_Icon_Init 初始化按钮参数
 * 
 */
void Touch_Icon_Init(void)
{
  /*第一列，主要为颜色按钮*/
  icon[0].start_x = ICON_START_X;
  icon[0].start_y = ICON_START_Y;
  icon[0].end_x = ICON_START_X + ICON_SIZE ;
  icon[0].end_y = ICON_START_Y + ICON_SIZE;
  icon[0].device = DEVICE_FAN;
  icon[0].status = 0;
  icon[0].touch_flag = 0;  
  icon[0].draw_icon = Draw_Icon ;
  icon[0].icon_command = Control_Device;
  
  icon[1].start_x = ICON_START_X + ICON_SIZE;
  icon[1].start_y = ICON_START_Y;
  icon[1].end_x = ICON_START_X + ICON_SIZE*2 ;
  icon[1].end_y = ICON_START_Y + ICON_SIZE;
  icon[1].device = DEVICE_LIGHT;
  icon[1].status = 0;
  icon[1].touch_flag = 0;  
  icon[1].draw_icon = Draw_Icon ;
  icon[1].icon_command = Control_Device ;
  
  icon[2].start_x = ICON_START_X + ICON_SIZE*2;
  icon[2].start_y = ICON_START_Y;
  icon[2].end_x = ICON_START_X + ICON_SIZE*3 ;
  icon[2].end_y = ICON_START_Y + ICON_SIZE;
  icon[2].device = DEVICE_CURTAIN;
  icon[2].status = 0;
  icon[2].touch_flag = 0;  
  icon[2].draw_icon = Draw_Icon ;
  icon[2].icon_command = Control_Device ;

  icon[3].start_x = ICON_START_X + ICON_SIZE*3;
  icon[3].start_y = ICON_START_Y;
  icon[3].end_x = ICON_START_X + ICON_SIZE*4 ;
  icon[3].end_y = ICON_START_Y + ICON_SIZE;
  icon[3].device = DEVICE_AC;
  icon[3].status = 0;
  icon[3].touch_flag = 0;  
  icon[3].draw_icon = Draw_Icon ;
  icon[3].icon_command = Control_Device ;
 

}

/**
* @brief  Touch_icon_Down 按键被按下时调用的函数，由触摸屏调用
* @param  x 触摸位置的x坐标
* @param  y 触摸位置的y坐标
* @retval 无
*/
void Touch_Icon_Down(uint16_t x,uint16_t y)
{
  uint8_t i;
  for(i=0;i<ICON_NUM;i++)
  {
    /* 触摸到了按钮 */
    if(x<=icon[i].end_x && y<=icon[i].end_y && y>=icon[i].start_y && x>=icon[i].start_x )
    {
      if(icon[i].touch_flag == 0)     /*原本的状态为没有按下，则更新状态*/
      {
      icon[i].touch_flag = 1;         /* 记录按下标志 */
      
      icon[i].draw_icon(&icon[i]);  /*重绘按钮*/
      }        
      
    }
    else if(icon[i].touch_flag == 1) /* 触摸移出了按键的范围且之前有按下按钮 */
    {
      icon[i].touch_flag = 0;         /* 清除按下标志，判断为误操作*/
      
      icon[i].draw_icon(&icon[i]);   /*重绘按钮*/
    }

  }

}

/**
 * @brief Touch_Icon_Up 按键被释放时调用的函数，由触摸屏调用
 * 
 * @param x 触摸最后释放时的x坐标
 * @param y 触摸最后释放时的y坐标
 */
void Touch_Icon_Up(uint16_t x,uint16_t y)
{
   uint8_t i; 
   for(i=0;i<ICON_NUM;i++)
   {
     /* 触笔在按钮区域释放 */
      if((x<icon[i].end_x && x>icon[i].start_x && y<icon[i].end_y && y>icon[i].start_y))
      {        
        icon[i].touch_flag = 0;       /*释放触摸标志*/

        (icon[i].status == 0) ? (icon[i].status = 1):(icon[i].status = 0); /* 反转用电器状态 */
        
        icon[i].draw_icon(&icon[i]); /*重绘按钮*/        
      
        icon[i].icon_command(&icon[i]);  /*执行按键的功能命令*/
        
        break;
      }
    }  

}

/**
 * @brief Draw_Icon 图标绘制函数
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Draw_Icon(void *icon){
  Touch_Icon *ptr = (Touch_Icon *)icon;
  const unsigned char* gImage;
  
  /*释放按键*/
  if(ptr->touch_flag == 0)
  {

      switch (ptr->device)
      {
      case DEVICE_FAN:
          gImage = DEVICE_STATUS_TOGGLE(fan,ptr->status);
          break;
      case DEVICE_LIGHT:
          gImage = DEVICE_STATUS_TOGGLE(light,ptr->status);
          break;
      case DEVICE_CURTAIN:
          gImage = DEVICE_STATUS_TOGGLE(curtain,ptr->status);
          break;
      case DEVICE_AC:
          gImage = DEVICE_STATUS_TOGGLE(ac,ptr->status);
          break;
      
      default:
          break;
      }
	LCD_DisplayPicture(ptr->start_x,ptr->start_y,ICON_SIZE,ICON_SIZE,gImage);
  }
  else  /*按键按下*/
  {
 
  } 

}

/**
 * @brief Control_Device 用电器控制命令函数
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Control_Device(void *icon){
  Touch_Icon *ptr = (Touch_Icon *)icon;
/*
  ptr->device,ptr->
*/

}
