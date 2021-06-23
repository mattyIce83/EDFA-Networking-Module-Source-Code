/*
 *
 * $Id: configNotification.c,v 1.2 2010-03-08 20:06:02 eugene Exp $
 *
 * $Log: configNotification.c,v $
 * Revision 1.2  2010-03-08 20:06:02  eugene
 * Bug_908: Modified configNotification() to replace hardcoded size of
 * variables[] array with the NUM_OIDS_IN_TRAP macro.
 *
 *
 */

#define WANT_MIBMAN


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <tx_api.h>
#include <reg_def.h>
#include <man_api.h>
#include <asn1.h>
#include <sockapi.h>
#include <tservapi.h>
#include <string.h>


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
#include "STACK_SIZE.h"
#include "ascii.h"
#include "configNotification.h"
#include "trap.h"


#define snmpThreadTMO	15000

#define MASK(x) ((int)1 << x)
#define	ALARM_INTERLOCK_FLAG	 10
#define	ALARM_KEYSWITCH_FLAG	 14

#define VALUE_UNDEFINED 0
#define POWER_MODE "POWER"
#define POWER_MODE_VAR 1

#define CURRENT_MODE "CURRENT"
#define CURRENT_MODE_VAR 2

#define GAIN_MODE "GAIN"
#define GAIN_MODE_VAR 3

#define NORMAL "Normal"
#define TRIPPED "Tripped"

#define NORMAL_STATE 1
#define TRIPPED_STATE 2

#define DISABLE_RELAY 0
#define ENABLE_RELAY 1



#define MAX_TRC		 800
#define	REPETITIONS	   2
#define MASK(x) 	((int)1 << x)
#define TMO_VALUE	  10

#include "MAX_ALARM.h"

#define	unsigned32MAX		4026531839	// 10

extern unsigned int alarmsSet;

extern unsigned int trapHistoryTableIndex;
extern unsigned short FIRST;
extern unsigned short wrapFlag;


extern TX_TIMER timer_snmpApp;	// found in root.c

void root_breakpoint(long unsigned int *);

extern unsigned int SNMPAppFlag ;
extern unsigned int savedSemWaitCount;
extern unsigned int semWaitCount;
extern unsigned long savedIntVal;
extern unsigned long savedLaserCurrentValue;
extern unsigned long preserveLowCurrent;
extern unsigned long currentPowerValue;
extern unsigned long preserveLowPower;
extern unsigned long lowPowerSetpoint ;
extern unsigned long lowCurrentSetpoint ;
extern unsigned short chassisReset ;
extern unsigned short alreadyREAD ;

extern int count;

extern unsigned long g_snmp_send_trap_timeout;
extern int g_snmp_send_trap_failed;
extern int g_snmp_send_trap_last_result;

void configErrorNotification(struct varBind *OIDInfo, char *msg)
{
    manVarType *oID;

    oID = OIDInfo->vp->manInfo;

    configNotification(oID->id, msg);
}


//------------------------------------------------------------------------


void configChangeNotification(struct varBind *OIDInfo)
{
    manVarType *oID;

    oID = OIDInfo->vp->manInfo;

    configNotification(oID->id, "none (3)");
}


//------------------------------------------------------------------------

char * theStrings [] = {
    "unmatchable_string_ooga_booga_boo",
    EARMIB_earControlMode,
    EARMIB_earRtcTime,
    EARMIB_earDigiSelectedFirmwareBank,
    EARMIB_earDigiFirmwareConfirm,
    EARMIB_earUpgradeDigiFirmware,
    EARMIB_earTelnetTimeout,
    EARMIB_earIpAddress,
    EARMIB_earSubnetMask,
    EARMIB_earGateway,
    EARMIB_earGainControlSetpoint,
    EARMIB_earOutputPowerSetpointValue,
    EARMIB_earCurrentSetpointValue,
    EARMIB_earInputPowerLowAlarmLevel,
    EARMIB_earSetRelayInputPowerLowAlarm,
    EARMIB_earSetRelayInputPowerHighAlarm,
    EARMIB_earSetRelayInputPowerTooHighAlarm,
    EARMIB_earSetRelayOutputPowerLowAlarm,
    EARMIB_earSetRelayOutputPowerHighAlarm,
    EARMIB_earSetRelayOutputPowerTooHighAlarm,
    EARMIB_earSetRelayFanFailureAlarm,
    EARMIB_earSetRelayAmbientTemperatureHighAlarm,
    EARMIB_earPowerSafetyMode,
    EARMIB_earPumpsOff,
    EARMIB_earChassisReboot,
    "1.3.6.1.4.1.19775.1.1.6.25", /* EARMIB_earMgmtStationAddEvent */
    "1.3.6.1.4.1.19775.1.1.6.26", /* EARMIB_earMgmtStationRemoveEvent */
    "1.3.6.1.4.1.19775.1.1.6.27", /* EARMIB_earPrivateCommunityStrEvent */
    "1.3.6.1.4.1.19775.1.1.6.28", /* EARMIB_earPublicCommunityStringEvent */
    EARMIB_earTemperatureAlarmThreshold,
    EARMIB_earTrapPort,
    (char *) 0
};

static unsigned long
string_to_event_type (char * p_string) {
    unsigned long type = 1;
    
    while ( (NULL != theStrings[type]) &&
            (strcmp (theStrings[type], p_string)) ) {
        ++type;
    }

    if ( NULL == theStrings[type] ) {
        type = -1;
    }

    return type;
}

//------------------------------------------------------------------------


//  Alarm bits may be set or cleared between calls to this function.  We need
//  to check for both and send trap messages indicating new alarms have been 
//  set or previously set alarms have been cleared.

/* This function sends "event" traps */

int g_snmp_toggle = 0;
int g_snmp_mgmt_stn_idx = -1;

#define TIMESTAMP_BUFFER_SIZE 32
/* note, parameter 2 is ignored */
void configNotification(char *id, char *severity)
{
    static int status;
    struct variable_t variables[NUM_OIDS_IN_TRAP];
    unsigned long event_type;
    unsigned long severity_id;
    unsigned long history_type;
    char timeStamp[TIMESTAMP_BUFFER_SIZE];
    int variables_size;

    variables_size =
        (((void *) &(variables[1])) - ((void *)&(variables[0]))) * NUM_OIDS_IN_TRAP;

    event_type = string_to_event_type (id);
    /* history_type = event_type + MAX_ALARM_OFFSET + MAX_ALARM_RESET_OFFSET; */
    history_type = convertMIBEventTypeToMIBHistoryType (event_type);
    severity_id = EARMIB_EarEventLevel_none;  // none (3)

    fillTimeBuffer (&(timeStamp[0]), TIMESTAMP_BUFFER_SIZE);

    trapHistoryTableIndex++;

    setTheTrapVariables
        (trapHistoryTableIndex,
         severity_id,
         history_type,
         &(timeStamp[0]),
         &(variables[0]));

    sendTheTrap (event_type,
                 trapHistoryTableIndex,
                 &(variables[0]),
                 variables_size,
                 2 /* 0 = set; 1 = clear; 2 = event */ );

    populateHistoryTableRow (history_type, timeStamp, severity_id);
}


