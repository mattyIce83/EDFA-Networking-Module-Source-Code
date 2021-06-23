/*
 * $Id: menu.c,v 1.3 2011-10-11 13:51:36 hazem Exp $
 *
 *     Module Name: menu.c
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
 * This file containe routines which send and decode the
 * telnet menus. PLEASE BE CAREFUL OF THAT NVM IS CHANGED IN 
 * TWO PLACES.
 *
 *
 * Edit Date/Ver   Edit Description
 * ==============  =======================================================
 * LZ   03/28/00   ported to NET_OS from PSOS.
 * PMJ  08/22/00   Changed menu.h to telnmenu.h. See ER 1338
 * DXL	11/09/01   NAsetSysAccess() need to NULL passed in IP parameter
 *					inorder to change password.
 * DXL	04/23/01   Resolved compiler warning
 * JZW  08/13/03   Updated to use ace parameters and "custom" calls
 * JZW  09/15/03   Fixed the poorly worded error message when an invalid password
 *                 is entered
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "boardParams.h"
#include <pthread.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ pwl +
#include <rtc.h>
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include "readFrom2U2.h"
#include "ace_params.h"
#include "boardParams.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ pwl -


#include <tservapi.h>
#include <sysAccess.h>
#include "telnmenu.h"
#include "telnet.h"
#include "earmib.h"
#include "ipgsnmp.h"
#include "reg_def.h"
#include "../../../h/fbanks.h"
#include "ipgFlash.h"
#include "watch.h"
#include "find_highwater.h"
#include "semaphores.h"
#include "MAGIC.h"
#include "ascii.h"
#include "root.h"
#include "earmib.h"

#define MASK(x) ((unsigned long)1 << x)

#define ALARM_TEMPERATURE_FLAG      (MASK(0))
#define ALARM_FAN_FLAG              (MASK(1))
#define ALARM_OUTPUT_TOO_HIGH_FLAG  (MASK(2))
#define ALARM_OUTPUT_HIGH_FLAG      (MASK(3))

#define ALARM_OUTPUT_LOW_FLAG       (MASK(4))
#define ALARM_INPUT_TOO_HIGH_FLAG   (MASK(5))
#define ALARM_INPUT_HIGH_FLAG       (MASK(6))
#define ALARM_INPUT_LOW_FLAG        (MASK(7))

#define ALARM_OUTPUT_OFF_FLAG       (MASK(8))
#define ALARM_INPUT_OFF_FLAG        (MASK(9))
#define ALARM_INTERLOCK_FLAG        (MASK(10))
#define ALARM_PS1_FAIL_FLAG         (MASK(11))

#define ALARM_PS2_FAIL_FLAG         (MASK(12))
#define ALARM_POWER_SAFETY_FLAG     (MASK(13))
#define ALARM_FUSE_FLAG             (MASK(14))
#define ALARM_48V_A_FLAG            (MASK(15))

#define ALARM_48V_B_FLAG            (MASK(16))
#define ALARM_INPUT2_TOO_HIGH_FLAG  (MASK(17))
#define ALARM_INPUT2_HIGH_FLAG      (MASK(18))
#define ALARM_INPUT2_LOW_FLAG       (MASK(19))

#define ALARM_INPUT2_OFF_FLAG       (MASK(20))
#define ALARM_OUTPUT2_OFF_FLAG      (MASK(21))
#define ALARM_OUTPUT2_LOW_FLAG      (MASK(22))
#define ALARM_OUTPUT2_HIGH_FLAG     (MASK(23))

#define ALARM_OUTPUT2_TOO_HIGH_FLAG (MASK(24))
#define ALARM_PUMP_FLAG             (MASK(25))
#define ALARM_BACK_REF_TOO_HIGH_FLAG (MASK(26))

unsigned char *snmpStackBegin;
int snmpStackSize;

unsigned char *telnetStackBegin;
int telnetStackSize;

unsigned char *alarmStackBegin;
int alarmStackSize;

unsigned char *serialStackBegin;
int serialStackSize;

unsigned char *collectionStackBegin;
int collectionStackSize;

#define SNMP_SEND_TRAP_TIMEOUT_MIN 200
unsigned long g_snmp_send_trap_timeout = 400;
int g_snmp_send_trap_failed = 0;
int g_snmp_send_trap_last_result = 0;

//#define SNMP_SEND_ALIVE_TRAP_PERIOD_MIN 3
//#define SNMP_SEND_ALIVE_TRAP_PERIOD_MAX 20
unsigned int g_snmp_send_alive_trap_period = 15; // 15 min trap (to turn off set to zero or valid vzlues 3-20)

// extern TX_SEMAPHORE semaphore_0;
// extern int semaphore_0_tries;
// extern int semaphore_0_unwedged;
// extern TX_SEMAPHORE semHistTblMutex;

extern TX_THREAD alarmTrap_app;
extern TX_THREAD snmpTrap_app;
extern TX_THREAD telnet_app;
extern TX_THREAD ftp_app;
extern TX_THREAD collection_app;

extern int root_break_count;
#if UGLY_NETWORK_HACK == 1
extern int g_ugly_network_hack;
#endif

extern int g_snmp_toggle;
extern int g_snmp_mgmt_stn_idx;

extern unsigned char productSWType;
extern unsigned char PwrType;
extern unsigned char AliveTimerNVRamSave;
extern unsigned int TrapPeriod;

void root_breakpoint(long unsigned int *);
int handle_main_menu(char *, int len, unsigned long);
int handle_serial_number_menu(char *, int len, unsigned long);
int handle_mac_address_menu(char *, int len, unsigned long);
int handle_ip_parameters_menu(char *, int len, unsigned long);
int get_ip_parameters(int, char *);

//.........................................................
int get_snmp_ip_parameters(int, char *);
//.........................................................

int handle_user_password_menu(char *, int len, unsigned long);
int handle_ip_addr_menu(char *, int len, unsigned long);

//.........................................................
int handle_snmp_ip_addr_menu(char *, int len, unsigned long);	// 06 13 05
int handle_snmp_edit_menu(char *cmd, int len, unsigned long);	// 06 27 05
int handle_snmp_station_edit_menu(char *cmd, int len, unsigned long);	// 06 27 05
int handle_snmp_community_menu(char *cmd, int len, unsigned long);	// 06 29 05
int handle_rtc_menu(char *cmd, int len, unsigned long);	// 09 09 05
int handle_private_community_password_menu(char *cmd, int len, unsigned long);	// 06 29 05
int handle_public_community_password_menu(char *cmd, int len, unsigned long);	// 06 29 05
int handle_rtc_parameters_menu(char *cmd, int len, unsigned long);	// 09 09 05

int handle_firmware_upgrade_menu(char *, int len, unsigned long);	// 09 13 05
int handle_timeout_menu(char *, int len, unsigned long);
int handle_alarm_menu(char *, int len, unsigned long);
//.........................................................

int handle_subnet_mask_menu(char *, int len, unsigned long);
int handle_gateway_addr_menu(char *, int len, unsigned long);
int handle_snmp_send_trap_timeout_menu(char *, int len, unsigned long);
int handle_snmp_send_alive_trap_period_menu(char *, int len, unsigned long);
int handle_chassis_reboot_menu(char *, int len, unsigned long);
void customizeGetIPParameters(unsigned long *, unsigned long *,
			      unsigned long *);
unsigned int getAccessLevel(unsigned long session_id);

//  There is one of these arrays for each menu which can be displayed.  
//  These menu's match the enums in telnmenu.h.  Each menu must end with a NULL string.

static char *main_menu[] = {
//      "\r\n\t~*~ FB1 Main Menu: FB1 ~*~\r\n",
//      "\r\n\t+0+ FB2 Main Menu: FB2 +0+\r\n",
    "\t1.) IP Parameters - Add\r\n",
    "\t2.) Change Password\r\n",
    "\t3.) SNMP Management Station - Remove\r\n",
    "\t4.) SNMP Community Strings\r\n",
    "\t5.) RTC Time\r\n",
    "\t6.) Firmware Upgrade\r\n",
    "\t7.) Set Timeout\r\n",
    "\t8.) Read Serial Number\r\n",
    "\t9.) Set MAC Address\r\n",
	"\t10.) Display Current Alarms\r\n",
    "\t11.) Quit\r\n",
    "\r\nEnter Selection: ",
    (char *) NULL
};

static char *strings_password_menu[] = {
    "\r\nEnter old community string: ",
    (char *) NULL
};

static char *user_password_menu[] = {
    "\r\nEnter user name: ",
    (char *) NULL
};

static char *ip_parameters_menu[] = {
    "\r\n\tIP Parameters:\r\n",
    "\t1.) IP Address",
    "\t2.) Subnet Mask",
    "\t3.) Default Gateway",
    "\t4.) SNMP Management Station(s)\r\n",
    "\t5.) Change SNMP Send Trap timeout\r\n",
    "\t6.) Change I am Alive Trap Period\r\n",
	"\t7.) Reboot Chassis\r\n",
    "\t8.) Main Menu\r\n",
    "\r\nEnter Selection: ",
    (char *) NULL
};

static char *ip_addr_menu[] = {
    "\r\nEnter IP address: ",
    (char *) NULL
};

char *subnet_mask_menu[] = {
    "\r\nEnter subnet mask: ",
    (char *) NULL
};

static char *gateway_addr_menu[] = {
    "\r\nEnter gateway address: ",
    (char *) NULL
};

static char *snmp_send_trap_timeout_menu[] = {
    "\r\nEnter new SNMPSendTrap timeout: ",
    (char *) NULL
};

// Added by Hazem

static char *snmp_send_alive_trap_period_menu[] = {
    "\r\nEnter new SNMP Alive Trap period 0=off, valid range = 3-20 minutes: ",
    (char *) NULL
};

static char *chassis_reboot_menu[] = {
    "\r\nHit any character key to proceed or Enter to quit rebooting: ",
    (char *) NULL
};

static char *snmp_station_edit_menu[] = {
    "\r\n\tSNMP Management Station Edit:\r\n",
    "\t1.) Remove SNMP Management Station\r\n",
    "\t2.) Main Menu\r\n",
    "\r\nEnter Selection: ",
    (char *) NULL
};

static char *snmp_community_parameters_menu[] = {
    "\r\n\tSNMP Community Strings:\r\n",
    "\t1.) Public Community String\r\n",
    "\t2.) Private Community String\r\n",
    "\t3.) Main Menu\r\n",
    "\r\nEnter Selection: ",
    (char *) NULL
};

static char *snmp_ip_addr_menu[] = {
    "\r\nEnter SNMP management station address: ",
    (char *) NULL
};

static char *rtc_parameters_menu[] = {
    "\r\n\tRTC Parameters:\r\n",
    "\t1.) Change the RTC Settings\r\n",
    "\t2.) Main Menu\r\n",
    "\r\nEnter Selection: ",
    (char *) NULL
};

static char *initial_rtc_prompt[] = {
    "\r\nEnter the year [0...99]:           ",
    (char *) NULL
};

static char *firmware_upgrade_menu[] = {
    "\r\n\tFirmware Upgrade:\r\n\r\n",
    "\t1.) Display flash bank information\r\n",
    "\t2.) Upgrade boot loader with rom.bin\r\n",
    "\t3.) Upgrade bank 1 with image.bin\r\n",
    "\t4.) Upgrade bank 2 with image.bin\r\n",
    "\t5.) Confirm active bank.\r\n",
    "\t6.) Switch active bank.\r\n",
    "\t7.) Main Menu\r\n",

    "\r\nEnter Selection: ",
    (char *) NULL
};

static char *timeout_prompt[] = {
    "\r\nTimeout Session Edit:\r\n\r\n"
	"\r\nHit 'Enter' to check current timeout value or\r\n"
	"Enter new timeout in seconds [30...3600]: ", 
    (char *) NULL
};

// updated serial number prompt 
static char *serial_number_menu[] = {
	"\r\nHit Any Key to display current Serial Number...\r\n",
    //"\r\nHit Any Key to display current Serial #\r\nor Enter new serial #: ",
    (char *) NULL		/* GORD redundant? */
};

