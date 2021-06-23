///////////////////////////////////////////////////////////////////////////////
//
//      IPG Photonics Corporation
//      COPYRIGHT (c) 2001 IPG Photonics Coroporation ALL RIGHTS RESERVED
//  This program may not be reproduced, in whole or in part in any
//  form or any means whatsoever without the written permission of:
//
//  IPG Photonics Corporation
//  50 Old Webster Road
//  Oxford, MA 01540, USA
//
//
//  Filename:           appconf.h
//
//  Description:
//  applicationStart() is the entry point for user applications.  This
//  This function will be called after the kernel has started and the
//  TCP/IP stack has loaded.  applicationTcpDown() is called periodically
//  after the kernel has started while the system is waiting for the
//  TCP/IP stack to start.
//
//  Author:             Victor Semlek
//
//  Target Processor:   ARM NS7520 55Mhz
//
//  Target Compiler:    gcc
//
//  Revision:           1.00
//
//  Revision History:
//
//      $Log: appconf.h,v $
//      Revision 1.1.1.1  2006-05-04 14:38:39  eugene
//      Commit of the latest Digi source code retrieved from Fidus (ver 1.140).
//
//      Revision 1.2  2006-05-04 14:38:39  gdey
//      capture what should be post 115 and pre 116
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////


#ifndef appconf_h

#define appconf_h

#include <stdio.h>
#include <stdlib.h>



/*
 * APP_ERROR_HANDLER defines the user error handler for fatal errors.  It should either be set
 * to NULL, or set to the name of a routine that should be called if a fatal error occurs.  The
 * routine should be of the form:
 *
 *      void error_handler (char *errorDescription, int errorType)
 *
 * If APP_ERROR_HANDLER is NULL, then the default error handler will be used.  This error handler
 * blinks the LEDs on the development board in an infinite loop.  The LEDs are blinked in a pattern
 * that indicates the errorType.  The green LED is blinked (errorType/10) times, and the yellow LED
 * is blinked (errorType % 10) times.  The LEDs are blinked at a rate of 10 Hz with a 1 second pause
 * between interations.
 *
 */
#define APP_ERROR_HANDLER       NULL


/*
 * APP_ROOT_STACK_STACK sets the size of the root thread's stack.
 */
#define APP_ROOT_STACK_SIZE     8192

/*
 * APP_ROOT_PRIORITY sets the priority level for the root thread.
 */
#define APP_ROOT_PRIORITY       16


/*
 * APP_DEFAULT_API_PRIORITY determines the priority level threads internal to the
 * NET+Works APIs will be use.  The global variable bsp_default_api_priority is
 * loaded with this value.
 */
#define APP_DEFAULT_API_PRIORITY        (NATranslatePriorityLevel(naDefaultPriority))

/*
 * APP_FILE_SYSTEM_SIZE determines the numbers of file the Advanced Web server(AWS)
 * will allocated for file system. The default value is 9, the number of connections 
 * The AWS alocated. Increasing APP_FILE_SYSTEM_SIZE will reduce memory available. 
 */
#define APP_FILE_SYSTEM_SIZE          9


/*
 * APP_DIALOG_PORT determines whether or not the initialization code will prompt for configuration
 * parameters.  It should be set to a string that contains the serial port to prompt with.  If it
 * is not set, or if it contains an invalid port, then the initialization code will not prompt for
 * configuration parameters.
 */
#define APP_DIALOG_PORT         "/com/0"

/*
 * APP_DIALOG_APP_NAME determines the name of the current application program.  This is the name
 * that dialog will use to prompt whether or not the application should be run.
 */
#define APP_DIALOG_APP_NAME     "IPG SNMP MIB Server Application"


/*
 * APP_ROOT_PASSWORD determines the root account password.  This account has all privileges and
 * can be used for any service requiring a login (i.e. FTP, telnet).  The root password mechanism 
 * can be disabled by commenting out APP_ROOT_PASSWORD definition.
 */
#define APP_ROOT_PASSWORD       "ipg1"


/*
 * APP_POST determines whether or not the Power On Self Test routine will be run.  Set it to a
 * non-zero value to automatically run the POST routines at startup.  It is defined in
 * root.c.
 */
extern const int APP_POST;


/*
 * APP_BURN_IN_TEST determines whether or not the manufacturing burn in test should be run.
 * This test is destructive.  It will erase NVRAM.  It also requires loop back plugs on the
 * serial, parallel, and Ethernet ports.  This test will cycle in an infinite loop once it is
 * started.  If an error is detected, then the LEDs will be blinked in a pattern that indicates
 * the nature of the failure.
 *
 * If APP_DIALOG_PORT is set, then the dialog will prompt the user before running this test.
 *
 * APP_BURN_IN_TEST is defined in root.c.
 */
extern int APP_BURN_IN_TEST;


/*
 * APP_USE_NETWORK_TIME_PROTOCOL uses the NVRAM configuration to store Network Time Protocol
 * Servers used for the NET+OS SNTP Client on a NAstartSntp() call.
 */
//#define APP_USE_NETWORK_TIME_PROTOCOL


/*
 * APP_STDIO_PORT determines which serial port stdin, stdout, and stderr should be redirected to.
 * It should be set to the device name of the port or NULL.
 */
#define APP_STDIO_PORT              "/com/0"


