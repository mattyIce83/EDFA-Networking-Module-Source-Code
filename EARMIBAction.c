/*
 *
 * $Id: EARMIBAction.c,v 1.2 2011-10-11 13:49:12 hazem Exp $
 *
 */

#define WANT_MIBMAN

#include <stdlib.h>
#include <asn1.h>
#include <snmpimpl.h>
#include <man_api.h>
#include <snmp.h>
#include <snmpvars.h>
#include "man_agnt.h"
#include "MibMan.h"
#include "EARMIB.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tx_api.h>
#if 1
#include <reg_def.h>
#endif
#include <sockapi.h>
#include <tservapi.h>
#include <rtc.h>
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include <fbanks.h>

#include "boardParams.h"
#include "ipgStrings.h"
#include "telnet.h"
#include "ipgsnmp.h"

#include "semaphores.h"
#include "STACK_SIZE.h"
#include "ascii.h"
#include "readFrom2U2.h"
#include "root.h"
#include "ipgFlash.h"

#include "atmel.h"

/* #define snmpThreadTMO	15000 */

#define MASK(x) ((int)1 << x)
#define	ALARM_INTERLOCK_FLAG	 10
#define	ALARM_KEYSWITCH_FLAG	 14

#define VALUE_UNDEFINED 0
#define POWER_MODE "POWER"
#define CURRENT_MODE "CURRENT"
#define GAIN_MODE "GAIN"

#define NORMAL "Normal"
#define TRIPPED "Tripped"

#define NORMAL_STATE 1
#define TRIPPED_STATE 2

#define DISABLE_RELAY 0
#define ENABLE_RELAY 1


//---------------------------------------  pwl  ---------------------------------------

#define	REPETITIONS	   2
#define MASK(x) 	((int)1 << x)


#define	unsigned32MAX		4026531839	// 10

extern unsigned int alarmsSet;

extern unsigned int manHistoryTableKey;
unsigned short FIRST;
extern unsigned short wrapFlag;

/* char pl_buffer[MAX_2U_RESPONSE]; */

extern TX_TIMER timer_snmpApp;	// found in root.c

// extern void writeTo2U(char buffer[MAX_FIELD_LEN]);

void root_breakpoint(long unsigned int *);

unsigned int SNMPAppFlag = FALSE;
unsigned int savedSemWaitCount;
unsigned int semWaitCount = 0;
unsigned long savedIntVal = 0;
unsigned long savedLaserCurrentValue;
unsigned long preserveLowCurrent;
unsigned long currentPowerValue;
unsigned long preserveLowPower;
unsigned long lowPowerSetpoint = 150;	// turn the pumps down!
unsigned long lowCurrentSetpoint = 0;	// turn the pumps down!
unsigned short chassisReset = 1;	// turn the pumps down!
unsigned short alreadyREAD = FALSE;

int count = 0;

extern unsigned int g_snmp_send_alive_trap_period;
extern unsigned char AliveTimerNVRamSave;
extern unsigned int TrapPeriod;
extern unsigned char SpecPattern;

/*
 *
 *  Function: void *earSerialNumberRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSerialNumber.
 *
 *      Serial number
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
//static unsigned char earSerialNumberRead_buffer[MAX_2U_RESPONSE];
void *earSerialNumberRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earSerialNumber(1.3.6.1.4.1.19775.1.1.1.2)
     */

    if (!scalar_handler(vp, name, length, isGet, varLen)) {
        return resultBuffer;
    }


 /*   resultBuffer = askAtmelForStringValue
        ("ZIMRSN",
         "ZRSN",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_DO_CHECKSUM,
         4, 
         &(MibLeaves.earSerialNumberRead_buffer[0]),
         &bufferLength,
         10155); */
//    if ( NULL != resultBuffer ) {
        *varLen = strlen(Mib_Leaves->earSerialNumber);
        *setVar = (setMethod) vp->writeFn;
//    }

    return Mib_Leaves->earSerialNumber; //resultBuffer;
}






/*
 *
 *  Function: void *earDateOfManufactureRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDateOfManufacture.
 *
 *      Date of manufacture
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
//static unsigned char earDateOfManufactureRead_buffer[MAX_2U_RESPONSE];
void *earDateOfManufactureRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earDateOfManufacture(1.3.6.1.4.1.19775.1.1.1.3)
     */

    if (!scalar_handler(vp, name, length, isGet, varLen)) {
        return resultBuffer;
    }

/*    resultBuffer = askAtmelForStringValue
        ("ZIMRDATE",
         "ZRDAT",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_DO_CHECKSUM,
         4, 
         &(MibLeaves.earDateOfManufactureRead_buffer[0]),
         &bufferLength,
    	 10107);
*/ 
//    if ( NULL != resultBuffer ) {
        *varLen = strlen(Mib_Leaves->earDateOfManufacture);
        *setVar = (setMethod) vp->writeFn;
//    }

    return Mib_Leaves->earDateOfManufacture; //resultBuffer;
}






/*
 *
 *  Function: void *earFirmwareRevisionRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earFirmwareRevision.
 *
 *      Firmware revision level
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
//static unsigned char earFirmwareRevisionRead_buffer[MAX_2U_RESPONSE];
void *earFirmwareRevisionRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earFirmwareRevision(1.3.6.1.4.1.19775.1.1.1.4)
     */

    if (!scalar_handler(vp, name, length, isGet, varLen)) {
        return NULL;
    }

/*   resultBuffer = askAtmelForStringValue
        ("ZIMRREV",
         "ZRREV",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries
         &(MibLeaves.earFirmwareRevisionRead_buffer[0]),
         &bufferLength,
         10110);
*/
//    if ( NULL != resultBuffer ) {
        *varLen = strlen(Mib_Leaves->earFirmwareRevision);
        *setVar = (setMethod) vp->writeFn;
//    }

    return Mib_Leaves->earFirmwareRevision; //resultBuffer;
}






/*
 *
 *  Function: int earControlModeWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earControlMode.
 *
 *      Set the control mode type
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
int earControlModeWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int controlMode;
    int val, status;

    if (actionCode == SNMP_SET_ACTION) {
        controlMode = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        controlMode = info->val.intVal;
    }
    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10113);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (controlMode) {
	    case VALUE_UNDEFINED:
		break;
	    case EARMIB_earControlMode_power:
//                writeTo2U ("ZIMSMPC\r");
		printf("ZIMSMPC\r");
		fflush(stdout);
		break;
	    case EARMIB_earControlMode_current:
//                writeTo2U ("ZIMSMCC\r");
		printf("ZIMSMCC\r");
		fflush(stdout);
		break;
	    case EARMIB_earControlMode_gain:
//                writeTo2U ("ZIMSMGC\r");
		printf("ZIMSMGC\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10113);
        }

        if (info->setTo.intVal != info->val.intVal)	// setTo. has the new value - info. has the current value
            /* configChangeNotification(info); */
            configNotification (EARMIB_earControlMode,
                                "FIXME; second parm ignored by callee");
    }

    return result;
}






/*
 *
 *  Function: void *earControlModeRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earControlMode.
 *
 *      Set the control mode type
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
//int earControlModeRead_value;
void *earControlModeRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned char buffer_[MAX_2U_RESPONSE];
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earControlMode(1.3.6.1.4.1.19775.1.1.1.5)
     */

    if (!scalar_handler(vp, name, length, isGet, varLen)) {
        return NULL;
    }

 /*   resultBuffer = askAtmelForStringValue
        ("ZIMRCM",
         "ZRCM",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries *
         &(buffer_[0]),
         &bufferLength,
         10115);
    if ( NULL != resultBuffer ) { */
        /* this can be a switch because the initial letter
         * of each possibility is different
         */
/*        switch ( buffer_[0] ) {
        case ATMEL_CONTROLMODE_POWER:
            earControlModeRead_value = EARMIB_earControlMode_power;
            break;
        case ATMEL_CONTROLMODE_CURRENT:
            earControlModeRead_value = EARMIB_earControlMode_current;
            break;
        case ATMEL_CONTROLMODE_GAIN:
            earControlModeRead_value = EARMIB_earControlMode_gain;
            break;
        default:
            // MIB should probably be extended to have UNKNOWN 
            earControlModeRead_value = 4;
            break;
        }
		MibLeaves.earControlMode = earControlModeRead_value; 
        //resultBuffer = &earControlModeRead_value;
		resultBuffer = &MibLeaves.earControlMode; */
        *varLen = sizeof (int);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earControlMode; //resultBuffer;
}






/*
 *
 *  Function: int earRtcTimeWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earRtcTime.
 *
 *      Real-time controller date and time MM/dd/yyyy HH:mm:ss:SS
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
/*    See other file */






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
 *      Real-time controller date and time MM/dd/yyyy HH:mm:ss:SS
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
/*    See other file */







/*
 *
 *  Function: void *earDigiFirmwareRevisionRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDigiFirmwareRevision.
 *
 *      Digi firmware revision level
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
char earDigiFirmwareRevisionRead_Buffer[MAX_2U_RESPONSE];
void *earDigiFirmwareRevisionRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    /*
     * earDigiFirmwareRevision(1.3.6.1.4.1.19775.1.1.1.7)
     */

    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    *varLen = sprintf(earDigiFirmwareRevisionRead_Buffer, 
            "%d.%d", MAJOR_VERSION, MINOR_VERSION);

    *setVar = (setMethod) vp->writeFn;

    return (void *)&(earDigiFirmwareRevisionRead_Buffer[0]);
}