static char *mac_address_menu[] = {
    "\r\nEnter new MAC address ( format A1:2B:C3:4D:E5:6F ) ",
    (char *) NULL
};

static char *alarm_menu[] = {
    "\r\nHit Enter to Display Active Alarms... \r\n ",
    (char *) NULL
};

//-----------------------------------------------------------------------------


/*
 * This is the array of menus for telnet, the array must
 * correspond to the enum in telnmenu.h which is used as an index
 * into this array.  Each member in the array contains a function
 * which is called when an item is selected in that menu.  Also
 * a function pointer is provided which is called to retrieve
 * the current value.
 */
struct telnet_menu_t telnet_menu_array[] = {
    {
     handle_main_menu,
     main_menu,
     (int (*)(int, char *)) NULL},
    {
     handle_user_password_menu,
     user_password_menu,
     (int (*)(int, char *)) NULL},
//.........................................................
    {
     handle_snmp_edit_menu,	// send SNMP Mgmt Station Edit menu session
     snmp_station_edit_menu,	// the SNMP Management Station Edit menu 
     get_snmp_ip_parameters,	// display the current values
     },
    {
     handle_snmp_station_edit_menu,	// edit and update app_nvParams
     snmp_ip_addr_menu,
     (int (*)(int, char *)) NULL},
//  Add the community strings pieces in here
    {
     handle_snmp_community_menu,
     snmp_community_parameters_menu,
     },
    {
     handle_public_community_password_menu,
     strings_password_menu,
     (int (*)(int, char *)) NULL},
    {
     handle_private_community_password_menu,
     strings_password_menu,
     (int (*)(int, char *)) NULL},
//.........................................................
    {
     handle_ip_parameters_menu,	// send the IP Parameters menu session
     ip_parameters_menu,	// the IP Parameters menu display
     get_ip_parameters		// display the current values
     },
    {
     handle_ip_addr_menu,	// process the user input
     ip_addr_menu,
     (int (*)(int, char *)) NULL},
    {
     handle_subnet_mask_menu,
     subnet_mask_menu,
     (int (*)(int, char *)) NULL},

    {
     handle_gateway_addr_menu,
     gateway_addr_menu,
     (int (*)(int, char *)) NULL},
    {
     handle_snmp_send_trap_timeout_menu,
     snmp_send_trap_timeout_menu,
     (int (*)(int, char*)) NULL},
    {  // Added by Hazem for the Alive Trap Period
     handle_snmp_send_alive_trap_period_menu,
     snmp_send_alive_trap_period_menu,
     (int (*)(int, char*)) NULL}, 
	   // Added by Hazem for CLI Chassis Reboot
	{
	 handle_chassis_reboot_menu,
	 chassis_reboot_menu,
	 (int (*)(int, char *)) NULL},
    {				// modified 1800 06-27-05 - moved here
     handle_snmp_ip_addr_menu,
     snmp_ip_addr_menu,
     (int (*)(int, char *)) NULL},
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 09 10 05 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Add the RTC pieces here
    {
     handle_rtc_menu,
     rtc_parameters_menu,
     },
    {
     handle_rtc_parameters_menu,
     initial_rtc_prompt,
     (int (*)(int, char *)) NULL},
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 09 10 05 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
     handle_firmware_upgrade_menu,
     firmware_upgrade_menu,
     (int (*)(int, char *)) NULL},
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 09 13 05 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
     handle_timeout_menu,
     timeout_prompt,
     (int (*)(int, char *)) NULL},
    {
     handle_serial_number_menu,
     serial_number_menu,
     (int (*)(int, char *)) NULL},
    {
     handle_mac_address_menu,
     mac_address_menu,
     (int (*)(int, char *)) NULL},
	{
     handle_alarm_menu,
     alarm_menu,
     (int (*)(int, char *)) NULL},
    {
     (int (*)(char *, int, unsigned long)) NULL,
     (char **) 0,
     (int (*)(int, char *)) NULL},
};


//................. for debug .............................
//   unsigned long snmp_ip_addrs[MAX_STATIONS];
//.........................................................


//-----------------------------------------------------------------------------


// Send the specified menu

int telnet_send_menu(unsigned long session_id)
{
    int i = 0, ret;

    char **menu;

    unsigned char BankFlag;

    TS_SESSION_TYPE *session_ptr;


    session_ptr = &TS_session_tab[session_id];

    menu = telnet_menu_array[session_ptr->menu].menu_items;


    GetFlashBankFlag(&BankFlag);

    if (BankFlag & FLASH_USE_BANK2)
	TSSendString(session_id, "Flash Bank 2 is selected ");
    else
	TSSendString(session_id, "Flash Bank 1 is selected ");

    if (BankFlag & FLASH_UNCONFIRMED)
	TSSendString(session_id, "but not confirmed.\r\n");
    else
	TSSendString(session_id, "and confirmed.\r\n");


    for (i = 0; *(menu + i) != (char *) NULL; i++) {
	ret = TSSendString(session_id, *(menu + i));
	if (ret < 0) {
//            printf("telnetd: send failed [%d]\n", getErrno());
	    return -1;
	}
	// Try to get the current value
	if (telnet_menu_array[session_ptr->menu].current_value_func !=
	    (int (*)(int, char *)) NULL) {
	    ret =
		telnet_menu_array[session_ptr->menu].current_value_func(i,
									session_ptr->
									buf);
	    if (ret == 0) {
		TSSendString(session_id, session_ptr->buf);
		TSSendString(session_id, "\r\n");
	    }
	}
    }

    return 0;
}


//-----------------------------------------------------------------------------


// Decode and handle functions from the main menu.

