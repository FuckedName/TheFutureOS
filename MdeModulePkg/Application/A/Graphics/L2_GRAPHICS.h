#pragma once

#include <Library/BaseLib.h>

#include <L1_GRAPHICS.h>

typedef enum 
{
    START_MENU_INIT_CLICKED_EVENT = 0,
    START_MENU_CLICKED_EVENT,
    MY_COMPUTER_CLICKED_EVENT,
    SETTING_CLICKED_EVENT,
    MEMORY_INFORMATION_CLICKED_EVENT,
    MY_COMPUTER_PARTITION_ITEM_CLICKED_EVENT,
    MY_COMPUTER_FOLDER_ITEM_CLICKED_EVENT,
    MY_COMPUTER_CLOSE_CLICKED_EVENT,
    SYSTEM_LOG_CLOSE_CLICKED_EVENT,
    MEMORY_INFORMATION_CLOSE_CLICKED_EVENT,
    SYSTEM_SETTING_CLOSE_CLICKED_EVENT,
    SYSTEM_LOG_CLICKED_EVENT,
    SYSTEM_QUIT_CLICKED_EVENT,
    WALLPAPER_SETTING_CLICKED_EVENT,
    WALLPAPER_RESET_CLICKED_EVENT,
    MAX_CLICKED_EVENT
}MOUSE_CLICK_EVENT;

typedef enum 
{
    CLICK_INIT_STATE = 0,
    MENU_CLICKED_STATE,
    MY_COMPUTER_CLICKED_STATE,
    MEMORY_INFORMATION_CLICKED_STATE,
    SYSTEM_SETTING_CLICKED_STATE,
    SYSTEM_LOG_STATE,
    MY_COMPUTER_PARTITION_CLICKED_STATE,
    MY_COMPUTER_FOLDER_CLICKED_STATE,
    SYSTEM_QUIT_STATE
}START_MENU_STATE;


typedef struct
{
    GRAPHICS_LAYER_ID           GraphicsLayerID;
    MOUSE_CLICK_EVENT           (*pClickEventGet)(); 
    VOID                        (*pClickEventHandle)(MOUSE_CLICK_EVENT); 
}GRAPHICS_LAYER_EVENT_GET;


typedef struct
{
    START_MENU_STATE                    CurrentState;
    MOUSE_CLICK_EVENT            		event;
    START_MENU_STATE                    NextState;
    EFI_STATUS                          (*pFunc)(); 
}START_MENU_STATE_TRANSFORM;




void L2_GRAPHICS_ParameterInit();

VOID EFIAPI L2_DEBUG_Print1 (UINT16 x, UINT16 y,  IN  CONST CHAR8  *Format, ...);

VOID EFIAPI L2_DEBUG_Print3 (UINT16 x, UINT16 y, WINDOW_LAYER_ITEM layer, IN  CONST CHAR8  *Format, ...);

EFI_STATUS L2_GRAPHICS_ScreenInit();

EFI_STATUS L2_GRAPHICS_SystemSettingInit();

EFI_STATUS L2_GRAPHICS_StartMenuInit();

EFI_STATUS L2_GRAPHICS_SayGoodBye();

UINT16 L2_MOUSE_ClickEventHandle();


VOID L2_MOUSE_Click();

