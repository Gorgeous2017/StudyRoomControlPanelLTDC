/**
 * @file icon.h
 * @author Gump 
 * @version V0.1
 * @date 2019-09-28
 * @brief 定义所有的图标数组
 * 
 * @copyright Copyright (c) <2019> <HANSHAN NORMAL UNIVERSITY(CHAOZHOU)> All rights Reserved.
 *   Media Recognition and Intelligent System Team
 * 
 * none
 */

#ifndef __ICON_H
#define __ICON_H

#include "stm32f4xx.h"

extern const unsigned char gImage_ac_off[27648];
extern const unsigned char gImage_ac_on[27648];
extern const unsigned char gImage_light_off[27648];
extern const unsigned char gImage_light_on[27648];
extern const unsigned char gImage_fan_off[27648];
extern const unsigned char gImage_fan_on[27648];
extern const unsigned char gImage_curtain_off[27648];
extern const unsigned char gImage_curtain_on[27648];
extern const unsigned char gImage_humi[27648];
extern const unsigned char gImage_temp[27648];
extern const unsigned char gImage_people[27648];
extern const unsigned char gImage_noise[27648];

void LCD_DisplayPicture(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, const uint8_t *image);

#endif //__ICON_H
