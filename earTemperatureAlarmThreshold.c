/*
 * $Id: earTemperatureAlarmThreshold.c,v 1.1.1.1 2007-01-08 15:46:32 eugene Exp $
 *
 *  Function: int earTemperatureAlarmThesholdWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earTemperatureAlarmTheshold.
 *
 *      The current minor alarm threshold value
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

#define WANT_MIBMAN

#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "tx_api.h"
#include "reg_def.h"
#include "man_api.h"
#include "asn1.h"
#include "sockapi.h"
#include "tservapi.h"

#include "rtc.h"
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include "fbanks.h"

#include "snmpapi.h"
#include "snmpvars.h"

#include "boardParams.h"
#include "man_agnt.h"
#include "MibMan.h"
#include "EARMIB.h"
#include "ipgStrings.h"		/* MAX_FIELD_LEN */
#include "telnet.h"
#include "ipgsnmp.h"
#include "MAX_ALARM.h"
#include "semaphores.h"
#include "readFrom2U2.h"
#include "atmel.h"


#define	REPETITIONS	   2
int earTemperatureAlarmThresholdWrite(int actionCode, struct varBind *info)
{
    char buffer[MAX_FIELD_LEN];
    unsigned int status;
    int result = SNMP_ERR_NOERROR;

    /*
     * earTemperatureAlarmThreshold(1.3.6.1.4.1.19775.1.1.3.1)
     */
    if (actionCode == SNMP_SET_ACTION) {
	status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 40403);
	if (status == TX_SUCCESS) {
	    /* I don't think this exists! */
	    sprintf(buffer, "ZIMSTEMPTHRESH %d\r", info->setTo.intVal);
	    printf("%s", buffer);
	    fflush(stdout);
	    status = semaphore_put_2 (&semaphore_0_info, 40403);
	}
    }

    if (result != SNMP_ERR_NOERROR) {
	result = SNMP_ERR_NOERROR;	/* breakpoint here */
    }
    return result;
}

/*
 *
 *  Function: void *earTemperatureAlarmThresholdRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTemperatureAlarmTheshold.
 *
 *      The current minor alarm threshold value
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

char earTemperatureAlarmThresholdRead_buffer[MAX_2U_RESPONSE];
int earTemperatureAlarmThresholdRead_length = MAX_2U_RESPONSE;
void *earTemperatureAlarmThresholdRead(struct variable *vp, oid * name,
				       int *length, int isGet, int *varLen,
				       setMethod * setVar)
{
    int TempVal1;
    int TempVal2;
    unsigned long tempValue;
    static int iTempValue;
    void *resultBuffer = NULL;
    char buffer[MAX_FIELD_LEN];
    char tempThresh[10];
    char value[16];


    if (!scalar_handler(vp, name, length, isGet, varLen)) {
        return resultBuffer;
    }

    /*
     * earTemperatureAlarmThreshold(1.3.6.1.4.1.19775.1.1.3.1)
     */
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRTEMPTHRESH",
         "ZRTMP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &tempValue,
         40405);

    if ( resultBuffer ) {
        iTempValue = tempValue;
        TempVal1 = (iTempValue / 1000);
        TempVal2 = iTempValue - (TempVal1 * 1000);

        if ((TempVal2 >= 5) || (TempVal2 <= -5)) {
            if (iTempValue >= 0) {
                iTempValue = (iTempValue / 1000) + 1;
            } else {
                iTempValue = (iTempValue / 1000) - 1;
            }
        } else {
            iTempValue = (iTempValue / 1000);
        }

        resultBuffer = &iTempValue; */
        *varLen = sizeof (int);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earTemperatureThreshold; //resultBuffer;
}

