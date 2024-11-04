/*=============================================================================
 * Drivers/Ext2/Spec.h
 * -------------------
 * Purpose: Ext2 file system structures, macros and constants
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

#ifndef __EFI_EXT2_SPEC_H__
#define __EFI_EXT2_SPEC_H__

/*==- Includes -=============================================================*/

#include "Macro.h"

/*==- Superblock -===========================================================*/

typedef struct
{
   UINT32 s_inodes_count;
   UINT32 s_blocks_count;
   UINT32 s_r_sblocks_count;
   UINT32 s_free_blocks_count;
   UINT32 s_free_inodes_count;
   UINT32 s_first_data_block;
   UINT32 s_log_block_size;
   UINT32 s_log_frag_size;
   UINT32 s_blocks_per_group;
   UINT32 s_frags_per_group;
   UINT32 s_inodes_per_group;
   UINT32 s_mtime;
   UINT32 s_wtime;
   UINT16 s_mnt_count;
   UINT16 s_max_mnt_count;
   UINT16 s_magic;
   UINT16 s_state;
   UINT16 s_errors;
   UINT16 s_minor_rev_level;
   UINT32 s_lastcheck;
   UINT32 s_checkinterval;
   UINT32 s_creator_os;
   UINT32 s_rev_level;
   UINT16 s_def_resuid;
   UINT16 s_def_resgid;
   /* Extended Superblock only (major version >= 1) */
   UINT32 s_first_ino;
   UINT16 s_inode_size;
   UINT16 s_block_group_nr;
   UINT32 s_feature_compat;
   UINT32 s_feature_incompat;
   UINT32 s_feature_ro_compat;
   CHAR8  s_uuid[16];
   CHAR8  s_volume_name[16];
   CHAR8  s_last_mounted[64];
   UINT32 s_algo_bitmap;
   UINT8  s_prealloc_blocks;
   UINT8  s_prealloc_dir_blocks;
   UINT16 reserved_0;
   CHAR8  s_journal_uuid[16];
   UINT32 s_journal_inum;
   UINT32 s_journal_dev;
   UINT32 s_last_orphan;
   UINT32 s_hash_seed[4];
   UINT8  s_def_hash_version;
   UINT8  reserved_1[3];
   UINT32 s_default_mount_options;
   UINT32 s_first_meta_bg;
   UINT8  reserved_2[760];
} EXT2_SUPERBLOCK;
size_assert(EXT2_SUPERBLOCK, 1024);

#define EXT2_SUPER_OFFSET  1024
#define EXT2_SUPER_MAGIC   0xEF53

#endif // __EFI_EXT2_SPEC_H__
