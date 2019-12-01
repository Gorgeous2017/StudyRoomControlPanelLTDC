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

#include "./touch/gt9xx.h" /* 触摸屏驱动 */
#include "./lcd/bsp_lcd.h" /* 显示屏驱动 */

#include "panel.h" /* panel.c的头文件 */
#include "panel_usart.h" 

/* 图标结构体数组 */
Touch_Icon status_icon[4];      /*!< 环境信息图标数组 */
Touch_Icon menu_icon[3];        /*!< 菜单栏模式选择图标数组 */
Touch_Icon single_ctrl_icon[4]; /*!< 独立控制页面 用电器类型选择图标数组 */
Touch_Icon centre_ctrl_icon[4]; /*!< 集中控制页面 用电器集中控制图标数组 */
Touch_Icon auto_ctrl_icon[2];   /*!< 自动控制页面 自动控制标签及控制开关图标数组 */

/**
 * @brief 用电器独立控制图标数组
 * @details 数组第一维表示用电器类型，第二维表示该类型的用电器
 */
Touch_Icon device[4][6]; 

/**
 * @brief 控制面板初始化函数
 * 
 */
void Panel_Init(void)
{

	LCD_Clear(BACK_COLOR); /* 淡蓝背景色 */

	/* 初始化图标 */
	Touch_Icon_Init();

	/* 绘制环境信息图标 */
	Draw_Widget(status_icon, 4);

	/* 注意以下两者的先后顺序，更改的话上电后会出现菜单栏的独立控制页签未选中的现象 */
	/* 绘制独立控制页面 */
	Draw_Single_Ctrl_Page(menu_icon);
	/* 绘制菜单栏 */
	Draw_Widget(menu_icon, 3);

	/* 显示环境信息数字 */
	LCD_SetFont(&Font48x96);
	AP_DisplayStatus();


	PANEL_DEBUG("Draw String down ");
	
}

/**
 * @brief 根据指令串显示环境信息
 * 
 */
void AP_DisplayStatus(void) {

	uint8_t ucTemp[5];

	LCD_SetTextColor(TEXT_COLOR);

	sprintf(ucTemp, "%d%s", statusMsg.MsgBuff[1], "%");
	LCD_DisplayStringLine(STATUS_ICON_START_Y, ucTemp);

	sprintf(ucTemp, "%d%s", statusMsg.MsgBuff[4], "db");
	LCD_DisplayStringLine(STATUS_ICON_START_Y + (STATUS_ICON_OFFSET + ICON_SIZE), ucTemp);

	sprintf(ucTemp, "%d", statusMsg.MsgBuff[2]);
	LCD_DisplayStringLine(STATUS_ICON_START_Y + (STATUS_ICON_OFFSET + ICON_SIZE) * 2, ucTemp);

	sprintf(ucTemp, "%d%s", statusMsg.MsgBuff[3], "%");
	LCD_DisplayStringLine(STATUS_ICON_START_Y + (STATUS_ICON_OFFSET + ICON_SIZE) * 3, ucTemp);

}

/**
 * @brief Touch_Icon_Init 初始化各个图标数组
 * 
 */
void Touch_Icon_Init(void)
{
	Menu_Icon_Init(); /* 菜单栏初始 */
	Status_Icon_Init(); /* 环境信息图标初始化 */

	Single_Ctrl_Icon_Init(); /* 独立控制页面初始化 */
	Auto_Ctrl_Icon_Init(); /* 集中控制页面初始化 */
	Centre_Ctrl_Icon_Init(); /* 自动控制页面初始化 */

	Device_Icon_Init(); /* 用电器图标初始化 */

}

/**
 * @brief menu_icon 菜单栏初始化，位于面板右上方
 * 
 */