/*
 *
 *  Function: void *earDigiBank1FirmwareRevisionRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDigiBank1FirmwareRevision.
 *
 *      Digi bank 1 firmware revision level
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
char earDigiBank1FirmwareRevisionRead_buffer[MAX_2U_RESPONSE];
void *earDigiBank1FirmwareRevisionRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    short Major;
    short Minor;

    /*
     * earDigiBank1FirmwareRevision(1.3.6.1.4.1.19775.1.1.1.8)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    GetFlashBankVersion(1, &Major, &Minor);

    *varLen = sprintf(earDigiBank1FirmwareRevisionRead_buffer, 
              "%d.%d", Major, Minor);
    *setVar = (setMethod) vp->writeFn;

    return (void *) &(earDigiBank1FirmwareRevisionRead_buffer[0]);
}






/*
 *
 *  Function: void *earDigiBank2FirmwareRevisionRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDigiBank2FirmwareRevision.
 *
 *      Digi bank 2 firmware revision level
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
char earDigiBank2FirmwareRevisionRead_buffer[MAX_2U_RESPONSE];
void *earDigiBank2FirmwareRevisionRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    short Major;
    short Minor;

    /*
     * earDigiBank2FirmwareRevision(1.3.6.1.4.1.19775.1.1.1.9)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    GetFlashBankVersion(2, &Major, &Minor);

    *varLen = sprintf(earDigiBank2FirmwareRevisionRead_buffer, 
              "%d.%d", Major, Minor);
    *setVar = (setMethod) vp->writeFn;
    return (void *) &(earDigiBank2FirmwareRevisionRead_buffer[0]);
}






/*
 *
 *  Function: int earDigiSelectedFirmwareBankWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earDigiSelectedFirmwareBank.
 *
 *      Digi selected firmware flash bank
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
int earDigiSelectedFirmwareBankWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int Bank;
    unsigned char BankFlag;
    int status;

    if (actionCode == SNMP_SET_ACTION) {
        Bank = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        Bank = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        GetFlashBankFlag(&BankFlag);
        switch (Bank) {
        case EARMIB_earDigiSelectedFirmwareBank_flashbank1:
            if (BankFlag & FLASH_USE_BANK2) {
                SetFlashBankFlag(FLASH_NEVER_LOADED | FLASH_UNCONFIRMED);
            }
            break;

        case EARMIB_earDigiSelectedFirmwareBank_flashbank2:
            if (!(BankFlag & FLASH_USE_BANK2)) {
                SetFlashBankFlag(FLASH_USE_BANK2 | FLASH_NEVER_LOADED |
                                 FLASH_UNCONFIRMED);
            }
            break;
        default:
            result = SNMP_ERR_WRONGVALUE;
            break;
        }

        // setTo. has the new value - info. has the current value
        if (info->setTo.intVal != info->val.intVal)
            configNotification (EARMIB_earDigiSelectedFirmwareBank,
                                "FIXME:  dummy");
    }
    return result;
}






/*
 *
 *  Function: void *earDigiSelectedFirmwareBankRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDigiSelectedFirmwareBank.
 *
 *      Digi selected firmware flash bank
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
int Bank;
void *earDigiSelectedFirmwareBankRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    unsigned char BankFlag;
    void *resultBuffer = &Bank;
    int status;

    /*
     * earDigiSelectedFirmwareBank(1.3.6.1.4.1.19775.1.1.1.10)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    GetFlashBankFlag(&BankFlag);
    if (BankFlag & FLASH_USE_BANK2)
	Bank = EARMIB_earDigiSelectedFirmwareBank_flashbank2;
    else
	Bank = EARMIB_earDigiSelectedFirmwareBank_flashbank1;

    *varLen = sizeof(int);
    *setVar = (setMethod) vp->writeFn;
    return resultBuffer;
}






/*
 *
 *  Function: int earDigiFirmwareConfirmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earDigiFirmwareConfirm.
 *
 *      Confirmation status of active bank
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
int earDigiFirmwareConfirmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int SetTo;
    unsigned char BankFlag;
    int status;

    if (actionCode == SNMP_SET_ACTION) {
        SetTo = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        SetTo = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        GetFlashBankFlag(&BankFlag);
        switch (SetTo) {
        case EARMIB_earDigiFirmwareConfirm_confirmed:
            if (BankFlag & FLASH_NEVER_LOADED) {
                // send trap - must reboot before confirm
                configNotification (EARMIB_earDigiFirmwareConfirm,
                                    "FIXME:  This arg is ignored");
            } else {
                SetFlashBankFlag(BankFlag & ~FLASH_UNCONFIRMED);
                // setTo. has the new value - info. has the current value
                if (info->setTo.intVal != info->val.intVal)
                    configNotification (EARMIB_earDigiFirmwareConfirm,
                                        "FIXME:  This arg is ignored");
            }
            break;

        case EARMIB_earDigiFirmwareConfirm_unconfirmed:
            // send trap - can't undo a confirm
            configNotification (EARMIB_earDigiFirmwareConfirm,
                                "FIXME:  This arg is ignored");
            break;

        default:
            result = SNMP_ERR_WRONGVALUE;
            break;
        }
    }

    return result;
}






/*
 *
 *  Function: void *earDigiFirmwareConfirmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDigiFirmwareConfirm.
 *
 *      Confirmation status of active bank
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
void *earDigiFirmwareConfirmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    unsigned char BankFlag;
    static int status;
    void *resultBuffer = &status;

    /*
     * earDigiFirmwareConfirm(1.3.6.1.4.1.19775.1.1.1.11)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    GetFlashBankFlag(&BankFlag);
    if (BankFlag & FLASH_UNCONFIRMED)
        status = EARMIB_earDigiFirmwareConfirm_unconfirmed;
    else
        status = EARMIB_earDigiFirmwareConfirm_confirmed;

    *varLen = sizeof(int);
    *setVar = (setMethod) vp->writeFn;

    return resultBuffer;
}






/*
 *
 *  Function: int earUpgradeDigiFirmwareWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earUpgradeDigiFirmware.
 *
 *      Upgrade Digi inactive flash bank with specified file
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
int earUpgradeDigiFirmwareWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    unsigned char BankFlag;
    unsigned char *filename;
    unsigned char path[100] = "";

    return SNMP_ERR_READONLY;

    if (actionCode == SNMP_SET_ACTION) {
        // find the last slash in the path/file 
        for (filename = info->setTo.string + info->setToLen - 1;
             filename >= info->setTo.string; filename--) {
            if (*filename == ASCII_SLASH) {
                filename++;
                break;
            }
        }
        strncpy(path, info->setTo.string, filename - info->setTo.string);

        GetFlashBankFlag(&BankFlag);

        if ( !(BankFlag & FLASH_UNCONFIRMED)
             && !(BankFlag & FLASH_NEVER_LOADED) ) {
            if ( BankFlag & FLASH_USE_BANK2 ) {
		// make sure that the bank we are running from has been confirmed
                if ( !ProgramAppToFlash(1, path, filename) ) {
                    SetFlashBankFlag(FLASH_NEVER_LOADED | FLASH_UNCONFIRMED);
                } else {
                    confignotification (EARMIB_earUpgradeDigiFirmware,
                                        "FIXME:  this parm ignored");
                    result = SNMP_ERR_GENERR;
                }
            } else {
		// make sure that the bank we are running from has been confirmed
                if ( !ProgramAppToFlash(2, path, filename) ) {
                    SetFlashBankFlag(FLASH_USE_BANK2 | FLASH_NEVER_LOADED |
                                     FLASH_UNCONFIRMED);
                } else {
                    configNotification (EARMIB_earUpgradeDigiFirmware,
                                        "FIXME:  this parm ignored");
                    result = SNMP_ERR_GENERR;
                }
            }
        } else {
            // can't upgrade until confirmed
            result = SNMP_ERR_GENERR;
        }
    }

    return result;
}






/*
 *
 *  Function: void *earUpgradeDigiFirmwareRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earUpgradeDigiFirmware.
 *
 *      Upgrade Digi inactive flash bank with specified file
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
char earUpgradeDigiFirmwareRead_buffer[MAX_2U_RESPONSE];
void *earUpgradeDigiFirmwareRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = &(earUpgradeDigiFirmwareRead_buffer[0]);

    /*
     * earUpgradeDigiFirmware(1.3.6.1.4.1.19775.1.1.1.12)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    // nothing to return here, this is write-only
    earUpgradeDigiFirmwareRead_buffer[0] = ASCII_NULL;
    *varLen = 0;

    *setVar = (setMethod) earUpgradeDigiFirmwareWrite;
    return resultBuffer;
}






/*
 *
 *  Function: void *earCleiCodeRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earCleiCode.
 *
 *      CLEI Code
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
char earCleiCodeRead_buffer[MAX_2U_RESPONSE];
void *earCleiCodeRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = &(earCleiCodeRead_buffer[0]);
    int status;

    /*
     * earCleiCode(1.3.6.1.4.1.19775.1.1.1.13)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    strcpy(earCleiCodeRead_buffer, CLEI_CODE);
    *varLen = sizeof (CLEI_CODE);
    *setVar = (setMethod) vp->writeFn;
    return resultBuffer;
}






/*
 *
 *  Function: int earTelnetTimeoutWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earTelnetTimeout.
 *
 *      Telnet timeout in seconds
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
int earTelnetTimeoutWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int SetTo;
    int status;
    devBoardParamsType nvParams;
    int sendNotify = 0;

    /*
     * earTelnetTimeout(1.3.6.1.4.1.19775.1.1.1.14)
     */
    SetTo = info->setTo.intVal;

    if (actionCode == SNMP_SET_COMMIT) {
	if ((SetTo < MIN_TELNET_TIMEOUT) || 
            (SetTo > MAX_TELNET_TIMEOUT) ){
	    result = SNMP_ERR_BADVALUE;
	}
    }

    if (actionCode == SNMP_SET_ACTION) {
	status = semaphore_get_2 (&semaphore_nvParams_info,
                                  SEMAPHORE_TMO, 10152);
	if (status != TX_SUCCESS) {
	    return SNMP_ERR_RESOURCEUNAVAILABLE;
	}

	customizeReadDevBoardParams(&nvParams);
	if ( TimeoutS != SetTo) {
	    nvParams.telnetTimeoutS = SetTo;
	    TimeoutS = SetTo;
            sendNotify = 1;
	    root_breakpoint(&nvParams.mgmtStations[0].ipAddress);
	    customizeWriteDevBoardParams(&nvParams);
	}

	status = semaphore_put_2 (&semaphore_nvParams_info, 10153);

        /* put configNotification outside the nvParams semaphore,
         * because it takes that semaphore itself
         */
        if ( 0 != sendNotify ) {
	    configNotification (EARMIB_earTelnetTimeout, "none");
        }
    }

    return result;
}






/*
 *
 *  Function: void *earTelnetTimeoutRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTelnetTimeout.
 *
 *      Telnet timeout in seconds
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
char earTelnetTimeoutRead_buffer[MAX_2U_RESPONSE];
void *earTelnetTimeoutRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    int status;
    devBoardParamsType nvParams;

    /*
     * earTelnetTimeout(1.3.6.1.4.1.19775.1.1.1.14)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    customizeReadDevBoardParams(&nvParams);
    /* GORD why bother copying it into the buffer?
     * -- Because this function "earTelnetTimeoutRead" is a callback,
     * and the value must persist for the caller to use -- bjb
     */
    memcpy(earTelnetTimeoutRead_buffer,
           &(nvParams.telnetTimeoutS),
           sizeof(unsigned long));
    *varLen = sizeof(unsigned long);
    resultBuffer = (void *) &(earTelnetTimeoutRead_buffer[0]);
    *setVar = (setMethod) vp->writeFn;

    return resultBuffer;
}





#if 0
/*
 *
 *  Function: int earIpAddressWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earIpAddress.
 *
 *      Chassis IP address
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
int earIpAddressWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int SetTo;
    char buffer[MAX_FIELD_LEN];
    char chAddr[17];
    int status;

    if ( info -> setTo.intVal == info->val.intVal ) {
        return result;
    }

    if ( SNMP_SET_ACTION == actionCode ) {
        SetTo = info->setTo.intVal;
    } 
    if ( SNMP_SET_UNDO == actionCode ) {
        SetTo = info->val.intVal;
    }

    if ( (SNMP_SET_ACTION == actionCode) ||
         (SNMP_SET_UNDO == actionCode) ) {

        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10156);
        if ( TX_SUCCESS != status ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    sprintf(chAddr, "%ld.%ld.%ld.%ld",
		    (SetTo >> 24) & 0xff,
		    (SetTo >> 16) & 0xff,
		    (SetTo >> 8) & 0xff,
                    SetTo & 0xff);

	    sprintf(buffer, "ZIMSIP %s\r", chAddr);
	    printf(buffer);
	    fflush(stdout);
            status = semaphore_put_2 (&semaphore_0_info, 10156);

            configNotification (EARMIB_earIpAddress, "FIXME:  dummy arg");
        }
    }

    return result;
}
#endif





/*
 *
 *  Function: void *earIpAddressRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earIpAddress.
 *
 *      Chassis IP address
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
//unsigned long ipAddress = 0;
void *earIpAddressRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;

    /*
     * earIpAddress(1.3.6.1.4.1.19775.1.1.1.15)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }
/*
    resultBuffer = askAtmelForIPAddressValue
        ("ZIMGETIP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4,
         &Mib_Leaves->earIpAddress,
         10158
         );
*/
//    if ( resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earIpAddress; //resultBuffer;
}





#if 0
/*
 *
 *  Function: int earSubnetMaskWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSubnetMask.
 *
 *      Chassis subnet mask
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
int earSubnetMaskWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int SetTo;
    char buffer[MAX_FIELD_LEN];
    char chAddr[17];
    int status ;

    if (info->setTo.intVal == info->val.intVal)	 {
       return result;
    }

    if ( SNMP_SET_ACTION == actionCode ) {
        SetTo = info->setTo.intVal;
    } 
    if ( SNMP_SET_UNDO == actionCode ) {
        SetTo = info->val.intVal;
    }

    if ( (SNMP_SET_ACTION == actionCode) ||
         (SNMP_SET_UNDO == actionCode) ) {

        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10160);
        if ( TX_SUCCESS != status ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
            sprintf(chAddr, "%ld.%ld.%ld.%ld",
                    (SetTo >> 24) & 0xff,
                    (SetTo >> 16) & 0xff,
                    (SetTo >> 8) & 0xff,
                    SetTo & 0xff);

            sprintf(buffer, "ZIMSNET %s\r", chAddr);
            printf(buffer);
            fflush(stdout);
            semaphore_put_2 (&semaphore_0_info, 10160);

            configNotification (EARMIB_earSubnetMask, "FIXME:  dummy arg");
        }
    }

    return result;
}
#endif





/*
 *
 *  Function: void *earSubnetMaskRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSubnetMask.
 *
 *      Chassis subnet mask
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
//unsigned long subnetMask = 0;
void *earSubnetMaskRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;

    /*
     * earSubnetMask(1.3.6.1.4.1.19775.1.1.1.16)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }
/*
    resultBuffer = askAtmelForIPAddressValue
        ("ZIMGETMASK",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4,
         &Mib_Leaves->earSubnetMask,
         10164
         );
*/
//    if ( resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earSubnetMask; //resultBuffer;
}





#if 0
/*
 *
 *  Function: int earGatewayWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earGateway.
 *
 *      Chassis default gateway
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
int earGatewayWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int SetTo;
    char buffer[MAX_FIELD_LEN];
    char chAddr[17];
    int status ;

    if ( info->setTo.intVal == info->val.intVal ) {
       return result;
    }

    if ( SNMP_SET_ACTION == actionCode ) {
        SetTo = info->setTo.intVal;
    } 
    if ( SNMP_SET_UNDO == actionCode ) {
        SetTo = info->val.intVal;
    }

    if ( (SNMP_SET_ACTION == actionCode) ||
         (SNMP_SET_UNDO == actionCode) ) {

        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10165);
        if ( TX_SUCCESS != status ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
            sprintf(chAddr, "%ld.%ld.%ld.%ld",
                    (SetTo >> 24) & 0xff,
                    (SetTo >> 16) & 0xff,
                    (SetTo >> 8) & 0xff,
                    SetTo & 0xff);

            sprintf(buffer, "ZIMSGATE %s\r", chAddr);
            printf(buffer);
            fflush(stdout);
            semaphore_put_2 (&semaphore_0_info, 10165);

            configNotification (EARMIB_earGateway, "FIXME:  dummy arg");
        }
    }

    return result;
}
#endif





/*
 *
 *  Function: void *earGatewayRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earGateway.
 *
 *      Chassis default gateway
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
//unsigned long gateway = 0;
void *earGatewayRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;

    /*
     * earGateway(1.3.6.1.4.1.19775.1.1.1.17)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }
/*
    resultBuffer = askAtmelForIPAddressValue
        ("ZIMGETGATEWAY",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4,
         &Mib_Leaves->earGateway,
         10169
         );
*/
//    if ( resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earGateway; //resultBuffer;
}






/*
 *
 *  Function: void *earVendorProductNameAndSerialNumberRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earVendorProductNameAndSerialNumber.
 *
 *      Vendors product name and serial number
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
char earVendorProductNameAndSerialNumberRead_buffer[MAX_2U_RESPONSE];
void *earVendorProductNameAndSerialNumberRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
	void *resultBuffer = &(earVendorProductNameAndSerialNumberRead_buffer[0]);
    int status;

    /*
     * earVendorProductNameAndSerialNumber(1.3.6.1.4.1.19775.1.1.1.18)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    strcpy(&(earVendorProductNameAndSerialNumberRead_buffer[0]),
           MOT_PROD_NAME);
    *varLen = sizeof (MOT_PROD_NAME);
    *setVar = (setMethod) vp->writeFn;
    return resultBuffer;
}


/*
 *
 *  Function: void *earLastTrapSentRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earLastTrapSent.
 *
 *      The earHistoryIndex of the last trap sent by the IPG EDFA device.
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
void *earLastTrapSentRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    static unsigned int historyKey;
    void *resultBuffer = &historyKey;

    /*
     * earLastTrapSent(1.3.6.1.4.1.19775.1.1.1.19)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    // since the index had already been incremented
    historyKey = manHistoryTableKey - 1;

    *varLen = sizeof(unsigned int);
    *setVar = (setMethod) vp->writeFn;
    return resultBuffer;
}






/*
 *
 *  Function: int earTrapPortWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earTrapPort.
 *
 *      SNMP Trap Port
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
/*   See other file */






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
 *      SNMP Trap Port
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
/*    See other file. */






