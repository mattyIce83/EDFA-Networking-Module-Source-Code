///////////////////////////////////////////////////////////////////////////////
// $Id: root.c,v 1.4 2011-10-11 13:56:53 hazem Exp $
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
//  Filename:           root.c 
//
//  Description:        
//  applicationStart() is the entry point for user applications.  This
//  This function will be called after the kernel has started and the
//  TCP/IP stack has loaded.  applicationTcpDown() is called periodically
//  after the kernel has started while the system is waiting for the
//  TCP/IP stack to start.  
//
//  $Log: root.c,v $
//  Revision 1.4  2011-10-11 13:56:53  hazem
//  This version adds 15Min Trap meassage and Allow to remote reset the
//  system via CLI and SNMP.
//
//  Revision 1.2  2010/03/08 20:08:32  eugene
//  Bug_908: Replaced hardcoded size of the global variables_[] array with
//  the NUM_OIDS_IN_TRAP macro. And the same - for the local variables[]
//  array inside of the alarmCheck() function.
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <tx_api.h>
#include <man_api.h>
#include <snmpapi.h>
#include <sysAccess.h>
#include <termios.h>

#include <http_awsapi.h>
#include <sysAccess.h>
#include "appconf.h"
#include "server.h" /* For username/password */

//--------------------------------------------- pwl + ------------------------------------------

#include "../../bsp/platforms/connectme/bsp.h"
#include "ipgFlash.h"


#include "../../bsp/platforms/connectme/gpio_def.h"

#include <tservapi.h>
#include <fbanks.h>

#include "ace_params.h"
#include "boardParams.h"

#include "ipgStrings.h"
#include "../../../h/fs.h"
#include "../../../h/rtc.h"
#include "../../../h/sysAccess.h"
#include "ascii.h"
#include "earmib.h"

#ifdef ADD_SECURE_AGENT
#include <NAsntp.h>
#include <NASecurity.h>
#include <NAKerberos.h>
#endif

#include <sysAccess.h>
#include "appconf.h"
#include "telnet.h"
#include "root.h"
#include "semaphores.h"
#include "MAGIC.h"
#include "MASK.h"
#include "readFrom2U2.h"
#include "atmel.h"


static char alarmClearName[][30] = {
  "1.3.6.1.4.1.19775.1.1.5.1",
  "1.3.6.1.4.1.19775.1.1.5.2",
  "1.3.6.1.4.1.19775.1.1.5.3",
  "1.3.6.1.4.1.19775.1.1.5.4",
  "1.3.6.1.4.1.19775.1.1.5.5",
  "1.3.6.1.4.1.19775.1.1.5.6",
  "1.3.6.1.4.1.19775.1.1.5.7",
  "1.3.6.1.4.1.19775.1.1.5.8",
  "1.3.6.1.4.1.19775.1.1.5.9",
  "1.3.6.1.4.1.19775.1.1.5.10",
  "1.3.6.1.4.1.19775.1.1.5.11",
  "1.3.6.1.4.1.19775.1.1.5.12",
  "1.3.6.1.4.1.19775.1.1.5.13",
  "1.3.6.1.4.1.19775.1.1.5.14",
  "1.3.6.1.4.1.19775.1.1.5.15",
  "1.3.6.1.4.1.19775.1.1.5.16",
  "1.3.6.1.4.1.19775.1.1.5.17",
  "1.3.6.1.4.1.19775.1.1.5.18",
  "1.3.6.1.4.1.19775.1.1.5.19",
  "1.3.6.1.4.1.19775.1.1.5.20"
};

static char *trap_addr[] = {
  (char *) NULL
};


#include "../../bsp/platforms/connectme/dialog.h"
#include "../../../h/threadx/tx_api.h"
#include "../../../h/man_api.h"
#include "../../../h/snmp/snmp.h"

#include "ipgsnmp.h"
#include "trap.h"
#include "MAX_ALARM.h"
#include "watch.h"

//#define DefaultTrapPeriod         0

#define BUFF_SIZE		   6
#define MAX_BUF			 200
#define MAX_BUF2		 400
#include "STACK_SIZE.h"

#define DEFAULT_ROOT_PWD        "ipg1"
#define DEFAULT_ADMIN_PWD       "photo2"
#define DEFAULT_SECURITY_PWD    "nics3"

#define TYPE_MASK				(SWType & 0x00000007)
#define POWER_MASK				(SWType & 0x00000008) >> 3
#define ALIVE_TRAP_MASK			(SWType & 0x000001F0) >> 4	
#define ALIVE_TRAP_NVRAM_MASK	(SWType & 0x00000200) >> 9
#define SPEC_PATTERN			(SWType & 0x00003C00) >> 10

// Declarations of temporary variables to pass data assigned to Mib_leaves to AWS Web Pages

extern unsigned char earModelNumber_temp[MAX_2U_RESPONSE];
extern unsigned char earSerialNumber_temp[MAX_2U_RESPONSE];
extern unsigned char earDateOfManufacture_temp[MAX_2U_RESPONSE];
extern unsigned char earFirmwareRevision_temp[MAX_2U_RESPONSE];
extern unsigned char earDigiFirmwareRev_temp[MAX_2U_RESPONSE];
extern unsigned char earDigiActiveFB_temp[MAX_2U_RESPONSE];
extern unsigned char earDigiActiveFBConfirmed_temp[MAX_2U_RESPONSE];
extern unsigned char earIpAddress_temp[16];
extern unsigned char earSubnetMask_temp[16];
extern unsigned char earGateway_temp[16];
extern signed char earGainControlSetpoint_temp[MAX_2U_RESPONSE];
extern signed char  earMeasuredGainValue_temp[MAX_2U_RESPONSE];
extern signed char earMinimumGainSetPoint_temp[MAX_2U_RESPONSE];
extern signed char earMaximumGainSetPoint_temp[MAX_2U_RESPONSE];
extern unsigned long earOutputPowerSetpointValue_temp;
extern signed char  earTotalOpticalOutputPower_temp[MAX_2U_RESPONSE];
extern signed char earMaximumPowerSetpoint_temp[MAX_2U_RESPONSE];
extern unsigned long earCurrentSetpointValue_temp;
extern unsigned long earMaximumCurrentSetpoint_temp;
extern unsigned long earPumpLaserCurrent_temp;
extern unsigned long earPumpLaserCurrent2_temp;
extern signed char earAmbientTemperature_temp[MAX_2U_RESPONSE];
extern signed char  earTotalOpticalInputPower_temp[MAX_2U_RESPONSE];
extern unsigned char earPowerSafetyMode_temp[MAX_2U_RESPONSE];
extern unsigned long earPumpOff_temp;
extern signed char  earControlMode_temp[MAX_2U_RESPONSE];
extern signed char  earControlModeFD_temp[MAX_2U_RESPONSE];
extern int earDeviceType_temp;
extern int earDeviceTypeFD_temp;
extern int earDataMode_temp;
extern signed char 	earTemperatureThreshold_temp[MAX_2U_RESPONSE];
extern unsigned long earOSCPowerSetpointValue_temp;
extern unsigned long earOutputPower2SetpointValue_temp;
extern signed char  earTotalOpticalOutputPower2_temp[MAX_2U_RESPONSE];
extern signed char earGainControlSetpoint2_temp[MAX_2U_RESPONSE];
extern signed char  earTotalOpticalInput2Power_temp[MAX_2U_RESPONSE];
extern signed char earTemperatureAlarmThreshold_temp[MAX_2U_RESPONSE];
extern signed char  earInputPowerLowAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earFactoryDefaultInputPowerLowAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char  earInputPowerOffAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earInputPowerHighAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earInputPowerTooHighAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPowerLowAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPowerOffAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPowerHighAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPowerTooHighAlarmLevel_temp[MAX_2U_RESPONSE];
extern unsigned long earStatusValue_temp;
extern unsigned long earRelayAlarmStatus_temp;
extern unsigned long earRelayStatus_temp;
extern unsigned long earAlarmStatus_temp;
extern unsigned long earSetRelayInputPowerLowAlarm_temp;
extern unsigned long earSetRelayInputPowerHighAlarm_temp;
extern unsigned long earSetRelayInputPowerTooHighAlarm_temp;
extern unsigned long earSetRelayOutputPowerLowAlarm_temp;
extern unsigned long earSetRelayOutputPowerHighAlarm_temp;
extern unsigned long earSetRelayOutputPowerTooHighAlarm_temp;
extern unsigned long earSetRelayFanFailureAlarm_temp;
extern unsigned long earSetRelayAmbientTemperatureHighAlarm_temp;
extern signed char earOutputPower2OffAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPower2LowAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPower2HighAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earOutputPower2TooHighAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earInput2PowerLowAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earInput2PowerOffAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earInput2PowerHighAlarmLevel_temp[MAX_2U_RESPONSE];
extern signed char earInput2PowerTooHighAlarmLevel_temp[MAX_2U_RESPONSE];

extern unsigned long temperatureHighAlarm;
extern unsigned long fanFailureAlarm; 
extern unsigned long preAmpOutTooHighAlarm;
extern unsigned long preAmpOutHighAlarm;
extern unsigned long preAmpOutLowAlarm;
extern unsigned long preAmpInTooHighAlarm;
extern unsigned long preAmpInHighAlarm;
extern unsigned long preAmpInLowAlarm;
extern unsigned long preAmpOutOffAlarm;
extern unsigned long preAmpInOffAlarm;
extern unsigned long interlockAlarm;
extern unsigned long powerSupply1Alarm;
extern unsigned long powerSupply2Alarm;
extern unsigned long powerSafetyModeAlarm;
extern unsigned long systemFuseAlarm;
extern unsigned long a48VAOffAlarm;
extern unsigned long b48VBOffAlarm;
extern unsigned long boosterInTooHighAlarm;
extern unsigned long boosterInHighAlarm;
extern unsigned long boosterInLowAlarm;
extern unsigned long boosterInOffAlarm;
extern unsigned long boosterOutOffAlarm;
extern unsigned long boosterOutLowAlarm;
extern unsigned long boosterOutHighAlarm;
extern unsigned long boosterOutTooHighAlarm;
extern unsigned long pumpCurrentAlarm;
extern unsigned long backReflectorAlarm;

//  Function prototypes section

void alarmCheck (int);

#if 0
void processTrap (struct variable_t *varArray, unsigned long *enterpriz);

