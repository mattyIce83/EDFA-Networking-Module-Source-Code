
/*
 *
 *     Module Name: telnet.c
 *	   Version: 1.00
 *   Original Date: 10/06/99	
 *	    Author: Dale Lee
 *	  Language: Ansi C
 * Compile Options:
 * Compile defines:
 *	 Libraries:
 *    Link Options:
 *
 *    Entry Points:
 *
 *  Copyrighted (c) by NETsilicon, Inc.  All Rights Reserved.
 *
 *  This program developed by NETsilicon, Inc. is 
 *  provided as reference material for use in implementing a product based
 *  upon the NET+ARM chipset. No other rights to reproduce, use, or disseminate
 *  NETsilicon, Inc. portions of this computer program, whether in 
 *  part or in whole are granted. 
 * 
 *  NETsilicon, Inc. makes no representation or warranties   
 *  with respect to the performance of this computer program, and     
 *  specifically disclaims any responsibility for any damages,        
 *  special or consequential, connected with the use of this program. 
 * 
 *
 * Description.
 * =======================================================================
 * This file contains functions of telnet session's connection process
 *
 *
 * Edit Date/Ver   Edit Description
 * ==============  =======================================================
 * LZ   03/27/00   Ported to NET_OS from PSOS.
 * PMJ  08/22/00   Changed menu.h to telnmenu.h. See ER 1338
 * DXL	02/05/02   Set buffer_timer, client_option, and option_flag when opening telnet port;
 * DXL	04/23/01   Resolved compiler warning
 * DXL	04.26.02   Changed session table to use MAX_SESSIONS; Check if session id is valid
 * JZW  08/13/03   Fixed bug where since the serial number is now 10 characters
 *                 the memcpy in ts_connect_session would cause an overflow and
 *                 crash. Also changed functions to correctly deal with the
 *                 new "customize" access routines dealign with NVRAM
 */


#include <stdio.h>
#include <stdlib.h>
#include <tx_api.h>
#include <tservapi.h>
#include "bsp_api.h"
#include "boardparams.h"
#include "readFrom2U2.h"
#include "telnmenu.h"
#include "telnet.h"
#include "earmib.h"


#define MAX_LEN_SERIAL_NUMBER 10	/*GORD why not BP_SERIAL_NUMBER_LENGTH */

TS_SESSION_TYPE TS_session_tab[MAX_SESSIONS];
extern struct telnet_menu_t telnet_menu_array[];

void telnetApp2(unsigned int session_id);
void telnetTimeout(ULONG session_id);

void root_breakpoint(long unsigned int *);

int TimeoutS;

/*
 *
 *  Function: telnetInit
 *
 *  Description:
 *
 *      Initializes telnet module.
 *
 */
void telnetInit(devBoardParamsType * app_nvParams)
{
    short i;
    int status;
    char buf[64];

    // read the telnet timeout from NVM
    if (app_nvParams->telnetTimeoutS < MIN_TELNET_TIMEOUT)
	app_nvParams->telnetTimeoutS = MIN_TELNET_TIMEOUT;
    if (app_nvParams->telnetTimeoutS > MAX_TELNET_TIMEOUT)
	app_nvParams->telnetTimeoutS = MAX_TELNET_TIMEOUT;
    TimeoutS = app_nvParams->telnetTimeoutS;

    for (i = 0; i < MAX_SESSIONS; i++) {
	// Create one inactivity timer per telnet session
	sprintf(buf, "Telnet ssn %d timeout", i);
	status = tx_timer_create(&(TS_session_tab[i].t_timeout),
				 buf,
				 telnetTimeout,
				 i,
				 NABspTicksPerSecond * TimeoutS,
				 0, TX_NO_ACTIVATE);
    }
}

/*
 *
 *  Function: telnetTimeout
 *
 *  Description:
 *
 *      Closes a telnet session due to inactivity timeout
 *
 */
void telnetTimeout(ULONG session_id)
{
    if (session_id >= MAX_SESSIONS) {
//      printf ("Invalid session id [%d]\n", session_id);
	return;
    }

    TSSendString(session_id, "\r\nInactivity timeout: Closing.\r\n");
    TSCloseSession(session_id);
}

/*
 *
 *  Function: int ts_connect_session (unsigned int session_id)
 *
 *  Description:
 *
 *      This routine is called when the telnet session receives a connection 
 *      request from a client.
 *
 *  Parameters:
 *
 *      session_id:
 *      A unique value that identifies a telnet session passed by the telnet server.
 *
 *  Return Values:
 *
 *      A return of 0 indicates success.
 *      A return of -1 notifies the server to close a session.
 *
 */