int handle_main_menu(char *cmd, int len, unsigned long session_id)
{
    int item;

    devBoardParamsType app_nvParams;
    TS_SESSION_TYPE *session_ptr;
    unsigned int access = getAccessLevel(session_id);


    session_ptr = &TS_session_tab[session_id];

    item = atoi(cmd);

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    if (app_nvParams.telnetMagic != MAGIC) {
	TSSendString(session_id,
		     "Unable to change info because internal record is corrupted\r\n");
	return (-1);
    }


    switch (item) {
    case 1:
	if (access >= NASYSACC_LEVEL_ADMINISTRATOR) {
	    session_ptr->menu = TS_IP_PARAMETERS_MENU;
	} else {
	    sprintf(session_ptr->buf,
		    "You are not authorized to modify IP parameters.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}
	break;

    case 2:
	session_ptr->menu = TS_USER_PASSWORD_MENU;
	session_ptr->state = 0;
	break;

    case 3:
	if (access >= NASYSACC_LEVEL_ADMINISTRATOR) {
	    session_ptr->menu = TS_SNMP_EDIT_MENU;
	} else {
	    sprintf(session_ptr->buf,
		    "You are not authorized to modify SNMP parameters.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}
	break;

    case 4:
	if (access >= NASYSACC_LEVEL_ADMINISTRATOR) {
	    session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
	} else {
	    sprintf(session_ptr->buf,
		    "You are not authorized to modify SNMP parameters.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}
	break;

    case 5:
	session_ptr->menu = TS_RTC_MENU;
	break;

    case 6:
	session_ptr->menu = TS_FIRMWARE_UPGRADE;
	break;

    case 7:
	if (access >= NASYSACC_LEVEL_ROOT) {
	    session_ptr->menu = TS_TIMEOUT;
	} else {
	    sprintf(session_ptr->buf,
		    "You are not authorized to change the timeout.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}

	break;

    case 8:
	session_ptr->menu = TS_SERIAL_NUMBER;
	break;

    case 9:
	session_ptr->menu = TS_MAC_ADDRESS;
	break;

	case 10:
	session_ptr->menu = TS_ALARM;
	break;


    case 11:
	return (-1);
	break;

    default:
	sprintf(session_ptr->buf, "Invalid selection [%d]\r\n", item);
	TSSendString(session_id, session_ptr->buf);
	break;
    }

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------


int handle_user_password_menu(char *cmd, int len, unsigned long session_id)
{
    int display = 0, result;
    unsigned int level;
    TS_SESSION_TYPE *session_ptr;
    static char password[NASYSACC_STRLEN_PASSWORD],
	username[NASYSACC_STRLEN_PASSWORD];
    unsigned char ipAddr[4];
    char strIpAddr[16];

    devBoardParamsType app_nvParams;

    session_ptr = &TS_session_tab[session_id];

    switch (session_ptr->state) {
    case 0:			// User name
	if (NAgetSysAccess(cmd, password, 0)) {
	    if (!strcmp(cmd, TSgetSessionUsername(session_id))) {
		strcpy(username, cmd);
		session_ptr->state = 1;
		TSSendString(session_id, "\r\nEnter old password: ");
//                TSSetServerOption (session_id, TS_CAMOUFLAGE, 1);
	    } else		// not the current user's name
	    {
		if (NAgetSysAccess
		    (TSgetSessionUsername(session_id), password,
		     0) >= NASYSACC_LEVEL_ROOT) {
		    if (NAgetSysAccess(cmd, password, 0)) {
			strcpy(username, cmd);
			session_ptr->state = 2;
			/* root doesn't have to enter old password */
			TSSendString(session_id,
				     "\r\nEnter new password: ");
//                        TSSetServerOption (session_id, TS_CAMOUFLAGE, 1);
		    } else	// user name not found
		    {
			sprintf(session_ptr->buf,
				"User name '%s' not found.\r\n", cmd);
			TSSendString(session_id, session_ptr->buf);
//                        TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
			session_ptr->menu = TS_MAIN_MENU;
			display = 1;
		    }
		} else		// user is not root
		{
		    sprintf(session_ptr->buf,
			    "You are not authorized to change the password for '%s'.\r\n",
			    cmd);
		    TSSendString(session_id, session_ptr->buf);
//                    TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
		    session_ptr->menu = TS_MAIN_MENU;
		    display = 1;
		}
	    }
	} else {
	    sprintf(session_ptr->buf, "Invalid user name '%s'.\r\n", cmd);
	    TSSendString(session_id, session_ptr->buf);
//            TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
	    session_ptr->menu = TS_MAIN_MENU;
	    display = 1;
	}
	break;
    case 1:
	/*  Old Password    */
	level = NAgetSysAccess(username, password, 0);

	/*** Get new password ***/
	if (strcmp(password, cmd) == 0) {
	    TSSendString(session_id, "\r\nEnter new password: ");
//            TSSetServerOption (session_id, TS_CAMOUFLAGE, 1);
	    session_ptr->state = 2;
	} else {
	    TSSendString(session_id,
			 "\r\nError: Invalid password: Requires valid password to change!\r\n ");
//            TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
	    session_ptr->menu = TS_MAIN_MENU;
	    display = 1;
	}
	break;

    default:
    case 2:
	if (strlen(cmd) > NASYSACC_STRLEN_PASSWORD) {
	    TSSendString(session_id,
			 "\r\nError: Invalid password: New password too long.\r\n ");
	    session_ptr->state = 1;
	} else {
	    TSSendString(session_id, "\r\nReenter new password: ");
	    strcpy(password, cmd);
	    session_ptr->state = 3;
	}
	break;

    case 3:
	if (strcmp(password, cmd) == 0) {
	    level =
		NAgetSysAccess(username, password,
			       (unsigned int *) &ipAddr[0]);

	    sprintf(strIpAddr, "%d.%d.%d.%d", ipAddr[0], ipAddr[1],
		    ipAddr[2], ipAddr[3]);

	    result =
		NAsetSysAccess(NASYSACC_DEL, username, password, level,
			       NULL);
	    if (result != 0) {
//                printf ("Could not remove account [%d].\n", result);
	    }

	    result =
		NAsetSysAccess(NASYSACC_ADD, username, cmd, level, NULL);
	    if (result != 0) {
//                printf ("Could not add account [%d].\n", result);
	    }

	    TSSendString(session_id, "\r\nNew password accepted\r\n");
//            TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);

	    customizeReadDevBoardParams(&app_nvParams);
	    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	    if (level == NASYSACC_LEVEL_ROOT)
		strcpy(app_nvParams.rootPassword, cmd);
	    if (level == NASYSACC_LEVEL_ADMINISTRATOR)
		strcpy(app_nvParams.adminPassword, cmd);
	    if (level == NASYSACC_LEVEL_RW)
		strcpy(app_nvParams.security_pwd, cmd);
	    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	    customizeWriteDevBoardParams(&app_nvParams);	// write back into NVM

	    session_ptr->menu = TS_MAIN_MENU;
	    display = 1;
	} else {
	    TSSendString(session_id,
			 "\r\nError: Password mismatch, new and retyped passwords do not match!\r\n ");
//            TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
	    session_ptr->menu = TS_MAIN_MENU;
	    display = 1;
	}
	session_ptr->state = 0;
	break;
    }

    if (display) {
	if (telnet_send_menu(session_id) < 0) {
	    return -1;
	}
    }

    return (0);
}


//-----------------------------------------------------------------------------


int handle_private_community_password_menu(char *cmd, int len,
					   unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;

    devBoardParamsType app_nvParams;
    int display = 0, result;

    static char new_password[32], password[32];

    unsigned int i, match;

//.............................................................................


    session_ptr = &TS_session_tab[session_id];

    switch (session_ptr->state) {
    case 0:
	strcpy(password, cmd);

	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	if (!strcmp(app_nvParams.mgmtStations[0].privateString, password)) {
	    TSSendString(session_id, "\r\nEnter new password: ");
	    session_ptr->state = 1;
	    break;
	} else {
	    TSSendString(session_id,
			 "\r\nError: Invalid password: Requires valid password to change!\r\n ");
//          TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
	    session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
	    session_ptr->state = 0;
	    display = 1;
	    break;
	}

    case 1:
	if (strlen(cmd) > NASYSACC_STRLEN_PASSWORD) {
	    TSSendString(session_id,
			 "\r\nError: Invalid password: New password too long.\r\n ");
	    session_ptr->state = 0;
	} else {
	    TSSendString(session_id, "\rReenter new password: ");
	    strcpy(new_password, cmd);
	    session_ptr->state = 2;
	}
	break;

    case 2:
	if (strcmp(new_password, cmd) == 0) {

	    customizeReadDevBoardParams(&app_nvParams);
	    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	    result =
		NAChangePassword(app_nvParams.mgmtStations[0].
				 privateAccountName, cmd);
	    if (result != 0) {
		TSSendString(session_id,
			     "\r\nNew password NOT accepted\r\n");
//             TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
		session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
		display = 1;
	    } else {
		TSSendString(session_id,
			     "\r\nNew private password accepted.  Updating.  Please wait.\r\n");
//             TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
		session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
		display = 1;

		customizeReadDevBoardParams(&app_nvParams);
		root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

		strcpy(app_nvParams.mgmtStations[0].privateString,
		       new_password);

		root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
		customizeWriteDevBoardParams(&app_nvParams);
		configNotification(PrivateCommunityStringEvent, "none");
	    }
	} else {
	    TSSendString(session_id,
			 "\r\nError: Password mismatch, new and retyped passwords do not match!\r\n ");
	}

	session_ptr->state = 0;
	break;

    default:
	break;
    }

    if (display) {
	if (telnet_send_menu(session_id) < 0) {
	    return -1;
	}
    }

    return (0);
}


//-----------------------------------------------------------------------------


int handle_public_community_password_menu(char *cmd, int len,
					  unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;

    devBoardParamsType app_nvParams;
    int display = 0, result;

    static char new_password[32], password[32];

    unsigned int i, match;

    unsigned long ip_addr;


//.............................................................................


    session_ptr = &TS_session_tab[session_id];

    switch (session_ptr->state) {
    case 0:
	strcpy(password, cmd);

	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	if (!strcmp(app_nvParams.mgmtStations[0].publicString, password)) {
	    TSSendString(session_id, "\r\nEnter new password: ");
	    session_ptr->state = 1;
	    break;
	} else {
	    TSSendString(session_id,
			 "\r\nError: Invalid password: Requires valid password to change!\r\n ");
//          TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
	    session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
	    session_ptr->state = 0;
	    display = 1;
	    break;
	}

    case 1:
	if (strlen(cmd) > NASYSACC_STRLEN_PASSWORD) {
	    TSSendString(session_id,
			 "\r\nError: Invalid password: New password too long.\r\n ");
	    session_ptr->state = 0;
	} else {
	    TSSendString(session_id, "\rReenter new password: ");
	    strcpy(new_password, cmd);
	    session_ptr->state = 2;
	}
	break;

    case 2:
	if (strcmp(new_password, cmd) == 0) {
	    result =
		NAChangePassword(app_nvParams.mgmtStations[0].
				 publicAccountName, cmd);
	    if (result != 0) {
		TSSendString(session_id,
			     "\r\nNew password NOT accepted\r\n");
//             TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
		session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
		display = 1;
	    } else {
		TSSendString(session_id,
			     "\r\nNew public password accepted.  Updating.  Please wait.\r\n");
//             TSSetServerOption (session_id, TS_CAMOUFLAGE, 0);
		session_ptr->menu = TS_SNMP_COMMUNITY_MENU;
		display = 1;

		customizeReadDevBoardParams(&app_nvParams);
		root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

		strcpy(app_nvParams.mgmtStations[0].publicString,
		       new_password);

		root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
		customizeWriteDevBoardParams(&app_nvParams);
		configNotification(PublicCommunityStringEvent, "none");
	    }
	} else {
	    TSSendString(session_id,
			 "\r\nError: Password mismatch, new and retyped passwords do not match!\r\n ");
	}

	session_ptr->state = 0;
	break;

    default:
	break;
    }

    if (display) {
	if (telnet_send_menu(session_id) < 0) {
	    return -1;
	}
    }

    return (0);
}


//-----------------------------------------------------------------------------


int handle_rtc_parameters_menu(char *cmd, int len,
			       unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;

    static int day,
	dayOfTheWeek, hour, minute, month, result, seconds, year;

    int display = 0;

    rtcDataType timedate;

    static char *rtc_prompts[] = {
	"\rEnter the year    [0...99]:        ",
	"\rEnter the month   [1...12]:        ",
	"\rEnter the day     [1...31]:        ",
	"\rEnter the day of the week [1...7]: ",
	"\rEnter the hour    [0...23]:        ",
	"\rEnter the minute  [0...59]:        ",
	"\rEnter the seconds [0...59]:        "
    };

    unsigned long gpioSelector;
    int status;

    session_ptr = &TS_session_tab[session_id];

    switch (session_ptr->state) {
    case 0:
	len = strlen(cmd);
	if (len > 0)
	    year = atoi(cmd);
	else
	    year = -1;

	if ((year < 0) || (year > 99)) {
	    TSSendString(session_id, "\r\nError: Invalid year.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 0;
	    break;
	} else {
	    session_ptr->state++;
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);	// obtain month value
	    break;
	}

    case 1:
	len = strlen(cmd);
	if (len > 0)
	    len = strlen(cmd);
	if (len > 0)
	    month = atoi(cmd);
	else
	    month = -1;
	if ((month < 1) || (month > 12)) {
	    TSSendString(session_id, "\r\nError: Invalid month.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 1;
	    break;
	} else {
	    session_ptr->state++;
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);	// obtain day value
	    break;
	}

    case 2:
	len = strlen(cmd);
	if (len > 0)
	    day = atoi(cmd);
	else
	    day = -1;
	if ((day < 1) || (day > 31)) {
	    TSSendString(session_id, "\r\nError: Invalid day.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 2;
	    break;
	} else {
	    session_ptr->state++;
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);	// obtain day of the week value
	    break;
	}

    case 3:
	len = strlen(cmd);
	if (len > 0)
	    dayOfTheWeek = atoi(cmd);
	else
	    dayOfTheWeek = -1;
	if ((dayOfTheWeek < 1) || (dayOfTheWeek > 7)) {
	    TSSendString(session_id,
			 "\r\nError: Invalid day_of_the_week.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 3;
	    break;
	} else {
	    session_ptr->state++;
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);	// obtain the hour value
	    break;
	}

    case 4:
	len = strlen(cmd);
	if (len > 0)
	    hour = atoi(cmd);
	else
	    hour = -1;
	if ((hour < 0) || (hour > 23)) {
	    TSSendString(session_id, "\r\nError: Invalid hour.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 4;
	    break;
	} else {
	    session_ptr->state++;
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);	// obtain the minute value
	    break;
	}

    case 5:
	len = strlen(cmd);
	if (len > 0)
	    minute = atoi(cmd);
	else
	    minute = -1;
	if ((minute < 0) || (minute > 59)) {
	    TSSendString(session_id, "\r\nError: Invalid minute.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 5;
	    break;
	} else {
	    session_ptr->state++;
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);	// obtain the seconds value
	    break;
	}

    case 6:
	len = strlen(cmd);
	if (len > 0)
	    seconds = atoi(cmd);
	else
	    seconds = -1;
	if ((seconds < 0) || (seconds > 59)) {
	    TSSendString(session_id, "\r\nError: Invalid seconds.\r\n ");
	    TSSendString(session_id, rtc_prompts[session_ptr->state]);
	    session_ptr->state = 6;
	    break;
	} else {
	    TSSendString(session_id, "\r\nUpdating.  Please wait.\r\n");

	    timedate.year = year;
	    timedate.month = month;
	    timedate.day = day;
	    timedate.dayOfWeek = dayOfTheWeek;
	    timedate.hours = hour;
	    timedate.minutes = minute;
	    timedate.seconds = seconds;

#if BSP_INCLUDE_RTC_DRIVER
            status = semaphore_get_2 (&semaphore_rtc_info, SEMAPHORE_TMO, 30001);
            if ( TX_SUCCESS == status ) {
	        if (rtcSet(&timedate) != RTC_SUCCESS) {
		    TSSendString(session_id, "ERROR: rtcSet() failed!\r\n");
	        }
                semaphore_put_2 (&semaphore_rtc_info, 30001);
            } else {
                TSSendString(session_id, "ERROR: rtcSet() failed (semaphore)!\r\n");
            }
#endif

	    configNotification(EARMIB_earRtcTime, "none");

	    session_ptr->menu = TS_RTC_MENU;
	    display = 1;
	    session_ptr->state = 0;
	    break;
	}

    default:
	break;
    }

    if (display) {
	if (telnet_send_menu(session_id) < 0) {
	    return -1;
	}
    }

    return (0);
}


//-----------------------------------------------------------------------------

static unsigned char earSerialNumber_Read_buffer[MAX_2U_RESPONSE];
int handle_serial_number_menu(char *cmd, int len, unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;
	void *resultBuffer = NULL;
	unsigned long bufferLength = MAX_2U_RESPONSE;
    devBoardParamsType app_nvParams;
    //int count;
    //char temp[BP_SERIAL_NUMBER_LENGTH /* GORD need space for eos */  + 1];

    /* GORD Recall that the space in the struct includes a +1 too */

    session_ptr = &TS_session_tab[session_id];

	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    /* won't bother writing if obviously no change */
    //if (len > 0) {
	/* GORD copy as much as we can into the serial number */
	//if (len > BP_SERIAL_NUMBER_LENGTH) {
	//    len = BP_SERIAL_NUMBER_LENGTH;
	//}

	//for (count = 0; count < len; count++) {
	//    app_nvParams.serialNumber[count] = *cmd++;
	//}
	/* jam a null at the end to make it a string */
	//app_nvParams.serialNumber[count] = 0;
	//root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	//customizeWriteDevBoardParams(&app_nvParams);
	//}
	/* GORD show the user now */
	TSSendString(session_id, "Serial number is: ");

	/*resultBuffer = askAtmelForStringValue
        ("ZIMRSN",
         "ZRSN",
         E_READ_FROM_2U_NO_ADJUST,
         E_READ_FROM_2U_NO_CHECKSUM,
         4, //retries 
         &(earSerialNumber_Read_buffer[0]),
		 &bufferLength,
         10157);*/
	resultBuffer = &Mib_Leaves->earSerialNumber;
    TSSendString(session_id, resultBuffer);
 
	//TSSendString(session_id, app_nvParams.serialNumber);
	TSSendString(session_id, "\r\n\r\n");

    /* back where we came from */
    session_ptr->menu = TS_MAIN_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return (0);
}


//-----------------------------------------------------------------------------


int handle_mac_address_menu(char *cmd, int len, unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;
    BYTE MacAddr[6];
    devBoardParamsType app_nvParams;

    int count;

    session_ptr = &TS_session_tab[session_id];

    if (NAConvertMacAddressString(MacAddr, cmd) == BP_SUCCESS) {
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	memcpy(app_nvParams.ethernetMac, MacAddr, 6);

	TSSendString(session_id, "New MAC Address:\t");
	sprintf(session_ptr->buf, "%02x:%02x:%02x %02x:%02x:%02x",
		app_nvParams.ethernetMac[0],
		app_nvParams.ethernetMac[1],
		app_nvParams.ethernetMac[2],
		app_nvParams.ethernetMac[3],
		app_nvParams.ethernetMac[4], app_nvParams.ethernetMac[5]);
	TSSendString(session_id, session_ptr->buf);
	TSSendString(session_id, ".\r\n");
	TSSendString(session_id,
		     "Reboot for new MAC address to take effect.\r\n");

	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	customizeWriteDevBoardParams(&app_nvParams);
    } else {
	TSSendString(session_id, "Error:Invalid MAC Address Format.\r\n");
    }

    /* back where we came from */
    session_ptr->menu = TS_MAIN_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return (0);
}


//-----------------------------------------------------------------------------
// Send the TCP/IP parameters menu.

int handle_ip_parameters_menu(char *cmd, int len, unsigned long session_id)
{
//.........................................................
    int i;
    int item;

    TS_SESSION_TYPE *session_ptr;

    //no customizeReadDevBoardParams (&app_nvParams);
    //no root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    session_ptr = &TS_session_tab[session_id];
    item = atoi(cmd);

    switch (item) {
    case 1:			// New IP address
	session_ptr->menu = TS_IP_ADDRESS_MENU;
	break;

    case 2:			// New subnet mask
	session_ptr->menu = TS_SUBNET_MASK_MENU;
	break;

    case 3:			// New gateway address
	session_ptr->menu = TS_GATEWAY_ADDRESS_MENU;
	break;

    case 4:			// New SNMP mgmt. station
	session_ptr->menu = TS_SNMP_IP_ADDRESS_MENU;
	break;

    case 5:
        session_ptr->menu = TS_SNMP_SEND_TRAP_TIMEOUT_MENU;
        break;
    
    case 6:
        session_ptr->menu = TS_SNMP_SEND_ALIVE_TRAP_PERIOD_MENU;
        break;

	case 7:
		session_ptr->menu = TS_CHASSIS_REBOOT_MENU;
		break;

    case 8:			// Go back to the main menu
	if (session_ptr->state) {
	    session_ptr->state = 0;

	    //no TSSendString (session_id, "\r\nUpdating.  Please wait.\r\n");

	    //no root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	    //no app_nvParams.telnetMagic = MAGIC;                               
	    //no customizeWriteDevBoardParams(&app_nvParams);
	}

	session_ptr->menu = TS_MAIN_MENU;
	break;

    default:
	sprintf(session_ptr->buf, "Invalid selection [%d]\r\n", item);
	break;
    }

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------


// Send the SNMP Management Station IP Address Edit menu.

int handle_snmp_edit_menu(char *cmd, int len, unsigned long session_id)
{
    int item;

    //devBoardParamsType  app_nvParams;
    TS_SESSION_TYPE *session_ptr;


    session_ptr = &TS_session_tab[session_id];
    item = atoi(cmd);
    //customizeReadDevBoardParams (&app_nvParams);
    //root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    switch (item) {
    case 1:			// Edit SNMP Management Station IP address
	session_ptr->menu = TS_SNMP_ADDRESS_EDIT_MENU;
	break;

    case 2:			// Back to the Main Menu
	//if (session_ptr->state)
	//{
	//   TSSendString (session_id, "\r\nUpdating.  Please wait.\r\n");
	//   session_ptr->state = 0;
	//   app_nvParams.telnetMagic = MAGIC;                          
	//   root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	//   customizeWriteDevBoardParams(&app_nvParams);// will write back into NVM
	//}

	session_ptr->menu = TS_MAIN_MENU;
	break;

    default:
//       printf("telnet: invalid selection [%d]\n", item);
	sprintf(session_ptr->buf, "Invalid selection [%d]\r\n", item);
	break;
    }

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return 0;
}


//-------------------------------------------------------------------------------------


// Send the SNMP Community String menu

int handle_snmp_community_menu(char *cmd, int len,
			       unsigned long session_id)
{
    int item;

    devBoardParamsType app_nvParams;
    TS_SESSION_TYPE *session_ptr;


    session_ptr = &TS_session_tab[session_id];
    item = atoi(cmd);
    //customizeReadDevBoardParams (&app_nvParams);
    //root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    switch (item) {
    case 1:
	session_ptr->menu = TS_SNMP_PUBLIC_COMMUNITY_MENU;
	break;

    case 2:
	session_ptr->menu = TS_SNMP_PRIVATE_COMMUNITY_MENU;
	break;

    case 3:			// Back to the Main Menu
	//if (session_ptr->state)
	//{
	//   session_ptr->state = 0;
	//   root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	//   app_nvParams.telnetMagic = MAGIC;                          
	//   customizeWriteDevBoardParams(&app_nvParams);
	//}
	session_ptr->menu = TS_MAIN_MENU;
	break;

    default:
	sprintf(session_ptr->buf, "Invalid selection [%d]\r\n", item);
	break;
    }

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
const   char  *AlarmReportStrings[] =
    {
    "Temperature\r",
    "Fan Failure\r",
    "PreAmp Out Too High\r",
    "PreAmp Out High\r",
    "PreAmp Out Low\r",
    "PreAmp In Too High\r",
    "PreAmp In High\r",
    "PreAmp In Low\r",
    "PreAmp Out Off\r",
    "PreAmp In Off\r",
    "Interlock\r",
    "Power Supply 1\r",
    "Power Supply 2\r",
    "Power Safety Mode\r",
    "System Fuse\r",
    "48V Input A\r",
    "48V Input B\r",
    "Booster In Too High\r",
    "Booster In High\r",
    "Booster In Low\r",
    "Booster In Off\r",
    "Booster Out Off\r",
    "Booster Out Low\r",
    "Booster Out High\r",
    "Booster Out Too High\r",
    "Pump Current\r",
    "Back Refl Too High\r"
    };

/*const   unsigned int AlarmTable[] =
    {
    ALARM_TEMPERATURE_FLAG,
    ALARM_FAN_FLAG,
    ALARM_OUTPUT_TOO_HIGH_FLAG,
    ALARM_OUTPUT_HIGH_FLAG,
    ALARM_OUTPUT_LOW_FLAG,
    ALARM_INPUT_TOO_HIGH_FLAG,
    ALARM_INPUT_HIGH_FLAG,
    ALARM_INPUT_LOW_FLAG,
    ALARM_OUTPUT_OFF_FLAG,
    ALARM_INPUT_OFF_FLAG,
    ALARM_INTERLOCK_FLAG,
    ALARM_PS1_FAIL_FLAG,
    ALARM_PS2_FAIL_FLAG,
    ALARM_POWER_SAFETY_FLAG,
    ALARM_FUSE_FLAG,
    ALARM_48V_A_FLAG,
    ALARM_48V_B_FLAG,
    ALARM_INPUT2_TOO_HIGH_FLAG,
    ALARM_INPUT2_HIGH_FLAG,
    ALARM_INPUT2_LOW_FLAG,
    ALARM_INPUT2_OFF_FLAG,
    ALARM_OUTPUT2_OFF_FLAG,
    ALARM_OUTPUT2_LOW_FLAG,
    ALARM_OUTPUT2_HIGH_FLAG,
    ALARM_OUTPUT2_TOO_HIGH_FLAG,
    ALARM_PUMP_FLAG,
    ALARM_BACK_REF_TOO_HIGH_FLAG
    };
*/
#define NUMBER_OF_ALARM_MESSAGES    (28) // Update if added more Alalrms - (sizeof(AlarmTable) / sizeof(AlarmTable[0]))

int handle_alarm_menu(char *cmd, int len, unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;
    devBoardParamsType app_nvParams;
	unsigned long bufferLength = MAX_2U_RESPONSE;
	unsigned int LoopIndex;
	unsigned long alarmStatus = 0;
	unsigned long alrmStatus;
	unsigned long bitmask = 0x00000001;

    session_ptr = &TS_session_tab[session_id];
/*
	if ( 0 != askAtmelForUnsignedValue ("ZIMRAL",
                                        "ZRAS",
                                        E_READ_FROM_2U_DO_ADJUST,
                                        E_READ_FROM_2U_DO_CHECKSUM,
                                        4, // retries
                                        &alarmStatus,
                                        20203) ) 
	{  */
	alarmStatus = Mib_Leaves->earAlarmStatus;
	//Mib_Leaves->earAlarmStatus = alarmStatus;
	sprintf(session_ptr->buf, "Raw Alarm Word = 0X%08X\r\n", alarmStatus);
	TSSendString(session_id, session_ptr->buf);
    if (alarmStatus == 0)
    {
        TSSendString(session_id, "No Alarms!\r\n");
    }
    else
    {
		alarmStatus &= 0xFFFFFFF;
		TSSendString(session_id, "\r\nActive Alarms:\r\n");
        for (LoopIndex = 0; LoopIndex < NUMBER_OF_ALARM_MESSAGES; LoopIndex++)
        {
			alrmStatus = alarmStatus & bitmask;
            if (alrmStatus == bitmask)
            {
				//sprintf(session_ptr->buf, "0X%08X\r\n");
				//TSSendString(session_id, session_ptr->buf);
				switch(alrmStatus)
				{
				case (1):
					sprintf(session_ptr->buf, "Temperature High\r\n");
					break;
				case (2):
					sprintf(session_ptr->buf, "Fan Failure\r\n");
					break;
				case (4):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Output Too High\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp Out Too High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster Out Too High    \r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Output Too High\r\n");
						break;
					}
					break;
				case (8):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Output High\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp Out High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 Out High\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Output High\r\n");
						break;
					}
					break;
				case (16):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Output Low\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp Out Low\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 Out Low\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Output Low\r\n");
						break;
					}
					break;
				case (32):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Input Too High\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp In Too High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 In Too High\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Input Too High\r\n");
						break;
					}
					break;	
				case (64):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Input High\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp In High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 In High\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Input High\r\n");
						break;
					}
					break;
				case (0x80):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Input Low\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp In Low\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 In Low\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Input Low\r\n");
						break;
					}
					break;
				case (0x100):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Output Off\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp Out Off\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 Out Off\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Output Off\r\n");
						break;
					}
					break;
				case (0x200):
					switch (productSWType)
					{
					case (0):
						sprintf(session_ptr->buf, "Input Off\r\n");
						break;
					case (1):
						sprintf(session_ptr->buf, "PreAmp In Off\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster1 In Off\r\n");
						break;
					default:
						sprintf(session_ptr->buf, "Input Off\r\n");
						break;
					}
					break;
				case (0x400):
					sprintf(session_ptr->buf, "Interlock\r\n");
					break;
				case (0x800):
					sprintf(session_ptr->buf, "Power Supply 1\r\n");
					break;
				case (0x1000):
					sprintf(session_ptr->buf, "Power Supply 2\r\n");
					break;	
				case (0x2000):
					sprintf(session_ptr->buf, "Power Safety Mode\r\n");
					break;	
				case (0x4000):
					if (!PwrType)
					{
						sprintf(session_ptr->buf, "System Fuse\r\n");
					}
					else
					{
						sprintf(session_ptr->buf, "              \r\n");
					}
					break;				
				case (0x8000):
					if (!PwrType)
					{
						sprintf(session_ptr->buf, "48V A OFF\r\n");
					}
					else
					{
						sprintf(session_ptr->buf, "              \r\n");
					}
					break;
				case (0x10000):
					if (!PwrType)
					{
						sprintf(session_ptr->buf, "48V B OFF\r\n");
					}
					else
					{
						sprintf(session_ptr->buf, "              \r\n");
					}
					break;				
				case (0x20000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster In Too High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 In Too High\r\n");
						break;
					default:
						break;
					}
					break;
				case (0x40000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster In High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 In High\r\n");
						break;
					default:
						break;
					}
					break;			
				case (0x80000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster In Low\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 In Low\r\n");
						break;
					default:
						break;
					}
					break;
				case (0x100000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster In Off\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 In Off\r\n");
						break;
					default:
						break;
					}
					break;		
				case (0x200000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster Out Off\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 Out Off\r\n");
						break;
					default:
						break;
					}
					break;		
				case (0x400000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster Out Low\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 Out Low\r\n");
						break;
					default:
						break;
					}
					break;
				case (0x800000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster Out High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 Out High\r\n");
						break;
					default:
						break;
					}
					break;
				case (0x1000000):
					switch (productSWType)
					{
					case (1):
						sprintf(session_ptr->buf, "Booster Out Too High\r\n");
					case (2):
						sprintf(session_ptr->buf, "Booster2 Out Too High\r\n");
						break;
					default:
						break;
					}
					break;
				case (0x2000000):
					sprintf(session_ptr->buf, "Pump Current\r\n");
					break;
				case (0x4000000):
					sprintf(session_ptr->buf, "Back Refl Too High\r\n");
					break;	
				case (0x8000000):
					sprintf(session_ptr->buf, "Key Switch OFF\r\n");
					break;			
				default:
					break;
				}
				TSSendString(session_id, session_ptr->buf);
            }
			bitmask = bitmask << 1;
		}
	}
    
	//}
    /* back where we came from */
    session_ptr->menu = TS_MAIN_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return (0);
}

//-------------------------------------------------------------------------------------

extern unsigned int g_readFrom2U2_cant_happen;
// Send the RTC menu
void stack_check(TS_SESSION_TYPE * session_ptr, unsigned long session_id,
		 unsigned char *Begin, int Size, TX_THREAD thread)
{
    unsigned int guardget();
    unsigned int guardgot;
    unsigned int now;

    if ( (0 == thread.tx_stack_ptr) || (0 == session_ptr) ) {
      return;
    }

    sprintf(session_ptr->buf, "%s: ", thread.tx_thread_name);
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "Ste %d ", thread.tx_state);
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "run %d ", thread.tx_run_count);
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "sta %06p siz %d ", Begin, Size);
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "hig %d ",
	    find_highwater(Begin + STACKGUARD_SIZE, STACKMARK,
			   Size - STACKGUARD_SIZE - STACKGUARD_SIZE));
    TSSendString(session_id, session_ptr->buf);

    now = (thread.tx_stack_end - thread.tx_stack_ptr);
    sprintf(session_ptr->buf, "now %d, ", now);
    TSSendString(session_id, session_ptr->buf);

    /* check the beginning stack guard */
    sprintf(session_ptr->buf, "grd ");
    TSSendString(session_id, session_ptr->buf);

    guardgot = guardget(Begin, STACKGUARD_SIZE);
    if (guardgot == STACKGUARD) {
	sprintf(session_ptr->buf, "bok ");
    } else {
	sprintf(session_ptr->buf, "bng ");
    }
    TSSendString(session_id, session_ptr->buf);

    /* check the ending stack guard */
    guardgot = guardget(Begin + Size - STACKGUARD_SIZE, STACKGUARD_SIZE);
    if (guardgot == STACKGUARD) {
	sprintf(session_ptr->buf, "eok");
    } else {
	sprintf(session_ptr->buf, "eng");
    }
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "\r\n");
    TSSendString(session_id, session_ptr->buf);
}
#if 0
void semaphore_check(TS_SESSION_TYPE * session_ptr,
		     unsigned long session_id, TX_SEMAPHORE * semi)
{
    sprintf(session_ptr->buf, "%s: ", semi->tx_semaphore_name);
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, " count %ld ", semi->tx_semaphore_count);
    TSSendString(session_id, session_ptr->buf);

    switch (semi->tx_semaphore_count) {
    case 0:
	sprintf(session_ptr->buf, "take ");
	break;
    case 1:
	sprintf(session_ptr->buf, "okay ");
	break;
    default:
	sprintf(session_ptr->buf, "mbad ");
	break;
    };
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "\r\n");
    TSSendString(session_id, session_ptr->buf);

}
#endif

