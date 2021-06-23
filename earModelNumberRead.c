/*
 *
 * $Id: earModelNumberRead.c,v 1.1.1.1 2007-01-10 17:14:00 eugene Exp $
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
#include <reg_def.h>
#include <asn1.h>
#include <sockapi.h>
#include <tservapi.h>

#include <rtc.h>
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include <fbanks.h>

#include <snmpapi.h>
#include <snmpvars.h>

#include "MibMan.h"
#include "EARMIB.h"
#include "readFrom2U2.h"
#include "atmel.h"


/*
 *
 *  Function: void *earModelNumberRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earModelNumber.
 *
 *      Model number
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
//static unsigned char earModelNumberRead_buffer[MAX_2U_RESPONSE];
void *earModelNumberRead(struct variable *vp, oid * name, int *length,
			 int isGet, int *varLen, setMethod * setVar)
{
    void *resultBuffer = NULL;
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earModelNumber(1.3.6.1.4.1.19775.1.1.1.1)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
        return resultBuffer;
    }
/*
    resultBuffer = askAtmelForStringValue
        ("ZIMRMDL",
         "ZRMDL",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_DO_CHECKSUM,
         4, // retries 
         &(earModelNumberRead_buffer[0]),
         &bufferLength,
         10101);

    if ( resultBuffer ) { */
        *varLen = strlen(Mib_Leaves->earModelNumber);
        *setVar = (setMethod) vp->writeFn;
//    }

    return (void *) &Mib_Leaves->earModelNumber[0]; //resultBuffer;
}

