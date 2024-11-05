/*=============================================================================
 * Drivers/Ext2/File.c
 * -------------------
 * Purpose: EXT2_FILE management
 * Author:  nightcall489
 * Created: Nov 04, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: Ext2AllocateFile
 * --------------------------
 * Allocates and initializes a new EXT2_FILE structure.
 *
 * Parameters:
 *    InodeNum - The inode for which to allocate a file structure
 *    File     - Pointer to the new file
 *
 * Returns:
 *    EFI_SUCCESS          - File was allocated successfully.
 *    EFI_OUT_OF_RESOURCES - Memory allocation error.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2AllocateFile (
   IN UINT32     InodeNum,
   OUT EXT2_FILE *File)
{
   File = AllocatePool (sizeof (EXT2_FILE));
   if (File == NULL)
   {
      return EFI_OUT_OF_RESOURCES;
   }

   return EFI_SUCCESS;
}

/*=============================================================================
 * Function: Ext2IsDir
 * -------------------
 * Checks if the given EXT2_FILE refers to a directory.
 *
 * Parameters:
 *    File - EXT2_FILE structure to check.
 *
 * Returns:
 *    TRUE or FALSE.
\*===========================================================================*/
[[nodiscard]] BOOLEAN
Ext2IsDir (
   IN EXT2_FILE *File)
{
   return ((File->Inode->i_mode & 0xF000) == EXT2_S_IFDIR);
}
