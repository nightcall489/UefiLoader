/*=============================================================================
 * Drivers/Ext2/Ext2.h
 * -------------------
 * Purpose: Core structures, macros and function prototypes
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

#ifndef __EFI_DRV_EXT2_H__
#define __EFI_DRV_EXT2_H__

/*=== Includes ==============================================================*/

#include <Uefi.h>

#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/DiskIo.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/BlockIo.h>

/*=== Macros and Constants ==================================================*/

#ifndef size_assert
#define size_assert( obj, size ) \
   typedef char obj##_size_off_[ ( !! ( sizeof( obj ) == size ) ) * 2 - 1 ]
#endif

#define ERRPRINT(status) \
   Print("Err: %r at %a:%d\n", status, __FILE__, __LINE__)

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

#define EXT2_PATH_SEPARATOR '/'

/*=== Structures ============================================================*/

typedef struct _EXT2_VOLUME EXT2_VOLUME;
typedef struct _EXT2_CFILE EXT2C_FILE;
typedef struct _EXT2_FILE EXT2_FILE;

struct EXT2_VOLUME
{

};

struct EXT2_CFILE
{

};

struct EXT2_FILE
{

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

/*=== Global Variables ======================================================*/

extern EFI_DRIVER_BINDING_PROTOCOL     gExt2DriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL     gExt2ComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL    gExt2ComponentName2;

extern EFI_SIMPLE_FILE_SYSTEM_PROTOCOL gExt2SimpleFileSystem;
extern EFI_FILE_PROTOCOL               gExt2File;

extern EFI_LOCK                        gExt2ShortLock;
extern EFI_LOCK                        gExt2LongLock;

#endif // __EFI_DRV_EXT2_H__
