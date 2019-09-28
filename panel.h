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

#define ICON_SIZE 96 /* 图标的大小 */
#define ICON_NUM 4   /* 图标的数量 */

/* 图标的起始位置 */
#define ICON_START_X 300
#define ICON_START_Y 240

/* 用电器类型 */
#define DEVICE_FAN 0x00
#define DEVICE_LIGHT 0x01
#define DEVICE_CURTAIN 0x02
#define DEVICE_AC 0x03

/* 拼接字符串的宏指令 */
#define DEVICE_STATUS_TOGGLE(device, status) (status) ? (gImage_##device##_off) : (gImage_##device##_on)
typedef struct
{
    uint16_t start_x;   /* 按键的x起始坐标  */
    uint16_t start_y;   /* 按键的y起始坐标  */
    uint16_t end_x;     /* 按键的x结束坐标  */
    uint16_t end_y;     /* 按键的y结束坐标  */
    uint8_t device;     /* 用电器类型       */
    uint8_t status;     /* 用电器状态       */
    uint8_t touch_flag; /* 按键按下的标志   */

    void (*draw_icon)(void *icon);    //按键描绘函数
    void (*icon_command)(void *icon); //按键功能执行函数，例如切换颜色、画刷

} Touch_Icon;

/*
	LCD 颜色代码，CL_是Color的简写
	16Bit由高位至低位， RRRR RGGG GGGB BBBB

	下面的RGB 宏将24位的RGB值转换为16位格式。
	启动windows的画笔程序，点击编辑颜色，选择自定义颜色，可以获得的RGB值。

	推荐使用迷你取色器软件获得你看到的界面颜色。
*/
#if LCD_RGB_888
/*RGB888颜色转换*/
#define RGB(R, G, B) ((R << 16) | (G << 8) | (B)) /* 将8位R,G,B转化为 24位RGB888格式 */

#else
/*RGB565 颜色转换*/
#define RGB(R, G, B) (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3)) /* 将8位R,G,B转化为 16位RGB565格式 */
#define RGB565_R(x) ((x >> 8) & 0xF8)
#define RGB565_G(x) ((x >> 3) & 0xFC)
#define RGB565_B(x) ((x << 3) & 0xF8)

#endif

void Palette_Init(void);
void Touch_Icon_Init(void);
extern void Touch_Icon_Down(uint16_t x, uint16_t y);
extern void Touch_Icon_Up(uint16_t x, uint16_t y);
extern void Control_Device(void *icon);
void Draw_Icon(void *icon);

#endif //__PANEL_H