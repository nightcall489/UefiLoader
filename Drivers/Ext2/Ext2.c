/*=============================================================================
 * Drivers/Ext2/Ext2.c
 * -------------------
 * Purpose: Ext2 file system driver entry point
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: DriverEntry
 * ---------------------
 * Primary entry point for the Ext2 file system driver.
 *
 * Parameters:
 *    ImageHandle - Firmware allocated EFI image handle.
 *    SystemTable - Pointer to the EFI system table.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
EFI_STATUS EFIAPI
DriverEntry (IN EFI_HANDLE       ImageHandle,
             IN EFI_SYSTEM_TABLE *SystemTable)
{
   return EFI_SUCCESS;
}
