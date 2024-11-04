/*=============================================================================
 * Drivers/Ext2/Init.c
 * -------------------
 * Purpose: Ext2 file system driver initialization and volume mounting
 * Author:  nightcall489
 * Created: Nov 04, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: Ext2AllocateVolume
 * ----------------------------
 * Validates wether the file system on this device is supported, and installs
 * the Simple File System Protocol on it. If successful, a new EXT2_VOLUME
 * structure is allocated for the volume, and its cache is initialized.
 *
 * Parameters:
 *    Handle   - Handle to the target controller.
 *    BlockIo  - Block I/O protocol opened on the controller.
 *    DiskIo   - Disk I/O protocol opened on the controller.
 *    DiskIo2  - Disk I/O 2 protocol opened on the controller.
 *
 * Returns:
 *    EFI_SUCCESS          - The volume was mounted successfully.
 *    EFI_OUT_OF_RESOURCES - Not enough memory.
 *    Other                - An error occurred during the mount process.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2ValidateFileSystem (
   IN EXT2_VOLUME *Volume)
{
   if (Volume == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   Volume->Superblock = AllocateZeroPool (sizeof (EXT2_SUPERBLOCK));
   if (Volume->Superblock == NULL)
   {
      return EFI_OUT_OF_RESOURCES;
   }

   CHECKSTRICT (Ext2ReadDisk (
      Volume,
      EXT2_SUPER_OFFSET,
      sizeof (EXT2_SUPERBLOCK),
      Volume->Superblock
   ));

   if (Volume->Superblock->s_magic != EXT2_SUPER_MAGIC)
   {
      return EFI_VOLUME_CORRUPTED;
   }

   Volume->InodeSize = Volume->Superblock->s_rev_level >= 1
      ? Volume->Superblock->s_inode_size
      : 128;

   Volume->BlockSize = 1024 << Volume->Superblock->s_log_block_size;
   Volume->FragSize  = 1024 << Volume->Superblock->s_log_frag_size;

   UINT32 NumGroups0 = (Volume->Superblock->s_blocks_count
                        + Volume->Superblock->s_blocks_per_group - 1)
                        / Volume->Superblock->s_blocks_per_group;

   UINT32 NumGroups1 = (Volume->Superblock->s_inodes_count
                        + Volume->Superblock->s_inodes_per_group - 1)
                        / Volume->Superblock->s_inodes_per_group;

   if (NumGroups0 != NumGroups1)
   {
      return EFI_VOLUME_CORRUPTED;
   }

   Volume->NumBlockGroups = NumGroups0;
   Volume->VolSize = Volume->Superblock->s_blocks_count * Volume->BlockSize;

   return EFI_SUCCESS;
}

[[nodiscard]] EFI_STATUS
Ext2MountVolume (
   IN EFI_HANDLE            DeviceHandle,
   IN EFI_BLOCK_IO_PROTOCOL *BlockIo,
   IN EFI_DISK_IO_PROTOCOL  *DiskIo,
   IN EFI_DISK_IO2_PROTOCOL *DiskIo2)
{
   EFI_STATUS  Status;
   EXT2_VOLUME *Volume;

   if (BlockIo == NULL || BlockIo->Media == NULL)
   {
      return EFI_INVALID_PARAMETER;
   }

   Volume = AllocateZeroPool (sizeof (EXT2_VOLUME));
   if (Volume == NULL)
   {
      return EFI_OUT_OF_RESOURCES;
   }

   Volume->Signature = EXT2_VOLUME_SIGNATURE;
   Volume->Handle    = DeviceHandle;
   Volume->BlockIo   = BlockIo;
   Volume->DiskIo    = DiskIo;
   Volume->DiskIo2   = DiskIo2;
   Volume->MediaId   = BlockIo->Media->MediaId;
   Volume->Readonly  = BlockIo->Media->ReadOnly;
   
   CopyMem (
      &Volume->Interface, 
      &gExt2SimpleFileSystem,
      sizeof (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL)
   );

   if (EFI_ERROR (Ext2ValidateFileSystem (Volume)))
   {
      goto Out;
   }

   gBS->InstallMultipleProtocolInterfaces (
      &Volume->Handle,
      &gEfiSimpleFileSystemProtocolGuid,
      &Volume->Interface,
      NULL
   );

Out:
   if (EFI_ERROR (Status))
   {
      if (Volume->Superblock)
      {
         FreePool (Volume->Superblock);
      }
      FreePool (Volume);
   }

   return Status;
}
