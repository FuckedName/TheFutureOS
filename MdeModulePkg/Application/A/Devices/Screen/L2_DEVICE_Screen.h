
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
#include <Library/UefiLib.h>



VOID L2_SCREEN_Init();

VOID EFIAPI L2_SCREEN_Draw (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *pBuffer,
									UINTN	SourceX,
									UINTN	SourceY,
									UINTN	DestinationX,
									UINTN	DestinationY,
									UINTN	Width,
									UINTN	Height);

