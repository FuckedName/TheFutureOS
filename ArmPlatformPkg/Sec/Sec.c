/** @file
*  Main file supporting the SEC Phase for Versatile Express
*
*  Copyright (c) 2011, ARM Limited. All rights reserved.
*  
*  This program and the accompanying materials                          
*  are licensed and made available under the terms and conditions of the BSD License         
*  which accompanies this distribution.  The full text of the license may be found at        
*  http://opensource.org/licenses/bsd-license.php                                            
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             
*
**/

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ArmLib.h>
#include <Library/SerialPortLib.h>
#include <Library/ArmPlatformLib.h>

#include <Chipset/ArmV7.h>
#include <Drivers/PL390Gic.h>

#define ARM_PRIMARY_CORE  0

#define SerialPrint(txt)  SerialPortWrite (txt, AsciiStrLen(txt)+1);

extern VOID *monitor_vector_table;

VOID
ArmSetupGicNonSecure (
  IN  INTN          GicDistributorBase,
  IN  INTN          GicInterruptInterfaceBase
);

// Vector Table for Sec Phase
VOID
SecVectorTable (
  VOID
  );

VOID
NonSecureWaitForFirmware (
  VOID
  );

VOID
enter_monitor_mode(
  IN VOID* Stack
  );

VOID
return_from_exception (
  IN UINTN NonSecureBase
  );

VOID
copy_cpsr_into_spsr (
  VOID
  );

VOID
CEntryPoint (
  IN  UINTN                     CoreId
  )
{
  CHAR8           Buffer[100];
  UINTN           CharCount;

  // Primary CPU clears out the SCU tag RAMs, secondaries wait
  if (CoreId == ARM_PRIMARY_CORE) {
    if (FixedPcdGet32(PcdMPCoreSupport)) {
      ArmInvalidScu();
    }

    // SEC phase needs to run library constructors by hand. This assumes we are linked against the SerialLib
    // In non SEC modules the init call is in autogenerated code.
    SerialPortInitialize ();

    // Start talking
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"UEFI firmware built at %a on %a\n\r",__TIME__, __DATE__);
    SerialPortWrite ((UINT8 *) Buffer, CharCount);

    // Now we've got UART, make the check:
    // - The Vector table must be 32-byte aligned
    ASSERT(((UINT32)SecVectorTable & ((1 << 5)-1)) == 0);
  }

  // Invalidate the data cache. Doesn't have to do the Data cache clean.
  ArmInvalidateDataCache();

  //Invalidate Instruction Cache
  ArmInvalidateInstructionCache();

  //Invalidate I & D TLBs
  ArmInvalidateInstructionAndDataTlb();

  // Enable Full Access to CoProcessors
  ArmWriteCPACR (CPACR_CP_FULL_ACCESS);

  // Enable SWP instructions
  ArmEnableSWPInstruction();

  // Enable program flow prediction, if supported.
  ArmEnableBranchPrediction();

  if (FixedPcdGet32(PcdVFPEnabled)) {
    ArmEnableVFP();
  }

  if (CoreId == ARM_PRIMARY_CORE) {
    // Initialize peripherals that must be done at the early stage
    // Example: Some L2x0 controllers must be initialized in Secure World
    ArmPlatformInitialize ();

    // If we skip the PEI Core we could want to initialize the DRAM in the SEC phase.
    // If we are in standalone, we need the initialization to copy the UEFI firmware into DRAM
    if (FeaturePcdGet(PcdSkipPeiCore) || !FeaturePcdGet(PcdStandalone)) {
      // Initialize system memory (DRAM)
      ArmPlatformInitializeSystemMemory();
    }

    // Some platform can change their physical memory mapping
    ArmPlatformBootRemapping();
  }

  // Test if Trustzone is supported on this platform
  if (ArmPlatformTrustzoneSupported()) {
    if (FixedPcdGet32(PcdMPCoreSupport)) {
      // Setup SMP in Non Secure world
      ArmSetupSmpNonSecure(CoreId);
    }

    // Enter Monitor Mode
    enter_monitor_mode((VOID*)(PcdGet32(PcdCPUCoresSecMonStackBase) + (PcdGet32(PcdCPUCoreSecMonStackSize) * CoreId)));

    //Write the monitor mode vector table address
    ArmWriteVMBar((UINT32) &monitor_vector_table);

    //-------------------- Monitor Mode ---------------------
    // Setup the Trustzone Chipsets
    if (CoreId == ARM_PRIMARY_CORE) {
      ArmPlatformTrustzoneInit();

      // Wake up the secondary cores by sending a interrupt to everyone else
      // NOTE 1: The Software Generated Interrupts are always enabled on Cortex-A9
      //         MPcore test chip on Versatile Express board, So the Software doesn't have to
      //         enable SGI's explicitly.
      //      2: As no other Interrupts are enabled,  doesn't have to worry about the priority.
      //      3: As all the cores are in secure state, use secure SGI's
      //

      PL390GicEnableDistributor (PcdGet32(PcdGicDistributorBase));
      PL390GicEnableInterruptInterface(PcdGet32(PcdGicInterruptInterfaceBase));

      // Send SGI to all Secondary core to wake them up from WFI state.
      PL390GicSendSgiTo (PcdGet32(PcdGicDistributorBase), GIC_ICDSGIR_FILTER_EVERYONEELSE, 0x0E);
    } else {
      // The secondary cores need to wait until the Trustzone chipsets configuration is done
      // before switching to Non Secure World

      // Enabled GIC CPU Interface
      PL390GicEnableInterruptInterface (PcdGet32(PcdGicInterruptInterfaceBase));

      // Waiting for the SGI from the primary core
      ArmCallWFI();

      // Acknowledge the interrupt and send End of Interrupt signal.
      PL390GicAcknowledgeSgiFrom(PcdGet32(PcdGicInterruptInterfaceBase), ARM_PRIMARY_CORE);
    }

    // Transfer the interrupt to Non-secure World
    PL390GicSetupNonSecure(PcdGet32(PcdGicDistributorBase),PcdGet32(PcdGicInterruptInterfaceBase));

    // Write to CP15 Non-secure Access Control Register :
    //   - Enable CP10 and CP11 accesses in NS World
    //   - Enable Access to Preload Engine in NS World
    //   - Enable lockable TLB entries allocation in NS world
    //   - Enable R/W access to SMP bit of Auxiliary Control Register in NS world
    ArmWriteNsacr(NSACR_NS_SMP | NSACR_TL | NSACR_PLE | NSACR_CP(10) | NSACR_CP(11));

    // CP15 Secure Configuration Register with Non Secure bit (SCR_NS), CPSR.A modified in any
    // security state (SCR_AW), CPSR.F modified in any security state (SCR_FW)
    ArmWriteScr(SCR_NS | SCR_FW | SCR_AW);
  } else {
    if (CoreId == ARM_PRIMARY_CORE) {
      SerialPrint ("Trust Zone Configuration is disabled\n\r");
    }

    // Trustzone is not enabled, just enable the Distributor and CPU interface
    if (CoreId == ARM_PRIMARY_CORE) {
      PL390GicEnableDistributor (PcdGet32(PcdGicDistributorBase));
    }
    PL390GicEnableInterruptInterface(PcdGet32(PcdGicInterruptInterfaceBase));

    // With Trustzone support the transition from Sec to Normal world is done by return_from_exception().
    // If we want to keep this function call we need to ensure the SVC's SPSR point to the same Program
    // Status Register as the the current one (CPSR).
    copy_cpsr_into_spsr();
  }

  // If ArmVe has not been built as Standalone then we need to patch the DRAM to add an infinite loop at the start address
  if (FeaturePcdGet(PcdStandalone) == FALSE) {
    if (CoreId == ARM_PRIMARY_CORE) {
      UINTN*   StartAddress = (UINTN*)PcdGet32(PcdNormalFdBaseAddress);

      // Patch the DRAM to make an infinite loop at the start address
      *StartAddress = 0xEAFFFFFE; // opcode for while(1)

      CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"Waiting for firmware at 0x%08X ...\n\r",StartAddress);
      SerialPortWrite ((UINT8 *) Buffer, CharCount);

      // To enter into Non Secure state, we need to make a return from exception
      return_from_exception(PcdGet32(PcdNormalFdBaseAddress));
    } else {
      // When the primary core is stopped by the hardware debugger to copy the firmware
      // into DRAM. The secondary cores are still running. As soon as the first bytes of
      // the firmware are written into DRAM, the secondary cores will start to execute the
      // code even if the firmware is not entirely written into the memory.
      // That's why the secondary cores need to be parked in WFI and wake up once the
      // firmware is ready.

      // Enter Secondary Cores into non Secure State. To enter into Non Secure state, we need to make a return from exception
      return_from_exception((UINTN)NonSecureWaitForFirmware);
    }
  } else {
    // To enter into Non Secure state, we need to make a return from exception
    return_from_exception(PcdGet32(PcdNormalFdBaseAddress));
  }
  //-------------------- Non Secure Mode ---------------------

  // PEI Core should always load and never return
  ASSERT (FALSE);
}