/*
 *
 *  Function: int earSnmpMngrTableWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of a columnar object in the table
 *      earSnmpMngrTable.
 *
 *      The table of SNMP network managers.
 *
 *      The Fusion agent breaks up the set operation into
 *      four phases.  During each phase, this routine is
 *      called once for each columnar object in the row.
 *      So this routine will be called 40 times to write
 *      a new row that contains 10 columnar objects.  The
 *      current phase is indicated by the value of
 *      actionCode.  It can have these values:
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
 *      info->setTo will contain the new value for the field 
 *      currently being written.
 *      info->val will contain the original value of the field
 *      currently being written.
 *
 *  Parameters:
 *
 *      actionCode  indicates which phase of the write operation
 *                  is being requested.  
 *      info        pointer to structure with information about
 *                  the variable and the value it is being set
 *                  to.  The structure is defined in SNMPIMPL.H.
 *
 *  Variables:
 *
 *      snmpIndices     loaded with indices decoded from OID value
 *                      passed to this function
 *
 *      result          used to store function result
 *
 *      manInfo         pointer to management info about variable
 *
 *      oldIndex        index of row before set
 *
 *      newIndex        index of row after set
 *
 *      row             pointer to row buffer
 *
 *      isInsert        set if there is no row at oldIndex
 *
 *      didWrite        set once the new row has been written
 *
 *      startedUndo     set once we start the undo process
 *
 *      fieldCode       identifies which columnar object in the 
 *                      table is being read
 *
 *      lastActionCode  stores actionCode of previous call
 *
 *      fieldsCommitted used to track which columnar objects in the
 *                      table are being set.  Each bit represents one
 *                      columnar object
 *
 *      fieldsUndone    used to track which fields have been undone
 *
 *      ccode           used to hold return values from management API
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
/*    See other file */





/*
 *
 *  Function: void *earSnmpMngrTableRead (struct variable *vp,  
 *              oid *name, int *length, int isGet, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get a columnar object from earSnmpMngrTable.
 *
 *      The table of SNMP network managers.
 *
 *      The parameter name contains the OID of the object being read
 *      and a table index.  The parameter length indicates the 
 *      length of the OID and index.  If a GETNEXT operation returns
 *      an object at a different index, then the index information
 *      in name and length must be updated accordingly.
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *              The index for the table lookup is encoded
 *              after the OID of the object.  If the index
 *              of the object read is different than the
 *              index value in name must be updated
 *              accordingly on exit.
 *      length  length of OID with index.  If the index of
 *              the object read is different than what is
 *              specified on input, then the this field 
 *              should be updated with the new length.
 *      isGet   specifies whether the console wants a GET
 *              or GETNEXT.  If set, then console wants
 *              the object that exactly matches the index
 *              encoded in the OID (a GET).  If zero, then
 *              the console wants the first object with an
 *              index greater than the OID value in NAME.
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this table, or
 *              to NULL if the table is read only.
 *
 *  Variables:
 *
 *      resultBuffer    used to hold return value which must
 *                      point to a persistent buffer that
 *                      holds the value for the object
 *
 *      manInfo         points to management info for variable
 *
 *      ccode           used to hold return values from 
 *                      management API functions
 *
 *      snmpIndex       used to hold index extracted from
 *                      OID passed in name parameter
 *
 *      manIndex        index information for management API
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read the row
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */
/*    See other file */






/*
 *
 *  Function: int earGainControlSetpointWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earGainControlSetpoint.
 *
 *      Gain setpoint value
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
int earGainControlSetpointWrite (int actionCode, struct varBind *info)
{
    int status;
    int result = SNMP_ERR_NOERROR;
    int setpoint;
    float setpointF;
    static char buffer[MAX_FIELD_LEN];

    /*
     * earGainControlSetpoint(1.3.6.1.4.1.19775.1.1.2.1)
     */
    if ( actionCode == SNMP_SET_ACTION ) {
        setpoint = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        setpoint = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10412);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    setpointF = (float) setpoint;
	    setpointF /= 10.0f;
	    sprintf(buffer, "ZIMSGS %1.1f\r", setpointF);
	    printf("%s", buffer);
	    fflush(stdout);
            status = semaphore_put_2(&semaphore_0_info, 10412);
        }
    }

    return result;
}






/*
 *
 *  Function: void *earGainControlSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earGainControlSetpoint.
 *
 *      Gain setpoint value
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
void *earGainControlSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long gainValue;

    /*
     * earGainControlSetpoint(1.3.6.1.4.1.19775.1.1.2.1)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRGS",
         "ZRGS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries *
         &gainValue,
         10376); */
//    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earGainControlSetpoint; //resultBuffer;
}






/*
 *
 *  Function: void *earMeasuredGainValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earMeasuredGainValue.
 *
 *      The current Gain value
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
void *earMeasuredGainValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long gainValue;

    /*
     * earMeasuredGainValue(1.3.6.1.4.1.19775.1.1.2.2)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRMG",
         "ZRMG",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries *
         &gainValue,
         10374); */
//    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earMeasuredGainValue; //resultBuffer;
}






/*
 *
 *  Function: void *earMinimumGainSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earMinimumGainSetpoint.
 *
 *      The minimum gain that can be set in AGC mode
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
void *earMinimumGainSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long gainValue;

    /*
     * earMinimumGainSetpoint(1.3.6.1.4.1.19775.1.1.2.3)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRMINGAIN",
         "ZRMNG",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &gainValue,
         10370);
   if ( NULL != resultBuffer ) { */ 
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earMinimumGainSetpoint; //resultBuffer;
}






/*
 *
 *  Function: void *earMaximumGainSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earMaximumGainSetpoint.
 *
 *      The maximum gain that can be set in AGC mode
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
void *earMaximumGainSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long gainValue;

    /*
     * earMaximumGainSetpoint(1.3.6.1.4.1.19775.1.1.2.4)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRMAXGAIN",
         "ZRMXG",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &gainValue,
         10372);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earMaximumGainSetpoint; //resultBuffer;
}






/*
 *
 *  Function: int earOutputPowerSetpointValueWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earOutputPowerSetpointValue.
 *
 *      Output power setpoint value
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
int earOutputPowerSetpointValueWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;

    char buffer[MAX_FIELD_LEN];
    int powerSetpoint, status;
    float floatVal;
    float floatVal2;

    if ( actionCode == SNMP_SET_ACTION ) {
        powerSetpoint = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        powerSetpoint = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10190);
        if (status != TX_SUCCESS) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    floatVal = (float) powerSetpoint;
	    floatVal2 = (floatVal / 10.0f);
	    sprintf(buffer, "ZIMSOP %1.1f\r", floatVal2);
	    printf("%s", buffer);
	    fflush(stdout);

            status = semaphore_put_2 (&semaphore_0_info, 10191);
        }
        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earOutputPowerSetpointValue,
                                "FIXME:  dummy parm");
    }

    return result;
}






/*
 *
 *  Function: void *earOutputPowerSetpointValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerSetpointValue.
 *
 *      Output power setpoint value
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
void *earOutputPowerSetpointValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutPowerSetpointValue(1.3.6.1.4.1.19775.1.1.2.5)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }

/*    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPS",
         "ZRPS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10192);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earOutputPowerSetpointValue; //resultBuffer;
}






/*
 *
 *  Function: void *earTotalOpticalOutputPowerRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTotalOpticalOutputPower.
 *
 *      The total optical output power
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
void *earTotalOpticalOutputPowerRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earTotalOpticalOutputPower(1.3.6.1.4.1.19775.1.1.2.6)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMROP",
         "ZROP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10195);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earTotalOpticalOutputPower; //resultBuffer;
}






/*
 *
 *  Function: void *earMaximumPowerSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earMaximumPowerSetpoint.
 *
 *      Maximum power setpoint
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
void *earMaximumPowerSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earMaximumPowerSetpoint(1.3.6.1.4.1.19775.1.1.2.7)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRMP",
         "ZRMP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10198);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earMaximumPowerSetpoint; //resultBuffer;
}






/*
 *
 *  Function: int earCurrentSetpointValueWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earCurrentSetpointValue.
 *
 *      Current setpoint value
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
int earCurrentSetpointValueWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    char buffer[MAX_FIELD_LEN];
    int status;
    int currentSetpoint;
    float floatVal;

    if (actionCode == SNMP_SET_ACTION) {
	currentSetpoint = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
	currentSetpoint = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10118);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
            /* it gets multiplied again on Atmel side */
            floatVal = (float) currentSetpoint / 1000.0;

            sprintf (buffer, "ZIMSLC %1.1f\r", floatVal);
            printf ("%s", buffer);
            fflush (stdout);

            status = semaphore_put_2 (&semaphore_0_info, 10118);
        }
        /* setTo. has the new value - info. has the current value */
        if ( info->setTo.intVal != info->val.intVal ) {
            configNotification (EARMIB_earOutputPowerSetpointValue,
                                "FIXME:  dummy parm");
        }
    }

    return result;
}






/*
 *
 *  Function: void *earCurrentSetpointValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earCurrentSetpointValue.
 *
 *      Current setpoint value
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
void *earCurrentSetpointValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long currentValue;

    /*
     * earCurrentSetpointValue(1.3.6.1.4.1.19775.1.1.2.8)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRCS",
         "ZRCS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &currentValue,
         10378);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earCurrentSetpointValue; //resultBuffer;
}






/*
 *
 *  Function: void *earMaximumCurrentSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earMaximumCurrentSetpoint.
 *
 *      Maximum current setpoint
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
void *earMaximumCurrentSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long currentValue;

    /*
     * earMaximumCurrentSetpoint(1.3.6.1.4.1.19775.1.1.2.9)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRMC",
         "ZRMC",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &currentValue,
         10205);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earMaximumCurrentSetpoint; //resultBuffer;
}






/*
 *
 *  Function: void *earPumpLaserCurrentRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earPumpLaserCurrent.
 *
 *      Pump laser current
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
void *earPumpLaserCurrentRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long currentValue;

    /*
     * earPumpLaserCurrent(1.3.6.1.4.1.19775.1.1.2.10)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRLC",
         "ZRLC",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &currentValue,
         10214);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earPumpLaserCurrent; //resultBuffer;
}

/*
 *
 *  Function: void *earPumpLaserCurrent2Read (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earPumpLaserCurrent2.
 *
 *      Pump laser current
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
void *earPumpLaserCurrent2Read (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long currentValue;

    /*
     * earPumpLaserCurrent2(1.3.6.1.4.1.19775.1.1.2.38)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRLC2",
         "ZRLC2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &currentValue,
         10294);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//   }

    return &Mib_Leaves->earPumpLaserCurrent2; //resultBuffer;
}




/*
 *
 *  Function: void *earAmbientTemperatureRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earAmbientTemperature.
 *
 *      The ambient temperature
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
void *earAmbientTemperatureRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned long returnValue;
    static long tempValue;

    /*
     * earAmbientTemperature(1.3.6.1.4.1.19775.1.1.2.11)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRAT",
         "ZRAT",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &Mib_Leaves->earAmbientTemperature,
         10211); */
    /* a peculiarity of the way this value is stored */
    tempValue =  Mib_Leaves->earAmbientTemperature / 1000;
    /* move resultBuffer to point to the final result */
    resultBuffer = &tempValue;
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earTotalOpticalInputPowerRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTotalOpticalInputPower.
 *
 *      The total optical Input power
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
void *earTotalOpticalInputPowerRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earTotalOpticalInputPower(1.3.6.1.4.1.19775.1.1.2.12)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRIP",
         "ZRIP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10214);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    //}

    return &Mib_Leaves->earTotalOpticalInputPower; //resultBuffer;
}






