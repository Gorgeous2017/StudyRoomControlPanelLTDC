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
Touch_Icon single_ctrl_icon[4];
Touch_Icon centre_ctrl_icon[4];
Touch_Icon auto_ctrl_icon[2];
Touch_Icon status_icon[4];
Touch_Icon menu_icon[3];

Touch_Icon device[4][6];

/**
 * @brief 控制面板初始化函数
 * 
 */
void Panel_Init(void)
{

    LCD_Clear(0xDBF0F9); /* 淡蓝背景色 */

    /* 初始化图标 */
    Touch_Icon_Init();

    /* 绘制环境信息图标 */
    Draw_Widget(status_icon, 4);

    PANEL_DEBUG("Draw Status down ");

    /* 绘制独立控制页面 */
    Draw_Single_Ctrl_Page(menu_icon);
    /* 绘制菜单栏 */
    Draw_Widget(menu_icon, 3);

    PANEL_DEBUG("Draw Menu down ");

    /* 显示环境信息数字 */
    LCD_SetFont(&Font48x96);
    LCD_SetTextColor(TEXT_COLOR);
    LCD_DisplayStringLine(STATUS_ICON_START_Y,"15%");
    LCD_DisplayStringLine(STATUS_ICON_START_Y + (STATUS_ICON_OFFSET + ICON_SIZE),"36db");
    LCD_DisplayStringLine(STATUS_ICON_START_Y + (STATUS_ICON_OFFSET + ICON_SIZE) * 2,"25");
    LCD_DisplayStringLine(STATUS_ICON_START_Y + (STATUS_ICON_OFFSET + ICON_SIZE) * 3,"15%");

    PANEL_DEBUG("Draw String down ");
    
}

/**
 * @brief Touch_Icon_Init 初始化图标参数
 * 
 */
void Touch_Icon_Init(void)
{
    Menu_Icon_Init();

    PANEL_DEBUG("Init Menu down ");

    Status_Icon_Init();

    PANEL_DEBUG("Init Status down ");

    Single_Ctrl_Icon_Init();
    Auto_Ctrl_Icon_Init();
    Centre_Ctrl_Icon_Init();

    PANEL_DEBUG("Init Ctrl down ");

    Device_Icon_Init();

    PANEL_DEBUG("Init Device down ");
}

/**
 * @brief 环境信息图标初始化 位于面板左侧
 * 
 */
void Menu_Icon_Init(void){

    Touch_Icon Icon_InitStruct;

    Icon_InitStruct.start_x = MENU_ICON_START_X;
    Icon_InitStruct.start_y = MENU_ICON_START_Y;
    Icon_InitStruct.width = MENU_ICON_W;
    Icon_InitStruct.height = MENU_ICON_H;
    Icon_InitStruct.status = 0;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.draw_icon = Draw_Icon;

    PANEL_DEBUG("Menu icon struct init above");

    Icon_Struct_Init(&Icon_InitStruct, menu_icon, 3);
    Matrix_Init(menu_icon, 1, 3, 0, MENU_ICON_OFFSET); // 三个图标横向紧贴着排列

    PANEL_DEBUG("Menu icon struct init below");
    
    menu_icon[0].status = 1; // 上电显示第独立控制界面
    menu_icon[0].type = MENU_SINGLE_CTRL;
    menu_icon[0].icon_command = Draw_Single_Ctrl_Page;
    menu_icon[0].gImage_icon[0] = gImage_single_sel;
    menu_icon[0].gImage_icon[1] = gImage_single_unsel;

    menu_icon[1].type = MENU_CENTRE_CTRL;
    menu_icon[1].icon_command = Draw_Centre_Ctrl_Page;
    menu_icon[1].gImage_icon[0] = gImage_centre_sel;
    menu_icon[1].gImage_icon[1] = gImage_centre_unsel;

    menu_icon[2].type = MENU_AUTO_CTRL;
    menu_icon[2].icon_command = Draw_Auto_Ctrl_Page;
    menu_icon[2].gImage_icon[0] = gImage_auto_sel;
    menu_icon[2].gImage_icon[1] = gImage_auto_unsel;

}

/**
 * @brief 环境信息图标初始化 位于面板左侧
 * 
 */
