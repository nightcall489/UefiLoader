/*=============================================================================
 * Drivers/Ext2/Cache.c
 * --------------------
 * Purpose: Cache initialization and management
 * Author:  nightcall489
 * Created: Nov 05, 2024
\*===========================================================================*/

/*==- Includes -=============================================================*/

#include "Ext2.h"

/*=============================================================================
 * Function: Ext2InitializeCache
 * -----------------------------
 * Initializes all caches for a given volume.
 *
 * Parameters:
 *    Volume - The volume for which to initialize the caches.
 *
 * Returns:
 *    EFI_SUCCESS or an appropriate EFI_STATUS error code.
\*===========================================================================*/
[[nodiscard]] EFI_STATUS
Ext2InitializeCache (
   EXT2_VOLUME *Volume)
{
   /* block group descriptor cache */

}