void semaphore_check_2 (TS_SESSION_TYPE * session_ptr,
			unsigned long session_id, semaphore_info_t * sem_info) {

    sprintf (session_ptr->buf, "%s", sem_info->sem->tx_semaphore_name);
    TSSendString (session_id, session_ptr->buf);

    sprintf(session_ptr->buf, " count %ld ",
	    sem_info->sem->tx_semaphore_count);
    TSSendString (session_id, session_ptr->buf);

    switch (sem_info->sem->tx_semaphore_count) {
    case 0:
	sprintf(session_ptr->buf, "take ");
	break;
    case 1:
	sprintf(session_ptr->buf, "okay ");
	break;
    default:
	sprintf(session_ptr->buf, "mbad ");
	break;
    };
    TSSendString(session_id, session_ptr->buf);

    sprintf (session_ptr->buf, "last get %d", *(sem_info->last_get));
    TSSendString(session_id, session_ptr->buf);

    sprintf (session_ptr->buf, "  put %d", *(sem_info->last_put));
    TSSendString(session_id, session_ptr->buf);

    sprintf (session_ptr->buf, "  gotten %d", *(sem_info->gotten));
    TSSendString(session_id, session_ptr->buf);

    sprintf (session_ptr->buf, "  tries %d", *(sem_info->tries));
    TSSendString(session_id, session_ptr->buf);

    sprintf (session_ptr->buf, "  unwedged %d", *(sem_info->unwedged));
    TSSendString(session_id, session_ptr->buf);

    sprintf (session_ptr->buf, "  failed-get %d", *(sem_info->get_failed));
    TSSendString(session_id, session_ptr->buf);

    sprintf(session_ptr->buf, "\r\n");
    TSSendString(session_id, session_ptr->buf);

}