void telnetStart (void);
void trapStart (void);
#endif
void alarmApp (ULONG thread_input);
void alarmAppRestart ();
void ftpApp (ULONG thread_input);
void monitorAlarmApp (ULONG thread_input);
void monitorSNMPApp (ULONG thread_input);
void snmpApp (ULONG thread_input);
void telnetApp (ULONG thread_input);
void serialApp (ULONG thread_input);
void collectionApp (void);
BOOL isValidPwd (char *pwd);

#if 0
void restorePumpVal (void);
void turnPumpsDown (void);
#endif

void fillTimeBuffer (char * p_timeBuffer, int p_bufSize);

extern void snmpAppRestart ();
extern unsigned long g_snmp_send_trap_timeout;
extern int g_snmp_send_trap_failed;
extern int g_snmp_send_trap_last_result;

extern unsigned int g_snmp_send_alive_trap_period;

TX_THREAD alarmTrap_app;
TX_THREAD snmpTrap_app;
TX_THREAD telnet_app;
TX_THREAD ftp_app;
TX_THREAD serial_app;
//TX_THREAD collection_app;

TX_TIMER timer_snmpApp;		// used in EARMIBAction.c

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char startValue[20];
char endValue[20];

int alarmsCleared;
int alarmsSet;

//why? unsigned long elapsedTime[1000];

//These 2 variable are added to send I am Alive Trap 
//every 15 Minutes.  Arris request.
unsigned int MinCount = 0;
// Initialize to a value > range to indicate 1st time in
unsigned int MinElapse = 60; 
unsigned int TrapPeriod = 15;
unsigned char FirstTimeIn = 1;

unsigned int m = 0;
unsigned int BankIndex = 0;
unsigned short FIRST;
unsigned long SWType = 0;
unsigned char productSWType = 0;
unsigned char PwrType = 0;  //0 = DC and 1 = AC Power Supply. (Bit 3 in mask)
unsigned char AliveTrapTimer; // Valid Range 0 = OFF and 3-20Sec
unsigned char AliveTimerNVRamSave;
unsigned char SpecPattern = 0;

/* trapHistoryTableIndex and manHistoryTableKey are maintained in parallel
 * They should be equal
 *
 * trapHistoryTableIndex
 *    is the unique id that is sent with the trap
 *    to the network management station
 *
 * manHistoryTableKey
 *     is the unique id that is stored in the local management database
 *     of history records
 *
 * Maybe one day they can be unified.  Not today.
 * manHistoryTableKey is maintained in ipgsnmp.c
 */
unsigned int trapHistoryTableIndex;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

char alrmTrace[MAX_BUF];
char settingsTrace[MAX_BUF];
char plTrace[MAX_BUF2];
/* char pl_buffer[MAX_2U_RESPONSE]; */

unsigned int alarmAppFlag = FALSE;
extern unsigned int SNMPAppFlag;

void *telnetAppStack;
void *alarmAppStack;
void *snmpAppStack;
void *serialAppStack;
int ftp_init_flash_download (void);

//void *collectionAppStack;


int g_variables_array_size;



extern int g_snmp_toggle;
extern int g_snmp_mgmt_stn_idx;

//--------------------------------------------- pwl - ------------------------------------------


//  Set this to 1 to run the system POST tests during startup.

const int APP_POST = 0;


//  Set this to 1 to run the manufacturing burn in tests.

int APP_BURN_IN_TEST = 0;


//--------------------------------------------------------------------------------

int root_break_count = 0;

void root_breakpoint (long unsigned int *);
void
root_breakpoint (long unsigned int *catch)
{
  if (*catch == 0)
    {
	/**catch = 0xc0a8dea3; */
      root_break_count++;
    }
}

//  Function: void applicationTcpDown (void)
//
//  Description:
//
//      This routine will be called by the NET+OS root thread once every 
//      clock tick while it is waiting for the TCP/IP stack to come up.  
//      This function can increment a counter everytime it's called to 
//      keep track of how long we've been waiting for the stack to start.
//      If we've been waiting too long, then this function can do something
//      to handle the error.  
//
//      This function will not be called once the stack has started.
//
//  Parameters:
//
//      none
//
//  Return Values:
//
//      none

void
applicationTcpDown (void)
{
  static int ticksPassed = 0;

  ticksPassed++;

//  Code to handle error condition if the stack doesn't come up goes here.

}


//--------------------------------------------------------------------------------

unsigned int
guardget (unsigned char *s, int l)
{
  unsigned int d;
  for (; l > 0; l--)
    {
      d <<= 8;
      d |= *s++;
    }
  return d;
}

void
guardset (unsigned char *d, unsigned int s, int l)
{
  d += l;
  for (; l > 0; l--)
    {
      *--d = s & 0xff;
      s >>= 8;
    }
}


//  Function: void applicationStart (void)
//
//  Description:
//
//      This routine is responsible for starting the user application.  It should 
//      create any threads or other resources the application needs.
//
//      ThreadX, the NET+OS device drivers, and the TCP/IP stack will be running
//      when this function is called.
//
//  Parameters:
//
//      none
//
//  Return Values:
//
//      none

void
applicationStart (void)
{
  int ccode;
  devBoardParamsType app_nvParams;
  memset (&Mib_Leave[0], 0, sizeof(MIB_Leaves));
  memset (&Mib_Leave[1], 0, sizeof(MIB_Leaves));
  BankIndex = 0;
  Mib_Leaves = &Mib_Leave[BankIndex];
  struct variable_t variables_[NUM_OIDS_IN_TRAP];

  /* does the nv arrive broken from netOS? */
  customizeReadDevBoardParams (&app_nvParams);	// read from NVM
  root_breakpoint (&app_nvParams.mgmtStations[0].ipAddress);

  ccode = semaphore_init_2 (&semaphore_0_info);
  if ( TX_SUCCESS != ccode ) { return; }
  /* GORD And exactly what *is* thread_shell going to do? */
  /* ccode = tx_semaphore_create (&semaphore_0, "serialSemaphore", 1); */

  ccode = semaphore_init_2 (&semaphore_history_table_info);
  if ( TX_SUCCESS != ccode ) { return; }
  /* GORD And exactly what *is* thread_shell going to do? */
  /* ccode = tx_semaphore_create (&semHistTblMutex, "HistoryTable", 1); */

  ccode = semaphore_init_2 (&semaphore_nvParams_info);
  if ( TX_SUCCESS != ccode ) { return; }
  /* GORD And exactly what *is* thread_shell going to do? */
  /* ccode = tx_semaphore_create (&semaphore_nvParams, "nvPrmSemaphore", 1); */

  ccode = semaphore_init_2 (&semaphore_rtc_info);
  if ( TX_SUCCESS != ccode ) { return; }
  /* GORD And exactly what *is* thread_shell going to do? */
  /* ccode = tx_semaphore_create (&semaphore_rtc, "rtcSemaphore", 1); */

  ccode = semaphore_init_2 (&semaphore_sbuf_info);
  if ( TX_SUCCESS != ccode ) { return; }
  /* GORD And exactly what *is* thread_shell going to do? */

  /* GORD watch needs us to export the runtime locations of things */
  snmpStackSize = STACKGUARD_SIZE + STACK_SIZE + STACKGUARD_SIZE;
  snmpAppStack = malloc (snmpStackSize);

  if (snmpAppStack == NULL)
    {
      netosFatalError ("Unable to allocate snmpAppStack thread stack.", 5, 5);
    }


  snmpStackBegin = snmpAppStack;
  snmpAppStack += STACKGUARD_SIZE;
  /*GORD guard both stack ends */
  guardset (snmpStackBegin, STACKGUARD, STACKGUARD_SIZE);
  ccode = guardget (snmpStackBegin, STACKGUARD_SIZE);
  guardset (snmpAppStack + STACK_SIZE, STACKGUARD, STACKGUARD_SIZE);
  ccode = guardget (snmpAppStack + STACK_SIZE, STACKGUARD_SIZE);

  memset (&snmpTrap_app, 0, sizeof (TX_THREAD));

// control block for thread
// thread name
// entry function
// argument
// start of stack
// size of stack
// priority
// preemption threshold
// time slice threshold
// start immediately

  ccode = tx_thread_create (&snmpTrap_app,
			    "SNMP App",
			    snmpApp,
			    0,
			    snmpAppStack,
			    snmpStackSize - STACKGUARD_SIZE -
			    STACKGUARD_SIZE, NABspHighPriority,
			    NABspHighPriority, 1, TX_AUTO_START);

  if (ccode != TX_SUCCESS)
    {
      free (&snmpStackBegin);
      snmpStackSize = 0;
      netosFatalError ("Unable to create snmp thread.", 5, 5);
    }

  telnetStackSize = STACKGUARD_SIZE + STACK_SIZE + STACKGUARD_SIZE;
  telnetAppStack = malloc (telnetStackSize);

  if (telnetAppStack == NULL)
    {
      netosFatalError ("Unable to allocate telnetAppStack thread stack.",
		       5, 5);
    }

  telnetStackBegin = telnetAppStack;
  telnetAppStack += STACKGUARD_SIZE;
  /*GORD guard both stack ends */
  guardset (telnetStackBegin, STACKGUARD, STACKGUARD_SIZE);
  guardset (telnetAppStack + STACK_SIZE, STACKGUARD, STACKGUARD_SIZE);

  memset (&telnet_app, 0, sizeof (TX_THREAD));

  ccode = tx_thread_create (&telnet_app,
			    "Telnet App",
			    telnetApp,
			    0,
			    telnetAppStack,
			    telnetStackSize - STACKGUARD_SIZE -
			    STACKGUARD_SIZE, NABspMediumPriority,
			    NABspMediumPriority, 1, TX_AUTO_START);

  if (ccode != TX_SUCCESS)
    {
      free (&telnetStackBegin);
      telnetStackSize = 0;
      netosFatalError ("Unable to create thread.", 5, 5);
    }

  alarmStackSize = STACKGUARD_SIZE + STACK_SIZE + STACKGUARD_SIZE;
  alarmAppStack = malloc (alarmStackSize);
  if (alarmAppStack == NULL)
    {
      netosFatalError ("Unable to allocate alarmAppStack thread stack.",
		       5, 5);
    }

  alarmStackBegin = alarmAppStack;
  alarmAppStack += STACKGUARD_SIZE;
  /*GORD guard both stack ends */
  guardset (alarmStackBegin, STACKGUARD, STACKGUARD_SIZE);
  guardset (alarmAppStack + STACK_SIZE, STACKGUARD, STACKGUARD_SIZE);

  memset (&alarmTrap_app, 0, sizeof (TX_THREAD));

  /* Do this once and use the calculated value.
   */

  g_variables_array_size = 
      ((void*) &(variables_[1]) - (void*) &(variables_[0])) * NUM_OIDS_IN_TRAP;

  ccode = tx_thread_create (&alarmTrap_app,
			    "Alarm App",
			    alarmApp,
			    0,
			    alarmAppStack,
			    alarmStackSize - STACKGUARD_SIZE -
			    STACKGUARD_SIZE, NABspMediumPriority,
			    NABspMediumPriority, 1, TX_AUTO_START);

  if (ccode != TX_SUCCESS)
    {
      free (&alarmStackBegin);
      alarmStackSize = 0;
      netosFatalError ("Unable to create thread.", 5, 5);
    }


  serialStackSize = STACKGUARD_SIZE + STACK_SIZE + STACKGUARD_SIZE;
  serialAppStack = malloc (serialStackSize);

  if (serialAppStack == NULL) {
      netosFatalError ("Unable to allocate serialAppStack thread stack.",
		       5, 5);
  }

  serialStackBegin = serialAppStack;
  serialAppStack += STACKGUARD_SIZE;
  /*GORD guard both stack ends */
  guardset (serialStackBegin, STACKGUARD, STACKGUARD_SIZE);
  guardset (serialAppStack + STACK_SIZE, STACKGUARD, STACKGUARD_SIZE);

  memset (&serial_app, 0, sizeof (TX_THREAD));

  ccode = tx_thread_create (&serial_app,
			    "Serial App",
			    serialApp,
			    0,
			    serialAppStack,
			    serialStackSize - STACKGUARD_SIZE -
			    STACKGUARD_SIZE, NABspMediumPriority,
			    NABspMediumPriority, 1, TX_AUTO_START);

  if (ccode != TX_SUCCESS)
    {
      free (&serialStackBegin);
      serialStackSize = 0;
      netosFatalError ("Unable to create thread.", 5, 5);
    }
///////////////////////////////////////
////Hazem adding snmpCollectionTask //
/////////////////////////////////////
/*
  collectionStackSize = STACKGUARD_SIZE + STACK_SIZE + STACKGUARD_SIZE;
  collectionAppStack = malloc (collectionStackSize);

  if (collectionAppStack == NULL) {
      netosFatalError ("Unable to allocate collectionAppStack thread stack.",
		       5, 5);
  }

  collectionStackBegin = collectionAppStack;
  collectionAppStack += STACKGUARD_SIZE;
  //GORD guard both stack ends 
  guardset (collectionStackBegin, STACKGUARD, STACKGUARD_SIZE);
  guardset (collectionAppStack + STACK_SIZE, STACKGUARD, STACKGUARD_SIZE);

  memset (&collection_app, 0, sizeof (TX_THREAD));

  ccode = tx_thread_create (&collection_app,
			    "Collection App",
			    collectionApp,
			    0,
			    collectionAppStack,
			    collectionStackSize - STACKGUARD_SIZE -
			    STACKGUARD_SIZE, NABspMediumPriority,
			    NABspMediumPriority, 1, TX_AUTO_START);

  if (ccode != TX_SUCCESS)
    {
      free (&collectionStackBegin);
      collectionStackSize = 0;
      netosFatalError ("Unable to create thread.", 5, 5);
    } */
}

