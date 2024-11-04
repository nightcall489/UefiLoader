/*=============================================================================
 * Drivers/Ext2/Ext2.c
 * -------------------
 * Purpose: Ext2 file system driver entry point
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*==- Global Variables -=====================================================*/

EFI_DRIVER_BINDING_PROTOCOL gExt2DriverBinding = {
   DriverBindingSupported,
   DriverBindingStart,
   DriverBindingStop,
   0x10,
   NULL,
   NULL
};

EFI_COMPONENT_NAME_PROTOCOL gExt2ComponentName = {
   
};

EFI_COMPONENT_NAME2_PROTOCOL gExt2ComponentName2 = {

};

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL gExt2SimpleFileSystem = {
   EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION,
   Ext2OpenVolume
};

EFI_FILE_PROTOCOL gExt2File = {

};

EFI_LOCK gExt2ShortLock = EFI_INITIALIZE_LOCK_VARIABLE (TPL_NOTIFY);
EFI_LOCK gExt2LongLock  = EFI_INITIALIZE_LOCK_VARIABLE (TPL_CALLBACK);

/*=============================================================================
 * Function: DriverEntry
 * ---------------------
 * Primary entry point for the Ext2 file system driver.
 *
 * Parameters:
 *    ImageHandle - Firmware allocated EFI image handle.
 *    SystemTable - Pointer to the EFI system table.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
EFI_STATUS EFIAPI
DriverEntry (IN EFI_HANDLE       ImageHandle,
             IN EFI_SYSTEM_TABLE *SystemTable)
{
   return EfiLibInstallDriverBindingComponentName2 (
         ImageHandle,
         SystemTable,
         &gExt2DriverBinding,
         ImageHandle,
         &gExt2ComponentName,
         &gExt2ComponentName2
   );
}

/*=============================================================================
 * Function: DriverUnload
 * ----------------------
 * Called when the driver image is unloaded.
 *
 * Parameters:
 *    ImageHandle - Firmware allocated EFI image handle.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
EFI_STATUS EFIAPI
DriverUnload (
   IN EFI_HANDLE ImageHandle)
{
   return EFI_UNSUPPORTED;
}

/*=============================================================================
 * Function: DriverBindingSupported
 * --------------------------------
 * Determines if our driver supports the given device. In our case, we only
 * require support for the Disk I/O and Block I/O protocols.
 *
 * Parameters:
 *    This                 - Pointer to this protocol instance.
 *    ControllerHandle     - Handle to the device.
 *    RemainingDevicePath  - Unused.
 *
 * Returns:
 *    EFI_SUCCESS          - Device is supported.
 *    EFI_ALREADY_STARTED  - Driver is already managing this device.
 *    Other                - Device is unsupported.
\*===========================================================================*/
EFI_STATUS EFIAPI
DriverBindingSupported (
   IN EFI_DRIVER_BINDING_PROTOCOL *This,
   IN EFI_HANDLE                  ControllerHandle,
   IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath OPTIONAL)
{
   EFI_STATUS           Status;
   EFI_DISK_IO_PROTOCOL *DiskIo;

   /* At this point, we're only checking if the device is a storage medium */

   Status = gBS->OpenProtocol (
         ControllerHandle,
         &gEfiDiskIoProtocolGuid,
         (VOID**)&DiskIo,
         This->DriverBindingHandle,
         ControllerHandle,
         EFI_OPEN_PROTOCOL_BY_DRIVER
   );
   CHECKSTRICT (Status);

   /* Error checks aren't necessary, as all errors are parameter related. */
   gBS->CloseProtocol (
         ControllerHandle,
         &gEfiDiskIoProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
   );

   return gBS->OpenProtocol (
         ControllerHandle,
         &gEfiBlockIoProtocolGuid,
         NULL,
         This->DriverBindingHandle,
         ControllerHandle,
         EFI_OPEN_PROTOCOL_TEST_PROTOCOL
   );
}

/*=============================================================================
 * Function: DriverBindingStart
 * ----------------------------
 * Starts the driver binding protocol on the given device. We can still bail
 * out at this point, so we'll do the file system checks here.
 *
 * Paraneters:
 *    This                 - Pointer to this protocol instance.
 *    ControllerHandle     - Handle to the device.
 *    RemainingDevicePath  - Unused.
 *
 * Returns:
 *    EFI_SUCCESS          - Device was mounted successfully.
 *    EFI_ALREADY_STARTED  - Device was already mounted.
 *    Other                - Failed to mount the device.
\*===========================================================================*/
EFI_STATUS EFIAPI
DriverBindingStart (
   IN EFI_DRIVER_BINDING_PROTOCOL *This,
   IN EFI_HANDLE                  ControllerHandle,
   IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath OPTIONAL)
{
   EFI_STATUS              Status;
   EFI_BLOCK_IO_PROTOCOL   *BlockIo;
   EFI_DISK_IO_PROTOCOL    *DiskIo;
   EFI_DISK_IO2_PROTOCOL   *DiskIo2;

   Status = gBS->OpenProtocol (
      ControllerHandle,
      &gEfiBlockIoProtocolGuid,
      (VOID **)&BlockIo,
      This->DriverBindingHandle,
      ControllerHandle,
      EFI_OPEN_PROTOCOL_GET_PROTOCOL
   );
   CHECKSTRICT (Status);

   Status = gBS->OpenProtocol (
      ControllerHandle,
      &gEfiDiskIoProtocolGuid,
      (VOID **)&DiskIo,
      This->DriverBindingHandle,
      ControllerHandle,
      EFI_OPEN_PROTOCOL_BY_DRIVER
   );
   CHECKSTRICT (Status);

   Status = gBS->OpenProtocol (
      ControllerHandle,
      &gEfiDiskIo2ProtocolGuid,
      (VOID **)&DiskIo2,
      This->DriverBindingHandle,
      ControllerHandle,
      EFI_OPEN_PROTOCOL_BY_DRIVER
   );
   if (EFI_ERROR (Status))
   {
      DiskIo2 = NULL;
   }

   return Ext2MountVolume (ControllerHandle, BlockIo, DiskIo, DiskIo2);
}

/*=============================================================================
 * Function: DriverBindingStop
 * ---------------------------
 * Stops the driver binding protocol on the given device.
 *
 * Parameters:
 *    This              - Pointer to this protocol instance.
 *    ControllerHandle  - Handle to the device.
 *    NumberOfChildren  - Unused.
 *    ChildHandleBuffer - Unused.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
EFI_STATUS EFIAPI
DriverBindingStop (
   IN EFI_DRIVER_BINDING_PROTOCOL *This,
   IN EFI_HANDLE                  ControllerHandle,
   IN UINTN                       NumberOfChildren,
   IN EFI_HANDLE                  *ChildHandleBuffer OPTIONAL)
{
   return EFI_SUCCESS;
}