void Status_Icon_Init(void){

    Touch_Icon Icon_InitStruct;

    Icon_InitStruct.start_x = STATUS_ICON_START_X;
    Icon_InitStruct.start_y = STATUS_ICON_START_Y;
    Icon_InitStruct.width = ICON_SIZE;
    Icon_InitStruct.height = ICON_SIZE;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.status = 0;
    Icon_InitStruct.draw_icon = Draw_Icon;
    Icon_InitStruct.gImage_icon[0] = gImage_people;

    Icon_Struct_Init(&Icon_InitStruct, status_icon, 4);
    Matrix_Init(status_icon, 4, 1, 14, 0); // 四个图标纵向排列，间距为14 pixel

    status_icon[0].gImage_icon[0] = gImage_people;

    status_icon[1].gImage_icon[0] = gImage_noise;

    status_icon[2].gImage_icon[0] = gImage_temp;

    status_icon[3].gImage_icon[0] = gImage_humi;
}

/**
 * @brief 用电器类型选择图标
 * 
 */
void Single_Ctrl_Icon_Init(void)
{
    Touch_Icon Icon_InitStruct;

    Icon_InitStruct.width = ICON_SIZE;
    Icon_InitStruct.height = ICON_SIZE;
    Icon_InitStruct.status = 0;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.draw_icon = Draw_Icon;
    Icon_InitStruct.icon_command = Select_Device;

    Icon_Struct_Init(&Icon_InitStruct, single_ctrl_icon, 4);

    single_ctrl_icon[0].start_x = 410;
    single_ctrl_icon[0].start_y = 140;
    single_ctrl_icon[0].type = DEVICE_FAN;
    single_ctrl_icon[0].gImage_icon[0] = gImage_fan_on;
    single_ctrl_icon[0].gImage_icon[1] = gImage_fan_on;


    single_ctrl_icon[1].start_x = 490;
    single_ctrl_icon[1].start_y = 50;
    single_ctrl_icon[1].type = DEVICE_LIGHT;
    single_ctrl_icon[1].gImage_icon[0] = gImage_light_on;
    single_ctrl_icon[1].gImage_icon[1] = gImage_light_on;

    single_ctrl_icon[2].start_x = 600;
    single_ctrl_icon[2].start_y = 50;
    single_ctrl_icon[2].type = DEVICE_CURTAIN;
    single_ctrl_icon[2].gImage_icon[0] = gImage_curtain_on;
    single_ctrl_icon[2].gImage_icon[1] = gImage_curtain_on;

    single_ctrl_icon[3].start_x = 680;
    single_ctrl_icon[3].start_y = 140;
    single_ctrl_icon[3].type = DEVICE_AC;
    single_ctrl_icon[3].gImage_icon[0] = gImage_ac_on;
    single_ctrl_icon[3].gImage_icon[1] = gImage_ac_on;

}


void Device_Icon_Init(void) {

    Touch_Icon Icon_InitStruct;

    /* 风扇初始化 */
    Icon_InitStruct.start_x = 430;
    Icon_InitStruct.start_y = 260;
    Icon_InitStruct.width = ICON_SIZE;
    Icon_InitStruct.height = ICON_SIZE;
    Icon_InitStruct.type = DEVICE_FAN;
    Icon_InitStruct.status = 0;
    Icon_InitStruct.no = 0;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.draw_icon = Draw_Icon;
    Icon_InitStruct.icon_command = Control_Device;
    Icon_InitStruct.gImage_icon[0] = gImage_fan_off;
    Icon_InitStruct.gImage_icon[1] = gImage_fan_on;

    Icon_Struct_Init(&Icon_InitStruct, device[Icon_InitStruct.type], 6);
    Matrix_Init(device[Icon_InitStruct.type], 2, 3, 14, 24);  

    /* 灯泡初始化 */
    Icon_InitStruct.type = DEVICE_LIGHT;
    Icon_InitStruct.gImage_icon[0] = gImage_light_off;
    Icon_InitStruct.gImage_icon[1] = gImage_light_on;

    Icon_Struct_Init(&Icon_InitStruct, device[Icon_InitStruct.type], 6);
    Matrix_Init(device[Icon_InitStruct.type], 2, 3, 14, 24);

    /* 窗帘初始化 */
    Icon_InitStruct.type = DEVICE_CURTAIN;
    Icon_InitStruct.gImage_icon[0] = gImage_curtain_off;
    Icon_InitStruct.gImage_icon[1] = gImage_curtain_on;

    Icon_Struct_Init(&Icon_InitStruct, device[Icon_InitStruct.type], 6);
    Matrix_Init(device[Icon_InitStruct.type], 2, 3, 14, 24);

    /* 空调初始化 */
    Icon_InitStruct.type = DEVICE_AC;
    Icon_InitStruct.gImage_icon[0] = gImage_ac_off;
    Icon_InitStruct.gImage_icon[1] = gImage_ac_on;

    Icon_Struct_Init(&Icon_InitStruct, device[Icon_InitStruct.type], 6);
    Matrix_Init(device[Icon_InitStruct.type], 2, 3, 14, 24);

}