//--------------------------------------------------------------------------------

void
telnetApp (ULONG thread_input)
{
  int i;
  int rc;
  int status;
  int changed = 0;
  int result;
  devBoardParamsType app_nvParams;

#ifdef ADD_FLASH_DOWNLOAD
  extern int ftp_init_flash_download ();
#endif

  TS_ICONFIG_TYPE init_config;



#ifdef ADD_FLASH_DOWNLOAD

  ftp_init_flash_download ();	// start the FTP Server -- only when we build a ROM
#endif

  customizeReadDevBoardParams (&app_nvParams);
  root_breakpoint (&app_nvParams.mgmtStations[0].ipAddress);
  telnetInit (&app_nvParams);	// initialize things in telnet module

  init_config.max_entries = MAX_ENTRIES;

  rc = TSInitServer (&init_config);
  if (rc != SUCCESS)
    {
      rc++;
      return;			/* halt and catch fire */
    }

  for (i = 0; i < MAX_ENTRIES; i++)
    {
      rc = ts_setup_port (i);
      if (rc != 0)
	break;
    }

  //  Insecure account root password ipg1 can be used anywhere to 
  //  access Telnet and FTP 
  rc = NAsetSysAccess (NASYSACC_DEL, "root", APP_ROOT_PASSWORD,
		        NASYSACC_LEVEL_ROOT, NULL);

  printf ("Start the AWS Web Server Application.\n");
  result = RpHSStartServer(); // Call to start AWS with default settings 

  if (result != 0)
    {
        printf("root: NAHSStartServer failure[%d].\n", result);
        return;
    }

  if (!isValidPwd (app_nvParams.rootPassword))
    {
      changed++;
      strcpy (app_nvParams.rootPassword, DEFAULT_ROOT_PWD);
    }
  rc = NAsetSysAccess (NASYSACC_ADD, "root", app_nvParams.rootPassword,
		       NASYSACC_LEVEL_ROOT, NULL);

  if (!isValidPwd (app_nvParams.adminPassword))
    {
      changed++;
      strcpy (app_nvParams.adminPassword, DEFAULT_ADMIN_PWD);
    }
  rc = NAsetSysAccess (NASYSACC_ADD, "admin", app_nvParams.adminPassword,
		        NASYSACC_LEVEL_ADMINISTRATOR, NULL);

  if (!isValidPwd (app_nvParams.security_pwd))
    {
      changed++;
      strcpy (app_nvParams.security_pwd, DEFAULT_SECURITY_PWD);
    }
  rc = NAsetSysAccess (NASYSACC_ADD, "security",
		       app_nvParams.security_pwd, NASYSACC_LEVEL_RW, NULL);

  rc = NAsetSysAccess (NASYSACC_ADD, "WebGUI", "Digi1",
		       NASYSACC_LEVEL_HTTP_R1 | NASYSACC_LEVEL_R, NULL);

  rc = NAsetSysAccess (NASYSACC_ADD, "Administrator", APP_ROOT_PASSWORD,
		       NASYSACC_LEVEL_HTTP_R2 | NASYSACC_LEVEL_RW, NULL);

  //  Secure account Safe can only be used through the Secure Gateway -- 
  //  this account's password will never be exposed!  
  NAsetSysAccess (NASYSACC_ADD, "Safe", "ipg1", NASYSACC_LEVEL_RW,
		  "127.0.0.1");

  if (changed > 0)
    {
      root_breakpoint (&app_nvParams.mgmtStations[0].ipAddress);
      customizeWriteDevBoardParams (&app_nvParams);
    }

}

//------------------------------------------------------------------------------


void
snmpApp (ULONG thread_input)
{
    /* this appears to be a one-shot thread - it runs till it's done
     * and ends
     */
  char *publicSNMPAccounts[] = {
    "u0",
    "u1",
    "u2",
    "u3",
    "u4",
    "u5",
  };

  char *privateSNMPAccounts[] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
  };


  int i, j, status;
  int write_nv = 0;
  int ccode;
  devBoardParamsType app_nvParams;
  unsigned long tempVar;

#if BSP_INCLUDE_RTC_DRIVER
  /* get the trapHistoryTableIndex from rtc scratch area */
  if (RTC_SUCCESS == (ccode = rtcReadExtraData (&tempVar))) {
      manHistoryTableKey = trapHistoryTableIndex = tempVar;
      ++manHistoryTableKey;	/* historical reasons */
      /* trapHistoryTableIndex is incremented just before it is used */
      /* manHistoryTableKey is incremented after it is used */
  } else {
      /* go kablooey, because "this shouldn't happen". */
  }
#endif

  while ((status =
	  semaphore_get_2 (&semaphore_nvParams_info, SEMAPHORE_TMO, 20201))
         != TX_SUCCESS) {
      tx_thread_sleep (SEMAPHORE_TMO);
  }

  if (status == TX_SUCCESS) {
      customizeReadDevBoardParams (&app_nvParams);
      root_breakpoint (&app_nvParams.mgmtStations[0].ipAddress);

      for (i = 0; i < MAX_STATIONS; ++i) {
	  if (!strlen (app_nvParams.mgmtStations[0].publicString)) {
	      strcpy (app_nvParams.mgmtStations[0].publicString, "ipg");
	      strcpy (app_nvParams.mgmtStations[i].publicAccountName,
		      publicSNMPAccounts[i]);
	      write_nv++;
          }

	  if (!strlen (app_nvParams.mgmtStations[0].privateString)) {
	      strcpy (app_nvParams.mgmtStations[0].privateString, "photon");
	      strcpy (app_nvParams.mgmtStations[i].privateAccountName,
		      privateSNMPAccounts[i]);
	      write_nv++;
          }
      }

      if (app_nvParams. /*snmp_port */ krbIpAddr == 0) {
	  app_nvParams.krbIpAddr = SNMP_TRAP_PORT;	/* h/snmp/snmp.h */
	  write_nv++;
      }
      // Only set up one account & set of strings for any IP address

      ccode =
	NAsetSysAccess (NASYSACC_ADD, publicSNMPAccounts[0],
			app_nvParams.mgmtStations[0].publicString,
			NASYSACC_LEVEL_SNMP_R, NULL);
      ccode =
	NAsetSysAccess (NASYSACC_ADD, privateSNMPAccounts[0],
			app_nvParams.mgmtStations[0].privateString,
			NASYSACC_LEVEL_SNMP_RW, NULL);

      /* should we commit the changes to nv? */
      if (write_nv > 0) {
	  root_breakpoint (&app_nvParams.mgmtStations[0].ipAddress);
	  app_nvParams.telnetMagic = MAGIC;
	  customizeWriteDevBoardParams (&app_nvParams);
      }

      snmpd_load (); /* this starts the daemon that listens for snmp requests */

      snmpRegisterManagementVariables ();  /* needed for snmp tables */


      naSnmpSetTrapSendPortNumber (app_nvParams.krbIpAddr /*snmp_port */ );
      tx_thread_sleep (100);

//      ftp_server_app_init ();
      ftp_init_flash_download ();

      SyncSnmpFromNvram (&app_nvParams);

      status = semaphore_put_2 (&semaphore_nvParams_info, 20201);
  }
}


