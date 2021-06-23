/*
 *            Copyright (c) 1998-2000 by NETsilicon Inc.   
 *
 *  This software is copyrighted by and is the sole property of 
 *  NETsilicon.  All rights, title, ownership, or other interests
 *  in the software remain the property of NETsilicon.  This
 *  software may only be used in accordance with the corresponding
 *  license agreement.  Any unauthorized use, duplication, transmission,  
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice may not be removed or modified without prior
 *  written consent of NETsilicon.
 *
 *  NETsilicon, reserves the right to modify this software
 *  without notice.
 *
 *  NETsilicon
 *  411 Waverley Oaks Road                  USA 781.647.1234
 *  Suite 227                               http://www.netsilicon.com
 *  Waltham, MA 02452                       AmericaSales@netsilicon.com
 *
 *************************************************************************
 *
 *     Module Name: SNMP_API.C
 *         Version: 1.00
 *   Original Date: 17-Sep-1999
 *          Author: 
 *        Language: Ansi C
 * Compile Options: 
 * Compile defines: 
 *       Libraries: 
 *    Link Options: 
 *
 *    Entry Points: 
 *
 * Description.
 * =======================================================================
 * This file implements the API to the NetSilicon SNMP agent.
 *                                                                        
 *                                                                        
 *  Edit Date/Ver   Edit Description
 * ==============   ===================================================
 *  PMJ 05/31/02    Corrrected problems (I caused) in NAcheckCommunity().
 *
 */
#include "gener_os.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Npttypes.h>
#include <snmpapi.h>
#include <errno.h>
#include <sysAccess.h>
#include "bsp_api.h"
#include <man_api.h>
#include "tx_api.h"
#include "boardParams.h"

void root_breakpoint(long unsigned int *);

#ifdef NET_OS
typedef void *(*FUNCPTR) (void *);
#endif


/* The SNMP_ENTERPRISE number is assigned from IANA for your enterprise MIB          */
unsigned long SNMP_ENTERPRISE = 19775 /*901 */ ;
unsigned long SNMP_TRAP_DESTINATION_ADDRESS = 0xc0a8dea3;	/*075C0103 7.92.1.3 */
#define COMMUNITY_LENGTH    24	/*HAS TO BE SAME IN SNMPIMPL.H */
char SNMP_READ_COMMUNITY_STRING[COMMUNITY_LENGTH] = "public";
char SNMP_WRITE_COMMUNITY_STRING[COMMUNITY_LENGTH] = "private";
char SNMP_TRAP_COMMUNITY_STRING[COMMUNITY_LENGTH] = "public";
int SNMP_ENABLE_SNMP = 1;

/* The same as MAX_STRING_LEN defined in asn1.h and btypes.h in netos\h\snmp */
#define SYS_BUFFER_LEN 255
char sysdescription[/*SYS_BUFFER_LEN*/] =
    "2U Erbium Doped Fibre Amplifier (EDFA)";
char syscontact[31+1 /*SYS_BUFFER_LEN*/] = "www.ipgphotonics.com";
char sysname[31+1 /*SYS_BUFFER_LEN*/] = "IPG Photonics";
char syslocation[127+1 /*SYS_BUFFER_LEN*/] =
    "411 Waverly Oaks Road, Waltham MA, 02452";


extern int doSnmpTask();
unsigned char snmpd_loaded = 0;
unsigned long snmpd_tid;
#include "STACK_SIZE.h"
#define SNMPD_NAME            "tSNMP"



/**
 * Start a task and load the SNMP deamon
 * 
 * @external
 * @category		SNMP
 * @since		6.0
 *
 * @return 0		Agent started successfully
 * @return -1		Error or timeout occurred; agent did not start
 */

int snmpd_load(void)
{

    pthread_attr_t attr;

    if (snmpd_loaded) {		/* if already loaded */
	return -1;		/* then just return */
    }



    if (pthread_attr_init(&attr) != 0)
	return -1;

    if (pthread_attr_setstacksize(&attr, SNMPD_STACK_SIZE) != 0)
	return -1;

    xpthread_attr_setname(&attr, SNMPD_NAME);

    if (pthread_create
	((pthread_t *) & snmpd_tid, &attr, (FUNCPTR) doSnmpTask, 0) != 0)
	return -1;

    if (xpthread_set_priority
	((pthread_t) snmpd_tid, NATranslatePriorityLevel(naSnmpPriority)))
	return -1;

    pthread_attr_destroy(&attr);

    while (!NASnmpAgentReady) {	/* wait for SNMP agent to complete start up */
	tx_thread_sleep(1);
    }

    snmpd_loaded = 1;

    return 0;
}