/**
 * @brief Touch_icon_Down 图标被按下时调用的函数，由触摸屏调用
 * 
 * @param x 触摸位置的x坐标
 * @param y 触摸位置的y坐标
 */
void Touch_Icon_Down(uint16_t x, uint16_t y){
    // uint8_t i;
    // for (i = 0; i < ICON_NUM; i++){
    //     /* 触摸到了图标 */
    //     if (x <= (icon[i].start_x + icon[i].width ) && y <= (icon[i].start_y + icon[i].height ) && y >= icon[i].start_y && x >= icon[i].start_x){
        
    //         if (icon[i].touch_flag == 0){ /*原本的状态为没有按下，则更新状态*/

    //             icon[i].touch_flag = 1; /* 记录按下标志 */
    //             icon[i].draw_icon(&icon[i]); /*重绘图标*/
    //         }

    //     } else if (icon[i].touch_flag == 1){ /* 触摸移出了图标的范围且之前有按下图标 */

    //         icon[i].touch_flag = 0; /* 清除按下标志，判断为误操作*/
    //         icon[i].draw_icon(&icon[i]); /*重绘图标*/

    //     }
    // }
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

    Widget_TouchUpHandler(menu_icon, 3, x, y);

    for ( i = 0; i < 3; i++ ) {

        PANEL_DEBUG("Menu %d status = %d",i ,menu_icon[i].status);
        
        if( menu_icon[i].status != 0){

            PANEL_DEBUG("Menu %d is choose, this menu type is %X", i, menu_icon[i].type);

            switch (menu_icon[i].type ) {
            
            case MENU_SINGLE_CTRL:
                PANEL_DEBUG("Case MENU_SINGLE_CTRL ");

                Widget_TouchUpHandler(single_ctrl_icon, 4, x, y);                

                Device_TouchUpHandler(single_ctrl_icon, x, y);
                break;

            case MENU_CENTRE_CTRL:
                Widget_TouchUpHandler(centre_ctrl_icon, 4, x, y);
                break;

            case MENU_AUTO_CTRL:
                Widget_TouchUpHandler(auto_ctrl_icon, 1, x, y);
                break;
            
            default:
                break;
            }

        }

    }
    
}

