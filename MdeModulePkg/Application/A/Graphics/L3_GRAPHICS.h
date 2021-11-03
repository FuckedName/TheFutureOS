
/*************************************************
    .
    File name:      	A.c
    Author：	        任启红
    ID：		00001
    Date:          	202107
    Description:    	整个模块的主入口函数
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

#include <L2_GRAPHICS.h>


VOID L3_APPLICATION_MyComputerWindow(UINT16 StartX, UINT16 StartY);

VOID L3_APPLICATION_MemoryInformationWindow(UINT16 StartX, UINT16 StartY);

VOID L3_APPLICATION_SystemLogWindow(UINT16 StartX, UINT16 StartY);

VOID L3_APPLICATION_WindowsInitial();