extern int g_readFrom2U2_count;
extern int g_readFrom2U2_garbage_in_buffer;
extern int g_readFrom2U2_reset_control_variable;
extern int g_readFrom2U2_getchar_calls;
extern int g_readFrom2U2_returns;
extern int g_readFrom2U2_EOS;
extern int g_readFrom2U2_mark;
extern int g_readFrom2U2_2U;

extern int g_alarmApp_caret;
#if 0
extern int g_alarmCheck_caret;
#endif
int handle_rtc_menu(char *cmd, int len, unsigned long session_id)
{
    int item;

    devBoardParamsType app_nvParams;
    TS_SESSION_TYPE *session_ptr;
    long unsigned storedNum;

    session_ptr = &TS_session_tab[session_id];
    item = atoi(cmd);
    //customizeReadDevBoardParams (&app_nvParams);
    //root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    switch (item) {
    case 1:			// Edit the RTC settings
	session_ptr->menu = TS_RTC_PROMPT_MENU;
	break;

    case 2:			// Back to the Main Menu
	//if (session_ptr->state)
	//{
	//   session_ptr->state = 0;
	//   root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	//   app_nvParams.telnetMagic = MAGIC;                          
	//   customizeWriteDevBoardParams(&app_nvParams);
	//}
	session_ptr->menu = TS_MAIN_MENU;
	break;

	/*GORD do a stack check */
    case 3:

	stack_check(session_ptr, session_id, snmpStackBegin, snmpStackSize,
		    snmpTrap_app);

	stack_check(session_ptr, session_id, telnetStackBegin,
		    telnetStackSize, telnet_app);

	stack_check(session_ptr, session_id, alarmStackBegin,
		    alarmStackSize, alarmTrap_app);
	semaphore_check_2 (session_ptr, session_id, &semaphore_0_info);
	semaphore_check_2 (session_ptr, session_id, &semaphore_nvParams_info);
	semaphore_check_2 (session_ptr, session_id, &semaphore_rtc_info);
	semaphore_check_2 (session_ptr, session_id,
                           &semaphore_history_table_info);
        semaphore_check_2 (session_ptr, session_id, &semaphore_sbuf_info);

        break;

    case 4:
        sprintf(session_ptr->buf,
                "readFrom2U2: cant[%d] gbg[%d] reset[%d] "
                "getc[%d] crs[%d] EOS[%d] mk[%d] 2U[%d]\r\n",
                g_readFrom2U2_cant_happen,
                g_readFrom2U2_garbage_in_buffer,
                g_readFrom2U2_reset_control_variable,
                g_readFrom2U2_getchar_calls,
                g_readFrom2U2_returns,
                g_readFrom2U2_EOS,
                g_readFrom2U2_mark,
                g_readFrom2U2_2U);
        TSSendString(session_id, session_ptr->buf);

#if 0
        sprintf(session_ptr->buf,
                "readFrom2U:  alarmApp[%d] alarmCheck[%d] total[%d]\r\n",
                g_alarmApp_caret,
                g_alarmCheck_caret,
                g_alarmApp_caret + g_alarmCheck_caret);
#endif
        sprintf(session_ptr->buf,
                "readFrom2U:  alarmApp[%d]\r\n", g_alarmApp_caret);
        TSSendString(session_id, session_ptr->buf);

	break;

    case 5:
	semaphore_check_2 (session_ptr, session_id, &semaphore_rtc_info);

        sprintf (session_ptr->buf,
	         "SNMPSendTrap:  toggle [%d]  idx[%d]\r\n",
	         g_snmp_toggle, g_snmp_mgmt_stn_idx);
	TSSendString(session_id, session_ptr->buf);

	if ( RTC_SUCCESS == rtcReadExtraData (&storedNum) ) {
	    sprintf (session_ptr->buf,
		     "rtcExtraData [%08x]\r\n",
		     storedNum);
	} else {
	    sprintf (session_ptr->buf,
		     "rtcExtraData failed read\r\n");
	}
	TSSendString (session_id, session_ptr->buf);
        break;

    case 6:
        sprintf (session_ptr->buf,
                 "SNMPSendTrap:  timeout[%d]\r\n",
                 g_snmp_send_trap_timeout);
        TSSendString(session_id, session_ptr->buf);

        sprintf (session_ptr->buf,
                 "SNMPSendTrap: num failures:[%d]; last error return[%d]\r\n",
                 g_snmp_send_trap_failed, g_snmp_send_trap_last_result);
        TSSendString(session_id, session_ptr->buf);
        break;

    case 7: {
        unsigned char buffer_[64];
        unsigned int bufLen = 64;
        int status = 0;

        status = getLineFromSerialBuffer (&(buffer_[0]), &bufLen);
        switch ( status ) {

        case SERIAL_READ_FAILED_READ:
            sprintf (session_ptr->buf,
                     "I regret to inform you that getLine FAILED (getchar)\r\n");
            break;
        case SERIAL_READ_NO_SEMAPHORE:
            sprintf (session_ptr->buf,
                     "I regret to inform you that getLine FAILED (semaphore)\r\n");
            break;
        case SERIAL_READ_EMPTY:
            sprintf (session_ptr->buf, "There was no line to get\r\n");
        case SERIAL_READ_SUCCESS:
            sprintf (session_ptr->buf, "getLine len[%d] string[%s]\r\n", bufLen,
                     &(buffer_[0]));
            break;
        case SERIAL_READ_TRUNCATE:
            sprintf (session_ptr->buf,
                     "getLine len[%d] string[%s] (truncated)\r\n", bufLen,
                     &(buffer_[0]));
            break;
        default:
            break;
        }
        TSSendString(session_id, session_ptr->buf);
    }
        break;

    case 8:
        {
            unsigned char BankFlag;

            TSSendString (session_id, "\r\nRemoving safeguards for flash upgrades");

            GetFlashBankFlag (&BankFlag);
            BankFlag &= ~FLASH_UNCONFIRMED;
            BankFlag &= ~FLASH_NEVER_LOADED;
            SetFlashBankFlag (BankFlag);

            TSSendString (session_id, ".\r\n");
        }
        break;

    case 9:
      {
	long unsigned oldNum;
	RTC_CCODE rtcReadExtraDataError;
	RTC_CCODE rtcWriteExtraDataError;

        /* read the rtc index and increment it */
	rtcReadExtraDataError = rtcReadExtraData (&oldNum);
        if ( RTC_SUCCESS == rtcReadExtraDataError ) {
	    sprintf (session_ptr->buf,
		     "rtcReadExtraData:  read[%08x] successfully\r\n",
		     oldNum);
	    TSSendString(session_id, session_ptr->buf);

	    ++oldNum;
	    rtcWriteExtraDataError = rtcWriteExtraData (oldNum);
	    if ( RTC_SUCCESS == rtcWriteExtraDataError ) {
	        sprintf (session_ptr->buf,
			 "rtcWriteExtraData:  write[%08x] successfully\r\n",
			 oldNum);
		TSSendString (session_id, session_ptr->buf);
	    } else {
	        sprintf (session_ptr->buf,
			 "rtcWriteExtraData:  failed.  error [%d]\r\n",
			 rtcWriteExtraDataError);
		TSSendString(session_id, session_ptr->buf);
	    }
	  
	} else {
	    sprintf (session_ptr->buf,
		     "rtcReadExtraData:  failed.  error [%d]\r\n",
		     rtcReadExtraDataError);
	    TSSendString(session_id, session_ptr->buf);
	}
      }
	break;

    default:
	sprintf(session_ptr->buf, "Invalid selection [%d]\r\n", item);
	break;
    }

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return 0;
}


