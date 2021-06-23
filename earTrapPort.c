/*
 *
 * $Id: earTrapPort.c,v 1.1.1.1 2007-01-08 17:41:43 eugene Exp $
 *
 *
 *  Function: int earTrapPortWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earTrapPort.
 *
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


#define	REPETITIONS	   2
// extern char pl_buffer[];

void root_breakpoint(long unsigned int *);

void configChangeNotification(struct varBind *info);
void configErrorNotification(struct varBind *OIDInfo, char *msg);

int earTrapPortWrite(int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int SetTo;
    devBoardParamsType nvParams;
    int status;

    /*
     * earTrapPort(1.3.6.1.4.1.19775.1.1.1.20)
     */

    if (actionCode == SNMP_SET_ACTION) {
	SetTo = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
	SetTo = info->val.intVal;
    }

    if ((actionCode == SNMP_SET_ACTION)) {
	if (SetTo < 1 || SetTo > 65534) { // allowable port range
	    result = SNMP_ERR_BADVALUE;
	} else {

            status = semaphore_get_2 (&semaphore_nvParams_info,
                                      SEMAPHORE_TMO, 40408);
            if ( TX_SUCCESS == status ) {
                customizeReadDevBoardParams(&nvParams);
                if (nvParams.krbIpAddr /*snmp_port */  != SetTo) {
                    nvParams.krbIpAddr /*snmp_port */  = SetTo;
                    root_breakpoint(&nvParams.mgmtStations[0].ipAddress);
                    customizeWriteDevBoardParams(&nvParams);

                    semaphore_put_2 (&semaphore_nvParams_info, 40408);

                    configNotification(EARMIB_earTrapPort, "none");
                    // call the SNMP agent API here to change the trap port
                    naSnmpSetTrapSendPortNumber(nvParams.
                                                krbIpAddr /*snmp_port */ );
                } else {
                    result = SNMP_ERR_RESOURCEUNAVAILABLE;
                }
	    }
	}
    }

    return result;
}


/*
 *
 *  Function: void *earTrapPortRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTrapPort.
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

char int_buf[16];
void *earTrapPortRead(struct variable *vp, oid * name, int *length,
		      int isGet, int *varLen, setMethod * setVar)
{

    void *resultBuffer = NULL;
    int status;
    devBoardParamsType nvParams;
    *varLen = 0;

    /*
     * earTrapPort(1.3.6.1.4.1.19775.1.1.1.20)
     */

    if (scalar_handler(vp, name, length, isGet, varLen)) {
        status = semaphore_get_2 (&semaphore_nvParams_info,
                                  SEMAPHORE_TMO, 40402);
        if ( TX_SUCCESS == status ) {
            customizeReadDevBoardParams(&nvParams);
            *((int *) int_buf) = nvParams.krbIpAddr /*snmp_port */ ;
            *varLen = sizeof(int);
            resultBuffer = (void *) &int_buf;
            *setVar = (setMethod) earTrapPortWrite /*vp->writeFn */ ;
            semaphore_put_2 (&semaphore_nvParams_info, 40402);
        }
    }
    return resultBuffer;
}
