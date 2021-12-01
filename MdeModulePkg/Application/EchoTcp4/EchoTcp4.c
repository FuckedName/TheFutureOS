/** @file
    A simple, basic, application showing how the Hello application could be
    built using the "Standard C Libraries" from StdLib.

    Robin's uefi framework application, 2020-3-20
**/
//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <wchar.h>
#include "Common.h"
#include "Keyboard.h"
#include "Network.h"

EFI_STATUS TCP4Test(UINT32 Ip32, UINT16 Port);
/***
  Demonstrates basic workings of the main() function by displaying a
  welcoming message.

  Note that the UEFI command line is composed of 16-bit UCS2 wide characters.
  The easiest way to access the command line parameters is to cast Argv as:
      wchar_t **wArgv = (wchar_t **)Argv;

  @param[in]  Argc    Number of argument tokens pointed to by Argv.
  @param[in]  Argv    Array of Argc pointers to command line tokens.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/

EFI_STATUS
EFIAPI
mainEchoTcp4(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE  *SystemTable
	)
{
	UINT64 flag;
	INFO(L"flag=%x\n", flag);
	flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO | RNG_OUT);
	INFO(L"flag=%x\n", flag);
	WaitKey();
	//DEBUG(());
	 
	//text out test
    INFO(L"Please input any key to continue....\n");
    
	//Testnetwork	
    TCP4Test(MYIPV4(192, 168, 3, 2), 8888);
    return 0;
}

EFI_STATUS TCP4Test(UINT32 Ip32,UINT16 Port)
{
    EFI_STATUS Status = 0;
    CHAR8 SendStr[] = "Hello, I'm a client of UEFI.";

    CHAR8 *RecvBuffer = (CHAR8*) AllocatePool(1024);
    UINTN recvLen = 0;
    INFO(L"%d TCP4Test: SendTCP4Socket, %r\n", __LINE__, Status);
    
    CreateTCP4Socket();

    //参数配置
    ConfigTCP4Socket(Ip32, Port);   
        
    Status = ConnectTCP4Socket();    
    INFO(L"%x\n", Status);
    
    //可以继续执行
    if(EFI_ERROR(Status))    
    {
        //return Status;
    }

    //向服务器发送数据。
    Status = SendTCP4Socket(SendStr, AsciiStrLen(SendStr));
    INFO(L"TCP4Test: SendTCP4Socket, %r\n", Status);
    if(EFI_ERROR(Status))    
    {
        return Status;
    }
    
    gBS->Stall(1000);
    INFO(L"TCP4Test: Length of SendStr is %d \n", AsciiStrLen(SendStr));

    //从服务器接收数据。
    Status = RecvTCP4Socket(RecvBuffer, 1024, &recvLen);
    INFO(L"TCP4Test: RecvTCP4Socket, %r\n", Status);
    if(EFI_ERROR(Status))    
    {
        return Status;
    }

    //显示接收的数据。
    INFO(L" TCP4Test Recv: %d bytes\n", recvLen);
    AsciiPrint("Recv raw data:%c %c %c %c \n", RecvBuffer[0],RecvBuffer[1],RecvBuffer[2],RecvBuffer[3]);
    RecvBuffer[recvLen] = '\0';
    INFO(L" TCP4Test: Recv data is: %a\n", RecvBuffer);

    //当前测试，暂不释放socket资源
    //Status = CloseTCP4Socket();
    
    //if(EFI_ERROR(Status))
    //    INFO(L" Close tcp4 error%X!\n\r", Status);

    //FreePool(RecvBuffer);

    return Status;
}
