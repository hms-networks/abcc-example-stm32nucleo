/*******************************************************************************
** Copyright 2025-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Platform dependent implementation of abcc_time functions
********************************************************************************
*/

#include "abcc_object_config.h"
#include "abcc_time.h"

#if BAC_IA_CURR_DATE_AND_TIME_ENABLE
BOOL ABCC_TIME_GetDateAndTime( ABCC_TIME_DateAndTimeType* psTime )
{
   /*
   ** PORTING ALERT!
   ** Needs to be implemented in order for get date and time over BACnet to work
   */
   #error PORTING ALERT!

   return FALSE;
}
#endif /* BAC_IA_CURR_DATE_AND_TIME_ENABLE */

#if BAC_IA_CURR_DATE_AND_TIME_ENABLE && BAC_IA_CURR_DATE_AND_TIME_SET
BOOL ABCC_TIME_SetDateAndTime( ABCC_TIME_DateAndTimeType* psTime )
{
   /*
   ** PORTING ALERT!
   ** Needs to be implemented in order for set date and time over BACnet to work
   */
   #error PORTING ALERT!

   return FALSE;
}
#endif /* BAC_IA_CURR_DATE_AND_TIME_ENABLE && BAC_IA_CURR_DATE_AND_TIME_SET */
