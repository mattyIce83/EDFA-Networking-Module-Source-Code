/*
 * DO NOT MODIFY THIS FILE.  IT WAS GENERATED BY MIBMAN AND MAY BE REGENERATED LATER.
 */

/*
 *     Module Name: MibMan.c
 *	       Version: 1.00
 *    Generated On: $Date: 2011-10-11 13:56:13 $
 *	        Author: MibMan Utility
 *	      Language: Ansi C
 * Compile Options:
 * Compile defines:
 *	     Libraries:
 *    Link Options:
 *
 *    Entry Points:
 *
 *
 * Description.
 * =======================================================================
 *  This C file contains routines used to initializes the C variables
 *  and structured created by MibMan to represent MIB variables.  The routine
 *  snmpAgentRegisterWithManagementApi() needs to be called after the SNMP
 *  agent is started in order to register the MIB variables with both the
 *  management API and with the SNMP agent.
 *
 *
 * Edit Date/Ver   Edit Description
 * ==============  =======================================================
 *
 *
 */

#include <asn1.h>
#include <snmpimpl.h>
#include <snmpvars.h>


/*
 * The blink counts below can be overridden with compile time defines.
 */
#ifndef RED_BLINKS
    #define RED_BLINKS      10
#endif

#ifndef GREEN_BLINKS
    #define GREEN_BLINKS    2
#endif
#include "MibMan.h"
#include "EARMIB.h"









/*
 *
 *  Function: static void handleError (char *mibName)
 *
 *  Description:
 *
 *      Crash the system if we can't register Mib variables with the 
 *      management API.
 *
 *  Parameters:
 *
 *      mibName         name of MIB we encountered the problem with
 *
 *  Return Values:
 *
 *      does not return
 *
 */

static void handleError (char *mibName)
{
    char buffer[200];

    sprintf (buffer, "Error registering MIB %s with the management API.", mibName);
    netosFatalError (buffer, RED_BLINKS, GREEN_BLINKS);
}












/*
 *
 *  Function: static void handleSnmpError (char *mibName)
 *
 *  Description:
 *
 *      Crash the system if we can't register Mib variables with the 
 *      SNMP agent.
 *
 *  Parameters:
 *
 *      mibName         name of MIB we encountered the problem with
 *
 *  Return Values:
 *
 *      does not return
 *
 */

static void handleSnmpError (char *mibName)
{
    char buffer[200];

    sprintf (buffer, "Error registering MIB %s with the SNMP agent.", mibName);
    netosFatalError(buffer, RED_BLINKS, GREEN_BLINKS);
}












/*
 *
 *  Function: static void handleSetError(MAN_ID_TYPE id, MAN_ERROR_TYPE ccode)
 *
 *  Description:
 *
 *      Crash the system if we cannot initialize a Mib object.
 *
 *  Parameters:
 *
 *      id          management API ID of the MIB object
 *      ccode       completion code of the set operation
 *
 *  Return Values:
 *
 *      none
 *
 */

static void handleSetError(MAN_ID_TYPE id, MAN_ERROR_TYPE ccode)
{
    if (ccode != MAN_SUCCESS)
    {
        char buffer[200];

        sprintf (buffer, "Error initializing management variable %s.", id);
        netosFatalError(buffer, RED_BLINKS, GREEN_BLINKS);
    }
}












/*
 *
 *  Function: MAN_ERROR_TYPE setOctetString(MAN_ID_TYPE id, unsigned char *stringValue, int length)
 *
 *  Description:
 *
 *      This function sets the value of an octet string.
 *
 *  Parameters:
 *
 *      id              id of variable to set
 *      stringValue     new value for variable
 *      length          length of value
 *
 *  Return Values:
 *
 *      error code returned by managment API
 *
 */

static MAN_ERROR_TYPE setOctetString(MAN_ID_TYPE id, unsigned char *stringValue, int length)
{
    MAN_OCTET_STRING_TYPE value;

    value.length = length;
    value.maxLength = length;
    value.buffer = stringValue;

    return manSetOctetString(id, &value, NULL, MAN_TIMEOUT_FOREVER);
}









/*
 * This function must be called to register the MIB variables
 * with the management API.
 */
static void registerWithManagementApi(void)
{
    if (manAddVariableList(EARMIB, 4) != MAN_SUCCESS)
    {
        handleError("EARMIB");
    }

}



/*
 * This function must be called to register the MIB variables
 * with the management API.
 */
static void registerWithSnmpAgent(void)
{
    struct subtree *mib;
    oid *name = (oid *) malloc(MAX_NAME_LEN * sizeof(oid));

    name[0] = 1;
    name[1] = 3;
    name[2] = 6;
    name[3] = 1;
    name[4] = 4;
    name[5] = 1;
    name[6] = 19775;
    name[7] = 1;
    name[8] = 1;
    name[9] = 1;
    mib = mibGroupRegister(name, 10, (struct variable *) EARMIBGroup1,23, sizeof(struct mVariableStruct4));
    if (mib == NULL)
    {
        handleSnmpError("EARMIBGroup1");
    }

    name[0] = 1;
    name[1] = 3;
    name[2] = 6;
    name[3] = 1;
    name[4] = 4;
    name[5] = 1;
    name[6] = 19775;
    name[7] = 1;
    name[8] = 1;
    name[9] = 2;
    mib = mibGroupRegister(name, 10, (struct variable *) EARMIBGroup2,38, sizeof(struct mVariableStruct2));
    if (mib == NULL)
    {
        handleSnmpError("EARMIBGroup2");
    }

    name[0] = 1;
    name[1] = 3;
    name[2] = 6;
    name[3] = 1;
    name[4] = 4;
    name[5] = 1;
    name[6] = 19775;
    name[7] = 1;
    name[8] = 1;
    name[9] = 3;
    mib = mibGroupRegister(name, 10, (struct variable *) EARMIBGroup3,34, sizeof(struct mVariableStruct4));
    if (mib == NULL)
    {
        handleSnmpError("EARMIBGroup3");
    }

    name[0] = 1;
    name[1] = 3;
    name[2] = 6;
    name[3] = 1;
    name[4] = 4;
    name[5] = 1;
    name[6] = 19775;
    name[7] = 1;
    name[8] = 1;
    name[9] = 6;
    mib = mibGroupRegister(name, 10, (struct variable *) EARMIBGroup4,4, sizeof(struct mVariableStruct4));
    if (mib == NULL)
    {
        handleSnmpError("EARMIBGroup4");
    }

    name[0] = 1;
    name[1] = 3;
    name[2] = 6;
    name[3] = 1;
    name[4] = 4;
    name[5] = 1;
    name[6] = 19775;
    name[7] = 1;
    name[8] = 1;
    name[9] = 7;
    mib = mibGroupRegister(name, 10, (struct variable *) EARMIBGroup5,4, sizeof(struct mVariableStruct4));
    if (mib == NULL)
    {
        handleSnmpError("EARMIBGroup5");
    }

    free(name);
}



/*
 * This function sets default values for scalar objects in the MIBs.
 */
static void setDefaultValues(void)
{

}



/*
 * This function sets default values for scalar objects in the MIBs.
 */
void snmpRegisterManagementVariables(void)
{
    registerWithManagementApi();
    setDefaultValues();
    registerWithSnmpAgent();
}