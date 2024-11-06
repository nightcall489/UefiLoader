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

/*=============================================================================
 * Function: Ext2ReadBlock
 * -----------------------
 * Reads a single block of native file system block size from the specified
 * volume.
 *
 * Parameters:
 *    Volume   - Volume to read from.
 *    BlockNum - Block to read.
 *    Buffer   - Buffer to read data into.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2ReadBlock (
   IN EXT2_VOLUME *Volume,
   IN UINT32      BlockNum,
   OUT UINT8      *Buffer)
{
   if (Volume == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   return Ext2ReadDisk (
      Volume,
      BlockNum * Volume->BlockSize,
      Volume->BlockSize,
      Buffer
   );
}

/*=============================================================================
 * Function: Ext2ReadInode
 * -----------------------
 * Reads the inode with the given number.
 *
 * Parameters:
 *    Volume   - The volume from which to read the inode.
 *    InodeNum - Number of the inode to read.
 *    Inode    - Caller allocated inode structure to read into.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2ReadInode (
   IN EXT2_VOLUME *Volume,
   IN UINT32      InodeNum,
   OUT EXT2_INODE *Inode)
{
   EFI_STATUS                  Status;
   EXT2_BLOCK_GROUP_DESCRIPTOR GroupDesc;

   UINT32 InodeIndex;
   UINT32 BlockGroup;
   UINT32 GroupIndex;
   UINT32 InodeTable;

   if (Inode == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   InodeIndex = InodeNum - 1;
   BlockGroup = InodeIndex / Volume->Superblock->s_inodes_per_group;
   GroupIndex = InodeIndex % Volume->Superblock->s_inodes_per_group;

   UINT32 GroupDescTableBlock = (Volume->BlockSize == 1024) ? 2 : 1;
   UINT32 GroupDescOffset = (GroupDescTableBlock * Volume->BlockSize)
      + (BlockGroup * sizeof (EXT2_BLOCK_GROUP_DESCRIPTOR));

   /* TODO: Implement group descriptor caching. */
   CHECKSTRICT (Ext2ReadDisk (
      Volume,
      GroupDescOffset,
      sizeof (EXT2_BLOCK_GROUP_DESCRIPTOR),
      &GroupDesc
   ));

   InodeTable = GroupDesc.InodeTableStartingBlock;

   UINT32 InodeOffset = (InodeTable * Volume->BlockSize)
      + (GroupIndex * Volume->InodeSize);

   CHECKSTRICT (Ext2ReadDisk (
      Volume,
      InodeOffset,
      sizeof (EXT2_INODE),
      Inode
   ));

   return Status;
}

/*=============================================================================
 * Function: Ext2ReadSymlink
 * -------------------------
 * Reads the contents of a symbolic link. The caller is responsible for
 * deallocating / freeing the returned buffer.
 *
 * Parameters;
 *    File     - File representing the symlink
 *    Buffer   - Buffer to read data into.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2ReadSymlink (
   IN EXT2_INODE  *File,
   OUT CHAR8      *Buffer)
{
   EFI_STATUS Status;
   CHAR8      *Target;
   UINT8      *Block;

   if (File->Inode->i_size <= 60)
   {
      Target = File->Inode->i_block;
   }
   else
   {
      Target = AllocatePool (File->Inode->i_size);
      if (Target == NULL)
      {
         return EFI_OUT_OF_RESOURCES;
      }

      Block = AllocatePool (File->Volume->BlockSize);
      if (Block == NULL)
      {
         return EFI_OUT_OF_RESOURCES;
      }

      CHECKSTRICT (Ext2ReadBlock (
         File->Volume,
         File->Inode->i_block[0],
         Block
      ));

      CopyMem (Target, Block, File->Inode->i_size);
   }

   Buffer = Target;
   return EFI_SUCCESS;
}

/*=============================================================================
 * Function: Ext2ReadBoundsCheck
 * -----------------------------
 * Validates if an attempted read would start beyond the size of the inode.
 *
 * Parameters:
 *    File - File to be read from.
 *
 * Returns:
 *    TRUE or FALSE.
\*===========================================================================*/
[[nodiscard]] BOOLEAN
Ext2ReadBoundsCheck (
   IN EXT2_FILE *File)
{
   if (File == NULL)
   {
      return TRUE;
   }

   return File->Position > i_true_size (
      File->Inode, File->Volume->Superblock->s_rev_level);
}

/*=============================================================================
 * Function: Ext2ReadDirEnt
 * ------------------------
 * Reads a directory entry from the given file, advances the file position
 * and caches the read block if possible.
 *
 * Parameters:
 *    File     - The file to read from.
 *    Buffer   - Contains the directory entry on successful exit.
 *
 * Returns:
 *    EFI_SUCCESS - Directory entry was read successfully.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2ReadDirEnt (
   IN EXT2_FILE             *File,
   OUT EXT2_DIRECTORY_ENTRY *Buffer)
{
   EFI_STATUS           Status;
   UINT32               BlockIndex;
   EXT2_DIRECTORY_ENTRY *Entry;

   if (Ext2ReadBoundsCheck (File))
   {
      return EFI_DEVICE_ERROR;
   }

   /* FIX: This is really janky and won't work on indirect block pointers. */
   BlockIndex = File->Position / File->Volume->BlockSize;

   if (File->BlockCache == NULL || BlockIndex != File->BlockCache.Indeix)
   {
      File->BlockCache = AllocatePool (File->Volume->BlockSize);
      if (File->BlockCache == NULL)
      {
         return EFI_OUT_OF_RESOURCES;
      }

      CHECKSTRICT (Ext2ReadBlock (
         File->Volume,
         File->Inode->i_block[BlockIndex],
         &File->BlockCache)
      );
   }

   Entry = (EXT2_DIRECTORY_ENTRY *) (File->BlockCache + Position);
   /* FIX: Janky, fix asap. */
   File->Position += Entry->Size;

   Buffer = Entry;
   return EFI_SUCCESS;
}

/*=============================================================================
 * Function: Ext2Read
 * ------------------
 * Implements [EFI_FILE_PROTOCOL.Read]
 *
 * Parameters:
 *    This        - File handle to read from.
 *    BufferSize  - How many bytes to read.
 *    Buffer      - Buffer to read data into.
 *
 * Returns:
 *    EFI_SUCCESS - Data was read successfully.
\*===========================================================================*/
EFI_STATUS EFIAPI
Ext2Read (
   IN EFI_FILE_PROTOCOL *This,
   IN OUT UINTN         *BufferSize,
   OUT VOID             *Buffer)
{
   EFI_STATUS Status;
   EXT2_FILE  *File;

   File = FILE_FROM_INTERFACE (This);
   if (File == NULL)
   {
      return EFI_VOLUME_CORRUPTED;
   }

   if (Ext2IsDir (File))
   {
      /* dir read logic */
   }
   else
   {
      /* file read logic */
   }

   return EFI_SUCCESS; 
}