static unsigned char earSerial_Number_Read_buffer[MAX_2U_RESPONSE];
int ts_connect_session(unsigned int session_id)
{
    devBoardParamsType app_nvParams;
	void *resultBuffer = NULL;
	unsigned long bufferLength = MAX_2U_RESPONSE;
	int count;
    TS_SESSION_TYPE *session_ptr;

    if (session_id >= MAX_SESSIONS) {
	return (-1);
    }

    session_ptr = &TS_session_tab[session_id];

    session_ptr->inuse = 1;

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);


    TSSendString(session_id,
		 "\r\nWelcome to IPG Photonics Configuration Utility ");
    sprintf(session_ptr->buf, "%d.%d\r\n", MAJOR_VERSION, MINOR_VERSION);
    TSSendString(session_id, session_ptr->buf);

    TSSendString(session_id, "Serial Number:\t");
	/*resultBuffer = askAtmelForStringValue
        ("ZIMRSN",
         "ZRSN",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, //retries 
         &(earSerial_Number_Read_buffer[0]),
		 &bufferLength,
         10154); */
	TSSendString(session_id, Mib_Leaves->earSerialNumber); // earSerial_Number_Read_buffer); //resultBuffer);
//	if (&app_nvParams.serialNumber != resultBuffer) {
//		for (count = 0; count < sizeof(app_nvParams.serialNumber); count++) {
//			app_nvParams.serialNumber[count] = earSerial_Number_Read_buffer[count];
//		}
		//&app_nvParams.serialNumber = earSerial_Number_Read_buffer[0];
//		root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
//		customizeWriteDevBoardParams(&app_nvParams);
//	}
    //TSSendString(session_id, app_nvParams.serialNumber);

    TSSendString(session_id, "; MAC Address:\t");
    sprintf(session_ptr->buf, "%02x:%02x:%02x %02x:%02x:%02x",
	    app_nvParams.ethernetMac[0],
	    app_nvParams.ethernetMac[1],
	    app_nvParams.ethernetMac[2],
	    app_nvParams.ethernetMac[3],
	    app_nvParams.ethernetMac[4], app_nvParams.ethernetMac[5]);
    TSSendString(session_id, session_ptr->buf);
    TSSendString(session_id, ".\r\n");

    // start the inactivity timer
    tx_timer_change(&(session_ptr->t_timeout),
		    NABspTicksPerSecond * TimeoutS, 0);
    tx_timer_activate(&(session_ptr->t_timeout));


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef LEFTOVERSTUFF
#define		STACK_SIZE	1024

    TX_THREAD telnetMonitor_app;


    int ccode;

    void *telnetAppStack;


    telnetAppStack = malloc(STACK_SIZE);
    if (telnetAppStack == NULL) {
	netosFatalError("Unable to allocate snmpAppStack thread stack.", 5,
			5);
    }

//   ccode = tx_thread_create (&telnetMonitor_app,                              // control block for thread
//                             "Telnet App",                                    // thread name
//                             telnetApp,                                               // entry function
//                             0,                                                       // parameter
//                             telnetAppStack,                                  // start of stack
//                             STACK_SIZE,                                      // size of stack
//                             NABspMediumPriority,                             // priority
//                             NABspMediumPriority,                             // preemption threshold
//                             1,                                                       // time slice threshold
//                             TX_AUTO_START);                                  // start immediately
//
//   if (ccode != TX_SUCCESS)
//   {
//      netosFatalError ("Unable to create thread.", 5, 5);
//   }

#endif				/* LEFTOVERSTUFF */



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    return (0);
}


//------------------------------------------------------------------------------------------------


void telnetApp2(unsigned int session_id)
{



}


//------------------------------------------------------------------------------------------------


/*
 *
 *  Function: int ts_login_session (unsigned int session_id, int user_id)
 *
 *  Description:
 *
 *      This routine is called when the telnet server accepts the client-requested 
 *      connection.
 *
 *  Parameters:
 *
 *      session_id:
 *      A unique value that identifies a telnet session passed by the telnet server.
 *
 *      user_id:
 *      ID associated with the username when a session is validated during login.
 *
 *  Return Values:
 *
 *      A return of 0 indicates success.
 *      A return of -1 notifies the server to close a session.
 *
 */