//--------------------------------------------------------------------------------
unsigned int g_alarmApp_caret = 0;
//  This driver routine pump some longword values for set and cleared alarm
//  entries to the processing routine that send traps to SNMP management
//  station(s).

rtcDataType g_timedate;


void
alarmApp (ULONG thread_input)
{
    int status;
	int count = 1;
	unsigned long tmpValue;
	unsigned long tempValue;
	void *resultBuffer = NULL;
    // ask the atmel for blips  - no need for this any more
    printf ("ZIMBLIPON\r");
    fflush (stdout);
	 
    while (1) {
		tx_thread_sleep (5);
        // cache the rtc time 
        status = semaphore_get_2 (&semaphore_rtc_info, SEMAPHORE_TMO, 30005);

        if (status == TX_SUCCESS) {
			rtcGet (&g_timedate);
			semaphore_put_2 (&semaphore_rtc_info, 30005);
        }

		resultBuffer = askAtmelForUnsignedValue ("ZIMPRODUCTSWTYPE",
												 "ZSPSWT",
												 E_READ_FROM_2U_NO_ADJUST,
												 E_READ_FROM_2U_NO_CHECKSUM,
												 4, /* retries */
												 &SWType,
												 10395);
		PwrType = (unsigned char) (POWER_MASK);
		AliveTrapTimer = (unsigned char)(ALIVE_TRAP_MASK);
		productSWType = (unsigned char)(TYPE_MASK);
		AliveTimerNVRamSave = (unsigned char)(ALIVE_TRAP_NVRAM_MASK);
		SpecPattern = (unsigned char)(SPEC_PATTERN);
		
		if (AliveTimerNVRamSave)
		{
			g_snmp_send_alive_trap_period = (unsigned int) (AliveTrapTimer);
			if (TrapPeriod != g_snmp_send_alive_trap_period) {
				TrapPeriod = g_snmp_send_alive_trap_period;
			}
		}

        // 
        // * Observation:
        // * It's possible that the Digi spends most of its time blocked waiting
        // * on getchar for the answer to ZIMRAL -- hmm.
        // * Certainly, the Digi only asks for a ZIMRAL once every 3 seconds,
        // * instead of 3 times per second.  Why? 
		//if (!count)
		//{
			if ( 0 != askAtmelForUnsignedValue ("ZIMRAL",
												"ZRAS",
												E_READ_FROM_2U_DO_ADJUST,
												E_READ_FROM_2U_DO_CHECKSUM,
												4, // retries 
												&tmpValue,
												20202) ) {
			
				 // Special Pattern is set to 0x1001 = 9
				 if (SpecPattern == 9)
				 {
					 Mib_Leaves->earAlarmStatus = tmpValue;
				 }
				 else
				 {
					 // MASK Fuse, 48VA and 48VB Alarms
					 tmpValue &= 0xFFFE3FFF;
					 Mib_Leaves->earAlarmStatus = tmpValue;

				 }
				 // Mapping of bitwise alarm variable to 

				 temperatureHighAlarm = ((1) & tmpValue)/((1) & tmpValue);
				 fanFailureAlarm = ((2) & tmpValue)/((2) & tmpValue);
				 preAmpOutTooHighAlarm = ((4) & tmpValue)/((4) & tmpValue);
				 preAmpOutHighAlarm = ((8) & tmpValue)/((8) & tmpValue);
				 preAmpOutLowAlarm = ((16) & tmpValue)/((16) & tmpValue);
				 preAmpInTooHighAlarm = ((32) & tmpValue)/((32) & tmpValue);
				 preAmpInHighAlarm = ((64) & tmpValue)/((64) & tmpValue);
				 preAmpInLowAlarm = ((0x80) & tmpValue)/((0x80) & tmpValue);
				 preAmpOutOffAlarm = ((0x100) & tmpValue)/((0x100) & tmpValue);
				 preAmpInOffAlarm = ((0x200) & tmpValue)/((0x200) & tmpValue);
				 interlockAlarm = ((0x400) & tmpValue)/((0x400) & tmpValue);
				 powerSupply1Alarm = ((0x800) & tmpValue)/((0x800) & tmpValue);
				 powerSupply2Alarm = ((0x1000) & tmpValue)/((0x1000) & tmpValue);
				 powerSafetyModeAlarm = ((0x2000) & tmpValue)/((0x2000) & tmpValue);
				 systemFuseAlarm = ((0x4000) & tmpValue)/((0x4000) & tmpValue);
				 a48VAOffAlarm = ((0x8000) & tmpValue)/((0x8000) & tmpValue);
				 b48VBOffAlarm = ((0x10000) & tmpValue)/((0x10000) & tmpValue);
				 boosterInTooHighAlarm = ((0x20000) & tmpValue)/((0x20000) & tmpValue);
				 boosterInHighAlarm = ((0x40000) & tmpValue)/((0x40000) & tmpValue);
				 boosterInLowAlarm = ((0x80000) & tmpValue)/((0x80000) & tmpValue);
				 boosterInOffAlarm = ((0x100000) & tmpValue)/((0x100000) & tmpValue);
				 boosterOutOffAlarm = ((0x200000) & tmpValue)/((0x200000) & tmpValue);
				 boosterOutLowAlarm = ((0x400000) & tmpValue)/((0x400000) & tmpValue);
				 boosterOutHighAlarm = ((0x800000) & tmpValue)/((0x800000) & tmpValue);
				 boosterOutTooHighAlarm = ((0x1000000) & tmpValue)/((0x1000000) & tmpValue);
				 pumpCurrentAlarm = ((0x2000000) & tmpValue)/((0x2000000) & tmpValue);
				 backReflectorAlarm = ((0x4000000) & tmpValue)/((0x4000000) & tmpValue);
				 alarmCheck (tmpValue);
			}

				resultBuffer = askAtmelForUnsignedValue ("ZIMROP",
									  "ZROP",
									  E_READ_FROM_2U_NO_ADJUST,
									  E_READ_FROM_2U_NO_CHECKSUM,
									  4, /* retries */
									  &tempValue,
									  10195);
			if (resultBuffer)
			{
				Mib_Leaves->earTotalOpticalOutputPower = tempValue;
				sprintf(earTotalOpticalOutputPower_temp, "%.1f", ((float)((signed long)tempValue))/10);
			}
			


			resultBuffer = NULL;
			if (productSWType)
			{
				resultBuffer = askAtmelForUnsignedValue ("ZIMROP2",
								  "ZROP2",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, // retries 
								  &tempValue,
								  10396);
			}
			if (resultBuffer)
			{
				Mib_Leaves->earTotalOpticalOutputPower2 = tempValue;
				sprintf(earTotalOpticalOutputPower2_temp, "%.1f", ((float)((signed long)tempValue))/10);
			}
			else
			{
				Mib_Leaves->earTotalOpticalOutputPower2 = 0;
				sprintf(earTotalOpticalOutputPower2_temp, "0");
			}

			resultBuffer = askAtmelForUnsignedValue ("ZIMRLC",
								  "ZRLC",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, // retries
								  &tempValue,
								  10214);
			if (resultBuffer)
			{
				Mib_Leaves->earPumpLaserCurrent = tempValue;
				earPumpLaserCurrent_temp = tempValue;
			}

            resultBuffer = NULL;
			resultBuffer = askAtmelForUnsignedValue ("ZIMRLC2",
								  "ZRLC2",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, //retries 
								  &tempValue,
								  10294);
			if (resultBuffer)
			{
				Mib_Leaves->earPumpLaserCurrent2 = tempValue;
				earPumpLaserCurrent2_temp = tempValue;
    		}
            else
            {
                Mib_Leaves->earPumpLaserCurrent2 = 0;
				earPumpLaserCurrent2_temp = 0;
            }
 
			resultBuffer = askAtmelForUnsignedValue ("ZIMRIP",
								  "ZRIP",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, // retries 
								  &tempValue,
								  10214);
			if (resultBuffer)
			{
				Mib_Leaves->earTotalOpticalInputPower = tempValue;
				sprintf(earTotalOpticalInputPower_temp, "%.1f", ((float)((signed long)tempValue))/10);
			}
			
			resultBuffer = NULL;
			if (productSWType)
			{
				resultBuffer = askAtmelForUnsignedValue ("ZIMRIP2",
									  "ZRIP2",
									  E_READ_FROM_2U_NO_ADJUST,
									  E_READ_FROM_2U_NO_CHECKSUM,
									  4, // retries 
									  &tempValue,
									  10214);
			}
			if (resultBuffer)
			{
				Mib_Leaves->earTotalOpticalInput2Power = tempValue;
				sprintf(earTotalOpticalInput2Power_temp, "%.1f", ((float)((signed long)tempValue))/10);
			}
			else
			{
				Mib_Leaves->earTotalOpticalInput2Power = 0;
				sprintf(earTotalOpticalInput2Power_temp, "0");
			}
		//}
		//count++;
		collectionApp();
		if (count == 3) count = 0;

    }  // while 
} 

