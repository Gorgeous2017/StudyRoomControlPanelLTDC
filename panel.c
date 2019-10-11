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
Touch_Icon status_icon[4];
Touch_Icon menu_icon[3];
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
    LCD_Clear(0xDBF0F9); /* 淡蓝背景色 */

	//LCD_DisplayStringLineEx(0,0,64,96,"28%",0);

    /* 初始化图标 */
    Touch_Icon_Init();

    /* 描绘图标 */
    for (i = 0; i < ICON_NUM; i++)
    {
       icon[i].draw_icon(&icon[i]);
       Delay(0xffff); /* 用于测试是否是因为连续显示时间间隔过短而导致初次上电只显示第一个图标 */
    }
    
    PANEL_DEBUG("Draw 8 icon down");

    for (i = 0; i < 4; i++)
    {
        status_icon[i].draw_icon(&status_icon[i]);
        Delay(0xffff);
    }

    PANEL_DEBUG("Draw 4 icon down");

    LCD_SetFont(&Font48x96);
    //LCD_SetFont(&Font12x12);
    LCD_DisplayStringLine(STATUS_ICON_START_Y,"15%");
    LCD_DisplayStringLine(STATUS_ICON_START_Y + STATUS_ICON_OFFSET,"36db");
    LCD_DisplayStringLine(STATUS_ICON_START_Y + STATUS_ICON_OFFSET * 2,"25");
    LCD_DisplayStringLine(STATUS_ICON_START_Y + STATUS_ICON_OFFSET * 3,"15%");

}

/**
 * @brief Touch_Icon_Init 初始化图标参数
 * 
 */
void Touch_Icon_Init(void)
{
    Device_Icon_Init();
    Menu_Icon_Init();
    Status_Icon_Init();
}

/**
 * @brief Touch_Icon_Init 初始化图标参数
 * 
 */
void Device_Icon_Init(void)
{
    /* 四个用电器图标 */
    icon[0].start_x = ICON_START_X;
    icon[0].start_y = ICON_START_Y;
    icon[0].width = ICON_SIZE;
    icon[0].height = ICON_SIZE;
    icon[0].type = DEVICE_FAN;
    icon[0].status = 0;
    icon[0].touch_flag = 0;
    icon[0].draw_icon = Draw_Icon;
    icon[0].icon_command = Control_Device;
    icon[0].gImage_icon[0] = gImage_fan_off;
    icon[0].gImage_icon[1] = gImage_fan_on;


    icon[1].start_x = ICON_START_X + ICON_SIZE;
    icon[1].start_y = ICON_START_Y;
    icon[1].width = ICON_SIZE;
    icon[1].height = ICON_SIZE;
    icon[1].type = DEVICE_LIGHT;
    icon[1].status = 0;
    icon[1].touch_flag = 0;
    icon[1].draw_icon = Draw_Icon;
    icon[1].icon_command = Control_Device;
    icon[1].gImage_icon[0] = gImage_light_off;
    icon[1].gImage_icon[1] = gImage_light_on;

    icon[2].start_x = ICON_START_X + ICON_SIZE * 2;
    icon[2].start_y = ICON_START_Y;
    icon[2].width = ICON_SIZE;
    icon[2].height = ICON_SIZE;
    icon[2].type = DEVICE_CURTAIN;
    icon[2].status = 0;
    icon[2].touch_flag = 0;
    icon[2].draw_icon = Draw_Icon;
    icon[2].icon_command = Control_Device;
    icon[2].gImage_icon[0] = gImage_curtain_off;
    icon[2].gImage_icon[1] = gImage_curtain_on;

    icon[3].start_x = ICON_START_X + ICON_SIZE * 3;
    icon[3].start_y = ICON_START_Y;
    icon[3].width = ICON_SIZE;
    icon[3].height = ICON_SIZE;
    icon[3].type = DEVICE_AC;
    icon[3].status = 0;
    icon[3].touch_flag = 0;
    icon[3].draw_icon = Draw_Icon;
    icon[3].icon_command = Control_Device;
    icon[3].gImage_icon[0] = gImage_ac_off;
    icon[3].gImage_icon[1] = gImage_ac_on;

}


