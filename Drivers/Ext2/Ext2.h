/*=============================================================================
 * Drivers/Ext2/Ext2.h
 * -------------------
 * Purpose: Core structures, macros and function prototypes
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

#ifndef __EFI_DRV_EXT2_H__
#define __EFI_DRV_EXT2_H__

/*==- Includes -=============================================================*/

#include "Spec.h"

#include <Uefi.h>

#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/DiskIo.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/BlockIo.h>

/*==- Macros and Constants -=================================================*/

#define ERRPRINT(status) \
   Print("Err: %r at %a:%d\n", status, __func__, __LINE__)

#ifdef DEBUG
#define CHECKSTRICT(status) if (EFI_ERROR(status)) { \
   ErrPrint (status); \
   return status; }
#else
#define CHECKSTRICT(status) if (EFI_ERROR(status)) { \
   return status; }
#endif

#define EXT2_VOLUME_SIGNATURE SIGNATURE_32 ('e', 'x', 't', 'v')
#define EXT2_DIR_SIGNATURE    SIGNATURE_32 ('e', 'x', 't', 'd')
#define EXT2_FILE_SIGNATURE   SIGNATURE_32 ('e', 'x', 't', 'f')

#define VOLUME_FROM_INTERFACE(a) \
   CR (a, EXT2_VOLUME, Interface, EXT2_VOLUME_SIGNATURE)

#define FILE_FROM_INTERFACE(a) \
   CR (a, EXT2_FILE, Interface, EXT2_FILE_SIGNATURE)

/*=== Structures ============================================================*/

typedef struct _EXT2_VOLUME EXT2_VOLUME;
typedef struct _EXT2_CFILE EXT2C_FILE;
typedef struct _EXT2_FILE EXT2_FILE;

struct _EXT2_VOLUME
{
   UINT32                           Signature;
   EFI_HANDLE                       Handle;
   EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  Interface;
   
   EFI_BLOCK_IO_PROTOCOL            *BlockIo;
   EFI_DISK_IO_PROTOCOL             *DiskIo;
   EFI_DISK_IO2_PROTOCOL            *DiskIo2;

   UINT32                           MediaId;
   BOOLEAN                          Readonly;

   EXT2_SUPERBLOCK                  *Superblock;
   UINT32                           InodeSize;
   UINT32                           BlockSize;
   UINT32                           FragSize;
   UINT32                           VolSize;
   UINT32                           NumBlockGroups;

   EXT2_FILE                        *RootDir;
};

struct _EXT2_CFILE
{

};

struct _EXT2_FILE
{
   UINT32            Signature;
   EFI_FILE_PROTOCOL Interface;

   EXT2_VOLUME       *Volume;
   EXT2_FILE         *Parent;

   UINT32            Position;
   UINT32            RefCount;
};

/*=== Function Prototypes [Ext2.c] ==========================================*/

EFI_STATUS EFIAPI
DriverEntry (
   IN EFI_HANDLE       ImageHandle,
   IN EFI_SYSTEM_TABLE *SystemTable
);

EFI_STATUS EFIAPI
DriverUnload (
   IN EFI_HANDLE ImageHandle
);

EFI_STATUS EFIAPI
DriverBindingSupported (
   IN EFI_DRIVER_BINDING_PROTOCOL *This,
   IN EFI_HANDLE                  ControllerHandle,
   IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath OPTIONAL
);

EFI_STATUS EFIAPI
DriverBindingStart (
   IN EFI_DRIVER_BINDING_PROTOCOL *This,
   IN EFI_HANDLE                  ControllerHandle,
   IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath OPTIONAL
);

EFI_STATUS EFIAPI
DriverBindingStop (
   IN EFI_DRIVER_BINDING_PROTOCOL *This,
   IN EFI_HANDLE                  ControllerHandle,
   IN UINTN                       NumberOfChildren,
   IN EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
);

/*=== Function Prototypes [Init.c] ==========================================*/