/*
 *
 *  Function: int earPowerSafetyModeWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earPowerSafetyMode.
 *
 *      Output power is reduced to 15 dBm.
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
char earPowerSafetyModeWrite_buffer[MAX_2U_RESPONSE];
int earPowerSafetyModeWrite_length = MAX_2U_RESPONSE;
int earPowerSafetyModeWrite (int actionCode, struct varBind *info)
{
    char buffer[MAX_FIELD_LEN];
    float floatVal, floatVal2;
    int result = SNMP_ERR_NOERROR;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
    } else if ( actionCode == SNMP_SET_UNDO ) {
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10217);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
            alreadyREAD = FALSE;
            if (alreadyREAD == FALSE) {
		do {		// obtain present POUT value for restoration
		    printf("ZIMROP\r");
		    fflush(stdout);

		    readFrom2U2(&(earPowerSafetyModeWrite_buffer[0]),
                                &earPowerSafetyModeWrite_length);
		}
		while (((strncmp(&(earPowerSafetyModeWrite_buffer[0]),
                                 "ZROP", 4)))
		       || (!(strstr(&(earPowerSafetyModeWrite_buffer[0]),
                                    "PORZ"))));

		char opower[10], value[16];

		sscanf(&(earPowerSafetyModeWrite_buffer[0]),
                       "%s%s", opower, value);
		strcpy(&(earPowerSafetyModeWrite_buffer[0]), value);

		currentPowerValue = strtol(&(earPowerSafetyModeWrite_buffer[0]),
                                           ASCII_NULL, 0);
		alreadyREAD = TRUE;
	    }
	    // validate the key switch & interlock are enabled
	    if ( (!((MASK(ALARM_KEYSWITCH_FLAG)) & alarmsSet)) &&
                 (!((MASK(ALARM_INTERLOCK_FLAG)) & alarmsSet)) ) {
                // this is 15 dBm we're turning the pumps down
		floatVal = (float) lowPowerSetpoint;
		floatVal2 = (floatVal / 10.0f);
		sprintf(buffer, "ZIMSOP %1.1f\r", floatVal2);
		printf("%s", buffer);

		fflush(stdout);
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10218);
        }

        info->setTo.intVal = lowPowerSetpoint;

        // setTo. has the new value - val has the current value
        if ( info->setTo.intVal != info->val.intVal ) {
            configNotification (EARMIB_earPowerSafetyMode,
                                "FIXME:  dummy arg");

            // toggle between low power and preserved values
            // validate the key switch & interlock are enabled
            if ( count == 0 ) {
                if ( (!((MASK(ALARM_KEYSWITCH_FLAG)) & alarmsSet)) &&
                     (!((MASK(ALARM_INTERLOCK_FLAG)) & alarmsSet)) ) {
                    preserveLowPower = lowPowerSetpoint;
                    lowPowerSetpoint = currentPowerValue;
                    // for the Query results window display
                    info->setTo.intVal = preserveLowPower;
                    count++;
                } else
                    // for the Query results window display
                    info->setTo.intVal = currentPowerValue;
            } else {
                // validate the key switch & interlock are enabled
                if ( (!((MASK(ALARM_KEYSWITCH_FLAG)) & alarmsSet)) &&
                     (!((MASK(ALARM_INTERLOCK_FLAG)) & alarmsSet)) ) {
                    lowPowerSetpoint = preserveLowPower;
                    // for the Query results window display
                    info->setTo.intVal = currentPowerValue;
                    count--;
                } else
                    // for the Query results window display 
                    info->setTo.intVal = preserveLowPower;
            }
        }			// end setTo.intVal != val.intVal
    }			// SNMP ACTION or UNDO

    return result;
}






/*
 *
 *  Function: void *earPowerSafetyModeRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earPowerSafetyMode.
 *
 *      Output power is reduced to 15 dBm.
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
void *earPowerSafetyModeRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earPowerSafetyMode(1.3.6.1.4.1.19775.1.1.2.13)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPS",
         "ZRPS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10220);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earPowerSafetyMode; //resultBuffer;
}






/*
 *
 *  Function: int earPumpsOffWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earPumpsOff.
 *
 *      Output power is reduced to 0 mW.
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
/*    See other file */






/*
 *
 *  Function: void *earPumpsOffRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earPumpsOff.
 *
 *      Output power is reduced to 0 mW.
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
/*    See other file */






/*
 *
 *  Function: int earChassisRebootWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earChassisReboot.
 *
 *      Reset the processor running the chassis.
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

/*
int earChassisRebootWrite(int actionCode, struct varBind *info)
{

    typedef struct {
	unsigned long volatile *regaddr;	// Register address
	unsigned char bit;	// Bit mask for desired GPIO
    } GPIO_DESC;

    GPIO_DESC gpio[] = {
	{NARM_PORTC_ADDR, BIT_0},	// 0, 1, 2, 3
	{NARM_PORTC_ADDR, BIT_1},	// 4, 5, 6, 7           we're interested in 5
	{NARM_PORTC_ADDR, BIT_2},
	{NARM_PORTC_ADDR, BIT_3},
	{NARM_PORTC_ADDR, BIT_4},
	{NARM_PORTC_ADDR, BIT_5},	// 20, 21, 22, 23       we're interested in 21
	{NARM_PORTC_ADDR, BIT_6},
	{NARM_PORTC_ADDR, BIT_7},	// 28, 29, 30, 31       we're interested in 29
    };

    unsigned char pin5_data,
	pin21_dir, pin29_mode, bit5_data, bit21_dir, bit29_mode;

    int result = SNMP_ERR_NOERROR;
    int status;

    //
    // earChassisReboot(1.3.6.1.4.1.19775.1.1.2.15)
    //
    switch ( actionCode ) {

    case SNMP_SET_COMMIT:
	if (info->setTo.intVal != 0 ) { // interesting case 
	    // send out the trap now because next time around (SET_ACTION) we do it 
	    configNotification(EARMIB_earChassisReboot, "FIXME:  dummy");
	}
    break;

    case SNMP_SET_ACTION:
	if (info->setTo.intVal ==
	    0 ) { // only non-zero is interesting, meaning reset
	    return result;
	}

	// Lorin's (and Simon's) suggestion: make this longer 
	tx_thread_sleep(1000);	// let the message propagate

	// we're going to get serious now, so get the semaphore 
    // semaphore not needed (not talking to Atmel over serial port)
    // but I see there were timing issues in the step above so leave it in
    // --bjb
    
	status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 60606);
	if (status != TX_SUCCESS) {
	    return SNMP_ERR_RESOURCEUNAVAILABLE;
	}
	// since we're going to reboot, does it matter what the value in the MIB was?
    // this might affect the value seen in the returned SNMP packet 
	info->setTo.intVal = chassisReset;	// this is zero (0) as we're turning the pumps off
	chassisReset = info->setTo.intVal;


	bit5_data = narm_read_reg(NARM_PORTX_REG, gpio[5].regaddr, data);

	bit21_dir = narm_read_reg(NARM_PORTX_REG, gpio[5].regaddr, dir);

	bit29_mode = narm_read_reg(NARM_PORTX_REG, gpio[5].regaddr, mode);

	pin29_mode = narm_read_reg(NARM_PORTX_REG,
				   gpio[5].regaddr, mode) & gpio[5].bit;

	if (pin29_mode & 0x20)
	    bit29_mode ^= gpio[5].bit;	// toggle (clear) the MODE bit

	narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, mode, bit29_mode);



	pin21_dir = narm_read_reg(NARM_PORTX_REG,
				  gpio[5].regaddr, dir) & gpio[5].bit;

	if (pin21_dir == 0x0)
	    bit21_dir |= gpio[5].bit;	// toggle (set) the DIR bit

	narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, dir, bit21_dir);



	pin5_data = narm_read_reg(NARM_PORTX_REG,
				  gpio[5].regaddr, data) & gpio[5].bit;

	//if (pin5_data & 0x20)
	//   bit5_data ^= gpio[5].bit;                 // toggle (clear) the DATA bit
	// forget toggling, set it low
	bit5_data &= ~gpio[5].bit;

	narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, data, bit5_data);

	status = semaphore_put_2 (&semaphore_0_info, 60606);
        break;

    case SNMP_SET_RESERVE:
    case SNMP_SET_FREE:
    case SNMP_SET_UNDO:
    default:
        break;
    }
    return result;
}

*/




/*
 *
 *  Function: void *earChassisRebootRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earChassisReboot.
 *
 *      Reset the processor running the chassis.
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
/*
void *earChassisRebootRead(struct variable *vp, oid * name, int *length,
			   int isGet, int *varLen, setMethod * setVar)
{
    static INT32 result = 0;
    void* resultBuffer = (void *) &result;
	//void *resultBuffer = NULL;
    //
    // earChassisReboot(1.3.6.1.4.1.19775.1.1.2.15)
    //
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    *varLen = sizeof (unsigned long);
    *setVar = (setMethod) vp->writeFn;

    return resultBuffer;
}

*/




/*
 *
 *  Function: void *earOutputPowerALSonFDSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerALSonFDSetpoint.
 *
 *      Default Laser 1 power setpoint for ALS on mode.
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
void *earOutputPowerALSonFDSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    static unsigned long powerValue;
    void *resultBuffer = NULL;

    /*
     * earOutputPowerALSonFDSetpoint(1.3.6.1.4.1.19775.1.1.2.16)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGFSALSONSET",
         "ZGFSALSONSET",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10350);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2ALSonFDSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2ALSonFDSetpoint.
 *
 *      Default Laser 2 power setpoint for ALS on mode.
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
void *earOutputPower2ALSonFDSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    static unsigned long powerValue;
    void *resultBuffer = NULL;

    /*
     * earOutputPower2ALSonFDSetpoint(1.3.6.1.4.1.19775.1.1.2.17)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGFSALSONSET2",
         // the following is due to a bug in the atmel; this is same as
         // user setting ZIMGALSONSET2 command (not factory default)
         //"ZGFSALSONSET2",
         "ZGALSONSET2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries
         &powerValue,
         10352);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPowerALSoffFDSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerALSoffFDSetpoint.
 *
 *      Default Laser 1 power setpoint for ALS off mode.
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
void *earOutputPowerALSoffFDSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /* 
     * earOutputPowerALSoffFDSetpoint(1.3.6.1.4.1.19775.1.1.2.18)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGFSALSOFFSET",
         "ZGFSALSOFFSET",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10354);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2ALSoffFDSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2ALSoffFDSetpoint.
 *
 *      Default Laser 2 power setpoint for ALS off mode.
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
void *earOutputPower2ALSoffFDSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2ALSoffFDSetpoint(1.3.6.1.4.1.19775.1.1.2.19)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGFSALSOFFSET2",
         "ZGFSALSOFFSET2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10356);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPowerALSonSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerALSonSetpoint.
 *
 *      Laser 1 power setpoint for ALS on mode.
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
void *earOutputPowerALSonSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPowerALSonSetpoint(1.3.6.1.4.1.19775.1.1.2.20)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGALSONSET",
         "ZGALSONSET",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10358);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2ALSonSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2ALSonSetpoint.
 *
 *      Laser 2 power setpoint for ALS on mode.
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
void *earOutputPower2ALSonSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2ALSonSetpoint(1.3.6.1.4.1.19775.1.1.2.21)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGALSONSET2",
         "ZGALSONSET2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10360);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPowerALSoffSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerALSoffSetpoint.
 *
 *      Laser 1 power setpoint for ALS off mode.
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
void *earOutputPowerALSoffSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPowerALSoffSetpoint(1.3.6.1.4.1.19775.1.1.2.22)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGALSOFFSET",
         "ZGALSOFFSET",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10362);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2ALSoffSetpointRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2ALSoffSetpoint.
 *
 *      Laser 2 power setpoint for ALS off mode.
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
void *earOutputPower2ALSoffSetpointRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2ALSoffSetpoint(1.3.6.1.4.1.19775.1.1.2.23)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMGALSOFFSET2",
         "ZGALSOFFSET2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
         10364);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earControlModeFDRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earControlModeFD.
 *
 *      Default Control Mode
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
static int earControlModeFD_value;
void *earControlModeFDRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned char buffer_[MAX_2U_RESPONSE];
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earControlModeFD(1.3.6.1.4.1.19775.1.1.2.24)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForStringValue
        ("ZIMRFDM",
         "ZRFDM",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &(buffer_[0]),
         &bufferLength,
         10366);

    if ( NULL != resultBuffer ) {
        // this can be a switch because the initial letter
        // of each possibility is different
        // 
        switch ( buffer_[0] ) {
        case ATMEL_CONTROLMODE_POWER:
            earControlModeFD_value = EARMIB_earControlMode_power;
            break;
        case ATMEL_CONTROLMODE_CURRENT:
            earControlModeFD_value = EARMIB_earControlMode_current;
            break;
        case ATMEL_CONTROLMODE_GAIN:
            earControlModeFD_value = EARMIB_earControlMode_gain;
            break;
        default:
            // MIB should probably be extended to have UNKNOWN 
            earControlModeFD_value = 4;
            break;
        } */

        resultBuffer = &Mib_Leaves->earControlModeFD;
        *varLen = sizeof (int);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earDeviceTypeRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDeviceType.
 *
 *      Device type legacy(1), master(2), slave(3)
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
static int earDeviceTypeRead_value;
void *earDeviceTypeRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned char buffer_[MAX_2U_RESPONSE];
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earDeviceType(1.3.6.1.4.1.19775.1.1.2.25)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForStringValue
        ("ZIMRDEVTYP",
         "ZRDEVTYP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &(buffer_[0]),
         &bufferLength,
         10380);
    if ( NULL != resultBuffer ) {
        switch ( buffer_[0] ) {
        case ATMEL_DEVICETYPE_LEGACY:
            earDeviceTypeRead_value = EARMIB_earDeviceType_legacy;
            break;
        case ATMEL_DEVICETYPE_MASTER:
            earDeviceTypeRead_value = EARMIB_earDeviceType_master;
            break;
        case ATMEL_DEVICETYPE_SLAVE:
            earDeviceTypeRead_value = EARMIB_earDeviceType_slave;
            break;
        default:
            earDeviceTypeRead_value = 4;
            break;
        } */
        resultBuffer = &Mib_Leaves->earDeviceType;
        *varLen = sizeof (int);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earDeviceTypeFDRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDeviceTypeFD.
 *
 *      Default device type legacy(1), master(2), slave(3)
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
int earDeviceTypeFD_value;
void *earDeviceTypeFDRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned char buffer_[MAX_2U_RESPONSE];
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earDeviceTypeFD(1.3.6.1.4.1.19775.1.1.2.26)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForStringValue
        ("ZIMRDEVTYPFD",
         "ZRDEVTYPFD",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &(buffer_[0]),
         &bufferLength,
         10382);
    if ( NULL != resultBuffer ) {
        switch ( buffer_[0] ) {
        case ATMEL_DEVICETYPE_LEGACY:
            earDeviceTypeFD_value = EARMIB_earDeviceTypeFD_legacy;
            break;
        case ATMEL_DEVICETYPE_MASTER:
            earDeviceTypeFD_value = EARMIB_earDeviceTypeFD_master;
            break;
        case ATMEL_DEVICETYPE_SLAVE:
            earDeviceTypeFD_value = EARMIB_earDeviceTypeFD_slave;
            break;
        default:
            earDeviceTypeFD_value = 4;
            break;
        } */
        resultBuffer = &Mib_Leaves->earDeviceTypeFD;
        *varLen = sizeof (int);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earALSonDurationRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earALSonDuration.
 *
 *      Default duration in milli-seconds for ALS on
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
void *earALSonDurationRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long duration;

    /*
     * earALSonDuration(1.3.6.1.4.1.19775.1.1.2.27)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRALSONTIME",
         "ZRALSONTIME",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &duration,
         10384); */
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earALSoffDurationRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earALSoffDuration.
 *
 *      Default duration in milli-seconds for ALS off
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
void *earALSoffDurationRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long duration;

    /*
     * earALSoffDuration(1.3.6.1.4.1.19775.1.1.2.28)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRALSOFFTIME",
         "ZRALSOFFTIME",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &duration,
         10386); */
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }

    return resultBuffer;
}