/**
 * @brief 环境信息图标初始化 位于面板左侧
 * 
 */
void Menu_Icon_Init(void){
    
    icon[MENU_INDEX + 0].start_x = MENU_ICON_START_X;
    icon[MENU_INDEX + 0].start_y = MENU_ICON_START_Y;
    icon[MENU_INDEX + 0].width = MENU_ICON_W;
    icon[MENU_INDEX + 0].height = MENU_ICON_H;
    icon[MENU_INDEX + 0].type = DEVICE_AC; // 待定
    icon[MENU_INDEX + 0].status = 1;
    icon[MENU_INDEX + 0].touch_flag = 0;
    icon[MENU_INDEX + 0].draw_icon = Draw_Icon;
    icon[MENU_INDEX + 0].icon_command = Tag_Change;
    icon[MENU_INDEX + 0].gImage_icon[0] = gImage_single_sel;
    icon[MENU_INDEX + 0].gImage_icon[1] = gImage_single_unsel;

    icon[MENU_INDEX + 1].start_x = MENU_ICON_START_X + MENU_ICON_OFFSET;
    icon[MENU_INDEX + 1].start_y = MENU_ICON_START_Y;
    icon[MENU_INDEX + 1].width = MENU_ICON_W;
    icon[MENU_INDEX + 1].height = MENU_ICON_H;
    icon[MENU_INDEX + 1].type = DEVICE_AC; // 待定
    icon[MENU_INDEX + 1].status = 0;
    icon[MENU_INDEX + 1].touch_flag = 0;
    icon[MENU_INDEX + 1].draw_icon = Draw_Icon;
    icon[MENU_INDEX + 1].icon_command = Tag_Change;
    icon[MENU_INDEX + 1].gImage_icon[0] = gImage_centre_sel;
    icon[MENU_INDEX + 1].gImage_icon[1] = gImage_centre_unsel;

    icon[MENU_INDEX + 2].start_x = MENU_ICON_START_X + MENU_ICON_OFFSET * 2;
    icon[MENU_INDEX + 2].start_y = MENU_ICON_START_Y;
    icon[MENU_INDEX + 2].width = MENU_ICON_W;
    icon[MENU_INDEX + 2].height = MENU_ICON_H;
    icon[MENU_INDEX + 2].type = DEVICE_AC; // 待定
    icon[MENU_INDEX + 2].status = 0;
    icon[MENU_INDEX + 2].touch_flag = 0;
    icon[MENU_INDEX + 2].draw_icon = Draw_Icon;
    icon[MENU_INDEX + 2].icon_command = Tag_Change;
    icon[MENU_INDEX + 2].gImage_icon[0] = gImage_auto_sel;
    icon[MENU_INDEX + 2].gImage_icon[1] = gImage_auto_unsel;

}

/**
 * @brief 环境信息图标初始化 位于面板左侧
 * 
 */
