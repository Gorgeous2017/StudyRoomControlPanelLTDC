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
#include "./touch/gt9xx.h"
#include "./lcd/bsp_lcd.h"

#include "panel.h"
 
/* 图标结构体数组 */
Touch_Icon icon[ICON_NUM];
static void Draw_Icon(void *icon);

/**
 * @brief 简单的延时函数
 * 
 * @param nCount 延时时间
 */
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}


/**
 * @brief 控制面板初始化函数
 * 
 */
void Panel_Init(void)
{
    uint8_t i;

    /* 整屏清为白色 */
    LCD_Clear(LCD_COLOR_WHITE); /* 清屏，显示全白 */

	LCD_DisplayStringLineEx(0,0,64,96,"28%",0);

    /* 初始化图标 */
    Touch_Icon_Init();

    /* 描绘图标 */
    for (i = 0; i < ICON_NUM; i++)
    {
        icon[i].draw_icon(&icon[i]);
        Delay(0xffff); /* 用于测试是否是因为连续显示时间间隔过短而导致初次上电只显示第一个图标 */
    }
    LCD_SetFont(&Font16x24);
    LCD_DisplayStringLine(LCD_LINE_7,"29");

}

/**
 * @brief Touch_Icon_Init 初始化图标参数
 * 
 */
void Touch_Icon_Init(void)
{
    /* 四个用电器图标 */
    icon[0].start_x = ICON_START_X;
    icon[0].start_y = ICON_START_Y;
    icon[0].device = DEVICE_FAN;
    icon[0].status = 0;
    icon[0].touch_flag = 0;
    icon[0].draw_icon = Draw_Icon;
    icon[0].icon_command = Control_Device;
    icon[0].gImage_icon[0] = gImage_fan_off;
    icon[0].gImage_icon[1] = gImage_fan_on;


    icon[1].start_x = ICON_START_X + ICON_SIZE;
    icon[1].start_y = ICON_START_Y;
    icon[1].device = DEVICE_LIGHT;
    icon[1].status = 0;
    icon[1].touch_flag = 0;
    icon[1].draw_icon = Draw_Icon;
    icon[1].icon_command = Control_Device;
    icon[1].gImage_icon[0] = gImage_light_off;
    icon[1].gImage_icon[1] = gImage_light_on;

    icon[2].start_x = ICON_START_X + ICON_SIZE * 2;
    icon[2].start_y = ICON_START_Y;
    icon[2].device = DEVICE_CURTAIN;
    icon[2].status = 0;
    icon[2].touch_flag = 0;
    icon[2].draw_icon = Draw_Icon;
    icon[2].icon_command = Control_Device;
    icon[2].gImage_icon[0] = gImage_curtain_off;
    icon[2].gImage_icon[1] = gImage_curtain_on;

    icon[3].start_x = ICON_START_X + ICON_SIZE * 3;
    icon[3].start_y = ICON_START_Y;
    icon[3].device = DEVICE_AC;
    icon[3].status = 0;
    icon[3].touch_flag = 0;
    icon[3].draw_icon = Draw_Icon;
    icon[3].icon_command = Control_Device;
    icon[3].gImage_icon[0] = gImage_ac_off;
    icon[3].gImage_icon[1] = gImage_ac_on;
}

/**
 * @brief Touch_icon_Down 图标被按下时调用的函数，由触摸屏调用
 * 
 * @param x 触摸位置的x坐标
 * @param y 触摸位置的y坐标
 */
void Touch_Icon_Down(uint16_t x, uint16_t y){
    uint8_t i;
    for (i = 0; i < ICON_NUM; i++){
        /* 触摸到了图标 */
        if (x <= (icon[i].start_x + ICON_SIZE) && y <= (icon[i].start_y + ICON_SIZE) && y >= icon[i].start_y && x >= icon[i].start_x){
        
            if (icon[i].touch_flag == 0){ /*原本的状态为没有按下，则更新状态*/

                icon[i].touch_flag = 1; /* 记录按下标志 */
                icon[i].draw_icon(&icon[i]); /*重绘图标*/
            }

        } else if (icon[i].touch_flag == 1){ /* 触摸移出了图标的范围且之前有按下图标 */

            icon[i].touch_flag = 0; /* 清除按下标志，判断为误操作*/
            icon[i].draw_icon(&icon[i]); /*重绘图标*/

        }
    }
}

/**
 * @brief Touch_Icon_Up 图标被释放时调用的函数，由触摸屏调用
 * 
 * @param x 触摸最后释放时的x坐标
 * @param y 触摸最后释放时的y坐标
 */
void Touch_Icon_Up(uint16_t x, uint16_t y){
    uint8_t i;
	
    PANEL_DEBUG("Funtion Touch_Icon_Up");
	
    for (i = 0; i < ICON_NUM; i++){

        PANEL_DEBUG("icon no = %d, x = %d, y = %d",i,icon[i].start_x,icon[i].start_y);

        /* 触笔在图标区域释放 */
        if (x <= (icon[i].start_x + ICON_SIZE) && y <= (icon[i].start_y + ICON_SIZE) && y >= icon[i].start_y && x >= icon[i].start_x){
        
            icon[i].touch_flag = 0; /*释放触摸标志*/

            icon[i].status = (icon[i].status == 0) ? 1 : 0; /* 反转用电器状态 */
            
            PANEL_DEBUG("Redraw the icon above");
				
            icon[i].draw_icon(&icon[i]); /*重绘图标*/
            
            PANEL_DEBUG("Redraw the icon below");

            icon[i].icon_command(&icon[i]); /*执行图标的功能命令*/

            break;
        }
    }
}

/**
 * @brief Draw_Icon 图标绘制函数
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Draw_Icon(void *icon)
{
    Touch_Icon *ptr = (Touch_Icon *)icon;
    const unsigned char *gImage_icon;

    /* 释放图标 */
    if (ptr->touch_flag == 0)
    {
        gImage_icon = *(ptr->gImage_icon + ( (ptr->status == 0 )? 0 : 1 ) );
        LCD_DisplayPicture(ptr->start_x, ptr->start_y, ICON_SIZE, ICON_SIZE, gImage_icon);
        
    }
    else /* 图标按下 */
    {

    }
}

/**
 * @brief Control_Device 用电器控制命令函数
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Control_Device(void *icon)
{
    Touch_Icon *ptr = (Touch_Icon *)icon;
    /*
    ptr->device, ptr->
    */
}