/*
 *
 *  Function: void *earNominalDurationRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earNominalDuration.
 *
 *      Default duration in milli-seconds for Nominal (s/b non-zero)
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
void *earNominalDurationRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long duration;

    /*
     * earNominalDuration(1.3.6.1.4.1.19775.1.1.2.29)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRALSNOMTIME",
         "ZRALSNOMTIME",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &duration,
         10388); */
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earDataModeRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earDataMode.
 *
 *      Whether payload data is flowing off(no) ALS(no) Nominal(yes)
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
unsigned long earDataModeRead_value;
void *earDataModeRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned char buffer_[MAX_2U_RESPONSE];
    unsigned long bufferLength = MAX_2U_RESPONSE;

    /*
     * earDataMode(1.3.6.1.4.1.19775.1.1.2.30)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForStringValue
        ("ZIMRDATAMODE",
         "ZRDATAMODE",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &(buffer_[0]),
         &bufferLength,
         10390); 
    if ( NULL != resultBuffer ) {
        switch ( buffer_[0] ) {
        case ATMEL_DATAMODE_OFF:
            earDataModeRead_value = EARMIB_earDataMode_off;
            break;
        case ATMEL_DATAMODE_ALS:
            earDataModeRead_value = EARMIB_earDataMode_als;
            break;
        case ATMEL_DATAMODE_NOMINAL:
            earDataModeRead_value = EARMIB_earDataMode_nominal;
            break;
        default:
            earDataModeRead_value = 4;
            break;
        } */

        resultBuffer = (void *) &Mib_Leaves->earDataMode;
        *varLen = sizeof (unsigned int);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: int earOSCPowerSetpointValueWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earOSCPowerSetpointValue.
 *
 *      Pusk power setpoint in dBm
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
int earOSCPowerSetpointValueWrite (int actionCode, struct varBind *info)
{
    int status;
    int result = SNMP_ERR_NOERROR;
    static char buffer[MAX_FIELD_LEN];
    int setpoint;
    float setpointF;

    /*
     * earOSCPowerSetpointValue(1.3.6.1.4.1.19775.1.1.2.31)
     */
    if ( SNMP_SET_ACTION == actionCode ) {
        setpoint = info->setTo.intVal;
    }
    if ( SNMP_SET_UNDO == actionCode ) {
        setpoint = info->val.intVal;
    }

    if ( (SNMP_SET_ACTION == actionCode) || (SNMP_SET_UNDO == actionCode) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10410);
        if (status != TX_SUCCESS) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {

            setpointF = (float) setpoint;
            setpointF /= 10.0f;
            sprintf (buffer, "ZIMSPUSKSP %1.1f\r", setpointF);
            printf ("%s", buffer);
            fflush (stdout); 
            status = semaphore_put_2 (&semaphore_0_info, 10410);
        }
    }

    return result;
}






/*
 *
 *  Function: void *earOSCPowerSetpointValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOSCPowerSetpointValue.
 *
 *      Pusk power setpoint in dBm
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
void *earOSCPowerSetpointValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOSCPowerSetpointValue(1.3.6.1.4.1.19775.1.1.2.31)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPUSKSP",
         "ZRPUSKSP",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10364); */
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }

    return resultBuffer;
}






/*
 *
 *  Function: int earOutputPower2SetpointValueWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earOutputPower2SetpointValue.
 *
 *      Laser 2 output power setpoint value.
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
int earOutputPower2SetpointValueWrite (int actionCode, struct varBind *info)
{
    int status;
    int result = SNMP_ERR_NOERROR;
    static char buffer[MAX_FIELD_LEN];
    int setpoint;
    float setpointF;

    /*
     * earOutputPower2SetpointValue(1.3.6.1.4.1.19775.1.1.2.32)
     */
    if ( SNMP_SET_ACTION == actionCode ) {
        setpoint = info->setTo.intVal;
    }
    if ( SNMP_SET_UNDO == actionCode ) {
        setpoint = info->val.intVal;
    }

    if ( (SNMP_SET_ACTION == actionCode) || (SNMP_SET_UNDO == actionCode) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10411);
        if (status != TX_SUCCESS) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {

            setpointF = (float) setpoint;
            setpointF /= 10.0f;
            sprintf (buffer, "ZIMSSP2 %1.1f\r", setpointF);
            printf ("%s", buffer);
            fflush (stdout); 
            status = semaphore_put_2 (&semaphore_0_info, 10411);
        }
    }

    return result;
}






/*
 *
 *  Function: void *earOutputPower2SetpointValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2SetpointValue.
 *
 *      Laser 2 output power setpoint value
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
void *earOutputPower2SetpointValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2SetpointValue(1.3.6.1.4.1.19775.1.1.2.32)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRSP2",
         "ZRSP2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, // retries 
         &powerValue,
    	 10394);
 
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earOutputPower2SetpointValue; //resultBuffer;
}






/*
 *
 *  Function: void *earTotalOpticalOutputPower2Read (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTotalOpticalOutputPower2.
 *
 *      Laser 2 total optical output power
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
void *earTotalOpticalOutputPower2Read (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earTotalOpticalOutputPower2(1.3.6.1.4.1.19775.1.1.2.33)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*/
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMROP2",
         "ZROP2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10396);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earTotalOpticalOutputPower2; //resultBuffer;
}






/*
 *
 *  Function: int earGainControlSetpoint2Write (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earGainControlSetpoint2.
 *
 *      Laser 2 gain setpoint value
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
int earGainControlSetpoint2Write (int actionCode, struct varBind *info)
{
    int status;
    int result = SNMP_ERR_NOERROR;
    int setpoint;
    float setpointF;
    static char buffer[MAX_FIELD_LEN];

    /*
     * earGainControlSetpoint2(1.3.6.1.4.1.19775.1.1.2.34)
     */
    if ( actionCode == SNMP_SET_ACTION ) {
        setpoint = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        setpoint = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10414);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    setpointF = (float) setpoint;
	    setpointF /= 10.0f;
	    sprintf(buffer, "ZIMSGS2 %1.1f\r", setpointF);
	    printf("%s", buffer);
	    fflush(stdout);
            status = semaphore_put_2 (&semaphore_0_info, 10414);
        }
    }

    return result;
}






/*
 *
 *  Function: void *earGainControlSetpoint2Read (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earGainControlSetpoint2.
 *
 *      Laser 2 gain setpoint value
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
void *earGainControlSetpoint2Read (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long gainValue;

    /*
     * earGainControlSetpoint2(1.3.6.1.4.1.19775.1.1.2.34)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRGS2",
         "ZRGS2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &gainValue,
         10398);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earGainControlSetpoint2; //resultBuffer;
}



/*
 *
 *  Function: void *earOSCOpticalPowerRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOSCOpticalPower.
 *
 *      Pusk measured output power
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
void *earOSCOpticalPowerRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOSCOpticalPower(1.3.6.1.4.1.19775.1.1.2.35)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMPUSKPOWER",
         "ZPUSKPOWER",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10400); */
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }

    return resultBuffer;
}



/*
 *
 *  Function: int earAliveTrapPeriodValueWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earAliveTrapPeriodValue.
 *
 *      I am Alive Trap Period value.
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
int earAliveTrapPeriodValueWrite (int actionCode, struct varBind *info)
{
    int status;
    int result = SNMP_ERR_NOERROR;
    unsigned int setperiod;
	char buffer [MAX_FIELD_LEN];
    /*
     * earAliveTrapPeriod(1.3.6.1.4.1.19775.1.1.2.36)
     */
    if ( SNMP_SET_ACTION == actionCode ) {
        setperiod = info->setTo.intVal;
    }
    if ( SNMP_SET_UNDO == actionCode ) {
        setperiod = info->val.intVal;
    }

    if ( (SNMP_SET_ACTION == actionCode) || (SNMP_SET_UNDO == actionCode) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 82966);
        if (status != TX_SUCCESS) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
            if ((setperiod == 0) || ((setperiod >= SNMP_SEND_ALIVE_TRAP_PERIOD_MIN) &&
                 (setperiod <= SNMP_SEND_ALIVE_TRAP_PERIOD_MAX))) {
				if ((!AliveTimerNVRamSave))
				{
					g_snmp_send_alive_trap_period  = setperiod;
					TrapPeriod = setperiod;
				}	
				else
				{
				   sprintf(buffer, "ZIMSALIVETIMER %d\r", setperiod);
				   printf("%s", buffer);
				   fflush(stdout);
				}
               status = semaphore_put_2 (&semaphore_0_info, 82966);
			}
			else {
			result = SNMP_ERR_BADVALUE;
			}
        }
    }

    return result;
}


/*
 *
 *  Function: void *earAliveTrapPeriodValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earAliveTrapPeriodValue.
 *
 *  	Read I am Alive Trap Period value.
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
void *earAliveTrapPeriodValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    unsigned int periodValue;

    /*
     * earAliveTrapPeriod(1.3.6.1.4.1.19775.1.1.2.36)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
    resultBuffer = &g_snmp_send_alive_trap_period;
    *varLen = sizeof (unsigned int);
    *setVar = (setMethod) vp->writeFn;

    return (resultBuffer);
}

/*
 *
 *  Function: void *earTotalOpticalInput2PowerRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTotalOpticalInput2Power.
 *
 *      The total optical Input power
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
void *earTotalOpticalInput2PowerRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earTotalOpticalInput2Power(1.3.6.1.4.1.19775.1.1.2.37)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRIP2",
         "ZRIP2",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10214);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earTotalOpticalInput2Power; //resultBuffer;
}


/*
 *
 *  Function: int earTemperatureAlarmThresholdWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *  	Set the value of earTemperature
 *  	Threshold.
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
/*    See other file */






/*
 *
 *  Function: void *earTemperatureAlarmThresholdRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earTemperatureAlarmThreshold.
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
/*    See other file */






/*
 *
 *  Function: int earInputPowerLowAlarmLevelWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earInputPowerLowAlarmLevel.
 *
 *      The INPUT POWER LOW alarm level.  Overrides factory 
 *       default setting
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
int earInputPowerLowAlarmLevelWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;

    int inputPower, status;
    float floatVal1;
    float floatVal2;
    char buffer[MAX_FIELD_LEN];

    /*
     * earInputPowerLowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.2)
     */
    if ( actionCode == SNMP_SET_ACTION ) {
        inputPower = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        inputPower = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10222);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    floatVal1 = (float) inputPower;
	    floatVal2 = (floatVal1 / 10.0f);
	    sprintf(buffer, "ZIMSPINLOW %1.1f\r", floatVal2);
	    printf("%s", buffer);
	    fflush(stdout);
            status = semaphore_put_2 (&semaphore_0_info, 10223);

            // setTo. has the new value - info. has the current value
	    if ( info->setTo.intVal != info->val.intVal )
		configNotification (EARMIB_earInputPowerLowAlarmLevel,
                                    "FIXME:  dummy parm");
        }
    }

    return result;
}

/*
 *
 *  Function: int earInput2PowerLowAlarmLevelWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earInput2PowerLowAlarmLevel.
 *
 *      The INPUT2 POWER LOW alarm level.  Overrides factory 
 *       default setting
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
int earInput2PowerLowAlarmLevelWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;

    int inputPower2, status;
    float floatVal1;
    float floatVal2;
    char buffer[MAX_FIELD_LEN];

    /*
     * earInput2PowerLowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.28)
     */
    if ( actionCode == SNMP_SET_ACTION ) {
        inputPower2 = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        inputPower2 = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10292);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    floatVal1 = (float) inputPower2;
	    floatVal2 = (floatVal1 / 10.0f);
	    sprintf(buffer, "ZIMSPIN2LOW %1.1f\r", floatVal2);
	    printf("%s", buffer);
	    fflush(stdout);
            status = semaphore_put_2 (&semaphore_0_info, 10292);

            // setTo. has the new value - info. has the current value
	    if ( info->setTo.intVal != info->val.intVal )
		configNotification (EARMIB_earInput2PowerLowAlarmLevel,
                                    "FIXME:  dummy parm");
        }
    }

    return result;
}

