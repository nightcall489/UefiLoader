/*=============================================================================
 * Drivers/Ext2/Data.c
 * -------------------
 * Purpose: Miscellaneous functions
 * Author:  nightcall489
 * Created: Nov 03, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: AcquireLock
 * ---------------------
 * Locks the volume (TPL_CALLBACK - non-atomic operations).
 *
 * Parameters:
 *    None.
 *
 * Returns:
 *    None.
\*===========================================================================*/
VOID
AcquireLock (
   VOID)
{
   EfiAcquireLock (&gExt2LongLock);
}

/*=============================================================================
 * Function: AcquireLockOrFail
 * ---------------------------
 * Attempts to lock the volume, returns an error if the lock has already
 * been acquired.
 *
 * Parameters:
 *    None.
 *
 * Returns:
 *    EFI_SUCCESS       - Lock was acquired successfully.
 *    EFI_ACCESS_DENIED - Volume is already locked.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
AcquireLockOrFail (
   VOID)
{
   return EfiAcquireLockOrFail (&gExt2LongLock);
}

/*=============================================================================
 * Function: ReleaseLock
 * ---------------------
 * Unlocks the volume.
 *
 * Parameters:
 *    None.
 *
 * Returns:
 *    None.
\*===========================================================================*/
VOID
ReleaseLock (
   VOID)
{
   EfiReleaseLock (&gExt2LongLock);
}