void Device_TouchUpHandler(Touch_Icon *device_ctrl_icon, uint16_t x, uint16_t y){

    uint8_t i;

    PANEL_DEBUG("Function: Device_TouchUpHandler in");

    for ( i = 0; i < 4; i++ ) {
        
        PANEL_DEBUG("Device %d status = %d",i ,device_ctrl_icon[i].status);

        if( device_ctrl_icon[i].status != 0) {

            PANEL_DEBUG("Device %d is choose, this device type is %#X", i, device_ctrl_icon[i].type);

            Widget_TouchUpHandler(device[ device_ctrl_icon[i].type ], 6, x, y);
            
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

/**
 * @brief Control_Device 用电器控制命令函数
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Control_Device(void *icon){

    Touch_Icon *ptr = (Touch_Icon *)icon;

    ptr->status = (ptr->status == 0) ? 1 : 0; /* 反转用电器状态 */

    /******************************************************************
     * 
     * 将被触控的用电器的类型、编号、状态通过串口传输出去
     * 
     ******************************************************************/

}

void Select_Device(void *icon){

    Touch_Icon *ptr = (Touch_Icon *)icon;

    PANEL_DEBUG("Function: Select_Device in ");

    single_ctrl_icon[0].status = 0;
    single_ctrl_icon[1].status = 0;
    single_ctrl_icon[2].status = 0;
    single_ctrl_icon[3].status = 0;
    
    ptr->status = 1; /* 置该用电器的选择状态位为真 */

    /* 将右下方的logo清为白色背景 */
    LCD_SetTextColor(0xffffff);
    LCD_DrawFullRect(400, 290, 390, 180);

    /* 在选择框与用电器框之间绘制一个用于标识的小白块 */
    LCD_SetTextColor(BACK_COLOR); /* 清屏 */
    LCD_DrawFullRect(400, 240, 390, 10); 
    LCD_SetTextColor(0xffffff); /* 绘制小白块 */
    LCD_DrawFullRect(ptr->start_x, 240, 96, 10);

    PANEL_DEBUG("Draw 6 device icon above ");

    Draw_Widget(device[ptr->type], 6);

    PANEL_DEBUG("Draw 6 device icon below ");

}

/**
 * @brief 用于切换菜单栏的页签，并显示不同的页面
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Tag_Change(void *ic){
    Touch_Icon *ptr = (Touch_Icon *)ic;

    PANEL_DEBUG("Tag_Change in");

    /* 重置菜单栏页签状态 */
    Set_IconStatus(menu_icon, 3, 0);
    Draw_Widget(menu_icon, 3);
    
    /* 重置菜单栏页签状态 */
    // menu_icon[0].status = 0;
    // Draw_Icon(&menu_icon[0]);
    // Delay(0xfff);
    // menu_icon[1].status = 0;
    // Draw_Icon(&menu_icon[1]);
    // Delay(0xfff);
    // menu_icon[2].status = 0;
    // Draw_Icon(&menu_icon[2]);
    // Delay(0xfff);

    /* 将当前页签置为选中状态 */
    ptr->status = 1;

}

/**
 * @brief 独立控制页面
 * 
 */
void Draw_Single_Ctrl_Page(void *icon){

    PANEL_DEBUG("Function: Draw_Single_Ctrl_Page in ");

    /* 变换页签状态 */
    Tag_Change(icon);

    /* 绘制白色矩形背景 */
    LCD_SetTextColor(0xffffff);
    LCD_DrawFullRect(400, 40, 390, 250);

    /* 在屏幕右下方绘制logo */
    Draw_Logo();

    /* 绘制用电器选择图标 */
    Draw_Widget(single_ctrl_icon, 4);
    
}

/**
 * @brief 
 * 
 */
void Draw_Centre_Ctrl_Page(void *icon){
    
    PANEL_DEBUG("Function: Draw_Centre_Ctrl_Page in ");

    /* 变换页签状态 */
    Tag_Change(icon);

    /* 重置用电器选择状态 */
    Set_IconStatus(single_ctrl_icon, 4, 0);

    /* 清屏菜单页面 */
    LCD_SetTextColor(BACK_COLOR);
    LCD_DrawFullRect(400, 40, 390, 430);

    /* 绘制白色矩形背景 */
    LCD_SetTextColor(0xffffff);
    LCD_DrawFullRect(400, 40, 390, 250);

    /* 在屏幕右下方绘制logo */
    Draw_Logo();

    /* 绘制用电器选择图标 */
    Draw_Widget(centre_ctrl_icon, 4);

}


void Centre_Ctrl_Icon_Init(void) {

    Touch_Icon Icon_InitStruct;

    Icon_InitStruct.start_x = CENTRE_CTRL_ICON_START_X;
    Icon_InitStruct.start_y = CENTRE_CTRL_ICON_START_Y;
    Icon_InitStruct.width = ICON_SIZE;
    Icon_InitStruct.height = ICON_SIZE;
    Icon_InitStruct.status = 0;
    Icon_InitStruct.no = 0;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.draw_icon = Draw_Icon;
    Icon_InitStruct.icon_command = Control_All_Device;

    Icon_Struct_Init(&Icon_InitStruct, centre_ctrl_icon, 4);
    Matrix_Init(centre_ctrl_icon, 2, 2, CENTRE_CTRL_ICON_OFFSET_LINE, CENTRE_CTRL_ICON_OFFSET_COLUMN);  

    /* 风扇初始化 */
    centre_ctrl_icon[0].type = DEVICE_FAN;
    centre_ctrl_icon[0].gImage_icon[0] = gImage_fan_off;
    centre_ctrl_icon[0].gImage_icon[1] = gImage_fan_on;

    /* 灯泡初始化 */
    centre_ctrl_icon[1].type = DEVICE_LIGHT;
    centre_ctrl_icon[1].gImage_icon[0] = gImage_light_off;
    centre_ctrl_icon[1].gImage_icon[1] = gImage_light_on;

    /* 窗帘初始化 */
    centre_ctrl_icon[2].type = DEVICE_CURTAIN;
    centre_ctrl_icon[2].gImage_icon[0] = gImage_curtain_off;
    centre_ctrl_icon[2].gImage_icon[1] = gImage_curtain_on;

    /* 空调初始化 */
    centre_ctrl_icon[3].type = DEVICE_AC;
    centre_ctrl_icon[3].gImage_icon[0] = gImage_ac_off;
    centre_ctrl_icon[3].gImage_icon[1] = gImage_ac_on;

}

/**
 * @brief Control_All_Device 用电器集中控制命令函数
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Control_All_Device(void *icon){

    Touch_Icon *ptr = (Touch_Icon *)icon;

    ptr->status = (ptr->status == 0) ? 1 : 0; /* 反转用电器状态 */

    /******************************************************************
     * 
     * 将被触控的用电器的类型、编号、状态通过串口传输出去
     * 
     ******************************************************************/

}


