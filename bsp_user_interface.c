/**
 * @file bsp_user_interface.c
 * @author Gump ()
 * @version V0.1
 * @date 2019-10-13
 * @brief 
 * 
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 * 
 * none
 */
#include "panel.h"

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
 * @brief 按照给定的icon结构体初始化目标结构体数组
 * 
 * @param source_icon 初始化好的图标结构体，由用户初始化后传入
 * @param aim_icon 需要被初始化的图标结构体数组
 * @param icon_num 需要被初始化的图标数
 */
void Icon_Struct_Init(Touch_Icon *source_icon, Touch_Icon *aim_icon, uint8_t icon_num){
    uint8_t i;

    PANEL_DEBUG("Function: Icon_Struct_Init in ");

    for(i = 0; i < icon_num; i++ ){

        aim_icon[i].start_x        =  source_icon->start_x;
        aim_icon[i].start_y        =  source_icon->start_y;
        aim_icon[i].width          =  source_icon->width;
        aim_icon[i].height         =  source_icon->height;
        aim_icon[i].type           =  source_icon->type;
        aim_icon[i].no             =  i;
        aim_icon[i].status         =  source_icon->status;
        aim_icon[i].touch_flag     =  source_icon->touch_flag;
        aim_icon[i].draw_icon      =  source_icon->draw_icon;
        aim_icon[i].icon_command   =  source_icon->icon_command;
        aim_icon[i].gImage_icon[0] =  source_icon->gImage_icon[0];
        aim_icon[i].gImage_icon[1] =  source_icon->gImage_icon[1];

    }

}

/**
 * @brief 用于绘制一个控件，即一组图标
 * 
 * @param icon_array 存储控件中各个图标的数组
 * @param num 控件中图标的数量
 */
void Draw_Widget(Touch_Icon *icon_array, uint8_t num)
{
    uint8_t i;
    
    PANEL_DEBUG("Function: Draw_Widget in ");

    for ( i = 0; i < num; i++ ) {

        icon_array[i].draw_icon(&icon_array[i]);

        PANEL_DEBUG("Draw %d icon ", i);

        Delay(0xffff);

    }

}

/**
 * @brief 控件被按下时的处理函数
 * 
 * @param widget 需要检测有无被触控的控件
 * @param num 控件中图标的数量
 */
void Widget_TouchUpHandler(Touch_Icon *widget, uint8_t num, uint16_t x, uint16_t y){
    uint8_t i;
	
    PANEL_DEBUG("Funtion: Widget_TouchUpHandler in");
	
    for (i = 0; i < num; i++){

        PANEL_DEBUG("widget no = %d, x = %d, y = %d",i,widget[i].start_x,widget[i].start_y);

        /* 触笔在图标区域释放 */
        if (x <= (widget[i].start_x + widget[i].width) && y <= (widget[i].start_y + widget[i].height) && y >= widget[i].start_y && x >= widget[i].start_x){
        
            widget[i].touch_flag = 0; /*释放触摸标志*/

            widget[i].icon_command(&widget[i]); /*执行图标的功能命令*/

            PANEL_DEBUG("Redraw the widget above");
				
            widget[i].draw_icon(&widget[i]); /*重绘图标*/
            
            PANEL_DEBUG("Redraw the widget below");

            break;
        }
    }

}

/**
 * @brief 将多个图标的坐标初始化成矩阵排列的形式
 * 
 * @param IconArray 需要初始化的图标数组
 * @param LineNum 每行的图标数量
 * @param ColumnNum 每列的图标数量
 * @param LineOffset 图标间的水平间距
 * @param ColumnOffset 图标间的垂直间距
 */
void Matrix_Init(Touch_Icon *IconArray, uint8_t LineNum, uint8_t ColumnNum, uint8_t LineOffset, uint8_t ColumnOffset){

    uint8_t i, j, k = 0;

    for ( i = 0; i < LineNum; i++ ) {

        for ( j = 0; j < ColumnNum; j++, k++) {

            IconArray[k].start_x =  IconArray[k].start_x + (IconArray[k].width + ColumnOffset) * j;
            IconArray[k].start_y = IconArray[k].start_y + (IconArray[k].height + LineOffset) * i;

        }
        
    }

}