void Menu_Icon_Init(void) {

	Touch_Icon Icon_InitStruct;

	PANEL_DEBUG("Function: Menu_Icon_Init in ");

	Icon_InitStruct.start_x = MENU_ICON_START_X;
	Icon_InitStruct.start_y = MENU_ICON_START_Y;
	Icon_InitStruct.width = MENU_ICON_W;
	Icon_InitStruct.height = MENU_ICON_H;
	Icon_InitStruct.status = 0;
	Icon_InitStruct.touch_flag = 0;
	Icon_InitStruct.draw_icon = Draw_Icon;

	Icon_Struct_Init(&Icon_InitStruct, menu_icon, 3);
	Matrix_Init(menu_icon, 1, 3, 0, MENU_ICON_OFFSET); // 三个图标横向紧贴着排列
	
	menu_icon[0].status = 1; // 上电显示独立控制界面
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
 * @brief status_icon 环境信息图标初始化 位于面板左侧
 * 
 */
void Status_Icon_Init(void) {

	Touch_Icon Icon_InitStruct;

	PANEL_DEBUG("Function: Status_Icon_Init in ");

	Icon_InitStruct.start_x = STATUS_ICON_START_X;
	Icon_InitStruct.start_y = STATUS_ICON_START_Y;
	Icon_InitStruct.width = ICON_SIZE;
	Icon_InitStruct.height = ICON_SIZE;
	Icon_InitStruct.touch_flag = 0;
	Icon_InitStruct.status = 0;
	Icon_InitStruct.draw_icon = Draw_Icon;
	Icon_InitStruct.gImage_icon[0] = gImage_people;

	Icon_Struct_Init(&Icon_InitStruct, status_icon, 4);
	Matrix_Init(status_icon, 4, 1, STATUS_ICON_OFFSET, 0); // 四个图标纵向排列，间距为STATUS_ICON_OFFSET

	status_icon[0].gImage_icon[0] = gImage_people;

	status_icon[1].gImage_icon[0] = gImage_noise;

	status_icon[2].gImage_icon[0] = gImage_temp;

	status_icon[3].gImage_icon[0] = gImage_humi;
}

/**
 * @brief single_ctrl_icon 用电器类型选择图标初始化，位于独立控制页面的面板右侧中上方
 * 
 */
void Single_Ctrl_Icon_Init(void) {

	Touch_Icon Icon_InitStruct;

	PANEL_DEBUG("Function: Single_Ctrl_Icon_Init in ");

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

/**
 * @brief centre_ctrl_icon 用电器集中控制图标初始化，位于集中控制页面的面板右侧中上方
 * 
 */
void Centre_Ctrl_Icon_Init(void) {

	Touch_Icon Icon_InitStruct;

	PANEL_DEBUG("Function: Centre_Ctrl_Icon_Init in ");

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
	/* 四个图标排列成2x2的矩阵 */
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
 * @brief auto_ctrl_icon 自动控制图标初始化，位于集中控制页面的面板右侧中上方
 * 
 */
void Auto_Ctrl_Icon_Init(void) {

	Touch_Icon Icon_InitStruct;

	PANEL_DEBUG("Function: Auto_Ctrl_Icon_Init in ");

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

/**
 * @brief device 用电器控制图标初始化，位于独立控制页面的面板右侧中下方
 * 
 */
void Device_Icon_Init(void) {

	Touch_Icon Icon_InitStruct;

	PANEL_DEBUG("Function: Device_Icon_Init in ");

	Icon_InitStruct.start_x = 430;
	Icon_InitStruct.start_y = 260;
	Icon_InitStruct.width = ICON_SIZE;
	Icon_InitStruct.height = ICON_SIZE;
	Icon_InitStruct.status = 0;
	Icon_InitStruct.no = 0;
	Icon_InitStruct.touch_flag = 0;
	Icon_InitStruct.draw_icon = Draw_Icon;
	Icon_InitStruct.icon_command = Control_Device;

	/* 风扇初始化 */
	Icon_InitStruct.type = DEVICE_FAN;
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
 * @param x 触摸最后释放时的x坐标
 * @param y 触摸最后释放时的y坐标
 * 
 * @note 触控交互主体逻辑。菜单栏是任意界面都存在的，所以无需判断当前所处页面直接调用处理函数即可。
 * 而其他属于某个界面的按钮需要判断当前所处页面，交由当前所处界面的触控处理函数去处理。
 */
void Touch_Icon_Up(uint16_t x, uint16_t y){

	uint8_t i;
	
	PANEL_DEBUG("Funtion: Touch_Icon_Up");

	/* 判断菜单栏是否被触控 */
	Widget_TouchUpHandler(menu_icon, 3, x, y);

	/* 判断当前所处页面是否被触控 */
	for ( i = 0; i < 3; i++ ) { /* 遍历菜单栏页签 */

		PANEL_DEBUG("Menu %d status = %d",i ,menu_icon[i].status);
		
		if( menu_icon[i].status != 0){ /* 当某页签被选中时 */

			PANEL_DEBUG("Menu %d is choose, this menu type is %#X", i, menu_icon[i].type);

			switch (menu_icon[i].type ) { /* 判断当前所处的页签 */
			
			case MENU_SINGLE_CTRL: /* 独立控制 */

				PANEL_DEBUG("Case MENU_SINGLE_CTRL ");

				Widget_TouchUpHandler(single_ctrl_icon, 4, x, y); /* 判断用电器选择图标是否被触控 */
				Device_TouchUpHandler(single_ctrl_icon, x, y); /* 判断用电器控制图标是否被触控 */
				break;

			case MENU_CENTRE_CTRL: /* 集中控制 */

				PANEL_DEBUG("Case MENU_CENTRE_CTRL ");
			
				Widget_TouchUpHandler(centre_ctrl_icon, 4, x, y);
				break;

			case MENU_AUTO_CTRL: /* 自动控制 */

				PANEL_DEBUG("Case MENU_AUTO_CTRL ");

				Widget_TouchUpHandler(auto_ctrl_icon, 1, x, y);
				break;
			
			default:
				break;
			}

			break; /* 结束遍历 */
		}

	}
	
}

/**
 * @brief Device_TouchUpHandler 用电器控制图标的触控处理函数 
 * 
 * @param device_ctrl_icon 需要检测用电器有无被触控的二维数组
 * @param x 触点横坐标
 * @param y 触点纵坐标
 * 
 * @note 用电器控制交互主体逻辑。跟 Touch_Icon_Up 函数的主体逻辑基本一致。
 */
void Device_TouchUpHandler(Touch_Icon *device_ctrl_icon, uint16_t x, uint16_t y){

	uint8_t i;

	PANEL_DEBUG("Function: Device_TouchUpHandler in");

	
	for ( i = 0; i < 4; i++ ) { /* 遍历用电器选择图标 */
		
		PANEL_DEBUG("Device %d status = %d",i ,device_ctrl_icon[i].status);

		if( device_ctrl_icon[i].status != 0) { /* 当某种用电器被选择时 */

			PANEL_DEBUG("Device %d is choose, this device type is %#X", i, device_ctrl_icon[i].type);

			/* 用电器控制数组是二维数组，其中用电器类型(type)即为数组第一维的下标 */
			Widget_TouchUpHandler(device[ device_ctrl_icon[i].type ], 6, x, y); /* 检测该种用电器图标有无被触控 */
						
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
	uint8_t command_buf[5]; /*!< 需要发送的用电器控制指令串 */

	ptr->status = (ptr->status == 0) ? 1 : 0; /* 反转用电器状态 */

	/******************************************************************
	 * 
	 * 将被触控的用电器的类型、编号、状态通过串口传输出去
	 * 
	 ******************************************************************/

	command_buf[0] = ctrlMsg.MsgFlag;
	command_buf[1] = ptr->type;
	command_buf[2] = ptr->no;
	command_buf[3] = ptr->status;
	command_buf[4] = ctrlMsg.MsgFlag;

	/* 发送给AP，用于控制用电器 */
	Usart_SendBuff(AP_USART, command_buf, ctrlMsg.MsgLenth);
	/* 发送给ST，用于更新物联网公有云上的数据 */
	Usart_SendBuff(ST_USART, command_buf, ctrlMsg.MsgLenth);

}

/**
 * @brief Select_Device 用电器类型选择
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Select_Device(void *icon){

	Touch_Icon *ptr = (Touch_Icon *)icon;

	PANEL_DEBUG("Function: Select_Device in ");

	/* 重置用电器选择状态 */
	Set_IconStatus(single_ctrl_icon, 4, 0);
	
	/* 置该用电器的选择状态为真 */
	ptr->status = 1; 

	/* 将右下方的logo清为白色背景 */
	LCD_SetTextColor(0xffffff);
	LCD_DrawFullRect(400, 290, 390, 180);

	/* 在选择框与用电器框之间绘制一个用于标识的小白块 */
	LCD_SetTextColor(BACK_COLOR); /* 将选择框与用电器框之间的间隙清屏 */
	LCD_DrawFullRect(400, 240, 390, 10); 
	LCD_SetTextColor(0xffffff); /* 绘制小白块 */
	LCD_DrawFullRect(ptr->start_x, 240, 96, 10);

	/* 绘制用电器控制控件 */
	Draw_Widget(device[ptr->type], 6); 

}

/**
 * @brief 用于切换菜单栏的页签，并显示不同的页面
 * 
 * @param icon Touch_Icon 类型的图标参数
 */
void Tag_Change(void *ic) {
	
	Touch_Icon *ptr = (Touch_Icon *)ic;

	PANEL_DEBUG("Tag_Change in");

	/* 重置菜单栏页签状态 */
	Set_IconStatus(menu_icon, 3, 0);
	Draw_Widget(menu_icon, 3); /*!< 需要重绘页签，否则在显示屏上之前选中页签仍为选中状态  */
	
	/* 将当前页签置为选中状态 */
	ptr->status = 1;

}

/**
 * @brief 绘制独立控制页面
 * 
 * @param icon Touch_Icon 结构体 参数只能为 single_icon
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
