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
#include "stm32f4xx.h"
#include "usart/bsp_debug_usart.h"
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
 * 
 * @note 用法示例：    
    
    Touch_Icon Icon_InitStruct; // Touch_Icon 结构体初始化变量

    // 设置初始化成员的值
    Icon_InitStruct.start_x = COMMON_ICON_START_X;
    Icon_InitStruct.start_y = COMMON_ICON_START_Y;
    Icon_InitStruct.width = COMMON_ICON_W;
    Icon_InitStruct.height = COMMON_ICON_H;
    Icon_InitStruct.type = COMMON_ICON_TYPE
    Icon_InitStruct.status = 0;
    Icon_InitStruct.no = 0;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.gImage_icon[0] = gImage_sel;
    Icon_InitStruct.gImage_icon[1] = gImage_unsel;
    Icon_InitStruct.icon_command = Common_Icon_Command;
    Icon_InitStruct.draw_icon = Common_Draw_Icon_Funtion;

    // 将所设置的初始化成员的值赋给Touch_Icon 类型的数组
    Icon_Struct_Init(&Icon_InitStruct, user_icon, user_icon_num);
    
    // 设置结构体中与初始化值不一样的成员
    user_icon[0].start_x = USER_ICON_START_X;
    user_icon[0].status = 1;
    user_icon[0].type = USER_ICON_TYPE_1;
    user_icon[0].icon_command = Draw_User_1_Function;
    user_icon[0].gImage_icon[0] = gImage_user_1_sel;
    user_icon[0].gImage_icon[1] = gImage_user_1_unsel;

    user_icon[1].start_x = USER_ICON_START_X + USER_ICON_OFFSET;
    user_icon[1].type = USER_ICON_TYPE_2;
    user_icon[1].icon_command = Draw_User_2_Function;
    user_icon[1].gImage_icon[0] = gImage_centre_sel;
    user_icon[1].gImage_icon[1] = gImage_centre_unsel;

    // ...more member 

 * @note 用于初始化的变量不一定要赋值所有的成员，可以只赋值每个结构体中都相同的成员。但是这样的话在初始化变
 * 量中没有赋值的成员必须在后面“设置结构体中与初始化值不一样的成员”中赋值，否则容易出现“硬错误(HardFault)”
 * 
 * @note Icon_Struct_Init 只能将source_icon的“全部成员”一同赋值给aim_icon，不能单独赋值某一个特定的成员，
 * 故此函数只适用于批量初始化 Touch_Icon 结构体中的成员，不适用于程序运行时批量修改 Touch_Icon 结构体中的
 * 某个成员
 * 
 * @note 若要将Matrix_Init()与Icon_Struct_Init()一起使用，务必将Matrix_Init()置于Icon_Struct_Init()之
 * 后调用，并在Icon_Struct_Init()中将矩阵第一个图标的坐标赋值 start_x 和 start_y 成员
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
 * @brief 控件的触控处理函数
 * 
 * @param widget 需要检测有无被触控的控件
 * @param num 控件中图标的数量
 * @param x 触点横坐标
 * @param y 触点纵坐标
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
 * @param LineOffset 图标间的垂直间距
 * @param ColumnOffset 图标间的水平间距
 * 
 * @note 用法示例：初始化如下图的 2 x 3 图标矩阵,行间距为30，列间距为20
 * 
 *         ——————              ——————              ——————  
 *        |      | <-- 20 --> |      | <-- 20 --> |      | 
 *        |      |            |      |            |      | 
 *         ——————              ——————              ——————  
 *           ^
 *           |
 *           30
 *           |
 *           ^ 
 *         ——————              ——————              ——————  
 *        |      | <-- 20 --> |      | <-- 20 --> |      | 
 *        |      |            |      |            |      | 
 *         ——————              ——————              ——————  
 * 
 * 调用语句如下：
 *     Matrix_Init(IconArray, 2, 3, 30, 20);
 * 
 * @note 若要将Matrix_Init()与Icon_Struct_Init()一起使用，务必将Matrix_Init()于Icon_Struct_Init()
 * 之后调用，并在Icon_Struct_Init()中将矩阵第一个图标的坐标赋给 start_x 和 start_y 成员
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