///////// Collection App /////////////
void collectionApp (void)
{
	int done;
    int status;
	int TempVal1;
    int TempVal2; 
    unsigned long tempValue;
    static int iTempValue;
	static int currentMode = EARMIB_earControlMode_power;
    unsigned long alarmStatus = 0;
	void *resultBuffer = NULL;
    unsigned char buffer_[MAX_2U_RESPONSE];
    unsigned long bufferLength = MAX_2U_RESPONSE;
	int read_int_value;
	static int stateCP = 0;
    unsigned char BankFlag;
	

	// In this task, we should collect all the static data at the beginning
	// These data are such as firmware revision, dats of Manufaturing and so on... 
	tx_thread_sleep (180);
		switch (stateCP)
		{
		case 0:

		resultBuffer = askAtmelForStringValue ("ZIMRSN",
								"ZRSN",
								E_READ_FROM_2U_NO_ADJUST,
								E_READ_FROM_2U_DO_CHECKSUM,
								4, /* retries */
								&(buffer_[0]),
								&bufferLength,
								10155);
		if ( NULL != resultBuffer ) {
			strcpy(Mib_Leaves->earSerialNumber, buffer_);
			strcpy(earSerialNumber_temp, buffer_);
		}
//		stateCP = 1;
//		break;


//		case 1:
		resultBuffer = askAtmelForStringValue ("ZIMRREV",
								"ZRREV",
								E_READ_FROM_2U_NO_ADJUST,
								E_READ_FROM_2U_NO_CHECKSUM,
								4, /* retries */
								&(buffer_[0]),
								&bufferLength,
								10110);
		if ( NULL != resultBuffer ) {
			strcpy(Mib_Leaves->earFirmwareRevision, buffer_);
			strcpy(earFirmwareRevision_temp, buffer_);
		}
//		stateCP = 2;
//		break;


//		case 2:
		resultBuffer = askAtmelForIPAddressValue ("ZIMGETIP",
								   E_READ_FROM_2U_NO_ADJUST,
								   E_READ_FROM_2U_NO_CHECKSUM,
								   4,
								   &(tempValue),
								   10158
								   );
		if (resultBuffer)
		{
			Mib_Leaves->earIpAddress = tempValue;
            sprintf (earIpAddress_temp, "%ld.%ld.%ld.%ld\n\r",
		    (tempValue >> 24) & 0xff,
		    (tempValue >> 16) & 0xff,
		    (tempValue >> 8) & 0xff,
		    tempValue  & 0xff);

			//earIpAddress_temp = tempValue;
		}

		stateCP = 3;
		break;


		case 3:

		resultBuffer = askAtmelForStringValue ("ZIMRDATE",
											   "ZRDAT",
											   E_READ_FROM_2U_NO_ADJUST,
											   E_READ_FROM_2U_DO_CHECKSUM,
											   4, /* retries */
											   &(buffer_[0]),
											   &bufferLength,
											   10107);
		if ( NULL != resultBuffer ) {
			strcpy (Mib_Leaves->earDateOfManufacture, buffer_);
			strcpy (earDateOfManufacture_temp, buffer_);
		}
//		stateCP = 4;
//		break;


//		case 4:

		resultBuffer = askAtmelForStringValue ("ZIMRCM",
											   "ZRCM",
											   E_READ_FROM_2U_NO_ADJUST,
											   E_READ_FROM_2U_NO_CHECKSUM,
											   4, /* retries */
											   &(buffer_[0]),
											   &bufferLength,
											   10115);
		if ( NULL != resultBuffer ) {
			/* this can be a switch because the initial letter
			* of each possibility is different
			*/
		switch ( buffer_[0] ) {
			case ATMEL_CONTROLMODE_POWER:
				read_int_value = EARMIB_earControlMode_power;
                sprintf(earControlMode_temp, "POWER");
				break;
			case ATMEL_CONTROLMODE_CURRENT:
				read_int_value = EARMIB_earControlMode_current;
                sprintf(earControlMode_temp, "CURRENT");
				break;
			case ATMEL_CONTROLMODE_GAIN:
				read_int_value = EARMIB_earControlMode_gain;
                sprintf(earControlMode_temp, "GAIN");
				break;
			default:
			/* MIB should probably be extended to have UNKNOWN */
				read_int_value = 4;
                sprintf(earControlMode_temp, "N/A");
				break;
			}
			Mib_Leaves->earControlMode = read_int_value;
			
		}
		
//		stateCP = 5;
//		break;


//		case 5:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRTEMPTHRESH",
												 "ZRTMP",
												 E_READ_FROM_2U_NO_ADJUST,
												 E_READ_FROM_2U_NO_CHECKSUM,
												 4, /* retries */
												 &tempValue,
												 40405);

		if ( resultBuffer ) {
			iTempValue = tempValue;
			TempVal1 = (iTempValue / 1000);
			TempVal2 = iTempValue - (TempVal1 * 1000);

			if ((TempVal2 >= 5) || (TempVal2 <= -5)) {
				if (iTempValue >= 0) {
					iTempValue = (iTempValue / 1000) + 1;
				} else {
					iTempValue = (iTempValue / 1000) - 1;
				}
			} else {
				iTempValue = (iTempValue / 1000);
			}

			Mib_Leaves->earTemperatureThreshold = iTempValue;
			sprintf(earTemperatureAlarmThreshold_temp, "%.1f", ((float)tempValue)/1000); // first try to make temp work
		}
		stateCP = 6;
		break;

		case 6:
		resultBuffer = askAtmelForIPAddressValue ("ZIMGETMASK",
								   E_READ_FROM_2U_NO_ADJUST,
								   E_READ_FROM_2U_NO_CHECKSUM,
								   4,
								   &(tempValue),
								   10164
								   ); 
		if (resultBuffer)
		{
			Mib_Leaves->earSubnetMask = tempValue;
            sprintf (earSubnetMask_temp, "%ld.%ld.%ld.%ld\n\r",
		    (tempValue >> 24) & 0xff,
		    (tempValue >> 16) & 0xff,
		    (tempValue >> 8) & 0xff,
		    tempValue & 0xff);
		}
//		stateCP = 7;
//		break;


//		case 7:
		resultBuffer = askAtmelForIPAddressValue ("ZIMGETGATEWAY",
								   E_READ_FROM_2U_NO_ADJUST,
								   E_READ_FROM_2U_NO_CHECKSUM,
								   4,
								   &(tempValue),
								   10169
								   );
		if (resultBuffer)
		{
			Mib_Leaves->earGateway = tempValue;
            sprintf (earGateway_temp, "%ld.%ld.%ld.%ld\n\r",
		    (tempValue >> 24) & 0xff,
		    (tempValue >> 16) & 0xff,
		    (tempValue >> 8) & 0xff,
		    tempValue & 0xff);
            
		}
//		stateCP = 8;
//		break;
		
//		case 8:

		resultBuffer = askAtmelForStringValue ("ZIMRMDL",
											   "ZRMDL",
											   E_READ_FROM_2U_NO_ADJUST,
											   E_READ_FROM_2U_DO_CHECKSUM,
											   4, /* retries */
											   &(buffer_[0]),
											   &bufferLength,
											   10101);
		if (resultBuffer)
		{
			strcpy (Mib_Leaves->earModelNumber, buffer_);
			strcpy (earModelNumber_temp, buffer_);
		}	
#ifdef ALS_Feature
		resultBuffer = askAtmelForStringValue ("ZIMRFDM",
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
				Mib_Leaves->earControlModeFD = EARMIB_earControlMode_power;
				sprintf(earControlModeFD_temp, "POWER");
				break;
			case ATMEL_CONTROLMODE_CURRENT:
				Mib_Leaves->earControlModeFD = EARMIB_earControlMode_current;
				sprintf(earControlModeFD_temp, "CURRENT");
				break;
			case ATMEL_CONTROLMODE_GAIN:
				Mib_Leaves->earControlModeFD = EARMIB_earControlMode_gain;
				sprintf(earControlModeFD_temp, "GAIN");
				break;
			default:
				// MIB should probably be extended to have UNKNOWN 
				Mib_Leaves->earControlModeFD = 4;
				sprintf(earControlModeFD_temp, "N/A");
				break;
			}
		}
#endif
		stateCP = 9;
		break;


		case 9:
		
			if (Mib_Leaves->earControlMode == EARMIB_earControlMode_gain)
			{
				resultBuffer = askAtmelForUnsignedValue ("ZIMRGS",
									  "ZRGS",
									  E_READ_FROM_2U_NO_ADJUST,
									  E_READ_FROM_2U_NO_CHECKSUM,
									  4, /* retries */
									  &tempValue,
									  10376);
				if (resultBuffer)
				{
					Mib_Leaves->earGainControlSetpoint = tempValue;
					sprintf(earGainControlSetpoint_temp, "%.1f", ((float)tempValue)/10);
				}
			}
			else 
			{
	
				Mib_Leaves->earGainControlSetpoint = 0;
				sprintf(earGainControlSetpoint_temp, "0");
			}
		

		resultBuffer = askAtmelForUnsignedValue ("ZIMRMG",
								  "ZRMG",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10374);
		if (resultBuffer)
		{
			Mib_Leaves->earMeasuredGainValue = tempValue;
			sprintf(earMeasuredGainValue_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
//		stateCP = 10;
//		break;

//		case 10:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRMINGAIN",
								  "ZRMNG",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */ 
								  &tempValue,
								  10370);
		if (resultBuffer)
		{
			Mib_Leaves->earMinimumGainSetpoint = tempValue;
			sprintf(earMinimumGainSetPoint_temp, "%.1f", ((float)tempValue)/10);
		}
		stateCP = 11;
		break;

		case 11:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRMAXGAIN",
								  "ZRMXG",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10372);
		if (resultBuffer)
		{
			Mib_Leaves->earMaximumGainSetpoint = tempValue;
			sprintf(earMaximumGainSetPoint_temp, "%.1f", ((float)tempValue)/10);
		}
//		stateCP = 12;
//		break;

//		case 12:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPS",
								  "ZRPS",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10192);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPowerSetpointValue = tempValue;
            earOutputPowerSetpointValue_temp = tempValue;
            //sprintf(earOutputPowerSetpointValue_temp, "%.1f", ((float)tempValue)/10);
		}
		
        resultBuffer = NULL;
		if (productSWType)
		{
			resultBuffer = askAtmelForUnsignedValue ("ZIMRSP2",
								  "ZRSP2",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10199);
		}
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPower2SetpointValue = tempValue;
            earOutputPower2SetpointValue_temp = tempValue;
			// sprintf(earOutputPower2SetpointValue_temp, "%.1f", ((float)tempValue)/10);
		}
		else
		{
			Mib_Leaves->earOutputPower2SetpointValue = 0;
            earOutputPower2SetpointValue_temp = 0;
			// sprintf(earOutputPower2SetpointValue_temp, "0");
		}
		
//		stateCP = 14;
//		break;

//		case 14:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRMP",
								  "ZRMP",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10198);
		if (resultBuffer)
		{
			Mib_Leaves->earMaximumPowerSetpoint = tempValue;
			sprintf(earMaximumPowerSetpoint_temp, "%.1f", ((float)tempValue)/10);
		}
//		stateCP = 15;
//		break;

//		case 15:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRCS",
								  "ZRCS",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10378);
		if (resultBuffer)
		{
			Mib_Leaves->earCurrentSetpointValue = tempValue;
			earCurrentSetpointValue_temp = tempValue;
		}
		stateCP = 16;
		break;

		case 16:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRMC",
								  "ZRMC",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10205);
		if (resultBuffer)
		{
			Mib_Leaves->earMaximumCurrentSetpoint = tempValue;
			earMaximumCurrentSetpoint_temp = tempValue;
		}
