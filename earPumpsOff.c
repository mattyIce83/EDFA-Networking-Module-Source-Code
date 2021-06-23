/*
 * $Id: earPumpsOff.c,v 1.1.1.1 2007-01-08 15:46:32 eugene Exp $
 *
 * Function: int earPumpsOffWrite (int actionCode, 
 * struct varBind *info)
 *
 * Description:
 *
 * Set the value of earPumpsOff.
 *
 * Output power is reduced to 0 mW.
 *
 * The Fusion agent breaks up the write into four different
 * phases and calls this function for each phase. The
 * current phase is indicated by the value of actionCode.
 * The possible values are:
 *
 * SNMP_SET_RESERVE reserve any buffer space or other
 * resources needed for the write.
 *
 * SNMP_SET_COMMIT copy the new value for the object
 * into the local buffer.
 *
 * SNMP_SET_ACTION write the value to the variable.
 *
 * SNMP_SET_FREE release the buffer or other 
 * resources previously allocated.
 *
 * SNMP_SET_UNDO restore the object's original value.
 *
 * info->setTo will contain the new value for the object.
 * info->val will contain the orignal value of the object.
 *
 * Parameters:
 *
 * actionCode indicates which phase of the write operation
 * is being requested. 
 * info pointer to structure with information about
 * the variable and the value it is being set
 * to. The structure is defined in SNMPIMPL.H.
 *
 *
 * Return Values:
 *
 * SNMP_ERR_NOERROR 
 * SNMP_ERR_TOOBIG 
 * SNMP_ERR_NOSUCHNAME 
 * SNMP_ERR_BADVALUE 
 * SNMP_ERR_READONLY 
 * SNMP_ERR_GENERR 
 * SNMP_ERR_NOACCESS 
 * SNMP_ERR_WRONGTYPE 
 * SNMP_ERR_WRONGLENGTH 
 * SNMP_ERR_WRONGENCODING 
 * SNMP_ERR_WRONGVALUE 
 * SNMP_ERR_NOCREATION 
 * SNMP_ERR_INCONSISTENTVALUE 
 * SNMP_ERR_RESOURCEUNAVAILABLE
 * SNMP_ERR_COMMITFAILED 
 * SNMP_ERR_UNDOFAILED 
 * SNMP_ERR_AUTHORIZATIONERROR 
 * SNMP_ERR_NOTWRITABLE 
 * SNMP_ERR_INCONSISTENTNAME 
 *
 */

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
#include "MASK.h"
#include "readFrom2U2.h"
#include "atmel.h"

#define	ALARM_INTERLOCK_FLAG	 10
#define	ALARM_KEYSWITCH_FLAG	 14

/* extern char pl_buffer[]; */
extern unsigned int alarmsSet;
extern unsigned long savedLaserCurrentValue;
extern unsigned long lowCurrentSetpoint;
extern unsigned long preserveLowCurrent;

/* toggle between low power and preserved values */
extern int count;

int earPumpsOffWrite(int actionCode, struct varBind *info)
{
    char buffer[MAX_FIELD_LEN];
    char opower[10];
    char value[16];

    int status;
    int alreadyREAD;
    int result = SNMP_ERR_NOERROR;

    /*
     * earPumpsOff(1.3.6.1.4.1.19775.1.1.2.14)
     */
    switch ( actionCode ) {
    case SNMP_SET_RESERVE:
    case SNMP_SET_FREE:
    case SNMP_SET_COMMIT:
        break;

    case SNMP_SET_ACTION:
    case SNMP_SET_UNDO:

        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 70701);
        if (status != TX_SUCCESS) {
            return SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {

            /* 
             * As of this writing (28APR06), the atmel doesn't do this right.
             * What it does does not actually properly turn the amp off,
             * it alternates between off and on, and there is no way for
             * us to know what stat it is in.
             * What we need to do is split this command into two:
             * off and on, so it's unambiguous, and fix the code on the
             * atmel so that it actually does what it's advertised to do.
             */
            printf("ZIMSLASEROFF\r");
            fflush(stdout);

            status = semaphore_put_2 (&semaphore_0_info, 70702);

            /* let the world know */
            //configChangeNotification(info);
        }
    }
        return result;
}

/*
 *
 * Function: void *earPumpsOffRead (struct variable *vp, 
 * oid *name, int *length, int exact, int *varLen, 
 * setMethod *setVar)
 *
 * Description:
 *
 * Get the value of earPumpsOff.
 *
 * Output power is reduced to 0 mW.
 *
 * Parameters:
 *
 * vp pointer to information about the object 
 * maintained by the agent.
 * name OID for the object supplied by the console.
 * length length of OID.
 * exact not used
 * varLen must be set by this function to the length
 * of the value returned.
 * setVar must be set by this function to the address
 * of the write function for this object, or
 * to NULL if this object is read only.
 *
 *
 * Return Values:
 *
 * NULL unable to read object
 * !NULL must point to a persistent buffer that 
 * contains the value of the object.
 *
 */
char earPumpsOffRead_buffer[MAX_2U_RESPONSE];
int earPumpsOffRead_length = MAX_2U_RESPONSE;
void *earPumpsOffRead(struct variable *vp, oid * name, int *length,
		      int isGet, int *varLen, setMethod * setVar)
{
    static unsigned long powerValue;
    void *resultBuffer = NULL;

    /*
     * earPumpsOff(1.3.6.1.4.1.19775.1.1.2.14)
     */
	if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    if ( scalar_handler (vp, name, length, isGet, varLen) ) {
        resultBuffer = askAtmelForUnsignedValue
            ("ZIMRLC",
             "ZRLC",
             E_READ_FROM_2U_NO_ADJUST,
             E_READ_FROM_2U_NO_CHECKSUM,
             4, // retries 
             &powerValue,
             70703);

        if ( resultBuffer ) {
            powerValue /= 10; */
            *varLen = sizeof(unsigned int);
            *setVar = (setMethod) earPumpsOffWrite; /*vp->writeFn */
//        }
//    }
    return &Mib_Leaves->earPumpOff; //resultBuffer;
}
