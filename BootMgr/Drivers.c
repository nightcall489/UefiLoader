/*=============================================================================
 * BootMgr/Drivers.c
 * -----------------
 * Purpose: Driver discovery, loading and initialization
 * Author:  nightcall489
 * Created: Nov 05, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "BootMgr.h"

/*=============================================================================
 * Function: LoadDrivers
 * ---------------------
 * Loads all available drivers from BOOTMGR_DIR_DRIVERS and attempts to
 * initialize each one.
 *
 * Parameters:
 *    None.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
LoadDrivers (
   IN VOID)
{
   return EFI_SUCCESS;
}