//		stateCP = 17;
//		break;

//		case 17:

		resultBuffer = askAtmelForUnsignedValue ("ZIMRAT",
								  "ZRAT",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10211);
		if (resultBuffer)
		{
			Mib_Leaves->earAmbientTemperature = tempValue;
			sprintf(earAmbientTemperature_temp, "%.1f", ((float)tempValue)/1000); // 2/10/15 edited MJC tempValue changed to iTempvalue
		}
//		stateCP = 18;
//		break;

//		case 18:

		resultBuffer = askAtmelForUnsignedValue ("ZIMRPINLOW",
								  "ZRPNL",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10224);
		if (resultBuffer)
		{
			Mib_Leaves->earInputPowerLowAlarmLevel = tempValue;
			sprintf(earInputPowerLowAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
		stateCP = 19;
		break;

		case 19:
		resultBuffer = NULL;
		if (productSWType)
		{
			resultBuffer = askAtmelForUnsignedValue ("ZIMRPIN2LOW",
								  "ZRPN2L",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10288);
		}
		if (resultBuffer)
		{
			Mib_Leaves->earInput2PowerLowAlarmLevel = tempValue;
			sprintf(earInput2PowerLowAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
		else
		{
			Mib_Leaves->earInput2PowerLowAlarmLevel = 0;
			sprintf(earInput2PowerLowAlarmLevel_temp, "0");
		}
//		stateCP = 20;
//		break;

//		case 20:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRFDPINLOW",
								  "ZRDPNL",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10227);
		if (resultBuffer)
		{
			Mib_Leaves->earFactoryDefaultInputPowerLowAlarmLevel = tempValue;
		}
//		stateCP = 21;
//		break;

//		case 21:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPINOFF",
								  "ZRPNF",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10230);
		if (resultBuffer)
		{
			Mib_Leaves->earInputPowerOffAlarmLevel = tempValue;
			// sprintf(earInputPowerOffAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
            sprintf(earInputPowerOffAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10); // If # scrambles because of negative sign
			
		}
		stateCP = 22;
		break;

        case 22:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPIN2OFF",
								  "ZRPN2F",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10289);
		if (resultBuffer)
		{
			Mib_Leaves->earInput2PowerOffAlarmLevel = tempValue;
			sprintf(earInput2PowerOffAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
        else
        {
            Mib_Leaves->earInput2PowerOffAlarmLevel = 0;
			sprintf(earInput2PowerOffAlarmLevel_temp, "0");
        }
//		stateCP = 23;
//		break;

//		case 23:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPINHIGH",
								  "ZRPNH",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10234);
		if (resultBuffer)
		{
			Mib_Leaves->earInputPowerHighAlarmLevel = tempValue;
            sprintf(earInputPowerHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
//		stateCP = 24;
//		break;

//		case 24:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPIN2HIGH",
								  "ZRPN2H",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10290);
		if (resultBuffer)
		{
			Mib_Leaves->earInput2PowerHighAlarmLevel = tempValue;
            sprintf(earInput2PowerHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
        else
        {
            Mib_Leaves->earInput2PowerHighAlarmLevel = 0;
            sprintf(earInput2PowerHighAlarmLevel_temp, "0");
        }
//		stateCP = 25;
//		break;

//		case 25:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPINTOOHIGH",
								  "ZRPN2H",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10237);
		if (resultBuffer)
		{
			Mib_Leaves->earInputPowerTooHighAlarmLevel = tempValue;
            sprintf(earInputPowerTooHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
		stateCP = 26;
		break;

        case 26:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPIN2TOOHIGH",
								  "ZRPN22H",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10291);
		if (resultBuffer)
		{
			Mib_Leaves->earInput2PowerTooHighAlarmLevel = tempValue;
            sprintf(earInput2PowerTooHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
        else
        {
            Mib_Leaves->earInput2PowerTooHighAlarmLevel = 0;
            sprintf(earInput2PowerTooHighAlarmLevel_temp, "0");
        }
//		stateCP = 27;
//		break;

//		case 27:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUTLOW",
								  "ZRPOL",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10240);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPowerLowAlarmLevel = tempValue;
            sprintf(earOutputPowerLowAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
//		stateCP = 28;
//		break;

//		case 28:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUTOFF",
								  "ZRPOF",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10243);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPowerOffAlarmLevel = tempValue;
            sprintf(earOutputPowerOffAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
//		stateCP = 29;
//		break;

//		case 29:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUTHIGH",
								  "ZRPOH",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10246);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPowerHighAlarmLevel = tempValue;
            sprintf(earOutputPowerHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
		stateCP = 30;
		break;

		case 30:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUTTOOHIGH",
								  "ZRPO2H",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10249);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPowerTooHighAlarmLevel = tempValue;
            sprintf(earOutputPowerTooHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
//		stateCP = 31;
//		break;

//		case 31:
		resultBuffer = askAtmelForUnsignedValue ("ZIMSTA",
								  "ZRSTA",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10252);
		if (resultBuffer)
		{
			Mib_Leaves->earStatusValue = tempValue;
		}
//		stateCP = 32;
//		break;

//		case 32:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRRAS",
								  "ZRRAS",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10255);
		if (resultBuffer)
		{
			Mib_Leaves->earRelayAlarmStatus = tempValue;
			
		}
//		stateCP = 33;
//		break;

//		case 33:
		resultBuffer = askAtmelForUnsignedValue ("ZIMRRS",
								  "ZRRS",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10259);
		if (resultBuffer)
		{
			Mib_Leaves->earRelayStatus = tempValue;
		}
		stateCP = 34;
		break;

        case 34:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUT2OFF",
								  "ZRPOUT2OFF",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10402);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPower2OffAlarmLevel = tempValue;
            sprintf(earOutputPower2OffAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
        else
        {
            Mib_Leaves->earOutputPower2OffAlarmLevel = 0;
            sprintf(earOutputPower2OffAlarmLevel_temp, "0");
        }
//		stateCP = 35;
//		break;

//		case 35:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUT2LOW",
								  "ZRPOUT2LOW",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10404);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPower2LowAlarmLevel = tempValue;
            sprintf(earOutputPower2LowAlarmLevel_temp, "%.1f", ((float)((signed long)tempValue))/10);
		}
        else
        {
            Mib_Leaves->earOutputPower2LowAlarmLevel = 0;
            sprintf(earOutputPower2LowAlarmLevel_temp, "0");
        }
//		stateCP = 36;
//		break;

//		case 36:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUT2HIGH",
								  "ZRPOUT2HIGH",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10406);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPower2HighAlarmLevel = tempValue;
            sprintf(earOutputPower2HighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
        else
        {
            Mib_Leaves->earOutputPower2HighAlarmLevel = 0;
            sprintf(earOutputPower2HighAlarmLevel_temp, "0");
        }
		stateCP = 37;
		break;

        case 37:
        resultBuffer = NULL;
		resultBuffer = askAtmelForUnsignedValue ("ZIMRPOUT2TOOHIGH",
								  "ZRPOUT2TOOHIGH",
								  E_READ_FROM_2U_NO_ADJUST,
								  E_READ_FROM_2U_NO_CHECKSUM,
								  4, /* retries */
								  &tempValue,
								  10408);
		if (resultBuffer)
		{
			Mib_Leaves->earOutputPower2TooHighAlarmLevel = tempValue;
            sprintf(earOutputPower2TooHighAlarmLevel_temp, "%.1f", ((float)tempValue)/10);
		}
        else
        {
            Mib_Leaves->earOutputPower2TooHighAlarmLevel = 0;
            sprintf(earOutputPower2TooHighAlarmLevel_temp, "0");
        }

#ifdef ALS_Feature
		resultBuffer = askAtmelForStringValue ("ZIMRDATAMODE",
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
				   Mib_Leaves->earDataMode = EARMIB_earDataMode_off;
				   earDataMode_temp = EARMIB_earDataMode_off;
				   break;
			   case ATMEL_DATAMODE_ALS:
				   Mib_Leaves->earDataMode = EARMIB_earDataMode_als;
				   earDataMode_temp = EARMIB_earDataMode_als;
				   break;
			   case ATMEL_DATAMODE_NOMINAL:
				   Mib_Leaves->earDataMode = EARMIB_earDataMode_nominal;
				   earDataMode_temp = EARMIB_earDataMode_nominal;
				   break;
			   default:
				   Mib_Leaves->earDataMode = 4;
				   earDataMode_temp = 4;
				   break;
			}
		}
#endif

//		stateCP = 38;
//		break;

//		case 38:
		resultBuffer = askAtmelForStringValue ("ZIMRDEVTYP",
											   "ZRDEVTYP",
											   E_READ_FROM_2U_NO_ADJUST,
											   E_READ_FROM_2U_NO_CHECKSUM,
											   4, /* retries */
											   &(buffer_[0]),
											   &bufferLength,
											   10380);
		if ( NULL != resultBuffer ) {
			switch ( buffer_[0] ) {
				case ATMEL_DEVICETYPE_LEGACY:
					Mib_Leaves->earDeviceType = EARMIB_earDeviceType_legacy;
					break;
				case ATMEL_DEVICETYPE_MASTER:
					Mib_Leaves->earDeviceType = EARMIB_earDeviceType_master;
					break;
				case ATMEL_DEVICETYPE_SLAVE:
					Mib_Leaves->earDeviceType = EARMIB_earDeviceType_slave;
					break;
				default:
					Mib_Leaves->earDeviceType = 4;
					break;
			}
        }
//		stateCP = 39;
//		break;

//		case 39:

			if (Mib_Leaves->earControlMode == EARMIB_earControlMode_gain)
			{
				resultBuffer = askAtmelForUnsignedValue ("ZIMRGS2",
									  "ZRGS2",
									  E_READ_FROM_2U_NO_ADJUST,
									  E_READ_FROM_2U_NO_CHECKSUM,
									  4, // retries 
									  &tempValue,
									  10398);
				if (resultBuffer)
				{
					Mib_Leaves->earGainControlSetpoint2 = tempValue;
					sprintf(earGainControlSetpoint2_temp, "%1.1f", ((float)tempValue)/10);
				}
			}
			else
			{
				Mib_Leaves->earGainControlSetpoint2 = 0;
				sprintf(earGainControlSetpoint2_temp, "0");
			}

            sprintf(earPowerSafetyMode_temp, "NA");

            sprintf(earDigiFirmwareRev_temp, "%d.%d", MAJOR_VERSION, MINOR_VERSION);

            // app bank selected in flash
            GetFlashBankFlag(&BankFlag);
            if (BankFlag & FLASH_USE_BANK2)
                sprintf(earDigiActiveFB_temp, "2");
            else
                sprintf(earDigiActiveFB_temp, "1");

            if (BankFlag & FLASH_UNCONFIRMED)
                sprintf(earDigiActiveFBConfirmed_temp, "unconfirmed");
            else
                sprintf(earDigiActiveFBConfirmed_temp, "confirmed");

#ifdef ALS_Feature
		resultBuffer = askAtmelForStringValue ("ZIMRDEVTYPFD",
											   "ZRDEVTYPFD",
											   E_READ_FROM_2U_NO_ADJUST,
											   E_READ_FROM_2U_NO_CHECKSUM,
											   4, // retries
											   &(buffer_[0]),
											   &bufferLength,
											   10382);
		if ( NULL != resultBuffer ) {
			switch ( buffer_[0] ) {
				case ATMEL_DEVICETYPE_LEGACY:
					Mib_Leaves->earDeviceTypeFD = EARMIB_earDeviceTypeFD_legacy;
					earDeviceTypeFD_temp = EARMIB_earDeviceTypeFD_legacy;
					break;
				case ATMEL_DEVICETYPE_MASTER:
					Mib_Leaves->earDeviceTypeFD = EARMIB_earDeviceTypeFD_master;
					earDeviceTypeFD_temp = EARMIB_earDeviceTypeFD_master;
					break;
				case ATMEL_DEVICETYPE_SLAVE:
					Mib_Leaves->earDeviceTypeFD = EARMIB_earDeviceTypeFD_slave;
					earDeviceTypeFD_temp = EARMIB_earDeviceTypeFD_slave;
					break;
				default:
					Mib_Leaves->earDeviceTypeFD = 4;
					earDeviceTypeFD_temp = 4;
					break;
			}
		} 
#endif
		
		stateCP = 0;
		// DB Update is done, swap banks
		//BankIndex? (BankIndex = 0) : (BankIndex = 1);
		//Mib_Leaves = &Mib_Leave[BankIndex];
		break;

		default:
			stateCP = 0;
			break;

		} /* Switch Case */
}

#define TIME_BUFFER_SIZE 32
//------------------------------------------------------------------------

//  Alarm bits may be set or cleared between calls to this function.  We need
//  to check for both and send trap messages indicating new alarms have been 
//  set or previously set alarms have been cleared.
void
alarmCheck (int p_responseBuffer)
{
  static int subsequent_passes = 0;
  static int original = 0;
  int originalComplement;
  int newAlarms;
  int clearedAlarms;
  unsigned long notification_id;
  unsigned long history_type;
  char timeStamp[TIME_BUFFER_SIZE];
  int alarm_bit;
  int severity;
  struct variable_t variables[NUM_OIDS_IN_TRAP];
  //devBoardParamsType app_nvParams;

  if (0 != subsequent_passes) {
      if (0 == original) {
          /* this probably shouldn't go out - without taking the
             serial semaphore */
          /* 
	  printf
	    ("alarmCheck:  WARNING!  subsequent_passes && original == 0\r");
	  fflush (stdout);
          */
      }
  }

  originalComplement = ~original;
  newAlarms = p_responseBuffer & originalComplement;

  // Added for Arris to send a trap every x Minutes;  
  // 0 min = Trap off.  To inform the system is well
  if ((MinElapse == 60) && (FirstTimeIn))
  {
	  MinElapse = g_timedate.minutes;
	  FirstTimeIn = 0;
  }
  if ((!FirstTimeIn) && (TrapPeriod)) // TrapPeriod = 0 no trap
  {
	 //customizeReadDevBoardParams (&app_nvParams);
     //TrapPeriod = app_nvParams.aliveTrapPeriod;
	 //Moved up for the new cost reduction chassis
	 if (!AliveTimerNVRamSave)
	 {
		 if (TrapPeriod != g_snmp_send_alive_trap_period) {
			 TrapPeriod = g_snmp_send_alive_trap_period;
		 }
	 }
	 // Because in minutes, it will be here many
	 // times before the if statement is true
     if (g_timedate.minutes != MinElapse)
     {
	    MinCount++;
	    MinElapse = g_timedate.minutes;
	    if (MinCount >= TrapPeriod)
	    {
		   MinCount = 0;
           severity = EARMIB_EarEventLevel_none;
           fillTimeBuffer (&(timeStamp[0]), TIME_BUFFER_SIZE);
		   trapHistoryTableIndex++;
		   setTheTrapVariables (trapHistoryTableIndex,  /* 1..1000 */
                                severity,
                                87,           /* 1..70 */
                                &(timeStamp[0]),
                                &(variables[0]));

		   sendTheTrap (31,  /* (not an alarms */
                        trapHistoryTableIndex,  /* 1..1000 */
                        &(variables[0]),
                        g_variables_array_size,
                        2 ); /* 0 = set; 1 = clear; 2 = event */
		   //populateHistoryTableRow (history_type, timeStamp, severity);
	    }
	 }
  }

  /* Scan across the bit mask looking for set alarm bits.  */
  for (alarm_bit = 0; alarm_bit < MAX_ALARM_BITS; alarm_bit++) {
	  // skip related 48V stuff when configured as AC
	  // logic is as follows: if AC PS and bits of Fuse, 48VA or 48VB Skip sending that trap
	  if (!((PwrType) && ((alarm_bit >= 14) && (alarm_bit <= 16))))  
	  {
		  if ((MASK (alarm_bit)) & newAlarms) {
			  notification_id = convertAtmelAlarmToMIBAlarmType (alarm_bit);
			  history_type = 
				  (unsigned long) convertAtmelAlarmToMIBHistoryType (alarm_bit,
																	 CONVERT_alarm);
	
			  fillTimeBuffer (&(timeStamp[0]), TIME_BUFFER_SIZE);
	
		  trapHistoryTableIndex++;
	
			  if ( (8 == alarm_bit) || (9 == alarm_bit) ) {
				  severity = EARMIB_EarEventLevel_major;
			  } else {
				  severity = EARMIB_EarEventLevel_minor;
			  }
	
			  setTheTrapVariables
				  (trapHistoryTableIndex,  /* 1..1000 */
				   severity,
				   history_type,           /* 1..71 */
				   &(timeStamp[0]),
				   &(variables[0]));
	
			  /* Need 1st var to be trap mib #
			   */
			  sendTheTrap (notification_id,  /* (1..20) for alarms */
						   trapHistoryTableIndex,  /* 1..1000 */
						   &(variables[0]),
						   g_variables_array_size,
						   0 );  /* 0 = set; 1 = clear; 2 = event */
	
		  //  add this row to the Alarm Table
		  PopulateAlarmTableRow (notification_id, timeStamp, severity);
	
		  //  add this row to the History Table
			  /* need 1st var to be history table id type (1..70) */
		  populateHistoryTableRow (history_type, timeStamp, severity);
	
		  } // if MASK(i) & newAlarms
	  }
  }	// for i < MAX_ALARM_BITS


//==============================================================================


  if (0 != subsequent_passes) {	// use the complement of the received
      clearedAlarms = ~(p_responseBuffer | originalComplement);
//      printf ("clearedAlarms is: 0x%x\n", clearedAlarms); // ORed with the 'original'

      for (alarm_bit = 0; alarm_bit < MAX_ALARM_BITS; ++alarm_bit) {
	  if ((MASK (alarm_bit)) & clearedAlarms) {
              /* printf ("clearedAlarm setting [%d]: %s\n", alarm_bit,
                 alarmClearName[alarm_bit]); */
              notification_id = convertAtmelAlarmToMIBClearType (alarm_bit);
              history_type = convertAtmelAlarmToMIBHistoryType (alarm_bit,
                                                                CONVERT_clear);

              fillTimeBuffer (&(timeStamp[0]), TIME_BUFFER_SIZE);

	      trapHistoryTableIndex++;

              severity = EARMIB_EarEventLevel_none;

              setTheTrapVariables
                  (trapHistoryTableIndex,  /* 1..1000 */
                   severity,
                   history_type,    /* 1..70 */
                   &(timeStamp[0]),
                   &(variables[0]));

              sendTheTrap (notification_id,  /* 1..20 "clear" notification id */
                           trapHistoryTableIndex,  /* 1..1000 */
                           &(variables[0]),
                           g_variables_array_size,
                           1 );  /* 0 = set; 1 = clear; 2 = event */

	      // remove this row from the Alarm Table
	      RemoveAlarmTableRow (notification_id); //(alarm_bit + 1);

	      //  add this row to the History Table
              /* need 1st var to be history table id type (1..70) */
	      populateHistoryTableRow (history_type, timeStamp, severity);

          }		// if MASK(alarm_bit) & clearedAlarms
      }			// for alarm_bit < MAX_ALARM_BITS
  }			// if subsequent_passes

  subsequent_passes = TRUE;
  /* hereafter, compare the received with the previous buffer's value
   */
  original = p_responseBuffer;
}



/* Read the serial port and fill the ring buffer
 * This is the producer for the ring buffer - it blocks on getchar
 * The consumer that gets lines out of the ring buffer is not
 * blocking and might return "empty" if there is no full line available.
 */
#define SERIAL_BUFFER_SIZE 0x100
unsigned char g_serialBuffer[SERIAL_BUFFER_SIZE];
unsigned long g_serialReadIdx = 0;
unsigned long g_serialWriteIdx = 0;
unsigned long g_serialBufferWrapped = 0;
unsigned long g_serialBufferOverrun = 0;
unsigned long g_serialLostCharNoSemaphore = 0;

int
increment_serial_index (unsigned long * p_pIdx, unsigned int p_pBufSize) {
    ++(*p_pIdx);
    *p_pIdx %= p_pBufSize;

    return *p_pIdx;
}

int
decrement_serial_index (unsigned long * p_pIdx, unsigned int p_pBufSize) {
    --(*p_pIdx);

    if ( p_pBufSize < *p_pIdx ) {
        *p_pIdx = p_pBufSize - 1;
    }

    return *p_pIdx;
}

void
serialApp (ULONG thread_input) {
    int gotChar;
    int status;
    /* initialize the ring buffer */

    memset (&(g_serialBuffer[0]), ASCII_NULL, SERIAL_BUFFER_SIZE);
    g_serialReadIdx = 0;
    g_serialWriteIdx = 0;
    g_serialBufferWrapped = 0;
    g_serialBufferOverrun = 0;
    g_serialLostCharNoSemaphore = 0;

    /* continuously get characters and put them in the buffer */
    while ( 1 ) {
        gotChar = getchar ();
        switch ( gotChar ) {
        case  0: case  1: case  2: case  3: case  4: case  5: case  6: case  7:
        case  8: case  9: case 10: case 11: case 12: case 13: case 14: case 15:
        case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:
        case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
        case -1:
            gotChar = ASCII_CR;
            break;
        default:
            /* ignore the other cases */
            break;
        }

        /* do not put the blocking getchar inside the semaphore */
        status = semaphore_get_2 (&semaphore_sbuf_info, SEMAPHORE_TMO, 20500);
        if ( TX_SUCCESS == status ) {

            if ( ((g_serialWriteIdx + 1) == g_serialReadIdx) ||
                 ((g_serialReadIdx == 0) &&
                  (g_serialWriteIdx == (SERIAL_BUFFER_SIZE - 1))) ) {

                /* there was no room in the buffer - record the event */
                ++g_serialBufferOverrun;

            } else {
                g_serialBuffer[g_serialWriteIdx] = (unsigned char) gotChar;
                ++g_serialWriteIdx;

                if ( SERIAL_BUFFER_SIZE <= g_serialWriteIdx ) {
                    g_serialWriteIdx = 0;
                    ++g_serialBufferWrapped;
                }
            }
            semaphore_put_2 (&semaphore_sbuf_info, 20500);
        } else {
            ++g_serialLostCharNoSemaphore;
        }
    }
}


int
flushBufferToLastCR (void) {
    unsigned long idx;
    int status;
    int rc = SERIAL_READ_SUCCESS;

    status = semaphore_get_2 (&semaphore_sbuf_info, SEMAPHORE_TMO, 20502);
    if ( TX_SUCCESS == status ) {
        idx = g_serialWriteIdx;
        while ( (g_serialReadIdx != idx) &&
                (ASCII_CR != g_serialBuffer[idx]) ) {
            decrement_serial_index (&idx, SERIAL_BUFFER_SIZE);
        }

        /* now, either idx == g_serialReadIdx,
           or ASCII_CR == g_serialBuffer[idx],
           or both
         */

        g_serialReadIdx = idx;
        if ( (ASCII_CR == g_serialBuffer[idx]) &&
             (g_serialReadIdx != g_serialWriteIdx) ) {
            increment_serial_index (&g_serialReadIdx, SERIAL_BUFFER_SIZE);
        }

        semaphore_put_2 (&semaphore_sbuf_info, 20502);
    } else {
        rc = SERIAL_READ_NO_SEMAPHORE;
    }
    return rc;
}

#define SERIAL_FD_COM0  0x102
#define SERIAL_FD_COM1  0x103

int
flushBuffer (void) {
    int status;
    int rc = SERIAL_READ_SUCCESS;
    int rc_f = 0;

    status = semaphore_get_2 (&semaphore_sbuf_info, SEMAPHORE_TMO, 20503);
    if ( 0 != (rc_f = tcflush (SERIAL_FD_COM0, TCIFLUSH)) ) {
    }

    if ( 0 != (rc_f = fflush (stdin)) ) {
    }

    if ( TX_SUCCESS == status ) {
        g_serialReadIdx = g_serialWriteIdx;
        semaphore_put_2 (&semaphore_sbuf_info, 20503);
    } else {
        rc = SERIAL_READ_NO_SEMAPHORE;
    }

    return rc;
}

/* 
 * put the next full line in the caller-supplied buffer
 * and report the length found (return 0 (success))
 * if the caller-supplied buffer isn't big enough, fill it up
 * and return 0(success), leaving the rest of the line in the buffer
 * If there isn't a full line to return (no CR) then just
 * return an empty string and return code -1 (failure)
 * the size that is returned is the number of characters in
 * p_buffer that have been written to (including ASCII_NULL at the end)
 * NOTE:  *p_pSize must be greater than 2
 */
int
getLineFromSerialBuffer (unsigned char * p_buffer, unsigned int *p_pSize) {
    int rc = SERIAL_READ_EMPTY;
    unsigned long idx = g_serialReadIdx;  /* index into serial buffer */
    int uIdx = 0; /* index into user-supplied buffer */
    int status;

    status = semaphore_get_2 (&semaphore_sbuf_info, SEMAPHORE_TMO, 20501);
    if ( TX_SUCCESS == status ) {
        /* look ahead in the buffer for a CR; if none return -1 */
        /* FIXME:  look ahead for LAST CR; throw away all the stuff
         * before that last line
         */
        while ( (g_serialWriteIdx != idx) &&
                (ASCII_CR != g_serialBuffer[idx]) ) {
            increment_serial_index (&idx, SERIAL_BUFFER_SIZE);
        }

        /* either we found a CR or we're at the end of the chars in the buffer */
        if ( ASCII_CR == g_serialBuffer[idx] ) {
            rc = SERIAL_READ_SUCCESS;
            if ( g_serialReadIdx > idx ) {
                while ( (uIdx < (*p_pSize - 2)) &&
                        (g_serialReadIdx < SERIAL_BUFFER_SIZE) ) {
                    p_buffer[uIdx] = g_serialBuffer[g_serialReadIdx];
                    /* g_serialBuffer[g_serialReadIdx] = ASCII_NULL; */
                    ++uIdx;
                    increment_serial_index (&g_serialReadIdx,
                                            SERIAL_BUFFER_SIZE);
                }
            }
            /* either we're at the end of g_serialBuffer and have wrapped,
             * or we didn't have to wrap and we're just starting to copy,
             * or we ran out of space in p_buffer
             */
            while ( (uIdx < (*p_pSize - 2)) &&
                    (ASCII_CR != g_serialBuffer[g_serialReadIdx]) ) {
                p_buffer[uIdx] = g_serialBuffer[g_serialReadIdx];
                /* g_serialBuffer[g_serialReadIdx] = ASCII_NULL; */
                ++uIdx;
                increment_serial_index (&g_serialReadIdx, SERIAL_BUFFER_SIZE);
            }
            p_buffer[uIdx] = ASCII_NULL;
            ++uIdx;
            if ( (*p_pSize == uIdx + 1) &&
                 (ASCII_CR != g_serialBuffer[g_serialReadIdx]) ) {
                rc = SERIAL_READ_TRUNCATE;
            }
            *p_pSize = uIdx;
            rc = 0;
            /* bring g_serialReadIdx past the CR in the serialBuffer
             */
            if ( ASCII_CR == g_serialBuffer[g_serialReadIdx] ) {
                /* g_serialBuffer[g_serialReadIdx] = ASCII_NULL; */
                increment_serial_index (&g_serialReadIdx, SERIAL_BUFFER_SIZE);
            }
        } else {
            *p_pSize = 0;
            rc = SERIAL_READ_EMPTY;
        }
        semaphore_put_2 (&semaphore_sbuf_info, 20501);
    } else {
        rc = SERIAL_READ_NO_SEMAPHORE;
    }

    return rc;
}


#if 0
/* referred to from menu.c */
unsigned int g_alarmCheck_caret = 0;
//------------------------------------------------------------------------


void
restorePumpVal (void) {
    float floatVal, floatVal2;
    long powerValue;
    char buffer[MAX_FIELD_LEN];

    /*  no!  powerValue wasn't initialized in the place where
     *  I took it from (alarmCheck), either!
     *  Except in the few cases where an alarm was set and cleared
     *  in the same call to alarmCheck (which is impossible)
     */
    floatVal = (float) powerValue;
    floatVal2 = (floatVal / 10.0f);
    sprintf (buffer, "ZIMSOP %1.1f\r", floatVal2);
    printf ("%s", buffer);
    fflush (stdout);
}


void
turnPumpsDown (void) {
    float floatVal, floatVal2;
    long powerValue;
    unsigned long lowPowerSetpoint = 150;	// turn the pumps down!
    char input_buffer[MAX_2U_RESPONSE];
    int input_length = MAX_2U_RESPONSE;
    char buffer[MAX_FIELD_LEN];

    do { // obtain present POUT value for restoration
        printf ("ZIMROP\r");
        fflush (stdout);

	/* secret signal to 2U2 to make it behave like 2U did */
        input_buffer[0] = ASCII_CARET;

        ++g_alarmCheck_caret;
        readFrom2U2 (&(input_buffer[0]), &input_length);
    }
    while (((strncmp (input_buffer, "ZROP", 4)))
           || (!(strstr (input_buffer, "PORZ"))));

    char opower[10], value[16];

    sscanf (input_buffer, "%s%s", opower, value);
    strcpy (input_buffer, value);

    powerValue = strtol (input_buffer, '\0', 0);

    floatVal = (float) lowPowerSetpoint;	// this is 15 dBm - we're turning the pumps down
    floatVal2 = (floatVal / 10.0f);
    sprintf (buffer, "ZIMSOP %1.1f\r", floatVal2);
    printf ("%s", buffer);
    fflush (stdout);

}
#endif

void
fillTimeBuffer (
                char * p_timeBuffer,
                int p_bufSize
) {
    int status;
    rtcDataType timedate;

#if BSP_INCLUDE_RTC_DRIVER
    status = semaphore_get_2 (&semaphore_rtc_info, SEMAPHORE_TMO, 30000);
    if (TX_SUCCESS == status) {
        memcpy (&timedate, &g_timedate, sizeof (rtcDataType));
        semaphore_put_2 (&semaphore_rtc_info, 30000);
    }
#endif
    if (TX_SUCCESS == status) {
        timedate.year += 2000;
        /* does NET/OS have snprintf? */
        snprintf (p_timeBuffer, p_bufSize, "%d/%d/%d  %2d:%2d:%2d",
                  timedate.month, timedate.day, timedate.year,
                  timedate.hours, timedate.minutes, timedate.seconds);
    }
}

//------------------------------------------------------------------------------------------


BOOL
isValidPwd (char *pwd)
{
  return (strlen (pwd) > 0 && strlen (pwd) < NASYSACC_STRLEN_PASSWORD);
}