/**
 * @brief 
 * 
 * @param icon 
 */
void Draw_Auto_Ctrl_Page(void *icon){

    Tag_Change(icon);

    PANEL_DEBUG("Function: Draw_Auto_Ctrl_Page in ");

    /* 清屏菜单页面 */
    LCD_SetTextColor(BACK_COLOR);
    LCD_DrawFullRect(400, 40, 390, 430);

    /* 绘制白色矩形背景 */
    LCD_SetTextColor(0xffffff);
    LCD_DrawFullRect(400, 40, 390, 250);

    /* 在屏幕右下方绘制logo */
    Draw_Logo();

    /* 绘制自动控制标签及开关按钮 */
    Draw_Widget(auto_ctrl_icon, 2);

}

void Auto_Ctrl_Icon_Init(void) {

    Touch_Icon Icon_InitStruct;

    Icon_InitStruct.start_y = 135;
    Icon_InitStruct.status = 0;
    Icon_InitStruct.touch_flag = 0;
    Icon_InitStruct.draw_icon = Draw_Icon;

    Icon_Struct_Init(&Icon_InitStruct, auto_ctrl_icon, 2);

    auto_ctrl_icon[0].start_x = 650;
    auto_ctrl_icon[0].width = 113;
    auto_ctrl_icon[0].height = 59;
    auto_ctrl_icon[0].icon_command = Switch_Model;
    auto_ctrl_icon[0].gImage_icon[0] = gImage_switch_off ;
    auto_ctrl_icon[0].gImage_icon[1] = gImage_switch_on;

    auto_ctrl_icon[1].start_x = 420;
    auto_ctrl_icon[1].width = 200;
    auto_ctrl_icon[1].height = 60;
    auto_ctrl_icon[1].gImage_icon[0] = gImage_auto_ctrl;

}

void Switch_Model(void *sw){
    Touch_Icon *ptr = (Touch_Icon *)sw;

    PANEL_DEBUG("Function: Switch_Model in");

    ptr->status = (ptr->status == 0) ? 1 : 0; /* 反转开关状态 */

    /***************************************************************
     * 
     * 添加切换成自动模式的语句
     * 
     **************************************************************/

}

void Draw_Logo(void){

    LCD_DisplayPicture(400, 290, 390, 180, gImage_logo);

}

/**
 * @brief 设置 Touch_Icon 结构体数组 status 成员的状态
 * 
 * @param IconArray 需要设置状态的 Touch_Icon 结构体数组
 * @param num 需要设置状态的数组成员数量
 * @param value 需要设置的 status 值
 */
void Set_IconStatus(Touch_Icon *IconArray, uint8_t num, uint8_t value){

    uint8_t i;
    
    for ( i = 0; i < num; i++) {

        IconArray[i].status = value;

    }

}
