/**
 * @file icon.h
 * @author Gump 
 * @version V0.1
 * @date 2019-09-28
 * @brief 定义UI界面的图标数组，包括空调、电灯、风扇、窗帘用电器的开、关图标；
 *        人群密度、噪音、温度、湿度等室内环境信息图标。
 * 
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 * 
 * none
 */

#ifndef __ICON_H
#define __ICON_H

#include "stm32f4xx.h"

/* 用电器状态图标 */
extern const unsigned char gImage_ac_off[27648];      /* 空调关 */
extern const unsigned char gImage_ac_on[27648];       /* 空调开 */
extern const unsigned char gImage_light_off[27648];   /* 电灯关 */
extern const unsigned char gImage_light_on[27648];    /* 电灯开 */
extern const unsigned char gImage_fan_off[27648];     /* 风扇关 */
extern const unsigned char gImage_fan_on[27648];      /* 风扇开 */
extern const unsigned char gImage_curtain_off[27648]; /* 窗帘关 */
extern const unsigned char gImage_curtain_on[27648];  /* 窗帘开 */

/* 环境信息图标 */
extern const unsigned char gImage_people[27648]; /* 人群密度 */
extern const unsigned char gImage_noise[27648];  /* 环境噪音 */
extern const unsigned char gImage_humi[27648];   /* 环境湿度 */
extern const unsigned char gImage_temp[27648];   /* 环境温度 */

/* 控制模式菜单栏标签 */
extern const unsigned char gImage_auto_sel[11310];     /* 自动控制模式选中   */
extern const unsigned char gImage_auto_unsel[11310];   /* 自动控制模式未选中 */
extern const unsigned char gImage_centre_sel[11310];   /* 集中控制模式选中   */
extern const unsigned char gImage_centre_unsel[11310]; /* 集中控制模式未选中 */
extern const unsigned char gImage_single_sel[11310];   /* 独立控制模式选中   */
extern const unsigned char gImage_single_unsel[11310]; /* 独立控制模式未选中 */

/* 自动模式标签及开关图标 */
extern const unsigned char gImage_auto_ctrl[36000];  /* 自动控制标签 */
extern const unsigned char gImage_switch_off[20001]; /* 控制开关 关  */
extern const unsigned char gImage_switch_on[20001];  /* 控制开关 开  */

/* 图标显示函数 */
void LCD_DisplayPicture(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, const uint8_t *image);

#endif //__ICON_H
