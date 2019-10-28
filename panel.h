/**
 * @file panel.h
 * @author Gump ()
 * @version V0.1
 * @date 2019-09-28
 * @brief 室内控制面板UI界面头文件
 * 
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 * 
 * none
 */

#ifndef __PANEL_H
#define __PANEL_H

#include "icon/icon.h" /* 包含图标图片数组 */

#define TEXT_COLOR 0x333333 /*!< 面板文字色 */
#define BACK_COLOR 0xDBF0F9 /*!< 面板背景色 */

/* 图标大小 */
#define ICON_SIZE 96 /*!< 图标的大小 */

#define MENU_ICON_W 130 /*!< 菜单栏页签宽度 */
#define MENU_ICON_H 30  /*!< 菜单栏页签高度 */

/* 图标起始位置 */
#define STATUS_ICON_START_X 40 /*!< 环境信息图标起始横坐标 */
#define STATUS_ICON_START_Y 30 /*!< 环境信息图标起始纵坐标 */
#define STATUS_ICON_OFFSET 14  /*!< 环境信息图标之间的间距 */

#define MENU_ICON_START_X 400 /*!< 菜单栏起始横坐标 */
#define MENU_ICON_START_Y 10  /*!< 菜单栏起始纵坐标 */
#define MENU_ICON_OFFSET 0    /*!< 菜单栏页签之间的间距 */

#define CENTRE_CTRL_ICON_START_X 470
#define CENTRE_CTRL_ICON_START_Y 60
#define CENTRE_CTRL_ICON_OFFSET_COLUMN 64
#define CENTRE_CTRL_ICON_OFFSET_LINE 24

/* 用电器类型 */
#define DEVICE_FAN 0x00     /*!< 风扇 */
#define DEVICE_LIGHT 0x01   /*!< 电灯 */
#define DEVICE_CURTAIN 0x02 /*!< 窗帘 */
#define DEVICE_AC 0x03      /*!< 空调 */

/* 菜单栏类型 */
#define MENU_SINGLE_CTRL 0x00 /*!< 独立控制 */
#define MENU_CENTRE_CTRL 0x01 /*!< 集中控制 */
#define MENU_AUTO_CTRL 0x02   /*!< 自动控制 */

/* 拼接字符串的宏指令 */
#define DEVICE_STATUS_TOGGLE(device, status) (status) ? (gImage_##device##_off) : (gImage_##device##_on)

typedef struct
{
    uint16_t start_x;   /*!< 图标的x起始坐标 */
    uint16_t start_y;   /*!< 图标的y起始坐标 */
    uint8_t width;      /*!< 图标宽度 */
    uint8_t height;     /*!< 图标高度 */
    uint8_t type;       /*!< 图标类型 */
    uint8_t status;     /*!< 图标状态 */
    uint8_t no;         /*!< 图标编号 */
    uint8_t touch_flag; /*!< 图标按下的标志 */

    /**
     * @brief 图标图片数组
     * @param *gImage_icon[2]
     *  @arg gImage_icon[0] status等于0时显示的图标
     *  @arg gImage_icon[1] status等于1时显示的图标
     */
    const unsigned char *gImage_icon[2];

    void (*draw_icon)(void *icon);    /*!< 图标描绘函数 */
    void (*icon_command)(void *icon); /*!< 图标功能执行函数 */

} Touch_Icon;

/*
	LCD 颜色代码，CL_是Color的简写
	16Bit由高位至低位， RRRR RGGG GGGB BBBB

	下面的RGB 宏将24位的RGB值转换为16位格式。
	启动windows的画笔程序，点击编辑颜色，选择自定义颜色，可以获得的RGB值。

	推荐使用迷你取色器软件获得你看到的界面颜色。
*/
#if LCD_RGB_888
/* RGB888颜色转换 */
#define RGB(R, G, B) ((R << 16) | (G << 8) | (B)) /* 将8位R,G,B转化为 24位RGB888格式 */

#else
/* RGB565 颜色转换 */
#define RGB(R, G, B) (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3)) /* 将8位R,G,B转化为 16位RGB565格式 */
#define RGB565_R(x) ((x >> 8) & 0xF8)
#define RGB565_G(x) ((x >> 3) & 0xFC)
#define RGB565_B(x) ((x << 3) & 0xF8)

#endif

#define PANEL
#define PANEL_DEBUG_ON 1
#define PANEL_DEBUG(fmt, ...)                        \
    do                                               \
    {                                                \
        if (PANEL_DEBUG_ON)                          \
            printf("<<-PANEL-DEBUG->> File:"__FILE__ \
                   "  LINE:[%d]  " fmt "\n",         \
                   __LINE__, ##__VA_ARGS__);         \
    } while (0)

/* 参数初始化函数 */
void Panel_Init(void);
void Touch_Icon_Init(void);
void Device_Icon_Init(void);
void Menu_Icon_Init(void);
void Status_Icon_Init(void);
void Single_Ctrl_Icon_Init(void);
void Auto_Ctrl_Icon_Init(void);
void Centre_Ctrl_Icon_Init(void);

/* 绘制函数 */
void Draw_Icon(void *icon);
void Draw_Logo(void);

/* 菜单页签绘制函数 */
void Draw_Single_Ctrl_Page(void *icon);
void Draw_Centre_Ctrl_Page(void *icon);
void Draw_Auto_Ctrl_Page(void *icon);

/* 触点判断函数 */
void Device_TouchUpHandler(Touch_Icon *device_ctrl_icon, uint16_t x, uint16_t y);

/* 触控命令函数 */
void Select_Device(void *icon);
void Tag_Change(void *icon);
void Control_Device(void *icon);
void Switch_Model(void *sw);
void Control_All_Device(void *icon);

/* 图标状态设置函数 */
void Set_IconStatus(Touch_Icon *IconArray, uint8_t num, uint8_t value);

/* 定义成外部函数，可以被触摸屏调用 */
extern void Touch_Icon_Down(uint16_t x, uint16_t y);
extern void Touch_Icon_Up(uint16_t x, uint16_t y);

/* bsp_user_interface.c */
extern void Delay(__IO u32 nCount);
extern void Icon_Struct_Init(Touch_Icon *source_icon, Touch_Icon *aim_icon, uint8_t icon_num);
extern void Matrix_Init(Touch_Icon *IconArray, uint8_t LineNum, uint8_t ColumnNum, uint8_t LineOffset, uint8_t ColumnOffset);
extern void Widget_TouchUpHandler(Touch_Icon *widget, uint8_t num, uint16_t x, uint16_t y);
extern void Draw_Widget(Touch_Icon *icon_array, uint8_t num);

#endif //__PANEL_H
