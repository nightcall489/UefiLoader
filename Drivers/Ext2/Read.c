/*=============================================================================
 * Drivers/Ext2/Read.c
 * -------------------
 * Purpose: Disk read abstractions
 * Author:  nightcall489
 * Created: Nov 04, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: Ext2ReadDisk
 * ----------------------
 * Reads an arbitrary, unaligned amount of bytes using the Disk I/O protocol.
 * The caller is responsible for allocating a large enough buffer.
 *
 * Parameters:
 *    Volume      - The volume to read from.
 *    Offset      - Offset to begin reading at.
 *    BufferSize  - Amount of bytes to read.
 *    Buffer      - The buffer to read data into.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2ReadDisk (
   IN EXT2_VOLUME *Volume,
   IN UINT32      Offset,
   IN UINT32      BufferSize,
   OUT VOID       *Buffer)
{
   if (Volume == NULL || Buffer == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   return Volume->DiskIo->ReadDisk (
      Volume->DiskIo,
      Volume->MediaId,
      Offset,
      BufferSize,
      Buffer
   );
}