/**
 * Returns a pointer to an array that contains the sysObjectID variable.
 *
 * @external		
 * @category		SNMP
 * @since		6.0
 * 
 * @param size		Pointer to an integer set to the number of elements
 *			(unsigned longs) in the array returned
 *
 * @return 0		No sysObjectID is available.
 * @return string	Pointer to the sysObjectID array.
 */

unsigned long *SNMPGetSysObjectID(int *size)
{
    static unsigned long objid[8] = { 1, 3, 6, 1, 4, 1, 0, 1 };

    NA_ASSERT(size != NULL);
    objid[6] = SNMP_ENTERPRISE;
    *size = 8;
    return objid;
}

/**
 * Returns a NULL-terminated string containing the SysDescr variable.
 *
 * @external			
 * @category		SNMP
 * @since		6.0
 *
 *
 * @return 0		No system description is available.
 * @return string	Pointer to the system description.
 */

char *SNMPGetSysDescr(void)
{
    return (char *) sysdescription;
}

/**
 * Returns a NULL-terminated string pointing to the sysContact variable.
 *
 * @external			
 * @category		SNMP
 * @since			6.0
 *
 * @return 0		No system contact is available.
 * @return string	Pointer to the system contact.
 */

char *SNMPGetSysContact(void)
{
    devBoardParamsType app_nvParams;

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    /* give either the default or what we remember */
    if (strlen(app_nvParams.krbPassword) > 0) {
	strncpy(syscontact, app_nvParams.krbPassword, 31);
    }
    return (char *) syscontact;
}


/**
 * Sets the SysContact variable.
 *
 * @external			
 * @category			SNMP
 * @since			6.0
 * 
 * @param sys_contact		Pointer to a NULL- terminated string that contains the definition of SysContact
 * @param size			Number of octets in the string; not currently used
 *
 * @return 0			No SysContact is available..
 * @return string		Pointer to SysContact.
 */

char *SNMPSetSysContact(char *sys_contact, int size)
{
    devBoardParamsType nvParams;

    customizeReadDevBoardParams(&nvParams);
    root_breakpoint(&nvParams.mgmtStations[0].ipAddress);

    /* worry about copying past the end of the array */
    if (strlen(sys_contact) > 31) {
	strncpy(nvParams.krbPassword, sys_contact, 31);
    } else {
	strcpy(nvParams.krbPassword, sys_contact);
    }
    customizeWriteDevBoardParams(&nvParams);

    /* we know that the length of syscontact is longer than krbRealmname */
    strcpy(syscontact, nvParams.krbPassword);
    return (char *) syscontact;
}


/**
 * Returns a NULL-terminated string pointing to the sysName variable.
 *
 * @external
 * @category			SNMP
 * @since				6.0
 *
 * @return 0			No system name is available.
 * @return string		Pointer to the system name.
 */

char *SNMPGetSysName(void)
{
    devBoardParamsType app_nvParams;

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    /* give either the default or what we remember */
    if (strlen(app_nvParams.krbUsername) > 0) {
	strncpy(sysname, app_nvParams.krbUsername, 31);
    }
    return (char *) sysname;
}

/**
 * Sets the sysName variable.
 *
 * @external			
 * @category			SNMP
 * @since				6.0
 * 
 * @param sys_name		Pointer to a NULL- terminated string that contains the definition of SysName
 * @param size			Number of octets in the string; not currently used
 *
 * @return 0			No system name is available.
 * @return string		Pointer to system name.
 */
char *SNMPSetSysName(char *sys_name, int size)
{
    devBoardParamsType nvParams;

    customizeReadDevBoardParams(&nvParams);
    root_breakpoint(&nvParams.mgmtStations[0].ipAddress);

    /* worry about copying past the end of the array */
    if (strlen(sys_name) > 31) {
	strncpy(nvParams.krbUsername, sys_name, 31);
    } else {
	strcpy(nvParams.krbUsername, sys_name);
    }
    customizeWriteDevBoardParams(&nvParams);

    /* we know that the length of sysname is longer than krbRealmname */
    strcpy(sysname, nvParams.krbUsername);
    return (char *) sysname;
}

/**
 * Returns a NULL-terminated string pointing to the sysLocation variable.
 *
 * @external			
 * @category			SNMP
 * @since				6.0
 *
 * @return 0			No system location is available.
 * @return string		Pointer to the system location.
 */

char *SNMPGetSysLocation(void)
{
    devBoardParamsType app_nvParams;

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    /* give either the default or what we remember */
    if (strlen(app_nvParams.krbRealmname) > 0) {
	strncpy(syslocation, app_nvParams.krbRealmname, 127);
    }
    return (char *) syslocation;
}

/**
 * Returns a NULL-terminated string pointing to the sysLocation variable.
 *
 * @external			
 * @category			SNMP
 * @since				6.0
 * 
 * @param sys_location		Pointer to a NULL- terminated string that contains the definition of SysLocation
 * @param size				Number of octets in the string; not currently used
 *
 * @return 0				No location is available.
 * @return string			Pointer to the location.
 */