int ts_login_session(unsigned int session_id, int user_id)
{
    char text[64];

    TS_SESSION_TYPE *session_ptr;

    if (session_id >= MAX_SESSIONS) {
//              printf ("Invalid session id [%d]\n", session_id);
	return (-1);
    }
    session_ptr = &TS_session_tab[session_id];

    sprintf(text, "Hello %s\r\n", TSgetSessionUsername(session_id));
    TSSendData(session_id, text, strlen(text));

    // Now that the user has logged in, send the main menu.
    session_ptr->menu = TS_MAIN_MENU;
    session_ptr->state = 0;
    session_ptr->uid = user_id;

    if (telnet_send_menu(session_id) < 0) {
//         printf("telnet: couldn't send menu\n");
	return -1;
    }

    return (0);
}


/*
 *
 *  Function: int ts_disconnect_session (unsigned int session_id)
 *
 *  Description:
 *
 *      This routine is called when the telnet server disconnects the client
 *      connection.
 *
 *  Parameters:
 *
 *      session_id:
 *      A unique value that identifies a telnet session passed by the telnet server.
 *
 *  Return Values:
 *
 *      A return of 0 indicates success.
 *      A return of -1 notifies the server to close a session.
 *
 */
int ts_disconnect_session(unsigned int session_id)
{
    TS_SESSION_TYPE *session_ptr;

    if (session_id >= MAX_SESSIONS) {
//              printf ("Invalid session id [%d]\n", session_id);
	return (-1);
    }

    session_ptr = &TS_session_tab[session_id];

    // stop inactivity timer
    tx_timer_deactivate(&(session_ptr->t_timeout));

    session_ptr->inuse = 0;

    return (0);
}


/*
 *
 *  Function: int ts_receive_data (unsigned int session_id, void *buf, int len)
 *
 *  Description:
 *
 *      This routine is called when the telnet server receives incoming data from
 *      a client session. The buffer contains data ready to be processed by 
 *      application.
 *
 *  Parameters:
 *
 *      session_id:
 *      A unique value that identifies a telnet session passed by the telnet server.
 *
 *      buf:
 *      Buffer to store data.
 *
 *      len:
 *      Number of characters in the buffer.
 *
 
 *  Return Values:
 *
 *      A return of 0 indicates success.
 *      A return of -1 notifies the server to close a session.
 *
 */
int ts_receive_data(unsigned int session_id, void *buf, int len,
		    int notUsed)
{
    int rc;

    TS_SESSION_TYPE *session_ptr;

    if (session_id >= MAX_SESSIONS) {
//              printf ("Invalid session id [%d]\n", session_id);
	return (-1);
    }

    session_ptr = &TS_session_tab[session_id];

    /* printf ("[%d] %s", len, (char *)buf); */

    // reset the inactivity timer when anything is received, and disable it while the action is 
    // being performed
    tx_timer_deactivate(&(session_ptr->t_timeout));

    rc = telnet_menu_array[session_ptr->menu].menu_cmd_func((char *) buf,
							    len,
							    session_id);

    tx_timer_change(&(session_ptr->t_timeout),
		    NABspTicksPerSecond * TimeoutS, 0);

    tx_timer_activate(&(session_ptr->t_timeout));

    return (rc);
}




/*
 *
 *  Function: int ts_setup_port (unsigned long port)
 *
 *  Description:
 *
 *      This routine is to setup the configuration and start atelnet server on required port.
 *
 *  Parameters:
 *
 *      port:
 *      A number from 0 to (n-1), where n is the number of entries defined in TSInitServer().
 *
 *  Return Values:
 *
 *      A return of 0 indicates success.
 *      A return of -1 indicates failure.
 *
 */
int ts_setup_port(int forloopindex)
{
    int rc;
    unsigned long port;
    TS_UCONFIG_TYPE user_config;

    port = forloopindex;
    user_config.client_options = 0;
    user_config.server_options = TS_ECHO | TS_NOGA;
    user_config.telnet_port = TS_PORT_BASE + port;
    user_config.login_retries = 3;
    user_config.buffer_timer = 0;
    user_config.option_flag = 0;
    user_config.login_callback = ts_login_session;
    user_config.connect_callback = ts_connect_session;
    user_config.disconnect_callback = ts_disconnect_session;
    user_config.receive_callback = ts_receive_data;
    user_config.getbuf_callback = NULL;
    user_config.validate_callback = NULL;

    rc = TSOpenPort(&port, &user_config);
    if (rc) {
//              printf ("Unable to Open telnet port\n");
	return (-1);
    }

    return (0);
}