//-------------------------------------------------------------------------------------


int get_ip_parameters(int item, char *buffer)
{
    unsigned long gateway;
    unsigned long mask;
    unsigned long ip_addr;

    devBoardParamsType app_nvParams;
    int ret = 0;
    int count = 0;
    int i;
    int j;

    unsigned long snmp_ip_addrs[MAX_STATIONS];

    char tmpBuf[256];

    //for (i = 0; i < MAX_STATIONS; ++i) /*GORD should use BL_ */
    //   snmp_ip_addrs[i] = 0;

    //for (i = 0; i < 256; ++i) tmpBuf[i] = '\0';


//.........................................................

    switch (item) {
    case 1:			// IP address
	buffer[0] = 0;
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	ip_addr =
	    app_nvParams.aceConfig.ace_interface_info[0].static_config.
	    ip_address;
#ifndef _LITTLE_ENDIAN		// Insert tabs so it lines up
	sprintf(buffer, "\t\t%ld.%ld.%ld.%ld",
		(ip_addr >> 24) & 0xff,
		(ip_addr >> 16) & 0xff,
		(ip_addr >> 8) & 0xff, ip_addr & 0xff);
#else
	sprintf(buffer, "\t\t%ld.%ld.%ld.%ld",
		(ip_addr) & 0xff,
		(ip_addr >> 8) & 0xff,
		(ip_addr >> 16) & 0xff, ip_addr >> 24 & 0xff);
#endif

	break;

    case 2:			// Subnet mask
	buffer[0] = 0;
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	mask =
	    app_nvParams.aceConfig.ace_interface_info[0].static_config.
	    subnet_mask;
#ifndef _LITTLE_ENDIAN
	sprintf(buffer, "\t\t%ld.%ld.%ld.%ld",
		(mask >> 24) & 0xff,
		(mask >> 16) & 0xff, (mask >> 8) & 0xff, mask & 0xff);
#else
	sprintf(buffer, "\t\t%ld.%ld.%ld.%ld",
		(mask) & 0xff,
		(mask >> 8) & 0xff,
		(mask >> 16) & 0xff, mask >> 24 & 0xff);
#endif

	break;

    case 3:			// Gateway
	buffer[0] = 0;
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	gateway =
	    app_nvParams.aceConfig.ace_interface_info[0].static_config.
	    gateway;
#ifndef _LITTLE_ENDIAN
	sprintf(buffer, "\t%ld.%ld.%ld.%ld",
		(gateway >> 24) & 0xff,
		(gateway >> 16) & 0xff,
		(gateway >> 8) & 0xff, gateway & 0xff);
#else
	sprintf(buffer, "\t%ld.%ld.%ld.%ld",
		(gateway) & 0xff,
		(gateway >> 8) & 0xff,
		(gateway >> 16) & 0xff, gateway >> 24 & 0xff);
#endif
	break;

    case 4:
	buffer[0] = 0;
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	for (i = 0; i < MAX_STATIONS; i++) {
	    /* start at the beginning of the buffer */
	    tmpBuf[0] = 0;
	    snmp_ip_addrs[i] = app_nvParams.mgmtStations[i].ipAddress;

#ifndef _LITTLE_ENDIAN
	    sprintf(tmpBuf, "\t\t\t\t%d %ld.%ld.%ld.%ld\n\r",
		    i + 1,
		    (snmp_ip_addrs[i] >> 24) & 0xff,
		    (snmp_ip_addrs[i] >> 16) & 0xff,
		    (snmp_ip_addrs[i] >> 8) & 0xff,
		    snmp_ip_addrs[i] & 0xff);
#else
	    sprintf(tmpBuf, "\t\t\t\t%d %ld.%ld.%ld.%ld\n\r",
		    i + 1,
		    (snmp_ip_addrs[i]) & 0xff,
		    (snmp_ip_addrs[i] >> 8) & 0xff,
		    (snmp_ip_addrs[i] >> 16) & 0xff,
		    snmp_ip_addrs[i] >> 24 & 0xff);
#endif
	    strcat(buffer, tmpBuf);
	}
	break;

    default:
	ret = -1;
	break;
    }

    return (ret);
}


//-----------------------------------------------------------------------------------------