/*
 *
 *  Function: void *earInputPowerLowAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInputPowerLowAlarmLevel.
 *
 *      The INPUT POWER LOW alarm level.  Overrides factory 
 *       default setting
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
void *earInputPowerLowAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInputPowerLowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.2)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPINLOW",
         "ZRPNL",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10224);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInputPowerLowAlarmLevel; //resultBuffer;
}

/*
 *
 *  Function: void *earInput2PowerLowAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInput2PowerLowAlarmLevel.
 *
 *      The INPUT2 POWER LOW alarm level.  Overrides factory 
 *       default setting
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
void *earInput2PowerLowAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInput2PowerLowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.28)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPIN2LOW",
         "ZRPN2L",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10288);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInput2PowerLowAlarmLevel; //resultBuffer;
}




/*
 *
 *  Function: void *earFactoryDefaultInputPowerLowAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earFactoryDefaultInputPowerLowAlarmLevel.
 *
 *      Reads the factory default setting for the INPUT POWER LOW  
 *       alarm level
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
void *earFactoryDefaultInputPowerLowAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earFactoryDefaultInputPowerLowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.3)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRFDPINLOW",
         "ZRDPNL",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10227);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earFactoryDefaultInputPowerLowAlarmLevel;   //resultBuffer;
}






/*
 *
 *  Function: void *earInputPowerOffAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInputPowerOffAlarmLevel.
 *
 *      The INPUT POWER OFF alarm level
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
void *earInputPowerOffAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInputPowerOffAlarmLevel(1.3.6.1.4.1.19775.1.1.3.4)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPINOFF",
         "ZRPNF",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10230);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInputPowerOffAlarmLevel; //resultBuffer;
}

/*
 *
 *  Function: void *earInput2PowerOffAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInputPowerOffAlarmLevel.
 *
 *      The INPUT2 POWER OFF alarm level
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
void *earInput2PowerOffAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInput2PowerOffAlarmLevel(1.3.6.1.4.1.19775.1.1.3.29)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPIN2OFF",
         "ZRPN2F",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10289);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInput2PowerOffAlarmLevel; //resultBuffer;
}






/*
 *
 *  Function: void *earInputPowerHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInputPowerHighAlarmLevel.
 *
 *      The INPUT POWER HIGH alarm level
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
void *earInputPowerHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInputPowerHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.5)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPINHIGH",
         "ZRPNH",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries
         &powerValue,
         10234);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInputPowerHighAlarmLevel; //resultBuffer;
}

/*
 *
 *  Function: void *earInput2PowerHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInput2PowerHighAlarmLevel.
 *
 *      The INPUT2 POWER HIGH alarm level
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
void *earInput2PowerHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInput2PowerHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.30)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPIN2HIGH",
         "ZRPN2H",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10290);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInput2PowerHighAlarmLevel; //resultBuffer;
}




/*
 *
 *  Function: void *earInputPowerTooHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInputPowerTooHighAlarmLevel.
 *
 *      The INPUT POWER TOO HIGH alarm level
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
void *earInputPowerTooHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInputPowerTooHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.6)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPINTOOHIGH",
         "ZRPN2H",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10237);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
 //   }

    return &Mib_Leaves->earInputPowerTooHighAlarmLevel; //resultBuffer;
}

/*
 *
 *  Function: void *earInput2PowerTooHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earInput2PowerTooHighAlarmLevel.
 *
 *      The INPUT2 POWER TOO HIGH alarm level
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
void *earInput2PowerTooHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earInput2PowerTooHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.31)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPIN2TOOHIGH",
         "ZRPN22H",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10291);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earInput2PowerTooHighAlarmLevel; //resultBuffer;
}




/*
 *
 *  Function: void *earOutputPowerLowAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerLowAlarmLevel.
 *
 *      The threshold level for the OUTPUT POWER LOW alarm
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
void *earOutputPowerLowAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPowerLowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.7)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUTLOW",
         "ZRPOL",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10240);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPowerOffAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerOffAlarmLevel.
 *
 *      The threshold level for the OUTPUT POWER OFF alarm
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
void *earOutputPowerOffAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPowerOffAlarmLevel(1.3.6.1.4.1.19775.1.1.3.8)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUTOFF",
         "ZRPOF",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10243);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPowerHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerHighAlarmLevel.
 *
 *      The OUTPUT POWER HIGH alarm level
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
void *earOutputPowerHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPowerHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.9)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUTHIGH",
         "ZRPOH",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10246);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPowerTooHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPowerTooHighAlarmLevel.
 *
 *      The OUTPUT POWER TOO HIGH alarm level
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
void *earOutputPowerTooHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPowerTooHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.10)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/* 
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUTTOOHIGH",
         "ZRPO2H",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10249);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return resultBuffer;
}






/*
 *
 *  Function: void *earStatusValueRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earStatusValue.
 *
 *      The status of the unit encoded as a 32 bit integer
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
void *earStatusValueRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long statusValue;

    /*
     * earStatusValue(1.3.6.1.4.1.19775.1.1.3.11)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMSTA",
         "ZRSTA",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &statusValue,
         10252);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earStatusValue; //resultBuffer;
}






/*
 *
 *  Function: void *earRelayAlarmStatusRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earRelayAlarmStatus.
 *
 *      The alarms that are assigned to trip the alarm relay.   
 *       This is an encoded word
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
void *earRelayAlarmStatusRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long statusValue;

    /*
     * earRelayAlarmStatus(1.3.6.1.4.1.19775.1.1.3.12)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRRAS",
         "ZRRAS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &statusValue,
         10255);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earRelayAlarmStatus; //resultBuffer;
}






/*
 *
 *  Function: void *earRelayStatusRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earRelayStatus.
 *
 *      The state of the relay, NORMAL or TRIPPED
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
void *earRelayStatusRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long statusValue;

    /*
     * earRelayStatus(1.3.6.1.4.1.19775.1.1.3.13)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRRS",
         "ZRRS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &statusValue,
         10259);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earRelayStatus; //resultBuffer;
}






/*
 *
 *  Function: void *earAlarmStatusRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earAlarmStatus.
 *
 *      The individual alarm conditions within the laser.  This is 
 *       an encoded word
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
void *earAlarmStatusRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long statusValue;

    /*
     * earAlarmStatus(1.3.6.1.4.1.19775.1.1.3.14)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRAL",
         "ZRAS",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &statusValue,
         10262);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earAlarmStatus; //resultBuffer;
}






/*
 *
 *  Function: int earSetRelayInputPowerLowAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayInputPowerLowAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayInputPowerLowAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10266);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRAPINLOW 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRAPINLOW 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10267);
        }

        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earSetRelayInputPowerLowAlarm,
                                "FIXME:  dummy parm");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayInputPowerLowAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayInputPowerLowAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayInputPowerLowAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayInputPowerLowAlarm(1.3.6.1.4.1.19775.1.1.3.15)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayInputPowerHighAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayInputPowerHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayInputPowerHighAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10268);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRAPINHIGH 0\r");
		fflush(stdout);
	    case ENABLE_RELAY:
		printf("ZIMSRAPINHIGH 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10269);
        }

        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earSetRelayInputPowerHighAlarm,
                                "FIXME:  dummy parm");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayInputPowerHighAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayInputPowerHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayInputPowerHighAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayInputPowerHighAlarm(1.3.6.1.4.1.19775.1.1.3.16)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayInputPowerTooHighAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayInputPowerTooHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayInputPowerTooHighAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10270);
        if (status != TX_SUCCESS) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRAPINTOOHIGH 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRAPINTOOHIGH 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10271);
        }
        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification(EARMIB_earSetRelayInputPowerTooHighAlarm,
                               "FIXME:  dummy parm");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayInputPowerTooHighAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayInputPowerTooHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayInputPowerTooHighAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayInputPowerTooHighAlarm(1.3.6.1.4.1.19775.1.1.3.17)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayOutputPowerLowAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayOutputPowerLowAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayOutputPowerLowAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10272);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {

	    switch ( relayState ) {
	    case DISABLE_RELAY:
		printf("ZIMSRAPOUTLOW 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRAPOUTLOW 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10273);
        }

        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earSetRelayOutputPowerLowAlarm,
                                "FIXME:  dummy");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayOutputPowerLowAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayOutputPowerLowAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayOutputPowerLowAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayOutputPowerLowAlarm(1.3.6.1.4.1.19775.1.1.3.18)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayOutputPowerHighAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayOutputPowerHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayOutputPowerHighAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if (actionCode == SNMP_SET_UNDO) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10274);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRAPOUTHIGH 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRAPOUTHIGH 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10275);
        }

        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal)
            configNotification (EARMIB_earSetRelayOutputPowerHighAlarm,
                                "FIXME:  dummy arg");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayOutputPowerHighAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayOutputPowerHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayOutputPowerHighAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayOutputPowerHighAlarm(1.3.6.1.4.1.19775.1.1.3.19)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayOutputPowerTooHighAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayOutputPowerTooHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayOutputPowerTooHighAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;
    char buffer[MAX_FIELD_LEN];

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10276);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRAPOUTTOOHIGH 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRAPOUTTOOHIGH 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10277);
        }
        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earSetRelayOutputPowerTooHighAlarm,
                                "FIXME:  dummy arg");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayOutputPowerTooHighAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayOutputPowerTooHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayOutputPowerTooHighAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayOutputPowerTooHighAlarm(1.3.6.1.4.1.19775.1.1.3.20)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayFanFailureAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayFanFailureAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayFanFailureAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10278);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRAFAN 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRAFAN 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10279);
        }
        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earSetRelayFanFailureAlarm,
                                "FIXME:  dummy arg");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayFanFailureAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayFanFailureAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayFanFailureAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayFanFailureAlarm(1.3.6.1.4.1.19775.1.1.3.21)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: int earSetRelayAmbientTemperatureHighAlarmWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earSetRelayAmbientTemperatureHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
int earSetRelayAmbientTemperatureHighAlarmWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    int relayState;
    int status;

    if ( actionCode == SNMP_SET_ACTION ) {
        relayState = info->setTo.intVal;
    } else if ( actionCode == SNMP_SET_UNDO ) {
        relayState = info->val.intVal;
    }

    if ( (actionCode == SNMP_SET_ACTION) || (actionCode == SNMP_SET_UNDO) ) {
        status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 10280);
        if ( status != TX_SUCCESS ) {
            result = SNMP_ERR_RESOURCEUNAVAILABLE;
        } else {
	    switch (relayState) {
	    case DISABLE_RELAY:
		printf("ZIMSRATEMP 0\r");
		fflush(stdout);
		break;
	    case ENABLE_RELAY:
		printf("ZIMSRATEMP 1\r");
		fflush(stdout);
		break;
	    }

            status = semaphore_put_2 (&semaphore_0_info, 10281);
        }
        // setTo. has the new value - info. has the current value
        if ( info->setTo.intVal != info->val.intVal )
            configNotification (EARMIB_earSetRelayAmbientTemperatureHighAlarm,
                                "FIXME:  dummy arg");
    }

    return result;
}






/*
 *
 *  Function: void *earSetRelayAmbientTemperatureHighAlarmRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earSetRelayAmbientTemperatureHighAlarm.
 *
 *      Enables or disables activation of the relay for the alarm
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
void *earSetRelayAmbientTemperatureHighAlarmRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long value;

    /*
     * earSetRelayAmbientTemperatureHighAlarm(1.3.6.1.4.1.19775.1.1.3.22)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
#if 0
    value = 0;
    resultBuffer = &(value);
    if ( NULL != resultBuffer ) {
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
    }
#endif
    return resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2OffAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2OffAlarmLevel.
 *
 *      The threshold level for the 2nd laser OUTPUT POWER OFF alarm
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
void *earOutputPower2OffAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2OffAlarmLevel(1.3.6.1.4.1.19775.1.1.3.23)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUT2OFF",
         "ZRPOUT2OFF",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10402);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//   }

    return &Mib_Leaves->earOutputPower2OffAlarmLevel; //resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2LowAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2LowAlarmLevel.
 *
 *      The threshold level for the 2nd laser OUTPUT POWER LOW alarm
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
void *earOutputPower2LowAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2LowAlarmLevel(1.3.6.1.4.1.19775.1.1.3.24)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUT2LOW",
         "ZRPOUT2LOW",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10404);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earOutputPower2LowAlarmLevel; //resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2HighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2HighAlarmLevel.
 *
 *      The threshold level for the 2nd laser OUTPUT POWER HIGH alarm
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
void *earOutputPower2HighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2HighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.25)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUT2HIGH",
         "ZRPOUT2HIGH",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10406);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earOutputPower2HighAlarmLevel; //resultBuffer;
}






/*
 *
 *  Function: void *earOutputPower2TooHighAlarmLevelRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earOutputPower2TooHighAlarmLevel.
 *
 *      The threshold level for the 2nd laser OUTPUT POWER TOO HIGH alarm
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
void *earOutputPower2TooHighAlarmLevelRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    static unsigned long powerValue;

    /*
     * earOutputPower2TooHighAlarmLevel(1.3.6.1.4.1.19775.1.1.3.26)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return resultBuffer;
    }
/*
    resultBuffer = askAtmelForUnsignedValue
        ("ZIMRPOUT2TOOHIGH",
         "ZRPOUT2TOOHIGH",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, /* retries 
         &powerValue,
         10408);
    if ( NULL != resultBuffer ) { */
        *varLen = sizeof (unsigned long);
        *setVar = (setMethod) vp->writeFn;
//    }

    return &Mib_Leaves->earOutputPower2TooHighAlarmLevel; //resultBuffer;
}






