
/*************************************************
    .
    File name:      	*.*
    Author：	        	任启红
    ID：					00001
    Date:          		202107
    Description:    	
    Others:         	无

    History:        	无
	    1.  Date:
		    Author: 
		    ID:
		    Modification:
		    
	    2.  Date:
		    Author: 
		    ID:
		    Modification:
*************************************************/




#pragma once

#include <Protocol/GraphicsOutput.h>
#include <Library/BaseLib.h>

#include <Global/Global.h>

#include <L1_GRAPHICS.h>


// Master File Table
typedef enum
{
	MFT_ITEM_DOLLAR_MFT = 0,
	MFT_ITEM_DOLLAR_MFT_MIRR,
	MFT_ITEM_DOLLAR_LOG_FILE,
	MFT_ITEM_DOLLAR_VOLUME,
	MFT_ITEM_DOLLAR_ATTR_DEF,
	MFT_ITEM_DOLLAR_ROOT,
	MFT_ITEM_DOLLAR_BIT_MAP,
	MFT_ITEM_DOLLAR_BOOT,
	MFT_ITEM_DOLLAR_BAD_CLUS,
	MFT_ITEM_DOLLAR_QUOTA,
	MFT_ITEM_DOLLAR_SECURE,
	MFT_ITEM_DOLLAR_UP_CASE,
	MFT_ITEM_DOLLAR_EXTEND_METADATA_DIRECTORY,
	MFT_ITEM_DOLLAR_EXTEND_REPARSE,
	MFT_ITEM_DOLLAR_EXTEND_USNJML,
	MFT_ITEM_DOLLAR_EXTEND_QUOTA,
	MFT_ITEM_DOLLAR_EXTEND_OBJECT_ID,
	MFT_ITEM_DOLLAR_MAX
}MFT_ITEM;


//桌面对象对点击事件（）
typedef enum 
{
    DESKTOP_ITEM_INIT_CLICKED_EVENT = 0, 
    DESKTOP_ITEM_START_MENU_CLICKED_EVENT, //开始菜单
    DESKTOP_ITEM_MY_COMPUTER_CLICKED_EVENT,    //我的电脑
    DESKTOP_ITEM_SYSTEM_SETTING_CLICKED_EVENT, //系统设置
    DESKTOP_ITEM_RECYCLE_BIN_CLICKED_EVENT,    //回收站
    DESKTOP_ITEM_MAX_CLICKED_EVENT             //默认最大值
}DESKTOP_ITEM_CLICKED_EVENT;

//开始菜单事件（注意：如果有子菜单，需要新建一个枚举，各个子菜单定义时相互独立，减少耦合）
typedef enum 
{ 
    START_MENU_ITEM_INIT_EVENT = 0, 
    START_MENU_ITEM_MY_COMPUTER_CLICKED_EVENT,    //我的电脑
    START_MENU_ITEM_SYSTEM_SETTING_CLICKED_EVENT,     //系统设置
    START_MENU_ITEM_MEMORY_INFORMATION_CLICKED_EVENT, //内存使用信息
    START_MENU_ITEM_SYSTEM_LOG_CLICKED_EVENT,         //系统日志
    START_MENU_ITEM_SHUTDOWN_CLICKED_EVENT,           //系统关机
    START_MENU_ITEM_MAX_CLICKED_EVENT                 //默认最大值
}START_MENU_ITEM_CLICKED_EVENT;

//开始菜单中系统设置子菜单事件
typedef enum 
{ 
    START_MENU_SYSTEM_SETTING_SUBITEM_WALLPAPER_SETTING_CLICKED_EVENT = 0, //桌面壁纸设置
    START_MENU_SYSTEM_SETTING_SUBITEM_WALLPAPER_RESET_CLICKED_EVENT,       //桌面壁纸还原
    START_MENU_SYSTEM_SETTING_SUBITEM_CLOSE_SUBITEM_CLICKED_EVENT,         //桌面壁纸还原
    START_MENU_SYSTEM_SETTING_SUBITEM_MAX_CLICKED_EVENT                    //默认最大值
}START_MENU_SYSTEM_SETTING_SUBITEM_CLICKED_EVENT;

//我的电脑窗口事件
typedef enum 
{ 
    MY_COMPUTER_WINDOW_PARTITION_ITEM_CLICKED_EVENT = 0,  //我的电脑打开某个分区事件
    MY_COMPUTER_WINDOW_FOLDER_ITEM_CLICKED_EVENT,         //我的电脑打开某个目录事件
    MY_COMPUTER_WINDOW_FILE_ITEM_CLICKED_EVENT,           //我的电脑打开某个文件事件
    MY_COMPUTER_WINDOW_CLOSE_WINDOW_CLICKED_EVENT,        //关闭我的电脑窗口事件
    MY_COMPUTER_WINDOW_MAX_CLICKED_EVENT                  //默认最大值
}MY_COMPUTER_WINDOW_CLICKED_EVENT;

//系统日志窗口事件
typedef enum 
{ 
    SYSTEM_LOG_WINDOW_CLOSE_WINDOW_CLICKED_EVENT = 0,    //关闭窗口
    SYSTEM_LOG_WINDOW_MAX_CLICKED_EVENT                  //默认最大值
}SYSTEM_LOG_WINDOW_CLICKED_EVENT;

//内存使用窗口事件
typedef enum 
{ 
    MEMORY_INFORMATION_WINDOW_CLOSE_WINDOW_CLICKED_EVENT = 0,    //关闭窗口
    MEMORY_INFORMATION_WINDOW_MAX_CLICKED_EVENT                  //默认最大值
}MEMORY_INFORMATION_WINDOW_CLICKED_EVENT;

