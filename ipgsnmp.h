
#ifndef _IPGSNMP_H_
#define _IPGSNMP_H_


#include "boardParams.h"
#include "MAX_ALARM.h"

#include "../../../h/man_api.h"
#include "../../../h/snmp/snmp.h"

#define MAX_EVENTS              3		// 100
#define MAX_HISTORY          	1000		// 5

#define	unsigned32MAX		4026531839	// 10
#define	test32MAX		10

#define SEM_TIMEOUT 100 /*GORD how is this != SEMAPHORE_TMO? */

#define SNMP_SEND_ALIVE_TRAP_PERIOD_MIN 3
#define SNMP_SEND_ALIVE_TRAP_PERIOD_MAX 20

/* these are OIDs for traps that are not in EARMIB.h. Beware! */
#define SNMPMgmtStationAddEvent      "1.3.6.1.4.1.19775.1.1.6.25"
#define SNMPMgmtStationRemoveEvent   "1.3.6.1.4.1.19775.1.1.6.26"
#define PrivateCommunityStringEvent  "1.3.6.1.4.1.19775.1.1.6.27"
#define PublicCommunityStringEvent   "1.3.6.1.4.1.19775.1.1.6.28"


/* These are #defines for the enumerated types defined in the MIB
   I have no idea why the mib compiler doesn't generate these ? */
#define EARMIB_EarEventLevel_major 1
#define EARMIB_EarEventLevel_minor 2
#define EARMIB_EarEventLevel_none  3

#define EARMIB_EarEnabled_disabled 0
#define EARMIB_EarEnabled_enabled  1

#define EARMIB_EarDeviceType_legacy 1
#define EARMIB_EarDeviceType_master 2
#define EARMIB_EarDeviceType_slave  3

#define EARMIB_EarDataMode_off     1
#define EARMIB_EarDataMode_als     2
#define EARMIB_EarDataMode_nominal 3

#define EARMIB_EarControlMode_power   1
#define EARMIB_EarControlMode_current 2
#define EARMIB_EarControlMode_gain    3

#define EARMIB_EarDigiSelectedFirmwareBank_flashbank1 1
#define EARMIB_EarDigiSelectedFirmwareBank_flashbank2 2

#define EARMIB_EarDigiFirmwareConfirm_confirmed   1
#define EARMIB_EarDigiFirmwareConfirm_unconfirmed 2

#define EARMIB_EarRelayStatus_normal  1
#define EARMIB_EarRelayStatus_tripped 2

extern unsigned int manHistoryTableKey;

int 	AlarmTableIndexFn (void *index, void *row, void *indexInfo);
int 	EventTableIndexFn (void *index, void *row, void *indexInfo);
int 	HistoryTableIndexFn (void *index, void *row, void *indexInfo);
int 	SnmpMngrTableIndexFn (void *index, void *row, void *indexInfo);


void 	PopulateAlarmTableRow (int type, char *timestamp, int severity);
int 	GetFreeAlarmIndex (void);
void 	RemoveAlarmTableRow (int type);

void 	populateEventTableRow (int type, char *timestamp, int severity);
int  	getFreeEventIndex (void);
void 	RemoveEventTableRow (int indexVal);

void 	populateHistoryTableRow (int type, char *timestamp, int severity);
void 	RemoveHistoryTableRow (int indexVal);


int 	AddSnmpMngrEntry (devBoardParamsType *nvParams, INT32 IpAddr);
void 	RemoveSnmpMngrEntry (devBoardParamsType *nvParams, int station);
void    SyncSnmpFromNvram( devBoardParamsType *nvParams );
void    UpdateNvramFromSnmp( devBoardParamsType *nvParams );


void configNotification (char* id, char* severity);


#define CONVERT_alarm 0
#define CONVERT_clear 1
#define CONVERT_event 2

int     convertAtmelAlarmToMIBHistoryType (int p_iAlarm, int p_alarm_or_clear);
int     convertAtmelAlarmToMIBAlarmType (int p_iAlarm);
int     convertAtmelAlarmToMIBClearType (int p_iAlarm);

int     convertMIBHistoryTypeToAtmelAlarm (int p_iHistoryType);
int     convertMIBAlarmTypeToAtmelAlarm (int p_iAlarmType);
int     convertMIBClearTypeToAtmelAlarm (int p_iClearType);

int     convertMIBAlarmTypeToMIBHistoryType (int p_iAlarmType);
int     convertMIBClearTypeToMIBHistoryType (int p_iClearType);
int     convertMIBEventTypeToMIBHistoryType (int p_iEventType);

int     convertMIBHistoryTypeToMIBAlarmType (int p_iHistoryType);
int     convertMIBHistoryTypeToMIBClearType (int p_iHistoryType);
int     convertMIBHistoryTypeToMIBEventType (int p_iHistoryType);



#endif 								// _IPGSNMP_H_ 