/*
 *
 *  Function: int earAlarmTableWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of a columnar object in the table
 *      earAlarmTable.
 *
 *      Table for current alarm conditions present on the system
 *
 *      The Fusion agent breaks up the set operation into
 *      four phases.  During each phase, this routine is
 *      called once for each columnar object in the row.
 *      So this routine will be called 40 times to write
 *      a new row that contains 10 columnar objects.  The
 *      current phase is indicated by the value of
 *      actionCode.  It can have these values:
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
 *      info->setTo will contain the new value for the field 
 *      currently being written.
 *      info->val will contain the original value of the field
 *      currently being written.
 *
 *  Parameters:
 *
 *      actionCode  indicates which phase of the write operation
 *                  is being requested.  
 *      info        pointer to structure with information about
 *                  the variable and the value it is being set
 *                  to.  The structure is defined in SNMPIMPL.H.
 *
 *  Variables:
 *
 *      snmpIndices     loaded with indices decoded from OID value
 *                      passed to this function
 *
 *      result          used to store function result
 *
 *      manInfo         pointer to management info about variable
 *
 *      oldIndex        index of row before set
 *
 *      newIndex        index of row after set
 *
 *      row             pointer to row buffer
 *
 *      isInsert        set if there is no row at oldIndex
 *
 *      didWrite        set once the new row has been written
 *
 *      startedUndo     set once we start the undo process
 *
 *      fieldCode       identifies which columnar object in the 
 *                      table is being read
 *
 *      lastActionCode  stores actionCode of previous call
 *
 *      fieldsCommitted used to track which columnar objects in the
 *                      table are being set.  Each bit represents one
 *                      columnar object
 *
 *      fieldsUndone    used to track which fields have been undone
 *
 *      ccode           used to hold return values from management API
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
int earAlarmTableWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    snmpIndexType *snmpIndex = snmpExtractIndices(info->vp, info->oid, info->oidLen, IS_WRITE, 13);
    manVarType *manInfo = snmpGetVariableInfo(info->vp);
    manTableIndexType oldIndex;
    static manTableIndexType newIndex;
    static earAlarmTableType *row = NULL;
    static int isInsert = 0, didWrite = 0, startedUndo = 0;
    int fieldCode = snmpGetFieldCode(info->vp);
    static int lastActionCode = -1;
    static WORD32 fieldsCommitted[1];
    static WORD32 fieldsUndone[1];
    MAN_ERROR_TYPE ccode;
    int status;

    /*
     * The raw SNMP indices are stored in snmpIndex.  The
     * algorithm for using these indices should be described
     * somewhere in the MIB's RFC.
     *
     * To Do:  Write code to initialize oldIndex.
     */

    /* semaphore_0 probably not needed - is another semaphore appropriate? */
#if 0
    status = semaphore_get(&semaphore_0, SEMAPHORE_TMO, 10305);
    if (status != TX_SUCCESS) {
	return 0;
    } else {
#endif
	oldIndex.wantExact = 1;	/* always use exact index for writes */

	switch (actionCode) {
	case SNMP_SET_RESERVE:
	    if (row == NULL) {
		memset (fieldsCommitted, 0, sizeof (WORD32));
                memset (fieldsUndone, 0, sizeof (WORD32));
		isInsert = 0;
		didWrite = 0;
		startedUndo = 0;
		result =
		    snmpInitRow(manInfo, &oldIndex,
				sizeof(earAlarmTableType), &isInsert,
				(void **) &row);
	    }
	    if (result == SNMP_ERR_NOERROR) {
		result = snmpAllocateFieldBuffer(actionCode, info, row);
	    }
	    break;
	case SNMP_SET_COMMIT:
	    result = snmpSetField(actionCode, info, row);
	    fieldsCommitted[fieldCode / 32] |= 1 << (fieldCode & 0x1f);
	    break;
	case SNMP_SET_ACTION:
	    if (lastActionCode == SNMP_SET_COMMIT) {
		/*
		 * To Do:  The array fieldsCommitted will indicate which fields
		 *         the console has provided values for.  Set default values
		 *         for any fields that are missing, and verify that the row
		 *         contains valid data and can be written into the table.
		 */
		if (isInsert) {
		    memcpy(&newIndex, &oldIndex, sizeof (manTableIndexType));
		    ccode =
			manInsertSnmpRow(manInfo->id, &newIndex, row,
					 SEMAPHORE_TMO
					 /*MAN_TIMEOUT_FOREVER */ );
		} else {
		    /*
		     * To Do:  Initialize newIndex to indicate the row's new
		     *         position in the table.
		     */
		    newIndex.wantExact = 1;

		    ccode =
			manSetSnmpRow(manInfo->id, &oldIndex, &newIndex,
				      row, SEMAPHORE_TMO
				      /*MAN_TIMEOUT_FOREVER */ );
		}
		if (ccode == MAN_SUCCESS) {
		    didWrite = 1;
		}
		result = snmpErrorLookup[ccode];
	    }
	    break;
	case SNMP_SET_FREE:
	    if (row != NULL) {
		/*
		 * Free our buffers.  This is only done on the first FREE call.
		 */
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
		row = NULL;
	    }
	    break;
	case SNMP_SET_UNDO:
	    if ((row != NULL) && (!startedUndo)) {
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
		row = NULL;
	    }
	    if (didWrite) {
		if (isInsert) {
		    ccode =
			manDeleteSnmpRow(manInfo->id, &newIndex,
					 SEMAPHORE_TMO
					 /*MAN_TIMEOUT_FOREVER */ );
		    if (ccode != MAN_SUCCESS) {
			result = SNMP_ERR_UNDOFAILED;
		    }
		    didWrite = FALSE;
		} else {
		    if (!startedUndo) {
			result =
			    snmpInitRow(manInfo, &newIndex,
					sizeof(earAlarmTableType), NULL,
					(void **) &row);
			startedUndo = 1;
		    }
		    if (result == SNMP_ERR_NOERROR) {
			result =
			    snmpAllocateFieldBuffer(actionCode, info, row);
		    }
		    if (result != SNMP_ERR_NOERROR) {
			snmpFreeOctetStringBuffers(manInfo, row);
			free(row);
			row = NULL;
			result = SNMP_ERR_UNDOFAILED;
			break;
		    }
		    snmpSetField(actionCode, info, row);
		    fieldsUndone[fieldCode / 32] |=
			1 << (fieldCode & 0x1f);
		    if (memcmp
			(fieldsCommitted, fieldsUndone,
			 sizeof(WORD32)) == 0) {
			ccode =
			    manSetSnmpRow(manInfo->id, &newIndex,
					  &oldIndex, row, SEMAPHORE_TMO
					  /*MAN_TIMEOUT_FOREVER */ );
			if (ccode != MAN_SUCCESS) {
			    result = SNMP_ERR_UNDOFAILED;
			}
			snmpFreeOctetStringBuffers(manInfo, row);
			free(row);
			row = NULL;
			startedUndo = 0;
			didWrite = 0;
		    }
		}
	    }
	    break;
	}
	lastActionCode = actionCode;

	snmpFreeIndices(snmpIndex);
#if 0
	status = semaphore_put(&semaphore_0, 10306);
#endif
	return result;
#if 0
    }				// end else - semaphore obtained
#endif
}






/*
 *
 *  Function: void *earAlarmTableRead (struct variable *vp,  
 *              oid *name, int *length, int isGet, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get a columnar object from earAlarmTable.
 *
 *      Table for current alarm conditions present on the system
 *
 *      The parameter name contains the OID of the object being read
 *      and a table index.  The parameter length indicates the 
 *      length of the OID and index.  If a GETNEXT operation returns
 *      an object at a different index, then the index information
 *      in name and length must be updated accordingly.
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *              The index for the table lookup is encoded
 *              after the OID of the object.  If the index
 *              of the object read is different than the
 *              index value in name must be updated
 *              accordingly on exit.
 *      length  length of OID with index.  If the index of
 *              the object read is different than what is
 *              specified on input, then the this field 
 *              should be updated with the new length.
 *      isGet   specifies whether the console wants a GET
 *              or GETNEXT.  If set, then console wants
 *              the object that exactly matches the index
 *              encoded in the OID (a GET).  If zero, then
 *              the console wants the first object with an
 *              index greater than the OID value in NAME.
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this table, or
 *              to NULL if the table is read only.
 *
 *  Variables:
 *
 *      resultBuffer    used to hold return value which must
 *                      point to a persistent buffer that
 *                      holds the value for the object
 *
 *      manInfo         points to management info for variable
 *
 *      ccode           used to hold return values from 
 *                      management API functions
 *
 *      snmpIndex       used to hold index extracted from
 *                      OID passed in name parameter
 *
 *      manIndex        index information for management API
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read the row
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */
void *earAlarmTableRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    manVarType *manInfo = snmpGetVariableInfo(vp);
    MAN_ERROR_TYPE ccode;
    snmpIndexType *snmpIndex = snmpExtractIndices(vp, name, *length, IS_READ, 13);

    *varLen = 0;
    int status;
#if 0
    status = semaphore_get(&semaphore_0, SEMAPHORE_TMO, 10307);
    if (status != TX_SUCCESS) {
	return NULL;
    } else {
#endif
	if (snmpIndex != NULL) {
	    manTableIndexType manIndex;	// index for management API
	    earAlarmTableType *row;	// storage for one row from table
	    row = (earAlarmTableType *) malloc (sizeof (earAlarmTableType));


	    if (row != NULL) {
		memset(row, 0, sizeof (earAlarmTableType));

		manIndex.wantExact = isGet;
		if (snmpIndex->isNullIndex) {
		    manIndex.numericIndex = 0;
		    manIndex.snmpIndex = NULL;
		} else {
		    // The raw SNMP indices are stored in snmpIndex.  The algorithm for using these 
		    // indices should be described somewhere in the MIB's RFC.  This table uses a 
		    // simple integer index.  Increment the index if this is a GET-NEXT.  Then set 
		    // oldIndex.snmpIndex to point to it.
		    if (!isGet) {
			snmpIndex->index->value.intValue++;
		    }
		    manIndex.snmpIndex =
			(void *) &snmpIndex->index->value.intValue;
		}
		ccode = snmpReadRow(manInfo, &manIndex, row);
		if (ccode == MAN_SUCCESS) {
		    resultBuffer = snmpExtractField(vp, row, varLen, NULL);

		    // Copy row index into snmpIndex->index->value.intValue.
		    snmpIndex->index->value.intValue = row->earAlarmIndex;
		    memcpy(name, vp->name, vp->namelen * sizeof (WORD32));
		    *length = snmpEncodeIndices(vp, name, snmpIndex);

		    if (snmpFreeBufferLater(resultBuffer) !=
			SNMP_ERR_NOERROR) {
			free(resultBuffer);
			resultBuffer = NULL;
			*varLen = 0;
		    }
		}
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
	    } else {		/* if unable to allocate memory for row buffer */

		resultBuffer = NULL;
		*varLen = 0;
	    }
	    snmpFreeIndices(snmpIndex);
	}

	*setVar = (setMethod) vp->writeFn;
#if 0
	status = semaphore_put(&semaphore_0, 10308);
#endif
	return resultBuffer;
#if 0
    }				// end else - semaphore obtained
#endif
}






/*
 *
 *  Function: int earEventTableWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of a columnar object in the table
 *      earEventTable.
 *
 *      Table for current events present on the system
 *
 *      The Fusion agent breaks up the set operation into
 *      four phases.  During each phase, this routine is
 *      called once for each columnar object in the row.
 *      So this routine will be called 40 times to write
 *      a new row that contains 10 columnar objects.  The
 *      current phase is indicated by the value of
 *      actionCode.  It can have these values:
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
 *      info->setTo will contain the new value for the field 
 *      currently being written.
 *      info->val will contain the original value of the field
 *      currently being written.
 *
 *  Parameters:
 *
 *      actionCode  indicates which phase of the write operation
 *                  is being requested.  
 *      info        pointer to structure with information about
 *                  the variable and the value it is being set
 *                  to.  The structure is defined in SNMPIMPL.H.
 *
 *  Variables:
 *
 *      snmpIndices     loaded with indices decoded from OID value
 *                      passed to this function
 *
 *      result          used to store function result
 *
 *      manInfo         pointer to management info about variable
 *
 *      oldIndex        index of row before set
 *
 *      newIndex        index of row after set
 *
 *      row             pointer to row buffer
 *
 *      isInsert        set if there is no row at oldIndex
 *
 *      didWrite        set once the new row has been written
 *
 *      startedUndo     set once we start the undo process
 *
 *      fieldCode       identifies which columnar object in the 
 *                      table is being read
 *
 *      lastActionCode  stores actionCode of previous call
 *
 *      fieldsCommitted used to track which columnar objects in the
 *                      table are being set.  Each bit represents one
 *                      columnar object
 *
 *      fieldsUndone    used to track which fields have been undone
 *
 *      ccode           used to hold return values from management API
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
/* ?   */






/*
 *
 *  Function: void *earEventTableRead (struct variable *vp,  
 *              oid *name, int *length, int isGet, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get a columnar object from earEventTable.
 *
 *      Table for current events present on the system
 *
 *      The parameter name contains the OID of the object being read
 *      and a table index.  The parameter length indicates the 
 *      length of the OID and index.  If a GETNEXT operation returns
 *      an object at a different index, then the index information
 *      in name and length must be updated accordingly.
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *              The index for the table lookup is encoded
 *              after the OID of the object.  If the index
 *              of the object read is different than the
 *              index value in name must be updated
 *              accordingly on exit.
 *      length  length of OID with index.  If the index of
 *              the object read is different than what is
 *              specified on input, then the this field 
 *              should be updated with the new length.
 *      isGet   specifies whether the console wants a GET
 *              or GETNEXT.  If set, then console wants
 *              the object that exactly matches the index
 *              encoded in the OID (a GET).  If zero, then
 *              the console wants the first object with an
 *              index greater than the OID value in NAME.
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this table, or
 *              to NULL if the table is read only.
 *
 *  Variables:
 *
 *      resultBuffer    used to hold return value which must
 *                      point to a persistent buffer that
 *                      holds the value for the object
 *
 *      manInfo         points to management info for variable
 *
 *      ccode           used to hold return values from 
 *                      management API functions
 *
 *      snmpIndex       used to hold index extracted from
 *                      OID passed in name parameter
 *
 *      manIndex        index information for management API
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read the row
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */
void *earEventTableRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    manVarType *manInfo = snmpGetVariableInfo(vp);
    MAN_ERROR_TYPE ccode;
    snmpIndexType *snmpIndex = snmpExtractIndices(vp, name, *length, IS_READ, 13);

    *varLen = 0;
    int status;