/*
 * If APP_USE_NVRAM is set to a nonzero value, then the initialization code will attempt
 * to read the configuration values from NVRAM.  If the read is successful, then these
 * values will be used.  If not, then the values defined in this file will be used.  If
 * the configuration dialog is enabled, then the user will be prompted to change these
 * values.  If he does, then the new configuration values will be saved to NVRAM.
 *
 * If APP_USE_NVRAM is set to zero, then the values defined in this file will be used.
 * They will not be written to NVRAM.
 *
 * In both case, the same data structure is used:
 *
 * typedef struct
 {
    char serialNumber[8];                   for now we actually only use 6 digits
    unsigned long ipAddress;                IP Address
    unsigned long subnetMask;               subnet mask
    unsigned long gateway;                  IP address of gateway
    long useDhcp;                           set if we are to get address with DHCP
    unsigned long waitTime;                 amount of time to wait in seconds
    unsigned long baudrate;                 baudrate for serial port
    unsigned long telnetMagic;              telnet magic number
    unsigned long reserved[1];              save room for expansion
    unsigned long checksum;                 checksum for this structure
    unsigned long ipAddrPpp1;               PPP COM1 IP Address
    unsigned long ipAddrPpp2;               PPP COM2 IP Address
    unsigned long ipAddrPpp1peer;           PPP COM1 Peer IP Address
    unsigned long ipAddrPpp2peer;           PPP COM2 Peer IP Address
 } devBoardParamsType;
 *
 *
 * User may add more fields in this data structure as long as his physical NVM has space.
 *
 *
 */
#define APP_USE_NVRAM              1		// set to 0 - 05/12/05 pwl (it was 1) 
						// set back to 1 05/18/05 for installing new ME
/*
 * APP_NET_HEAP_SIZE sets the heap size for TCP/IP stack dinamic allocations.
 */

#define APP_NET_HEAP_SIZE	0x80000




/*
 *---------------------------------------------------------------------------------------------
 * All values above are compile time constants.  All values below may be stored in NVRAM if it
 * is present.
 *---------------------------------------------------------------------------------------------
 */

#define APP_RUN_APPLICATION         0
#define APP_RUN_ANGEL               1
#define APP_RUN_BURN_IN             2

/*
 * APP_STARTUP_MODE determines the default startup mode for the application.  It should be
 * set to one of the following values.
 *
 *  APP_RUN_APPLICATION             run the user application
 *  APP_RUN_ANGEL                   run the Angel debug monitor
 *  APP_RUN_BURN_IN                 run the manufacturing burn in test
 */
#define APP_STARTUP_MODE        APP_RUN_APPLICATION

/*
 * APP_ENABLE_PPP1 enables PPP on COM1.
 *
 *  Note when this is defined APP_DIALOG_PORT and APP_STDIO_PORT must not use
 *  COM1 ("/com/0")
 */
/*#define APP_ENABLE_PPP1         1*/

/*
 * APP_ENABLE_PPP2 enables PPP on COM2.
 *
 *  Note when this is defined APP_DIALOG_PORT and APP_STDIO_PORT must not use
 *  COM2 ("/com/1")
 */
/*#define APP_ENABLE_PPP2         0*/


/* The following SEVEN definitions are default values for the Netsilicon's development board when
 * APP_USE_NVRAM is defined to 0.  So be sure to update these values to your own
 * settings.
 */

/*
 * APP_IP_ADDRESS, APP_IP_SUBNET_MASK, and APP_IP_GATEWAY determine the unit's default IP address,
 * subnet mask, and gateway.  All values should be specified in dotted notation as strings.  If there
 * is no gateway, then set APP_IP_GATEWAY to "0.0.0.0".
 */
#define APP_IP_ADDRESS          "192.168.101.161"
#define APP_IP_SUBNET_MASK      "255.255.255.0"
#define APP_IP_GATEWAY          "192.168.101.254"


/*
 * APP_USE_NETWORK_TIME_PROTOCOL uses the NVRAM configuration to store Network Time Protocol
 * Servers used for the NET+OS SNTP Client on a NAstartSntp() call.
 */
/* #define APP_USE_NETWORK_TIME_PROTOCOL */

/*
 * APP_SNTP_PRIMARY and APP_SNTP_SECONDARY determine the unit's default primary and secondary
 * SNTP servers.  All values should be specified in dotted notation as strings.
 */
#define APP_SNTP_PRIMARY            "0.0.0.0" 
#define APP_SNTP_SECONDARY          "0.0.0.0" 


/*
 *APP_BSP_SERIAL_NUMBER, APP_BSP_DELAY, APP_BSP_BAUD_RATE define the additional parameter that are going
 *to be used in the narmsrln.c if the APP_USE_NVRAM is set to zero above
 */

#define APP_BSP_SERIAL_NUM	"99999999"
#define APP_BSP_DELAY		5
#define APP_BSP_BAUD_RATE	9600
/*
 * APP_USE_STATIC_IP is used to create a default configuration for ACE.  This value
 * only has effect if there is no valid configuration stored in NVRAM, or if APP_USE_NVRAM
 * is not set.  It determines whether or not the IP stack should use the static IP
 * parameters specified in APP_IP_ADDRESS, APP_IP_SUBNET_MASK, and APP_IP_GATEWAY, or
 * get an IP address from the network using DHCP, BOOTP, RARP, and Ping-ARP.  
 *
 * @external
 * @category general_purpose 
 * @since 6.0
 */
#define APP_USE_STATIC_IP            FALSE	// changed to TRUE 05/12/05 pwl 
						// changed to FALSE 05/18/05 for new ME install


#endif
