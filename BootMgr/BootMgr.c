/*=============================================================================
 * BootMgr/BootMgr.c
 * -----------------
 * Purpose: Boot manager entry point
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

#include "BootMgr.h"

/*=============================================================================
 * Function: UefiMain
 * ------------------
 * Primary entry point for the boot manager. Loads configuration and starts
 * up the main boot menu.
 *
 * Parameters:
 *    ImageHandle - Firmware allocated EFI image handle.
 *    SystemTable - Pointer to the EFI system table.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
 *===========================================================================*/
EFI_STATUS EFIAPI
UefiMain (IN EFI_HANDLE       ImageHandle,
          IN EFI_SYSTEM_TABLE *SystemTable)
{
   return EFI_SUCCESS;
}