#define TMPBUF_SIZE 256
int get_snmp_ip_parameters(int item, char *buffer)
{
    devBoardParamsType app_nvParams;
    char tmpBuf[TMPBUF_SIZE];

    int i;
    int idx;
    int ret = 0;

    unsigned long snmp_ip_addrs[MAX_STATIONS];


    //for (i = 0; i < MAX_STATIONS; ++i) snmp_ip_addrs[i] = 0;

    memset (tmpBuf, ASCII_NULL, TMPBUF_SIZE);

    switch (item) {
    case 1:
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

	for (i = 0; i < MAX_STATIONS; i++) {
            idx = i + 1;

	    snmp_ip_addrs[i] = app_nvParams.mgmtStations[i].ipAddress;

#ifndef _LITTLE_ENDIAN
	    sprintf(buffer, "ld.%ld.%ld.%ld\n\r",
		    i + 1,
		    (snmp_ip_addrs[i] >> 24) & 0xff,
		    (snmp_ip_addrs[i] >> 16) & 0xff,
		    (snmp_ip_addrs[i] >> 8) & 0xff,
		    snmp_ip_addrs[i] & 0xff);
#else
	    sprintf(tmpBuf, "\t\t\t\t%d %ld.%ld.%ld.%ld\n\r",
		    i + 1,
		    (snmp_ip_addrs[i]) & 0xff,
		    (snmp_ip_addrs[i] >> 8) & 0xff,
		    (snmp_ip_addrs[i] >> 16) & 0xff,
		    snmp_ip_addrs[i] >> 24 & 0xff);
#endif
	    strcat(buffer, tmpBuf);
	}
	break;

    default:
	ret = -1;
	break;
    }

    return (ret);
}



//-----------------------------------------------------------------------------------------


int handle_ip_addr_menu(char *cmd, int len, unsigned long session_id)
{
    unsigned long ip_addr;
    TS_SESSION_TYPE *session_ptr;

    devBoardParamsType app_nvParams;

    session_ptr = &TS_session_tab[session_id];

    ip_addr = NAInet_addr(cmd);

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    app_nvParams.aceConfig.ace_interface_info[0].static_config.ip_address =
	ip_addr;

    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
    customizeWriteDevBoardParams(&app_nvParams);

    sprintf(session_ptr->buf, "New IP address [%s] accepted\r\n", cmd);
    TSSendString(session_id, session_ptr->buf);
    configNotification(EARMIB_earIpAddress, "none");

    session_ptr->state++;
    session_ptr->menu = TS_IP_PARAMETERS_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return (-1);
    }
    return (0);
}


//-----------------------------------------------------------------------------------------


int handle_snmp_station_edit_menu(char *cmd, int len,
				  unsigned long session_id)
{
    int i, intfIndex, serverAddrIndex;

    unsigned int result = 1;

    unsigned long snmp_ip_addr;

    communityStrings snmp_ip_addrs[MAX_STATIONS];

    devBoardParamsType app_nvParams;

    TS_SESSION_TYPE *session_ptr;
    int addressAccepted = FALSE;

//.........................................................


    session_ptr = &TS_session_tab[session_id];
    snmp_ip_addr = NAInet_addr(cmd);	// obtain user input

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    for (i = 0; i < MAX_STATIONS; ++i) {
	if (app_nvParams.mgmtStations[i].ipAddress == snmp_ip_addr) {
	    app_nvParams.mgmtStations[i].ipAddress = 0;

	    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	    customizeWriteDevBoardParams(&app_nvParams);

	    SyncSnmpFromNvram(&app_nvParams);

	    configNotification(SNMPMgmtStationRemoveEvent, "none");
	    addressAccepted = TRUE;

	    break;
	}
    }

    if (addressAccepted) {
	sprintf(session_ptr->buf, "SNMP IP address [%s] removed\r\n", cmd);
    } else {
	sprintf(session_ptr->buf, "SNMP IP address [%s] not removed\r\n",
		cmd);
    }

    TSSendString(session_id, session_ptr->buf);

    session_ptr->state++;
    session_ptr->menu = TS_SNMP_EDIT_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return (-1);
    }

    return (0);
}


//-----------------------------------------------------------------------------------------


int handle_snmp_ip_addr_menu(char *cmd, int len, unsigned long session_id)
{

    unsigned long snmp_ip_addr;

    TS_SESSION_TYPE *session_ptr;

    devBoardParamsType app_nvParams;

    session_ptr = &TS_session_tab[session_id];
    //char sIpAddr[16]; 

//.........................................................

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    snmp_ip_addr = NAInet_addr(cmd);

    if (AddSnmpMngrEntry(&app_nvParams, snmp_ip_addr) != -1) {

	/* Add has overwritten the right place. we have to commit it. */
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	customizeWriteDevBoardParams(&app_nvParams);

	/* clue in snmp too */
	SyncSnmpFromNvram(&app_nvParams);

	/* and anybody listening */
	configNotification(SNMPMgmtStationAddEvent, "none");

	sprintf(session_ptr->buf, "New SNMP IP address [%s] accepted\r\n",
		cmd);
    } else {
	sprintf(session_ptr->buf,
		"New SNMP IP address [%s] unaccepted\r\n", cmd);
    }

    TSSendString(session_id, session_ptr->buf);

    session_ptr->state++;
    session_ptr->menu = TS_IP_PARAMETERS_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return (-1);
    }

    return (0);
}


//-----------------------------------------------------------------------------------------


int handle_subnet_mask_menu(char *cmd, int len, unsigned long session_id)
{
    unsigned long subnet_mask;

    TS_SESSION_TYPE *session_ptr;
    devBoardParamsType app_nvParams;

    session_ptr = &TS_session_tab[session_id];

    subnet_mask = NAInet_addr(cmd);

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    app_nvParams.aceConfig.ace_interface_info[0].static_config.
	subnet_mask = subnet_mask;

    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
    customizeWriteDevBoardParams(&app_nvParams);

    sprintf(session_ptr->buf, "New subnet mask [%s] accepted\r\n", cmd);
    TSSendString(session_id, session_ptr->buf);

    configNotification(EARMIB_earSubnetMask, "none");

    session_ptr->state++;
    session_ptr->menu = TS_IP_PARAMETERS_MENU;

    if (telnet_send_menu(session_id) < 0) {
	return (-1);
    }

    return (0);
}


//-----------------------------------------------------------------------------


int handle_gateway_addr_menu(char *cmd, int len, unsigned long session_id)
{

    unsigned long gateway_addr;

    devBoardParamsType app_nvParams;
    TS_SESSION_TYPE *session_ptr;


    session_ptr = &TS_session_tab[session_id];

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    gateway_addr = NAInet_addr(cmd);


    app_nvParams.aceConfig.ace_interface_info[0].static_config.gateway =
	gateway_addr;


    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
    customizeWriteDevBoardParams(&app_nvParams);

    sprintf(session_ptr->buf, "New gateway address [%s] accepted\r\n",
	    cmd);
    TSSendString(session_id, session_ptr->buf);

    configNotification(EARMIB_earGateway, "none");

    session_ptr->state++;
    session_ptr->menu = TS_IP_PARAMETERS_MENU;

    if (telnet_send_menu(session_id) < 0) {
//              printf("telnet: couldn't send menu\n");
	return (-1);
    }

    return (0);
}


//-----------------------------------------------------------------------------


int handle_snmp_send_trap_timeout_menu(char *cmd, int len, unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;
    unsigned long new_timeout;

    session_ptr = &TS_session_tab[session_id];
    new_timeout = strtol (cmd, 0, 0);
    /* new timeout must be bigger than SNMP_SEND_TRAP_TIMEOUT_MIN */
    if ( SNMP_SEND_TRAP_TIMEOUT_MIN < new_timeout ) {
        sprintf(session_ptr->buf,
		"New snmp send trap timeout [%s] accepted; was [%d]\r\n",
		cmd, g_snmp_send_trap_timeout);
        TSSendString(session_id, session_ptr->buf);

        g_snmp_send_trap_timeout = new_timeout;
    } else {
        sprintf (session_ptr->buf,
	         "New snmp send trap timeout [%s] REJECTED; keep [%d]\r\n",
		 cmd, g_snmp_send_trap_timeout);
        TSSendString(session_id, session_ptr->buf);
    }
    session_ptr->state++;
    session_ptr->menu = TS_IP_PARAMETERS_MENU;

    if (telnet_send_menu(session_id) < 0) {
//              printf("telnet: couldn't send menu\n");
	return (-1);
    }

    return (0);
}

int handle_snmp_send_alive_trap_period_menu(char *cmd, int len,
				 unsigned long session_id)
{
	//devBoardParamsType app_nvParams;
	TS_SESSION_TYPE *session_ptr;
    unsigned int new_period;
	char buffer [MAX_2U_RESPONSE];

    session_ptr = &TS_session_tab[session_id];
    new_period = atoi (cmd);
    /* new timeout must be bigger than SNMP_SEND_TRAP_TIMEOUT_MIN */
    if ((( SNMP_SEND_ALIVE_TRAP_PERIOD_MIN <= new_period ) &&
	     ( SNMP_SEND_ALIVE_TRAP_PERIOD_MAX >= new_period )) || (!new_period)) {
		   //customizeReadDevBoardParams(&app_nvParams);
           sprintf(session_ptr->buf,
		   "New snmp alive trap period [%s] accepted; was [%d]\r\n",
			cmd, g_snmp_send_alive_trap_period);	    
		   //cmd, app_nvParams.aliveTrapPeriod);
           TSSendString(session_id, session_ptr->buf);
           //app_nvParams.aliveTrapPeriod = new_period;
           //customizeWriteDevBoardParams(&app_nvParams);
		   if (!AliveTimerNVRamSave)
		   {
			   // Retain locally
			   g_snmp_send_alive_trap_period = new_period;
			   TrapPeriod = new_period;
		   }
		   else
		   {
			   // otherwise send it to Atmel to save in NVRAM and 
			   // then it will be read in root.c check_Alarm Task.
			   sprintf(buffer, "ZIMSALIVETIMER %d\r", new_period);
			   printf("%s", buffer);
			   fflush(stdout);
		   }
    } else {
           sprintf (session_ptr->buf,
	       "New snmp alive trap period [%s] REJECTED; keep [%d]\r\n",
		   cmd, g_snmp_send_alive_trap_period);
           TSSendString(session_id, session_ptr->buf);
    }
    session_ptr->state++;
    session_ptr->menu = TS_IP_PARAMETERS_MENU;

    if (telnet_send_menu(session_id) < 0) {
//              printf("telnet: couldn't send menu\n");
	return (-1);
    }

    return (0);
}
//-----------------------------------------------------------------------------
int handle_chassis_reboot_menu(char *cmd, int len,
				 unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;
    session_ptr = &TS_session_tab[session_id];
    
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
	//pin21_dir, pin29_mode, bit5_data, bit21_dir, bit29_mode, bit13_csf;

	int result = SNMP_ERR_NOERROR;
	int status;

	if (strlen(cmd) > 0) {
       // Initialyze GPIO PORTC5 to Output and set it high first if not already
       narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, mode,
                    narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, mode) & ~gpio[5].bit);
       narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, dir,
                    narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, dir) | gpio[5].bit);
       narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, reserved,
                    narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, reserved) & ~gpio[5].bit);
	   narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, data, 
					narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, data) | gpio[5].bit);

	   /* we're going to get serious now, so get the semaphore */
	   /* semaphore not needed (not talking to Atmel over serial port)
	    * but I see there were timing issues in the step above so leave it in
	    */
	   status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, 23231);
	   if (status != TX_SUCCESS) {
	   	  return (-1);
	   }
	   sprintf(session_ptr->buf,
		      "System Resetting, please standby ....\r\n");
       TSSendString(session_id, session_ptr->buf);
	   tx_thread_sleep(300);	/* let the message propagate if made it over here*/
       // Reset the system
	   narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, data, 
					narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, data) & ~gpio[5].bit);
	   status = semaphore_put_2 (&semaphore_0_info, 23231);
	}
	/* We are rebooting anyway.  It does not matter here.  But... */
	session_ptr->state++;
	session_ptr->menu = TS_IP_PARAMETERS_MENU;

	if (telnet_send_menu(session_id) < 0) {
	   return (-1);
	}

    return (0);
}