/*
typedef enum 
{
    START_MENU_INIT_CLICKED_EVENT = 0,
    START_MENU_CLICKED_EVENT, //X
    MY_COMPUTER_CLICKED_EVENT,//X
    SETTING_CLICKED_EVENT,//X
    MEMORY_INFORMATION_CLICKED_EVENT,//X
    MY_COMPUTER_PARTITION_ITEM_CLICKED_EVENT,//X
    MY_COMPUTER_FOLDER_ITEM_CLICKED_EVENT,//X
    MY_COMPUTER_CLOSE_CLICKED_EVENT,//X
    SYSTEM_LOG_CLOSE_CLICKED_EVENT,//X
    MEMORY_INFORMATION_CLOSE_CLICKED_EVENT,//X
    SYSTEM_SETTING_CLOSE_CLICKED_EVENT,//X
    SYSTEM_LOG_CLICKED_EVENT,//X
    SYSTEM_QUIT_CLICKED_EVENT,//X
    WALLPAPER_SETTING_CLICKED_EVENT,//X
    WALLPAPER_RESET_CLICKED_EVENT,//X
    MAX_CLICKED_EVENT
}MOUSE_CLICK_EVENT;
*/

//开始菜单的状态汇总
typedef enum 
{
    START_MENU_CLICK_INIT_STATE = 0, //初始状态
    START_MENU_MY_COMPUTER_CLICKED_STATE,
    START_MENU_MEMORY_INFORMATION_CLICKED_STATE,
    START_MENU_SYSTEM_SETTING_CLICKED_STATE,
    START_MENU_SYSTEM_LOG_STATE,
    START_MENU_SYSTEM_QUIT_STATE,
    START_MENU_MAX_STATE
}START_MENU_STATE;


typedef enum 
{
    MY_COMPUTER_INIT_STATE = 0,
    MY_COMPUTER_PARTITION_CLICKED_STATE,
    MY_COMPUTER_FOLDER_CLICKED_STATE,
    MY_COMPUTER_QUIT_STATE,
    MY_COMPUTER_MAX_STATE
}MY_COMPUTER_STATE;


extern START_MENU_STATE 	StartMenuNextState;


typedef struct
{
    GRAPHICS_LAYER_ID           GraphicsLayerID;
    UINT16                       (*pClickEventGet)(); 
    VOID                        (*pClickEventHandle)(MOUSE_CLICK_EVENT); 
}GRAPHICS_LAYER_EVENT_GET;


typedef struct
{
    UINT16                    CurrentState;
    UINT16            		  event;
    UINT16                    NextState;
    VOID                      (*pFunc)(); 
}STATE_TRANSFORM;




void L2_GRAPHICS_ParameterInit();

VOID EFIAPI L2_DEBUG_Print1 (UINT16 x, UINT16 y,  IN  CONST CHAR8  *Format, ...);

VOID EFIAPI L2_DEBUG_Print3 (UINT16 x, UINT16 y, WINDOW_LAYER_ITEM layer, IN  CONST CHAR8  *Format, ...);

EFI_STATUS L2_GRAPHICS_ScreenInit();

EFI_STATUS L2_GRAPHICS_SystemSettingInit();

EFI_STATUS L2_GRAPHICS_StartMenuInit();

EFI_STATUS L2_GRAPHICS_SayGoodBye();

UINT16 L2_MOUSE_ClickEventHandle();


VOID L2_MOUSE_Click();

EFI_STATUS L2_GRAPHICS_DeskInit();

EFI_STATUS L2_GRAPHICS_AsciiCharDraw3(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0,UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor, UINT16 AreaWidth);

EFI_STATUS L2_GRAPHICS_ChineseHalfDraw(UINT8 *pBuffer,UINT8 d,
        IN UINTN x0, UINTN y0,
        UINT8 width,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color, UINT8 fontWidth);

VOID L2_GRAPHICS_LayerCompute(UINT16 iMouseX, UINT16 iMouseY, UINT8 MouseClickFlag);

VOID L2_STORE_FolderItemsPrint();

void L2_GRAPHICS_BootScreenInit();

void L2_GRAPHICS_Init();

void L2_GRAPHICS_CopyBufferFromWindowsToDesk();

VOID L2_STORE_PartitionItemsPrint(UINT16 Index);

EFI_STATUS L2_GRAPHICS_AsciiCharDraw(UINT8 *pBufferDest,
        IN UINTN x0, UINTN y0, UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color);
        
EFI_STATUS L2_GRAPHICS_ChineseCharDraw12(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0, UINT32 offset,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color , UINT16 AreaWidth);
        
EFI_STATUS L2_GRAPHICS_ChineseCharDraw2(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0, UINT32 AreaCode, UINT32 BitCode,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color , UINT16 AreaWidth);

EFI_STATUS L2_GRAPHICS_AsciiCharDraw2(WINDOW_LAYER_ITEM layer,
        IN UINTN x0, UINTN y0, UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color);

VOID L2_GRAPHICS_RightClickMenu(UINT16 iMouseX, UINT16 iMouseY);

VOID L2_MOUSE_MoveOver(UINT16 LayerID);

VOID L2_MOUSE_Move();

VOID
EFIAPI
L2_TIMER_Print (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

EFI_STATUS L2_GRAPHICS_ChineseCharDraw(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0, UINT32 offset,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color , UINT16 AreaWidth);

void L1_MEMORY_CopyColor1(UINT8 *pBuffer, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINT16 x0, UINT16 y0, UINT16 AreaWidth);

void L1_MEMORY_CopyColor2(UINT8 *pBuffer, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINT16 x0, UINT16 y0);

void L1_MEMORY_CopyColor3(UINT8 *pBuffer, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINT16 x0, UINT16 y0, UINT8 AreaWidth);


