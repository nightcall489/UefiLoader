/*=============================================================================
 * Drivers/Ext2/Ext2.c
 * -------------------
 * Purpose: Logic pertaining to opening volumes, files or directories
 * Author:  nightcall489
 * Created: Nov 04, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: Ext2OpenVolume
 * ------------------------
 * Implements [EFI_SIMPLE_FILE_SYSTEM_PROTOCOL.OpenVolume].
 *
 * Parameters:
 *    This - Pointer to this protocol instance.
 *    Root - Handle to the root directory.
 *
 * Returns:
 *    EFI_SUCCESS          - The volume was opened.
 *    EFI_UNSUPPORTED      - Unsupported file system type.
 *    EFI_DEVICE_ERROR     - The device reported an error.
 *    EFI_VOLUME_CORRUPTED - File system structures are corrupted.
 *    EFI_ACCESS_DENIED    - Access to the file was denied.
 *    EFI_OUT_OF_RESOURCES - Memory allocation failure.
\*===========================================================================*/
EFI_STATUS EFIAPI
Ext2OpenVolume (
   IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
   OUT EFI_FILE_PROTOCOL              **Root)
{
   return EFI_SUCCESS;
}