#if 0
    status = semaphore_get(&semaphore_0, SEMAPHORE_TMO, 10309);
    if (status != TX_SUCCESS) {
	return NULL;
    } else {
#endif
	if (snmpIndex != NULL) {
	    manTableIndexType manIndex;	// index for management API 
	    earEventTableType *row;	// storage for one row from table
	    row = (earEventTableType *) malloc (sizeof (earEventTableType));


	    if (row != NULL) {
		memset(row, 0, sizeof (earEventTableType));

		manIndex.wantExact = isGet;
		if (snmpIndex->isNullIndex) {
		    manIndex.numericIndex = 0;
		    manIndex.snmpIndex = NULL;
		} else {
		    // The raw SNMP indices are stored in snmpIndex.  The algorithm for using 
		    // these indices should be described somewhere in the MIB's RFC.  This 
		    // table uses a simple integer index.  Increment the index if this is a 
		    // GET-NEXT.  Then set  oldIndex.snmpIndex to point to it.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~         Be sure to add this section!!!
		    if (!isGet) {
			snmpIndex->index->value.intValue++;
		    }
		    manIndex.snmpIndex =
			(void *) &snmpIndex->index->value.intValue;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		}
		ccode = snmpReadRow(manInfo, &manIndex, row);
		if (ccode == MAN_SUCCESS) {
		    resultBuffer = snmpExtractField(vp, row, varLen, NULL);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~         Be sure to add this section!!!
		    // Copy row index into snmpIndex->index->value.intValue.
		    snmpIndex->index->value.intValue = row->earEventIndex;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		    memcpy(name, vp->name, vp->namelen * sizeof (WORD32));
		    *length = snmpEncodeIndices(vp, name, snmpIndex);

		    if (snmpFreeBufferLater(resultBuffer) !=
			SNMP_ERR_NOERROR) {
			free(resultBuffer);
			resultBuffer = NULL;
			*varLen = 0;
		    }
		}
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
	    } else {		/* if unable to allocate memory for row buffer */

		resultBuffer = NULL;
		*varLen = 0;
	    }
	    snmpFreeIndices(snmpIndex);
	}
	*setVar = (setMethod) vp->writeFn;
#if 0
	status = semaphore_put(&semaphore_0, 10310);
#endif
	return resultBuffer;
#if 0
    }				// end else - semaphore obtained
#endif
}






/*
 *
 *  Function: int earHistoryTableWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of a columnar object in the table
 *      earHistoryTable.
 *
 *      Table for history of events which have occurred on the system
 *
 *      The Fusion agent breaks up the set operation into
 *      four phases.  During each phase, this routine is
 *      called once for each columnar object in the row.
 *      So this routine will be called 40 times to write
 *      a new row that contains 10 columnar objects.  The
 *      current phase is indicated by the value of
 *      actionCode.  It can have these values:
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
 *      info->setTo will contain the new value for the field 
 *      currently being written.
 *      info->val will contain the original value of the field
 *      currently being written.
 *
 *  Parameters:
 *
 *      actionCode  indicates which phase of the write operation
 *                  is being requested.  
 *      info        pointer to structure with information about
 *                  the variable and the value it is being set
 *                  to.  The structure is defined in SNMPIMPL.H.
 *
 *  Variables:
 *
 *      snmpIndices     loaded with indices decoded from OID value
 *                      passed to this function
 *
 *      result          used to store function result
 *
 *      manInfo         pointer to management info about variable
 *
 *      oldIndex        index of row before set
 *
 *      newIndex        index of row after set
 *
 *      row             pointer to row buffer
 *
 *      isInsert        set if there is no row at oldIndex
 *
 *      didWrite        set once the new row has been written
 *
 *      startedUndo     set once we start the undo process
 *
 *      fieldCode       identifies which columnar object in the 
 *                      table is being read
 *
 *      lastActionCode  stores actionCode of previous call
 *
 *      fieldsCommitted used to track which columnar objects in the
 *                      table are being set.  Each bit represents one
 *                      columnar object
 *
 *      fieldsUndone    used to track which fields have been undone
 *
 *      ccode           used to hold return values from management API
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
int earHistoryTableWrite (int actionCode, struct varBind *info)
{
    int result = SNMP_ERR_NOERROR;
    manTableIndexType oldIndex;
    static manTableIndexType newIndex;
    static earHistoryTableType *row = NULL;
    static int isInsert = 0, didWrite = 0, startedUndo = 0;
    int fieldCode = snmpGetFieldCode(info->vp);
    static int lastActionCode = -1;
    static WORD32 fieldsCommitted[1];
    static WORD32 fieldsUndone[1];
    MAN_ERROR_TYPE ccode;
    int status;
    snmpIndexType *snmpIndex
	/* = snmpExtractIndices(info->vp, info->oid, info->oidLen, IS_WRITE, 13) */
    ;
    manVarType *manInfo = snmpGetVariableInfo(info->vp);

    /*
     * The raw SNMP indices are stored in snmpIndex.  The
     * algorithm for using these indices should be described
     * somewhere in the MIB's RFC.
     *
     * To Do:  Write code to initialize oldIndex.
     */
#if 0
    status = semaphore_get(&semaphore_0, SEMAPHORE_TMO, 10311);	/*why are we not using the history table semaphore? */
    if (status != TX_SUCCESS) {
	return 0;
    } else {
#endif
	snmpIndex =
	    snmpExtractIndices(info->vp, info->oid, info->oidLen, IS_WRITE,
			       13);
	oldIndex.wantExact = 1;	/* always use exact index for writes */

	switch (actionCode) {
	case SNMP_SET_RESERVE:
	    if (row == NULL) {
		memset(fieldsCommitted, 0, sizeof (WORD32));
		memset(fieldsUndone, 0, sizeof(WORD32));
		isInsert = 0;
		didWrite = 0;
		startedUndo = 0;
		result =
		    snmpInitRow(manInfo, &oldIndex,
				sizeof (earHistoryTableType), &isInsert,
				(void **) &row);
	    }
	    if (result == SNMP_ERR_NOERROR) {
		result = snmpAllocateFieldBuffer(actionCode, info, row);
	    }
	    break;
	case SNMP_SET_COMMIT:
	    result = snmpSetField(actionCode, info, row);
	    fieldsCommitted[fieldCode / 32] |= 1 << (fieldCode & 0x1f);
	    break;
	case SNMP_SET_ACTION:
	    if (lastActionCode == SNMP_SET_COMMIT) {
		/*
		 * To Do:  The array fieldsCommitted will indicate which fields
		 *         the console has provided values for.  Set default values
		 *         for any fields that are missing, and verify that the row
		 *         contains valid data and can be written into the table.
		 */
		if (isInsert) {
		    memcpy(&newIndex, &oldIndex, sizeof (manTableIndexType));
		    ccode =
			manInsertSnmpRow(manInfo->id, &newIndex, row,
					 SEMAPHORE_TMO
					 /*MAN_TIMEOUT_FOREVER */ );
		} else {
		    /*
		     * To Do:  Initialize newIndex to indicate the row's new
		     *         position in the table.
		     */
		    newIndex.wantExact = 1;

		    ccode =
			manSetSnmpRow(manInfo->id, &oldIndex, &newIndex,
				      row, SEMAPHORE_TMO
				      /*MAN_TIMEOUT_FOREVER */ );
		}
		if (ccode == MAN_SUCCESS) {
		    didWrite = 1;
		}
		result = snmpErrorLookup[ccode];
	    }
	    break;
	case SNMP_SET_FREE:
	    if (row != NULL) {
		/*
		 * Free our buffers.  This is only done on the first FREE call.
		 */
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
		row = NULL;
	    }
	    break;
	case SNMP_SET_UNDO:
	    if ((row != NULL) && (!startedUndo)) {
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
		row = NULL;
	    }
	    if (didWrite) {
		if (isInsert) {
		    ccode =
			manDeleteSnmpRow(manInfo->id, &newIndex,
					 SEMAPHORE_TMO
					 /*MAN_TIMEOUT_FOREVER */ );
		    if (ccode != MAN_SUCCESS) {
			result = SNMP_ERR_UNDOFAILED;
		    }
		    didWrite = FALSE;
		} else {
		    if (!startedUndo) {
			result =
			    snmpInitRow(manInfo, &newIndex,
					sizeof (earHistoryTableType), NULL,
					(void **) &row);
			startedUndo = 1;
		    }
		    if (result == SNMP_ERR_NOERROR) {
			result =
			    snmpAllocateFieldBuffer(actionCode, info, row);
		    }
		    if (result != SNMP_ERR_NOERROR) {
			snmpFreeOctetStringBuffers(manInfo, row);
			free(row);
			row = NULL;
			result = SNMP_ERR_UNDOFAILED;
			break;
		    }
		    snmpSetField(actionCode, info, row);
		    fieldsUndone[fieldCode / 32] |=
			1 << (fieldCode & 0x1f);
		    if (memcmp
			(fieldsCommitted, fieldsUndone,
			 sizeof (WORD32)) == 0) {
			ccode =
			    manSetSnmpRow(manInfo->id, &newIndex,
					  &oldIndex, row, SEMAPHORE_TMO
					  /*MAN_TIMEOUT_FOREVER */ );
			if (ccode != MAN_SUCCESS) {
			    result = SNMP_ERR_UNDOFAILED;
			}
			snmpFreeOctetStringBuffers(manInfo, row);
			free(row);
			row = NULL;
			startedUndo = 0;
			didWrite = 0;
		    }
		}
	    }
	    break;
	}
	lastActionCode = actionCode;

	snmpFreeIndices(snmpIndex);
#if 0
	status = semaphore_put(&semaphore_0, 10312);
#endif
	return result;
#if 0
    }				// end else - semaphore obtained
#endif
}






/*
 *
 *  Function: void *earHistoryTableRead (struct variable *vp,  
 *              oid *name, int *length, int isGet, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get a columnar object from earHistoryTable.
 *
 *      Table for history of events which have occurred on the system
 *
 *      The parameter name contains the OID of the object being read
 *      and a table index.  The parameter length indicates the 
 *      length of the OID and index.  If a GETNEXT operation returns
 *      an object at a different index, then the index information
 *      in name and length must be updated accordingly.
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *              The index for the table lookup is encoded
 *              after the OID of the object.  If the index
 *              of the object read is different than the
 *              index value in name must be updated
 *              accordingly on exit.
 *      length  length of OID with index.  If the index of
 *              the object read is different than what is
 *              specified on input, then the this field 
 *              should be updated with the new length.
 *      isGet   specifies whether the console wants a GET
 *              or GETNEXT.  If set, then console wants
 *              the object that exactly matches the index
 *              encoded in the OID (a GET).  If zero, then
 *              the console wants the first object with an
 *              index greater than the OID value in NAME.
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this table, or
 *              to NULL if the table is read only.
 *
 *  Variables:
 *
 *      resultBuffer    used to hold return value which must
 *                      point to a persistent buffer that
 *                      holds the value for the object
 *
 *      manInfo         points to management info for variable
 *
 *      ccode           used to hold return values from 
 *                      management API functions
 *
 *      snmpIndex       used to hold index extracted from
 *                      OID passed in name parameter
 *
 *      manIndex        index information for management API
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read the row
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */
void *earHistoryTableRead (struct variable *vp, oid *name, int *length,
            int isGet, int *varLen, setMethod *setVar)
{
    void *resultBuffer = NULL;
    MAN_ERROR_TYPE ccode;
    int status;

    *varLen = 0;
    manTableIndexType manIndex;	/* index for management API */
    earHistoryTableType *row;	/* storage for one row from table */
    manVarType *manInfo = snmpGetVariableInfo(vp);
    snmpIndexType *snmpIndex =
	snmpExtractIndices(vp, name, *length, IS_READ, 13);
#if 0
    status = semaphore_get(&semaphore_0, SEMAPHORE_TMO, 10313);
    if (status != TX_SUCCESS) {
	return NULL;
    } else {
#endif

	if (snmpIndex != NULL) {
	    row = (earHistoryTableType *)
		malloc (sizeof (earHistoryTableType));

	    if (row != NULL) {
		memset (row, 0, sizeof (earHistoryTableType));
		manIndex.wantExact = isGet;

		if (snmpIndex->isNullIndex) {
		    manIndex.numericIndex = 0;
		    manIndex.snmpIndex = NULL;
		} else {

		    // The raw SNMP indices are stored in snmpIndex.  The algorithm for using 
		    // these indices should be described somewhere in the MIB's RFC.  This 
		    // table uses a simple integer index.  Increment the index if this is a 
		    // GET-NEXT.  Then set  oldIndex.snmpIndex to point to it.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~         Be sure to add this section!!!
		    if (!isGet) {
			snmpIndex->index->value.intValue++;
		    }
		    manIndex.snmpIndex =
			(void *) &snmpIndex->index->value.intValue;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		}

		ccode = snmpReadRow(manInfo, &manIndex, row);
		if (ccode == MAN_SUCCESS) {
		    resultBuffer = snmpExtractField(vp, row, varLen, NULL);
		    // Copy row index into snmpIndex->index->value.intValue.
		    snmpIndex->index->value.intValue =
			row->earHistoryIndex;

		    memcpy(name, vp->name, vp->namelen * sizeof(WORD32));

		    *length = snmpEncodeIndices(vp, name, snmpIndex);

		    if (snmpFreeBufferLater(resultBuffer) !=
			SNMP_ERR_NOERROR) {
			free(resultBuffer);
			resultBuffer = NULL;
			*varLen = 0;
		    }
		}		// ccode MAN_SUCCESS
		snmpFreeOctetStringBuffers(manInfo, row);
		free(row);
	    }			// row != NULL
	    else		// if no memory for row buffer
	    {
		resultBuffer = NULL;
		*varLen = 0;
	    }
	    snmpFreeIndices(snmpIndex);
	}

	*setVar = (setMethod) vp->writeFn;
#if 0
	status = semaphore_put(&semaphore_0, 10314);
#endif
	return resultBuffer;
#if 0
    }
#endif

}