void Status_Icon_Init(void){
    status_icon[0].start_x = STATUS_ICON_START_X;
    status_icon[0].start_y = STATUS_ICON_START_Y;
    status_icon[0].width = ICON_SIZE;
    status_icon[0].height = ICON_SIZE;
    status_icon[0].touch_flag = 0;
    status_icon[0].status = 0;
    status_icon[0].draw_icon = Draw_Icon;
    status_icon[0].gImage_icon[0] = gImage_people;

    status_icon[1].start_x = STATUS_ICON_START_X;
    status_icon[1].start_y = STATUS_ICON_START_Y + STATUS_ICON_OFFSET;
    status_icon[1].width = ICON_SIZE;
    status_icon[1].height = ICON_SIZE;
    status_icon[1].touch_flag = 0;
    status_icon[1].status = 0;
    status_icon[1].draw_icon = Draw_Icon;
    status_icon[1].gImage_icon[0] = gImage_noise;

    status_icon[2].start_x = STATUS_ICON_START_X;
    status_icon[2].start_y = STATUS_ICON_START_Y + STATUS_ICON_OFFSET * 2;
    status_icon[2].width = ICON_SIZE;
    status_icon[2].height = ICON_SIZE;
    status_icon[2].touch_flag = 0;
    status_icon[2].status = 0;
    status_icon[2].draw_icon = Draw_Icon;
    status_icon[2].gImage_icon[0] = gImage_temp;

    status_icon[3].start_x = STATUS_ICON_START_X;
    status_icon[3].start_y = STATUS_ICON_START_Y + STATUS_ICON_OFFSET * 3;
    status_icon[3].width = ICON_SIZE;
    status_icon[3].height = ICON_SIZE;
    status_icon[3].touch_flag = 0;
    status_icon[3].status = 0;
    status_icon[3].draw_icon = Draw_Icon;
    status_icon[3].gImage_icon[0] = gImage_humi;
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
        if (x <= (icon[i].start_x + icon[i].width ) && y <= (icon[i].start_y + icon[i].height ) && y >= icon[i].start_y && x >= icon[i].start_x){
        
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
        if (x <= (icon[i].start_x + icon[i].width) && y <= (icon[i].start_y + icon[i].height) && y >= icon[i].start_y && x >= icon[i].start_x){
        
            icon[i].touch_flag = 0; /*释放触摸标志*/

            //icon[i].status = (icon[i].status == 0) ? 1 : 0; /* 反转用电器状态 */

            icon[i].icon_command(&icon[i]); /*执行图标的功能命令*/

            PANEL_DEBUG("Redraw the icon above");
				
            icon[i].draw_icon(&icon[i]); /*重绘图标*/
            
            PANEL_DEBUG("Redraw the icon below");

            

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
        LCD_DisplayPicture(ptr->start_x, ptr->start_y, ptr->width, ptr->height, gImage_icon);
        
    }
    else /* 图标按下 */
    {

    }
}

void Draw_Menu_Icon(void *icon)
{
    Touch_Icon *ptr = (Touch_Icon *)icon;
    const unsigned char *gImage_icon;

    /* 释放图标 */
    if (ptr->touch_flag == 0)
    {
        gImage_icon = *(ptr->gImage_icon + ( (ptr->status == 0 )? 0 : 1 ) );
        LCD_DisplayPicture(ptr->start_x, ptr->start_y, MENU_ICON_W, MENU_ICON_H, gImage_icon);
        
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
void Control_Device(void *icon){

    Touch_Icon *ptr = (Touch_Icon *)icon;

    ptr->status = (ptr->status == 0) ? 1 : 0; /* 反转用电器状态 */

    /*
    ptr->device, ptr->
    */
}

/**
 * @brief 用于切换菜单栏的页签，并显示不同的页面
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Tag_Change(void *ic){

    Touch_Icon *ptr = (Touch_Icon *)ic;
    uint8_t i,target_index;

    for(i = 0; i < 3; i++){
        if( ptr->start_x == (MENU_ICON_START_X + MENU_ICON_OFFSET * i)){
            break;
        }
    }

    ptr->status = 1;

    target_index = MENU_INDEX + ((i + 1) % 3);
    icon[target_index].status = 0;
    icon[target_index].draw_icon(&icon[target_index]);
    
    PANEL_DEBUG("i = %d, target_index = %d",i,target_index);

    target_index = MENU_INDEX + ((i + 2) % 3);
    icon[target_index].status = 0;
    icon[target_index].draw_icon(&icon[target_index]);

    PANEL_DEBUG("i = %d, target_index = %d",i,target_index);

}

