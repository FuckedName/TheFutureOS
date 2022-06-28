
/*************************************************
    .
    File name:      	*.*
    Author：	        	任启红
    ID：					00001
    Date:          		202201
    Description:    	用于给应用提供接口
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





#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "L2_APPLICATIONS_Command_curl.h"

#include <Global/Global.h>


/****************************************************************************
*
*  描述:   操作系统运行应用程序
*
*  pELF_Buffer     ELF文件缓冲内存存储
*  pReturnCode：    返回从ELF文件解析的机器码
*  参数n： xxxxx
*
*  返回值： 成功：XXXX，失败：XXXXX
*
*****************************************************************************/
EFI_STATUS L2_APPLICATIONS_Command_curl(UINT8 parameters[PARAMETER_COUNT][PARAMETER_LENGTH], UINT8 *pReturnCode)
{
    UINT8 j = 0;
	L2_DEBUG_Print3(DISPLAY_LOG_ERROR_STATUS_X, DISPLAY_LOG_ERROR_STATUS_Y, WindowLayers.item[GRAPHICS_LAYER_SYSTEM_LOG_WINDOW], "%d: L2_APPLICATIONS_Command_curl:%a \n", __LINE__, parameters[1]);
	
	int TerminalWindowMaxLineCount = 0;
	TerminalWindowMaxLineCount = (ScreenHeight - 23) / 2;
	TerminalWindowMaxLineCount /= 16;
	
    //写入键盘缓存到终端窗口。
	L2_DEBUG_Print3(3, 23 + (++TerminalCurrentLineCount) % TerminalWindowMaxLineCount * 16, WindowLayers.item[GRAPHICS_LAYER_TERMINAL_WINDOW], "%a", "Curl");

	//HttpCreateService;
	//HttpDxeStart
	//RunHttp
	//DownloadFile

	/*EFI_HTTP_PROTOCOL  mEfiHttpTemplate = {
		EfiHttpGetModeData,
		EfiHttpConfigure,
		EfiHttpRequest,
		EfiHttpCancel,
		EfiHttpResponse,
		EfiHttpPoll
	};*/
}