[[nodiscard]] EFI_STATUS
Ext2MountVolume (
   IN EFI_HANDLE            DeviceHandle,
   IN EFI_BLOCK_IO_PROTOCOL *BlockIo,
   IN EFI_DISK_IO_PROTOCOL  *DiskIo,
   IN EFI_DISK_IO2_PROTOCOL *DiskIo2
);

/*=== Function Prototypes [Data.c] ==========================================*/

VOID
AcquireLock (
   VOID
);

EFI_STATUS
AcquireLockOrFail (
   VOID
);

VOID
ReleaseLock (
   VOID
);

/*=== Function Prototypes [Read.c] ==========================================*/

[[nodiscard]] EFI_STATUS
Ext2ReadDisk (
   IN EXT2_VOLUME *Volume,
   IN UINT32      Offset,
   IN UINT32      BufferSize,
   OUT VOID       *Buffer
);

/*=== Function Prototypes [EFI_SIMPLE_FILE_SYSTEM_PROTOCOL] =================*/

EFI_STATUS EFIAPI
Ext2OpenVolume (                                   /* Open.c */
   IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
   OUT EFI_FILE_PROTOCOL              **Root
);

/*=== Function Prototypes [EFI_FILE_PROTOCOL] ===============================*/

EFI_STATUS EFIAPI
Ext2Open (                                         /* Open.c */
   IN EFI_FILE_PROTOCOL  *This,
   OUT EFI_FILE_PROTOCOL **NewHandle,
   IN CHAR16             *FileName,
   IN UINT64             OpenMode,
   IN UINT64             Attributes
);

EFI_STATUS EFIAPI
Ext2Close (                                        /* Close.c */
   IN EFI_FILE_PROTOCOL *This
);

EFI_STATUS EFIAPI
Ext2Delete (                                       /* Write.c */
   IN EFI_FILE_PROTOCOL *This
);

EFI_STATUS EFIAPI
Ext2Read (                                         /* Read.c */
   IN EFI_FILE_PROTOCOL *This,
   IN OUT UINTN         *BufferSize,
   OUT VOID             *Buffer
);

EFI_STATUS EFIAPI
Ext2Write (                                        /* Write.c */
   IN EFI_FILE_PROTOCOL *This,
   IN OUT UINTN         *BufferSize,
   IN VOID              *Buffer
);

// OpenEx, ReadEx, WriteEx, FlushEx

EFI_STATUS EFIAPI
Ext2SetPosition (                                  /* Pos.c */
   IN EFI_FILE_PROTOCOL *This,
   IN UINT64            Position
);

EFI_STATUS EFIAPI
Ext2GetPosition (                                  /* Pos.c */
   IN EFI_FILE_PROTOCOL *This,
   OUT UINT64           *Position
);

EFI_STATUS EFIAPI
Ext2GetInfo (                                      /* Info.c */
   IN EFI_FILE_PROTOCOL *This,
   IN EFI_GUID          *InformationType,
   IN OUT UINTN         *BufferSize,
   OUT VOID             *Buffer
);

EFI_STATUS EFIAPI
Ext2SetInfo (                                      /* Info.c */
   IN EFI_FILE_PROTOCOL *This,
   IN EFI_GUID          *InformationType,
   IN UINTN             BufferSize,
   IN VOID              *Buffer
);

EFI_STATUS EFIAPI
Ext2Flush (                                        /* Write.c */
   IN EFI_FILE_PROTOCOL *This
);

/*=== Global Variables ======================================================*/

extern EFI_DRIVER_BINDING_PROTOCOL     gExt2DriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL     gExt2ComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL    gExt2ComponentName2;

extern EFI_SIMPLE_FILE_SYSTEM_PROTOCOL gExt2SimpleFileSystem;
extern EFI_FILE_PROTOCOL               gExt2File;

extern EFI_LOCK                        gExt2ShortLock;
extern EFI_LOCK                        gExt2LongLock;

#endif // __EFI_DRV_EXT2_H__