char *SNMPSetSysLocation(char *sys_location, int size)
{
    devBoardParamsType nvParams;

    customizeReadDevBoardParams(&nvParams);
    root_breakpoint(&nvParams.mgmtStations[0].ipAddress);

    /* worry about copying past the end of the array */
    if (strlen(sys_location) > 127) {
	strncpy(nvParams.krbRealmname, sys_location, 127);
    } else {
	strcpy(nvParams.krbRealmname, sys_location);
    }
    customizeWriteDevBoardParams(&nvParams);

    /* we know that the length of syslocation is longer than krbRealmname */
    strcpy(syslocation, nvParams.krbRealmname);
    return (char *) syslocation;
}


/**
 * Returns the value of the sysServices variable.
 *
 * @external			
 * @category			SNMP
 * @since				6.0
 *
 * @return 0			No system services are available.
 * @return string		Pointer to the system services.
 */
char SNMPGetSysServices(void)
{
    return (char) 72;
}


/*
 *
 *  Function: int NAcheckCommunity(char *community, int communityLen, int msg_type, unsigned int callerIpAddr)
 *
 *  Description:
 *
 *      Matches received community with the predefined one.
 *
 *  Parameters:
 *
 *      community:          community string
 *
 *      communityLen:       length of community string
 *
 *      reqWriteAccess:     Requires SNMP WRITE access (private string).
 *
 *      callerIpAddr:       Ip address (v4) of SNMP client. 
 *
 *  Return Values:
 *
 *      A return of 0 indicates the community string is legal.
 *
 *      A return of < 0 indicates an invalid community string.
 *
 */
int NAcheckCommunity(char *community, int communityLen, int reqWriteAccess,
		     unsigned int callerIpAddr)
{
    char strUserPass[NASYSACC_STRLEN_USERNAME + NASYSACC_STRLEN_PASSWORD +
		     16];
    char password[NASYSACC_STRLEN_PASSWORD + 1];
    unsigned int reqPriv, priv;
    unsigned int ipAddr;

    if (communityLen < NASYSACC_STRLEN_USERNAME + NASYSACC_STRLEN_PASSWORD + 16) {
	reqPriv =
	    (reqWriteAccess ==
	     1) ? NASYSACC_LEVEL_SNMP_RW : NASYSACC_LEVEL_SNMP_R;
	memcpy(strUserPass, community, communityLen);
	strUserPass[communityLen] = 0;

	/*  check for a public community user   */
	priv = NAgetSysAccess("public", password, &ipAddr);
	if (strcmp(strUserPass, password) == 0) {
	    if ((priv & reqPriv) == reqPriv) {
		if ((ipAddr == 0) || (ipAddr == callerIpAddr)) {
		    return (0);
		}
	    }
	}

	/*  check for a private community user   */
	priv = NAgetSysAccess("private", password, &ipAddr);
	if (strcmp(strUserPass, password) == 0) {
	    if ((priv & reqPriv) == reqPriv) {
		if ((ipAddr == 0) || (ipAddr == callerIpAddr)) {
		    return (0);
		}
	    }
	}
    }
    return -1;
}


/*ad*
 * Used as the return value from the function NAgetEnableSnmp.  This return
 * value controls access to the SNMP Agent.  Access can be limited to the  
 * loopback address (127.0.0.1) or can be open to any networking interface.
 *
 * @external
 * @category			SNMP
 * @since				6.2
 * @name SNMPAccessEnable           "SNMP Agent Networking Access"
 * @see @link NAgetEnableSnmp
 *
 * @param NA_SNMP_LOOPBACK_ONLY     Access available only through a loopback socket.
 *                                  This will prevent network access to SNMP.
 *
 * @param NA_SNMP_STANDARD_ACCESS   Standard access to SNMP, through a loopback socket   
 *                                  or any networking interface (ethernet, PPP).
*ad*/
#define NA_SNMP_LOOPBACK_ONLY       0
#define NA_SNMP_STANDARD_ACCESS     1



/*ad*
 * Returns the access level for the SNMP Agent.  This function can be modified 
 * to limit the SNMP Agent access to the local loopback address, or to any 
 * networking interface.  This function provides a security control to the SNMP
 * Agent. 
 *
 * @external			
 * @category			SNMP
 * @since				6.2
 * @see @link           SNMPAccessEnable
 *
 * @return NA_SNMP_LOOPBACK_ONLY    SNMP Agent access limited to the loopback address.
 * @return NA_SNMP_STANDARD_ACCESS  SNMP Agent access open to all network interfaces.
*ad*/
int NAgetEnableSnmp(void)
{
    return NA_SNMP_STANDARD_ACCESS;
}
