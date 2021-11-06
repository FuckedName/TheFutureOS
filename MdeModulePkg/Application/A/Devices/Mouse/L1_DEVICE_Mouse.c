
/*************************************************
    .
    File name:      	*.*
    Author：	        	任启红
    ID：					00001
    Date:          		202107
    Description:    	鼠标处理的初始化操作
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




#include <L1_DEVICE_Mouse.h>

EFI_SIMPLE_POINTER_PROTOCOL  *gMouse;
UINT16 mouse_count = 0;
UINT8 MouseClickFlag = MOUSE_NO_CLICKED;

CHAR8 x_move = 0;
CHAR8 y_move = 0;

INT16 iMouseX = 0;
INT16 iMouseY = 0;
UINT16 MouseClickWindowWidth = 300;
UINT16 MouseClickWindowHeight = 400;




/****************************************************************************
*
*  描述:   xxxxx
*
*  参数1： xxxxx
*  参数2： xxxxx
*  参数n： xxxxx
*
*  返回值： 成功：XXXX，失败：XXXXX
*
*****************************************************************************/
EFI_STATUS L2_MOUSE_Init()
{
    EFI_STATUS                         Status;
    EFI_HANDLE                         *PointerHandleBuffer = NULL;
    UINTN                              i = 0;
	MouseClickFlag 					   = MOUSE_NO_CLICKED;
    UINTN                              HandleCount = 0;
    
    //get the handles which supports
    Status = gBS->LocateHandleBuffer(
                                    ByProtocol,
                                    &gEfiSimplePointerProtocolGuid,
                                    NULL,
                                    &HandleCount,
                                    &PointerHandleBuffer
                                    );
        
    if (EFI_ERROR(Status))  return Status;      //unsupport
    
    for (i = 0; i < HandleCount; i++)
    {
        Status = gBS->HandleProtocol(PointerHandleBuffer[i],
                                    &gEfiSimplePointerProtocolGuid,
                                    (VOID**)&gMouse);
            
        if (EFI_ERROR(Status))  
            continue;
        
        else
        {
            return EFI_SUCCESS;
        }
    }   
    
    return EFI_SUCCESS;

}


