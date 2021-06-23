/*
 *
 * $Id: earRtcTime.c,v 1.2 2011-06-02 17:32:21 hazem Exp $
 *
 */

#define WANT_MIBMAN

//---------------------------------------  pwl  ---------------------------------------
// #define MAX_2U_RESPONSE 40	// 24 modified 121205   // 100 modified 11 15 05
//---------------------------------------  pwl  ---------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <tx_api.h>
#if 1
#include <reg_def.h>
#endif
#include <man_api.h>
#include <asn1.h>
#include <sockapi.h>
#include <tservapi.h>

//#include <snmp.h>

#include <rtc.h>
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include <fbanks.h>

#include <snmpapi.h>
#include <snmpvars.h>

#include "boardParams.h"
#include "man_agnt.h"
#include "MibMan.h"
#include "EARMIB.h"
#include "ipgStrings.h"
#include "telnet.h"
#include "ipgsnmp.h"

#include "semaphores.h"
#include "STACK_SIZE.h"
#include "root.h"


/*
 *
 *  Function: int earRtcTimeWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earRtcTime.
 *
 *      Real-time controller date and time
 *
 *      The Fusion agent breaks up the write into four different
 *      phases and calls this function for each phase.  The
 *      current phase is indicated by the value of actionCode.
 *      The possible values are:
 *
 *      SNMP_SET_RESERVE    reserve any buffer space or other
 *                          resources needed for the write.
 *
 *      SNMP_SET_COMMIT     copy the new value for the object
 *                          into the local buffer.
 *
 *      SNMP_SET_ACTION     write the value to the variable.
 *
 *      SNMP_SET_FREE       release the buffer or other 
 *                          resources previously allocated.
 *
 *      SNMP_SET_UNDO       restore the object's original value.
 *
 *      info->setTo will contain the new value for the object.
 *      info->val will contain the orignal value of the object.
 *
 *  Parameters:
 *
 *      actionCode  indicates which phase of the write operation
 *                  is being requested.  
 *      info        pointer to structure with information about
 *                  the variable and the value it is being set
 *                  to.  The structure is defined in SNMPIMPL.H.
 *
 *
 *  Return Values:
 *
 *      SNMP_ERR_NOERROR            
 *      SNMP_ERR_TOOBIG             
 *      SNMP_ERR_NOSUCHNAME         
 *      SNMP_ERR_BADVALUE           
 *      SNMP_ERR_READONLY           
 *      SNMP_ERR_GENERR             
 *      SNMP_ERR_NOACCESS           
 *      SNMP_ERR_WRONGTYPE          
 *      SNMP_ERR_WRONGLENGTH        
 *      SNMP_ERR_WRONGENCODING      
 *      SNMP_ERR_WRONGVALUE         
 *      SNMP_ERR_NOCREATION         
 *      SNMP_ERR_INCONSISTENTVALUE  
 *      SNMP_ERR_RESOURCEUNAVAILABLE
 *      SNMP_ERR_COMMITFAILED       
 *      SNMP_ERR_UNDOFAILED         
 *      SNMP_ERR_AUTHORIZATIONERROR 
 *      SNMP_ERR_NOTWRITABLE        
 *      SNMP_ERR_INCONSISTENTNAME   
 *
 */
int g_rtc_snmp_result = SNMP_ERR_NOERROR;
int earRtcTimeWrite(int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    rtcDataType timedate;
    int status;

    /*
     * earRtcTime(1.3.6.1.4.1.19775.1.1.1.6)
     */
    switch ( actionCode ) {
    case SNMP_SET_ACTION:
        status = semaphore_get_2 (&semaphore_rtc_info, SEMAPHORE_TMO, 30002);
        if (status != TX_SUCCESS) {
            return SNMP_ERR_RESOURCEUNAVAILABLE;
        }

	// Accept either MM/dd/yyyy HH:mm:ss or MM/dd/yyyy HH:mm:ss:SS, 
	// but ignore milliseconds if they are included 
	if ((info->setToLen == 19) || (info->setToLen == 22)) {
	    if ((info->setTo.string)[2] == '/' &&
		(info->setTo.string)[5] == '/' &&
		(info->setTo.string)[10] == ' ' &&
		(info->setTo.string)[13] == ':' &&
		(info->setTo.string)[16] == ':') {
		timedate.year = atoi(&(info->setTo.string)[6]) - 2000;
		timedate.month = atoi(&(info->setTo.string)[0]);
		timedate.day = atoi(&(info->setTo.string)[3]);
		timedate.hours = atoi(&(info->setTo.string)[11]);
		timedate.minutes = atoi(&(info->setTo.string)[14]);
		timedate.seconds = atoi(&(info->setTo.string)[17]);

		timedate.dayOfWeek = 1;
		if (rtcSet(&timedate) != RTC_SUCCESS) {
		    result = SNMP_ERR_COMMITFAILED;
		}
	    } else {
		result = SNMP_ERR_WRONGENCODING;
	    }
	} else {
	    result = SNMP_ERR_WRONGLENGTH;
	}
	g_rtc_snmp_result = result;
        status = semaphore_put_2 (&semaphore_rtc_info, 30002);

	if ( SNMP_ERR_NOERROR == result ) {
	  configNotification (EARMIB_earRtcTime, "none");
	}

        break;

    case SNMP_SET_UNDO:
    default:
        break;
    }

    return result;
}


/*
 *
 *  Function: void *earRtcTimeRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earRtcTime.
 *
 *     Real-time controller date and time
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *      length  length of OID.
 *      exact   not used
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this object, or
 *              to NULL if this object is read only.
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read object
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */
#define RTC_BUFFER_LENGTH 40
char rtc_buffer[RTC_BUFFER_LENGTH];
void *earRtcTimeRead(struct variable *vp, oid * name, int *length,
		     int isGet, int *varLen, setMethod * setVar)
{
    rtcDataType timedate;
    int status;
    void *resultBuffer = NULL;

    /*
     * earRtcTime(1.3.6.1.4.1.19775.1.1.1.6)
     */
    if (scalar_handler(vp, name, length, isGet, varLen)) {

        status = semaphore_get_2 (&semaphore_rtc_info, SEMAPHORE_TMO, 30007);
        if ( TX_SUCCESS == status ) {
            memcpy (&timedate, &g_timedate, sizeof (rtcDataType));
            semaphore_put_2 (&semaphore_rtc_info, 30007);
			// This is a US Date format
            //sprintf(rtc_buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            //       timedate.month,
            //       timedate.day,
            //       timedate.year + 2000,
            //       timedate.hours, 
            //       timedate.minutes, 
            //       timedate.seconds);
			// Change date format to European for Arris
			sprintf(rtc_buffer, "%04d-%02d-%02d %02d:%02d:%02d",
                    timedate.year + 2000,
					timedate.month,
                    timedate.day,
                    timedate.hours, 
                    timedate.minutes, 
                    timedate.seconds);		

            *varLen = strlen(rtc_buffer);

            *setVar = (setMethod) earRtcTimeWrite;

            resultBuffer = (void *) &(rtc_buffer[0]);
        }
    }

    return resultBuffer;

}


