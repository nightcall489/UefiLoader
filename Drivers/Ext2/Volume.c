/*=============================================================================
 * Drivers/Ext2/Volume.c
 * ---------------------
 * Purpose: Volume management
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
   EFI_STATUS  Status;
   EXT2_VOLUME *Volume;
   EXT2_FILE   *RootFile;

   if (Root == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   Volume = VOLUME_FROM_INTERFACE (This);
   if (Volume == NULL)
   {
      return EFI_DEVICE_ERROR;
   }

   if (Volume->RootDir == NULL)
   {
      // Allocate new root file
      
      if (EFI_ERROR (Status) || !Ext2IsDir(RootFile))
      {
         return EFI_VOLUME_CORRUPTED;
      }
   }

   *Root = &Volume->RootDir->Interface;

   return EFI_SUCCESS;
}

/*=============================================================================
 * Function: Ext2CreateVolume
 * --------------------------
 * Allocates a new EXT2_VOLUME structure and initializes its cache.
 *
 * Parameters:
 *    
 * Returns:
 *    EFI_SUCCESS          - Volume was allocated successfully.
 *    EFI_OUT_OF_RESOURCES - Memory allocation error.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2CreateVolume (
      )
{
   return EFI_SUCCESS;
}