// When the firmware is built as not Standalone, the secondary cores need to wait the firmware
// entirely written into DRAM. It is the firmware from DRAM which will wake up the secondary cores.
VOID
NonSecureWaitForFirmware (
  VOID
  )
{
  VOID (*secondary_start)(VOID);

  // The secondary cores will execute the firmware once wake from WFI.
  secondary_start = (VOID (*)())PcdGet32(PcdNormalFdBaseAddress);

  ArmCallWFI();

  // Acknowledge the interrupt and send End of Interrupt signal.
  PL390GicAcknowledgeSgiFrom(PcdGet32(PcdGicInterruptInterfaceBase),ARM_PRIMARY_CORE);

  // Jump to secondary core entry point.
  secondary_start();

  // PEI Core should always load and never return
  ASSERT (FALSE);
}

VOID
SecCommonExceptionEntry (
  IN UINT32 Entry,
  IN UINT32 LR
  )
{
  CHAR8           Buffer[100];
  UINTN           CharCount;

  switch (Entry) {
  case 0:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"Reset Exception at 0x%X\n\r",LR);
    break;
  case 1:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"Undefined Exception at 0x%X\n\r",LR);
    break;
  case 2:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"SWI Exception at 0x%X\n\r",LR);
    break;
  case 3:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"PrefetchAbort Exception at 0x%X\n\r",LR);
    break;
  case 4:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"DataAbort Exception at 0x%X\n\r",LR);
    break;
  case 5:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"Reserved Exception at 0x%X\n\r",LR);
    break;
  case 6:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"IRQ Exception at 0x%X\n\r",LR);
    break;
  case 7:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"FIQ Exception at 0x%X\n\r",LR);
    break;
  default:
    CharCount = AsciiSPrint (Buffer,sizeof (Buffer),"Unknown Exception at 0x%X\n\r",LR);
    break;
  }
  SerialPortWrite ((UINT8 *) Buffer, CharCount);
  while(1);
}