//-----------------------------------------------------------------------------
int handle_firmware_upgrade_menu(char *cmd, int len,
				 unsigned long session_id)
{
    int item;

    TS_SESSION_TYPE *session_ptr;

    unsigned char BankFlag;

    unsigned short Major;
    unsigned short Minor;


    session_ptr = &TS_session_tab[session_id];
    item = atoi(cmd);

    switch (item) {
    case 1:			// display version information
#if FTP_DIRECT_TO_FLASH == 0
	// version of fw running now
	sprintf(session_ptr->buf, "Firmware version %d.%d is running.\r\n",
		MAJOR_VERSION, MINOR_VERSION);
	TSSendString(session_id, session_ptr->buf);

	// app bank selected in flash
	GetFlashBankFlag(&BankFlag);
	if (BankFlag & FLASH_USE_BANK2)
	    TSSendString(session_id, "Flash Bank 2 is selected ");
	else
	    TSSendString(session_id, "Flash Bank 1 is selected ");

	if (BankFlag & FLASH_UNCONFIRMED)
	    TSSendString(session_id, "but not confirmed");
	else
	    TSSendString(session_id, "and confirmed");

	if (BankFlag & FLASH_NEVER_LOADED)
	    TSSendString(session_id, " and has not been loaded.\r\n");
	else
	    TSSendString(session_id, ".\r\n");

	// versions in flash banks
	GetFlashBankVersion(1, &Major, &Minor);
	sprintf(session_ptr->buf,
		"Flash Bank 1 contains Version %d.%d.\r\n", Major, Minor);
	TSSendString(session_id, session_ptr->buf);

	GetFlashBankVersion(2, &Major, &Minor);
	sprintf(session_ptr->buf,
		"Flash Bank 2 contains Version %d.%d.\r\n", Major, Minor);
	TSSendString(session_id, session_ptr->buf);
#endif
	break;

    case 2:			// Program boot loader
#if FTP_DIRECT_TO_FLASH == 0
	sprintf(session_ptr->buf,
		"Starting a copy of rom.bin into the bootloader area of flash.\r\n");
	TSSendString(session_id, session_ptr->buf);
	ProgramBootLoader();
	sprintf(session_ptr->buf,
		"Completed copying rom.bin into the bootloader area of flash.\r\n");
	TSSendString(session_id, session_ptr->buf);
	session_ptr->menu = TS_MAIN_MENU;
#endif
	break;

//      case 2:                                                 // Upgrade flash bank 1
    case 3:			// Upgrade flash bank 1
#if FTP_DIRECT_TO_FLASH == 0
	// determine which bank is currently active
	GetFlashBankFlag(&BankFlag);
	if (BankFlag & FLASH_USE_BANK2) {
	    if (!(BankFlag & FLASH_UNCONFIRMED)
		&& !(BankFlag & FLASH_NEVER_LOADED)) {
		// make sure that the bank we are running from has been confirmed
		sprintf(session_ptr->buf,
			"Starting a copy of image.bin into flash bank 1.\r\n");
		TSSendString(session_id, session_ptr->buf);
		ProgramAppToFlash(1, "FLASH0/", "image.bin");
		sprintf(session_ptr->buf,
			"Completed copying image.bin into flash bank 1.\r\n");
		TSSendString(session_id, session_ptr->buf);
		SetFlashBankFlag(FLASH_NEVER_LOADED | FLASH_UNCONFIRMED);
	    } else {
		sprintf(session_ptr->buf,
			"Cannot program until the active bank is confirmed and followed by a reboot.\r\n");
		TSSendString(session_id, session_ptr->buf);
	    }
	} else {
	    sprintf(session_ptr->buf,
		    "Cannot program bank 1 while bank 1 is active.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}
#endif
	break;

//      case 3:                                                 // Upgrade flash bank 2
    case 4:			// Upgrade flash bank 2
#if FTP_DIRECT_TO_FLASH == 0
	// determine which bank is currently active
	GetFlashBankFlag(&BankFlag);
	if (!(BankFlag & FLASH_USE_BANK2)) {
	    if (!(BankFlag & FLASH_UNCONFIRMED)
		&& !(BankFlag & FLASH_NEVER_LOADED)) {
		// make sure that the bank we are running from has been confirmed
		sprintf(session_ptr->buf,
			"Starting a copy of image.bin into flash bank 2.\r\n");
		TSSendString(session_id, session_ptr->buf);
		ProgramAppToFlash(2, "FLASH0/", "image.bin");
		sprintf(session_ptr->buf,
			"Completed copying image.bin into flash bank 2.\r\n");
		TSSendString(session_id, session_ptr->buf);
		SetFlashBankFlag(FLASH_USE_BANK2 | FLASH_NEVER_LOADED |
				 FLASH_UNCONFIRMED);
	    } else {
		sprintf(session_ptr->buf,
			"Cannot program until the active bank is confirmed and followed by a reboot.\r\n");
		TSSendString(session_id, session_ptr->buf);
	    }
	} else {
	    sprintf(session_ptr->buf,
		    "Cannot program bank 2 while bank 2 is active.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}
#endif
	break;

//      case 4:                                                 // Confirm active bank
    case 5:			// Confirm active bank
#if FTP_DIRECT_TO_FLASH == 0
	GetFlashBankFlag(&BankFlag);
	if (!(BankFlag & FLASH_NEVER_LOADED)) {
	    sprintf(session_ptr->buf,
		    "Starting the confirm of the flash bank.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	    SetFlashBankFlag(BankFlag & ~FLASH_UNCONFIRMED);
	    sprintf(session_ptr->buf, "Confirmation completed.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	    configNotification(EARMIB_earDigiFirmwareConfirm, "none");
	} else {
	    sprintf(session_ptr->buf,
		    "You must reboot before confirming.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	}
#endif
	break;

//      case 5:                                                 // Switch active bank
    case 6:			// Switch active bank
#if FTP_DIRECT_TO_FLASH == 0
	GetFlashBankFlag(&BankFlag);
	if (BankFlag & FLASH_USE_BANK2) {
	    sprintf(session_ptr->buf,
		    "Starting to switch the active flash bank.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	    SetFlashBankFlag(FLASH_NEVER_LOADED | FLASH_UNCONFIRMED);
	    sprintf(session_ptr->buf,
		    "Active flash bank switching operation complete.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	    configNotification(EARMIB_earDigiSelectedFirmwareBank, "none");
	} else {
	    sprintf(session_ptr->buf,
		    "Starting to switch the active flash bank.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	    SetFlashBankFlag(FLASH_USE_BANK2 | FLASH_NEVER_LOADED |
			     FLASH_UNCONFIRMED);
	    sprintf(session_ptr->buf,
		    "Active flash bank switching operation complete.\r\n");
	    TSSendString(session_id, session_ptr->buf);
	    configNotification(EARMIB_earDigiSelectedFirmwareBank, "none");
	}
#endif
	break;

    case 7:			// Back to main menu
	session_ptr->menu = TS_MAIN_MENU;
	break;

    case 1138: // back door menu command to allow quicker programming by production ppl
		sprintf(session_ptr->buf,
			"Starting a copy of image.bin into flash bank 1.\r\n");
		TSSendString(session_id, session_ptr->buf);
		ProgramAppToFlash(1, "FLASH0/", "image.bin");
		sprintf(session_ptr->buf,
			"Completed copying image.bin into flash bank 1.\r\n");
		TSSendString(session_id, session_ptr->buf);

		sprintf(session_ptr->buf,
			"Starting a copy of image.bin into flash bank 2.\r\n");
		TSSendString(session_id, session_ptr->buf);
		ProgramAppToFlash(2, "FLASH0/", "image.bin");
		sprintf(session_ptr->buf,
			"Completed copying image.bin into flash bank 2.\r\n");
		TSSendString(session_id, session_ptr->buf);

		SetFlashBankFlag( 0 ); // mark it as loaded and confirmed
    break;

    default:
	sprintf(session_ptr->buf, "Invalid selection [%d] menu\r\n", item);
	TSSendString(session_id, session_ptr->buf);
	break;
    }

    if (telnet_send_menu(session_id) < 0) {
	return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------


int handle_timeout_menu(char *cmd, int len, unsigned long session_id)
{
    TS_SESSION_TYPE *session_ptr;
    int timeout = 0;

    devBoardParamsType app_nvParams;
    session_ptr = &TS_session_tab[session_id];
    //Added by Hazem to display the current value
    //and added below the option to exit without change
    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
    sprintf(session_ptr->buf,
		"\r\nCurrent Timeout value prior change is %d seconds\r\n",
		app_nvParams.telnetTimeoutS);
    TSSendString(session_id, session_ptr->buf);
    if (strlen(cmd) > 0) {
	   timeout = atoi(cmd);
	}
    // Added by Hazem to allow not to change the value
    else {
      session_ptr->menu = TS_MAIN_MENU;
	  sprintf(session_ptr->buf,
		"\n\rPress Any Key to Continue..\r\n",
		MIN_TELNET_TIMEOUT);
	   TSSendString(session_id, session_ptr->buf);
      return 0;
    }

    if (timeout < MIN_TELNET_TIMEOUT) {
	   sprintf(session_ptr->buf,
		"Timeout is less than minimum of %d seconds\r\n",
		MIN_TELNET_TIMEOUT);
	   TSSendString(session_id, session_ptr->buf);

	   session_ptr->menu = TS_TIMEOUT;
    } else if (timeout > MAX_TELNET_TIMEOUT) {
	   sprintf(session_ptr->buf,
		 "Timeout is more than maximum of %d seconds\r\n",
		 MAX_TELNET_TIMEOUT);
	   TSSendString(session_id, session_ptr->buf);

	   session_ptr->menu = TS_TIMEOUT;
    } else {
	   app_nvParams.telnetTimeoutS = timeout;
	   sprintf(session_ptr->buf, 
			   "Wait while saving timeout prompt value into NVRAM...\r\n\r\n");
	   TSSendString(session_id, session_ptr->buf);
	   
	   root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	   customizeWriteDevBoardParams(&app_nvParams);	// will write back into NVM

	   TimeoutS = timeout;
	   configNotification(EARMIB_earTelnetTimeout, "none");

	   session_ptr->menu = TS_MAIN_MENU;
    }

    if (telnet_send_menu(session_id) < 0) {
	   return (-1);
    }

    return 0;
}

//-----------------------------------------------------------------------------

unsigned int getAccessLevel(unsigned long session_id)
{
    static char password[NASYSACC_STRLEN_PASSWORD];
    unsigned int level =
	NAgetSysAccess(TSgetSessionUsername(session_id), password, 0);
    return level;
}
