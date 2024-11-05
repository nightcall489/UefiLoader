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
 * Function: Ext2Open
 * ------------------
 * Implements [EFI_FILE_PROTOCOL.Open].
 *
 * Parameters:
 *    This        - File handle to the source location.
 *    NewHandle   - Pointer to the location for the new file.
 *    FileName    - Null-terminated file path.
 *    OpenMode    - Mode to open the file with.
 *    Attributes  - Attribute bits for the new file (EFI_FILE_MODE_CREATE)
 *
 * Returns:
 *    EFI_SUCCESS             - The file was opened.
 *    EFI_NOT_FOUND           - File was not found on the device.
 *    EFI_NO_MEDIA            - Device has no medium.
 *    EFI_DEVICE_ERROR        - Device reported an error.
 *    EFI_WRITE_PROTECTED     - Write or creation attempt on readonly medium
 *    EFI_VOLUME_FILL         - Volume is full.
 *    EFI_INVALID_PARAMETER   - Refers to a regular file, not a directory.
\*===========================================================================*/
EFI_STATUS EFIAPI
Ext2Open (
   IN EFI_FILE_PROTOCOL  *This,
   OUT EFI_FILE_PROTOCOL **NewHandle,
   IN CHAR16             *FileName,
   IN UINT64             OpenMode,
   IN UINT64             Attributes)
{
   //EXT2_VOLUME *Volume;
   EXT2_FILE   *File;

   File = FILE_FROM_INTERFACE (This);
   if (File == NULL)
   {
      return EFI_DEVICE_ERROR;
   }

   /* EFI_FILE_PROTOCOL.Open is only valid for directories. */
   if (!Ext2IsDir (File))
   {
      return EFI_INVALID_PARAMETER;
   }

   /* illegal flag combinations according to uefi spec 3.10 */
   if (OpenMode == EFI_FILE_MODE_WRITE || OpenMode == EFI_FILE_MODE_CREATE
      || OpenMode == (EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ)
      || OpenMode == (EFI_FILE_MODE_CREATE | EFI_FILE_MODE_WRITE))
   {
      return EFI_INVALID_PARAMETER;
   }

   return EFI_SUCCESS;
}
