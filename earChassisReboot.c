/*
 * $Id: earChassisReboot.c,v 1.2 2011-10-11 13:47:36 hazem Exp $
 *
 *
 *  Function: int earChassisRebootWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of earChassisReboot.
 *
 *      Reset the processor runing the chassis.
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
 */


#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "tx_api.h"
#include "reg_def.h"
#include "man_api.h"
#include "asn1.h"

#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"

#include "snmpapi.h"
#include "snmpvars.h"

#include "MibMan.h"
#include "EARMIB.h"

#include "semaphores.h"

extern unsigned short chassisReset /* = 1 */ ;


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

    //unsigned char pin5_data,
	//pin21_dir, pin29_mode, bit5_data, bit21_dir, bit29_mode;

    int result = SNMP_ERR_NOERROR;
    int status;

    /*
     * earChassisReboot(1.3.6.1.4.1.19775.1.1.2.15)
     */
    switch ( actionCode ) {

    case SNMP_SET_COMMIT:
	if (info->setTo.intVal != 0 /* interesting case */ ) {
	    /* send out the trap now because next time around (SET_ACTION) we do it */
	    configNotification(EARMIB_earChassisReboot, "FIXME:  dummy");
	}
    break;

    case SNMP_SET_ACTION:
	if (info->setTo.intVal ==
	    0 /* only non-zero is interesting, meaning reset */ ) {
	    return result;
	}

	/* Lorin's (and Simon's) suggestion: make this longer */
	tx_thread_sleep(200);	/* let the message propagate */

	/* we're going to get serious now, so get the semaphore */
        /* semaphore not needed (not talking to Atmel over serial port)
         * but I see there were timing issues in the step above so leave it in
         * --bjb
         */
	status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 60606);
	if (status != TX_SUCCESS) {
	    return SNMP_ERR_RESOURCEUNAVAILABLE;
	}
	// since we're going to reboot, does it matter what the value in the MIB was?
        /* this might affect the value seen in the returned SNMP packet */
	info->setTo.intVal = chassisReset;	// this is zero (0) as we're turning the pumps off
	chassisReset = info->setTo.intVal;
    
	 // Initialyze GPIO PORTC5 to Output and set it low for reset
     narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, mode,
                  narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, mode) & ~gpio[5].bit);
     narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, dir,
                  narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, dir) | gpio[5].bit);
     narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, reserved,
                  narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, reserved) & ~gpio[5].bit);
	 narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, data, 
			narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, data) & ~gpio[5].bit);

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






//------------------------------------------------------------------------------------------

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
 *      Reset the processor runing the chassis.
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
void *earChassisRebootRead(struct variable *vp, oid * name, int *length,
			   int isGet, int *varLen, setMethod * setVar)
{
    static INT32 result = 0;
    void* resultBuffer = (void *) &result;

    /*
     * earChassisReboot(1.3.6.1.4.1.19775.1.1.2.15)
     */
    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    *varLen = sizeof (unsigned long);
    *setVar = (setMethod) earChassisRebootWrite; //vp->writeFn;

    return resultBuffer;
}
