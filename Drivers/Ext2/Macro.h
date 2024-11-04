/*=============================================================================
 * Drivers/Ext2/Macro.h
 * --------------------
 * Purpose: General purpose Macros
 * Author:  nightcall489
 * Created: Nov 04, 2024
\*===========================================================================*/

#ifndef size_assert
#define size_assert( obj, size ) \
   typedef char obj##_size_off_[ ( !! ( sizeof( obj ) == size ) ) * 2 - 1 ]
#endif
