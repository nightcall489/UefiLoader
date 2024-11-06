/*=============================================================================
 * Drivers/Ext2/Close.c
 * --------------------
 * Purpose: File closing / cleanup logic
 * Author:  nightcall489
 * Created: Nov 06, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h" 

/*=============================================================================
 * Function: Ext2DeallocateFile
 * ----------------------------
 * Deallocates the specified EXT2_FILE structure.
 *
 * Parameters:
 *    File - The file to deallocate.
 *
 * Returns:
 *    EFI_SUCCESS - The file was deallocated successfully.
\*===========================================================================*/
EFI_STATUS
Ext2DeallocateFile (
   IN EXT2_FILE *File)
{
   if (File == NULL)
   {
      return EFI_SUCCESS;
   }

   Ext2Close (File->Parent->Interface);

   FreePool (File->Inode);
   if (File->BlockCache)
   {
      FreePool (File->BlockCache);
   }

   return EFI_SUCCESS;
}

/*=============================================================================
 * Function: Ext2Close
 * -------------------
 * Implements [EFI_FILE_PROTOCOL.Close]
 *
 * Parameters:
 *    This - Handle to the file to close.
 * 
 * Returns:
 *    EFI_SUCCESS - The file was closed.
\*===========================================================================*/
EFI_STATUS EFIAPI
Ext2Close (
   IN EFI_FILE_PROTOCOL *This)
{
   EFI_STATUS Status;
   EXT2_FILE  *File;

   File = FILE_FROM_INTERFACE (This);
   if (File == NULL)
   {
      /* file handle is already closed / invalid. */
      return EFI_SUCCESS;
   }

   File->RefCount = File->RefCount - 1;
   if (File->RefCount == 0)
   {
      Ext2DeleteFile (File);
   }
}
