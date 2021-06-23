///////////////////////////////////////////////////////////////////////////////
// $Id: ipgsnmp.c,v 1.3 2011-10-11 13:50:42 hazem Exp $
//
//      IPG Photonics Corporation
//      COPYRIGHT (c) 2005 IPG Photonics Coroporation ALL RIGHTS RESERVED
//  This program may not be reproduced, in whole or in part in any
//  form or any means whatsoever without the written permission of:
//
//  IPG Photonics Corporation
//  50 Old Webster Road 
//  Oxford, MA 01540, USA 
//
//  $Log: ipgsnmp.c,v $
//  Revision 1.3  2011-10-11 13:50:42  hazem
//  This version adds 15Min Trap meassage and Allow to remote reset the
//  system via CLI and SNMP.  But this file had no change.
//
//  Revision 1.2  2010/03/08 20:07:24  eugene
//  Bug_908: Modified setTheTrapVariables() and sendTheTrap() to support
//  sending sysName and sysLocation for every trap. Also created
//  ipgGetSysName() and ipgGetSysLocation() (though, probably,
//  SNMP library functions could be used).
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <snmpapi.h>
#include <fs.h>

#include <sockapi.h>
#include "ace_params.h"
#include "boardParams.h"

#include "EARMIB.h"
#include "MibMan.h"
#include "ipgsnmp.h"
#include "semaphores.h"
#include "menu.h"
#include "configNotification.h"
#include "trap.h"

//------------------------------------------------------------------------

extern char sysname[];
extern char syslocation[];

unsigned char TrapFirstTimeIn = 1; 
unsigned int manHistoryTableKey;

//extern manVarType EARMIB_earSnmpMngrTable;

static char *ipgGetSysName(void);
static char *ipgGetSysLocation(void);


static void
openSocket ()
{

  int fd;
  struct sockaddr_in sin2;

  //  open some UDP Services
  fd = socket (AF_INET, SOCK_DGRAM, 0);
  if (fd >= 0)
    {
      memset (&sin2, 0, sizeof (struct sockaddr_in));
      /* Shouldn't this be sin2.sin_family = htons (AF_INET); ? */
      sin2.sin_family = AF_INET;
      sin2.sin_addr.s_addr = 0;
      sin2.sin_port = htons (10002);
      bind (fd, &sin2, sizeof (struct sockaddr_in));
      return;
    }
}

//------------------------------------------------------------------------


/*
 *
 *  Function: int AlarmTableIndexFn (void *index, void *row, void *indexInfo)
 *
 *  Description:
 *
 *      This routine compares a row against an index value and determines
 *      whether the row comes before, at, or after the index value.
 *
 *      For this table, the index is a simple numeric value.
 *
 *  Parameters:
 *
 *      index       pointer to int that is numeric index
 *      row         pointer to row in table
 *      indexInfo   not used
 *
 *  Return Values:
 *
 *      <0      row comes before the index
 *      0       row is at the index position
 *      >0      row comes after the index
 *
 */

int AlarmTableIndexFn(void *index, void *row, void *indexInfo)
{
    earAlarmTableType *tableRow = (earAlarmTableType *) row;
    int *indexValue = (int *) index;

    return (tableRow->earAlarmIndex - *indexValue);
}


//------------------------------------------------------------------------


/*
 *
 *  Function: int SnmpMngrTableIndexFn (void *index, void *row, void *indexInfo)
 *
 *  Description:
 *
 *      This routine compares a row against an index value and determines
 *      whether the row comes before, at, or after the index value.
 *
 *  Parameters:
 *
 *      index       pointer to int that is numeric index
 *      row         pointer to row in table
 *      indexInfo   not used
 *
 *  Return Values:
 *
 *      <0      row comes before the index
 *      0       row is at the index position
 *      >0      row comes after the index
 *
 */

int SnmpMngrTableIndexFn(void *index, void *row, void *indexInfo)
{
    earSnmpMngrTableType *tableRow = (earSnmpMngrTableType *) row;
    int *indexValue = (int *) index;

    return (*(unsigned int *) (&(tableRow->earSnmpMngrIndex)) -
	    *indexValue);
}


//------------------------------------------------------------------------


/*
 *
 *  Function: PopulateAlarmTableRow
 *
 *  Description:
 *
 *      This routine adds a row to the alarm status table.
 *
 */
void PopulateAlarmTableRow(int type, char *timestamp, int severity)
{
    earAlarmTableType row;
    manTableIndexType manIndex;
    MAN_ERROR_TYPE ccode;
    //int pos;
    //int nextindex;

    /* Check if there is already an alarm of this type and if so, remove it */
    RemoveAlarmTableRow(type);

    row.earAlarmIndex = GetFreeAlarmIndex();

    row.earAlarmType = type;

    row.earAlarmTimeStamp.buffer = timestamp;
    row.earAlarmTimeStamp.length = strlen(timestamp);
    row.earAlarmTimeStamp.maxLength = strlen(timestamp);

    row.earAlarmSeverity = severity;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &row.earAlarmIndex;

    ccode =
	manInsertSnmpRow(EARMIB_earAlarmTable, &manIndex, &row,
			 SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
    if (ccode != MAN_SUCCESS) {
	InsertSnmpRowFailed++;	/* put breakpoint here */
//        printf ("manInsertSnmpRow() returned error code %d.\n", ccode);
    }
    //manIndex.wantExact = TRUE;
    //manIndex.snmpIndex = &row.earAlarmIndex;

    //ccode = manGetSnmpRowPos( EARMIB_earAlarmTable, &manIndex, &pos, MAN_TIMEOUT_FOREVER );
    //if( ccode != MAN_SUCCESS )
    //{
//        printf ("manGetSnmpRowPos() returned error code %d.\n", ccode);
    //}

}


//------------------------------------------------------------------------


/*
 *
 *  Function: RemoveAlarmTableRow
 *
 *  Description:
 *
 *      This routine removves a row to the alarm status table.
 *
 */
void RemoveAlarmTableRow(int type)
{
    static char r_timestamp[MAX_ALARM_BUF_LEN];

    manTableIndexType manIndex;
    manTableIndexType manOldIndex;

    earAlarmTableType row;
    MAN_ERROR_TYPE ccode;
    int pos;
    int index;
    int oldIndex;

    row.earAlarmTimeStamp.buffer = r_timestamp;
    row.earAlarmTimeStamp.length = 0;
    row.earAlarmTimeStamp.maxLength = MAX_ALARM_BUF_LEN;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &index;

    for (index = 1; index <= MAX_ALARMS; index++) {
	ccode =
	    manGetSnmpRow(EARMIB_earAlarmTable, &manIndex, &row,
			  SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode == MAN_SUCCESS && (row.earAlarmType == type)) {
	    ccode =
		manDeleteSnmpRow(EARMIB_earAlarmTable, &manIndex,
				 SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	    if (ccode != MAN_SUCCESS) {
		InsertSnmpRowFailed++;
//         printf ("manDeleteSnmpRow() returned error code %d.\n", ccode);
	    }
	    break;
	} else {
	    InsertSnmpRowFailed++;
	}
    }

    manOldIndex.wantExact = TRUE;
    manOldIndex.snmpIndex = &oldIndex;

    for (; index <= MAX_ALARMS; index++) {
	oldIndex = index + 1;
	ccode =
	    manGetSnmpRow(EARMIB_earAlarmTable, &manOldIndex, &row,
			  SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode == MAN_SUCCESS) {
	    row.earAlarmIndex = index;
	    manSetSnmpRow(EARMIB_earAlarmTable, &manOldIndex, &manIndex,
			  &row, SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	} else {
	    InsertSnmpRowFailed++;
	}
    }
}


//------------------------------------------------------------------------


/*
 *
 *  Function: GetFreeAlarmIndex
 *
 *  Description:
 *
 *      This routine finds the first free index in the alarm status table.
 *
 */
int GetFreeAlarmIndex(void)
{
    manTableIndexType manIndex;
    MAN_ERROR_TYPE ccode;
    int pos;
    int index;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &index;

    for (index = 1; index <= MAX_ALARMS; index++) {
	ccode =
	    manGetSnmpRowPos(EARMIB_earAlarmTable, &manIndex, &pos,
			     SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode != MAN_SUCCESS) {
	    /* need to understand this better */
	    break;
	}
    }

    return index;
}

void SyncSnmpFromNvram(devBoardParamsType * nvParams)
{
    int station;
    manTableIndexType manIndex;
    earSnmpMngrTableType row;
    MAN_ERROR_TYPE ccode;

    for (station = MAX_STATIONS; station > 0; station--) {
	row.earSnmpMngrIndex = station;

	manIndex.wantExact = TRUE;
	manIndex.snmpIndex = &row.earSnmpMngrIndex;

	ccode =
	    manDeleteSnmpRow(EARMIB_earSnmpMngrTable, &manIndex,
			     SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
    }

    for (station = 0; station < MAX_STATIONS; station++) {
	row.earSnmpMngrIndex = station + 1;
	*((INT32 *) row.earSnmpMngrIpAddr) =
	    nvParams->mgmtStations[station].ipAddress;

	if (nvParams->mgmtStations[station].ipAddress != 0) {
	    row.earSnmpMngrRowStatus = STATUS_ACTIVE;
	} else {
	    row.earSnmpMngrRowStatus = STATUS_NOT_IN_SERVICE;
	}

	manIndex.wantExact = TRUE;
	manIndex.snmpIndex = &row.earSnmpMngrIndex;

	ccode =
	    manInsertSnmpRow(EARMIB_earSnmpMngrTable, &manIndex, &row,
			     SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode != MAN_SUCCESS) {
	    InsertSnmpRowFailed++;
//                printf ("manInsertSnmpRow() returned error code %d.\n", ccode);
	}
    }
}

void UpdateNvramFromSnmp(devBoardParamsType * nvParams)
{
    int station;
    manTableIndexType manIndex;
    earSnmpMngrTableType row;
    MAN_ERROR_TYPE ccode;
    //extern manVarType EARMIB_earSnmpMngrTable;

    for (station = 0; station < MAX_STATIONS; station++) {
	row.earSnmpMngrIndex = station + 1;

	manIndex.wantExact = TRUE;
	manIndex.snmpIndex = &row.earSnmpMngrIndex;

	ccode =
	    manGetSnmpRow(EARMIB_earSnmpMngrTable, &manIndex, &row,
			  SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode == MAN_SUCCESS) {
	    nvParams->mgmtStations[station].ipAddress =
		*((INT32 *) row.earSnmpMngrIpAddr);
	} else {
	    InsertSnmpRowFailed++;
	}
    }
}


//------------------------------------------------------------------------


int AddSnmpMngrEntry(devBoardParamsType * nvParams, INT32 IpAddr)
{
    int station;

    // Check if this IP address is already in the table
    for (station = 0; station < MAX_STATIONS; station++) {
	if (nvParams->mgmtStations[station].ipAddress == IpAddr) {
	    return -1;
	}
    }

    // Add the new address in the first free spot
    for (station = 0; station < MAX_STATIONS; station++) {
	if (nvParams->mgmtStations[station].ipAddress == 0) {
	    nvParams->mgmtStations[station].ipAddress = IpAddr;
	    break;
	}
    }

    return station;
}


//------------------------------------------------------------------------


void RemoveSnmpMngrEntry(devBoardParamsType * nvParams, int station)
{
    int i;
    for (i = 0; i < MAX_STATIONS; i++) {
	if (i == station)
	    nvParams->mgmtStations[i].ipAddress = 0;
    }
}


//------------------------------------------------------------------------

#if 0

void populateEventTableRow(int type, char *timestamp, int severity)
{
    MAN_ERROR_TYPE ccode;

    earEventTableType row;

    int index, nextindex, pos;

    manTableIndexType manIndex;

    static unsigned short maxFlag = 0;

//.............................................................................

    row.earEventIndex = getFreeEventIndex();

    if (row.earEventIndex == MAX_EVENTS)	// we've reached the end of the table, scroll off!
	maxFlag++;

    if (maxFlag > 1)		// the top entry will be removed and the newest added
    {				// at the bottom from now on
	index = 1;
	RemoveEventTableRow(index);
	row.earEventIndex = getFreeEventIndex();
    }

    row.earEventType = type;

    row.earEventTimeStamp.buffer = timestamp;
    row.earEventTimeStamp.length = strlen(timestamp);
    row.earEventTimeStamp.maxLength = strlen(timestamp);

    row.earEventSeverity = severity;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &row.earEventIndex;

    ccode =
	manInsertSnmpRow(EARMIB_earEventTable, &manIndex, &row,
			 SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
    if (ccode != MAN_SUCCESS) {
//        printf ("manInsertSnmpRow() returned error code %d.\n", ccode);
    }

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &row.earEventIndex;

    ccode =
	manGetSnmpRowPos(EARMIB_earEventTable, &manIndex, &pos,
			 SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
    if (ccode != MAN_SUCCESS) {
//        printf ("manGetSnmpRowPos() returned error code %d.\n", ccode);
    }
}


//------------------------------------------------------------------------


int getFreeEventIndex(void)
{
    manTableIndexType manIndex;
    MAN_ERROR_TYPE ccode;
    int pos;
    int index;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &index;

    for (index = 1; index < MAX_EVENTS; index++) {
	ccode =
	    manGetSnmpRowPos(EARMIB_earEventTable, &manIndex, &pos,
			     SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode != MAN_SUCCESS) {
	    break;
	}
    }

    return index;
}


//------------------------------------------------------------------------


void RemoveEventTableRow(int indexVal)
{
    static char r_timestamp[MAX_ALARM_BUF_LEN];

    manTableIndexType manIndex;
    manTableIndexType manOldIndex;

    earEventTableType row;
    MAN_ERROR_TYPE ccode;
    int pos;
    int index;
    int oldIndex;

    row.earEventTimeStamp.buffer = r_timestamp;
    row.earEventTimeStamp.length = 0;
    row.earEventTimeStamp.maxLength = MAX_ALARM_BUF_LEN;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &index;

    for (index = 1; index <= MAX_EVENTS; index++) {
	ccode =
	    manGetSnmpRow(EARMIB_earEventTable, &manIndex, &row,
			  SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
//        if (ccode == MAN_SUCCESS && row.earEventType == type)
	if (ccode == MAN_SUCCESS && row.earEventIndex == indexVal) {
	    ccode =
		manDeleteSnmpRow(EARMIB_earEventTable, &manIndex,
				 SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	    if (ccode != MAN_SUCCESS) {
//                printf ("manDeleteSnmpRow() returned error code %d.\n", ccode);
	    }
	    break;
	}
    }

    manOldIndex.wantExact = TRUE;
    manOldIndex.snmpIndex = &oldIndex;

    for (; index <= MAX_EVENTS; index++) {
	oldIndex = index + 1;
	ccode =
	    manGetSnmpRow(EARMIB_earEventTable, &manOldIndex, &row,
			  SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode == MAN_SUCCESS) {
	    row.earEventIndex = index;
	    ccode =
		manSetSnmpRow(EARMIB_earEventTable, &manOldIndex,
			      &manIndex, &row,
			      SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	}
    }

}


//------------------------------------------------------------------------


int EventTableIndexFn(void *index, void *row, void *indexInfo)
{
    earEventTableType *tableRow = (earEventTableType *) row;
    int *indexValue = (int *) index;

    return (tableRow->earEventIndex - *indexValue);
}

#endif

//------------------------------------------------------------------------

/*GORD I think this is the active version */

void populateHistoryTableRow(int type, char *timestamp, int severity)
{
    MAN_ERROR_TYPE ccode;
    earHistoryTableType row, row2;
    manTableIndexType manIndex;

    if ( TX_SUCCESS ==
         semaphore_get_2 (&semaphore_history_table_info, SEM_TIMEOUT, 30301) ) {
	row.earHistoryIndex = manHistoryTableKey;

	if (manHistoryTableKey >= MAX_HISTORY) {
	    // delete the oldest entry to make room for another
	    RemoveHistoryTableRow(manHistoryTableKey - MAX_HISTORY);
	}

	row.earHistoryType = type;
	manHistoryTableKey++;

	row.earHistoryTimeStamp.buffer = timestamp;
	row.earHistoryTimeStamp.length = strlen(timestamp);
	row.earHistoryTimeStamp.maxLength = strlen(timestamp);

	row.earHistorySeverity = severity;

	manIndex.wantExact = TRUE;
	manIndex.snmpIndex = &row.earHistoryIndex;

	ccode =
	    manInsertSnmpRow(EARMIB_earHistoryTable, &manIndex, &row,
			     SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
	if (ccode != MAN_SUCCESS) {
	    InsertSnmpRowFailed++;
	    //        printf ("manInsertSnmpRow() returned error code %d.\n", ccode);
	}

	semaphore_put_2 (&semaphore_history_table_info, 30301);
    }
}


//------------------------------------------------------------------------


void RemoveHistoryTableRow(int keyVal)
{
    static char r_timestamp[MAX_ALARM_BUF_LEN];
    manTableIndexType manIndex;
    earHistoryTableType row;
    MAN_ERROR_TYPE ccode;
    // int tblrow;

    row.earHistoryTimeStamp.buffer = r_timestamp;
    row.earHistoryTimeStamp.length = 0;
    row.earHistoryTimeStamp.maxLength = MAX_ALARM_BUF_LEN;

    manIndex.wantExact = TRUE;
    manIndex.snmpIndex = &keyVal;

    // for (tblrow = 1; tblrow < manHistoryTableKey; tblrow++) {
	ccode =
	    manGetSnmpRow(EARMIB_earHistoryTable, &manIndex, &row,
			  SEMAPHORE_TMO);
	if (ccode == MAN_SUCCESS && row.earHistoryIndex == keyVal) {
	    ccode =
		manDeleteSnmpRow(EARMIB_earHistoryTable, &manIndex,
				 SEMAPHORE_TMO);
	    if (ccode != MAN_SUCCESS) {
		InsertSnmpRowFailed++;
	    }
	} else {
	    InsertSnmpRowFailed++;
	}
    // }

    return;
}


//------------------------------------------------------------------------


int HistoryTableIndexFn(void *key, void *row, void *indexInfo)
{
    earHistoryTableType *tableRow = (earHistoryTableType *) row;
    int *keyValue = (int *) key;

    return (tableRow->earHistoryIndex - *keyValue);
}

//------------------------------------------------------------------------


/* return earHistoryType or 0 if there isn't one */
int
convertAtmelAlarmToMIBHistoryType (int p_iAlarm, int p_alarm_or_clear) {
    int iHistory;

    switch ( p_iAlarm ) {

    case  0:  /* ALARM_TEMPERATURE_FLAG      alarmTemperatureFlagRaised */
        iHistory =  1;  break;
    case  1:  /* ALARM_FAN_FLAG              alarmFanFlagRaised */
        iHistory =  2;  break;
    case  2:  /* ALARM_OUTPUT_TOO_HIGH_FLAG  alarmOutputTooHighFlagRaised */
        iHistory =  3;  break;
    case  3:  /* ALARM_OUTPUT_HIGH_FLAG      alarmOutputHighFlagRaised */
        iHistory =  4;  break;

    case  4:  /* ALARM_OUTPUT_LOW_FLAG       alarmOutputLowFlagRaised */
        iHistory =  5;  break;
    case  5:  /* ALARM_INPUT_TOO_HIGH_FLAG   alarmInputTooHighFlagRaised */
        iHistory =  6;  break;
    case  6:  /* ALARM_INPUT_HIGH_FLAG       alarmInputHighFlagRaised */
        iHistory =  7;  break;
    case  7:  /* ALARM_INPUT_LOW_FLAG        alarmInputLowFlagRaised */
        iHistory =  8;  break;

    case  8:  /* ALARM_OUTPUT_OFF_FLAG       alarmOutputOffFlagRaised */
        iHistory =  9;  break;
    case  9:  /* ALARM_INPUT_OFF_FLAG        alarmInputOffFlagRaised */
        iHistory = 10;  break;
    case 10:  /* ALARM_INTERLOCK_FLAG        alarmInterlockFlagRaised */
        iHistory = 11;  break;
    case 11:  /* ALARM_PS1_FAIL_FLAG         alarmPs1FailFlagRaised */
        iHistory = 12;  break;

    case 12:  /* ALARM_PS2_FAIL_FLAG         alarmPs2FailFlagRaised */
        iHistory = 13;  break;
    case 13:  /* ALARM_POWER_SAFETY_FLAG     alarmPowerSafetyModeRaised */
        iHistory = 14;  break;
    case 14:  /* ALARM_FUSE_FLAG             alarmKeyswitchFlagRaised ?? */
        iHistory = 15;  break;
    case 15:  /* ALARM_48V_A_FLAG */
        iHistory = 25;  break;

    case 16:  /* ALARM_48V_B_FLAG */
        iHistory = 26; break;
    case 17:  /* ALARM_INPUT2_TOO_HIGH_FLAG */
        iHistory = 21; break;
    case 18:  /* ALARM_INPUT2_HIGH_FLAG */
        iHistory = 22; break;
    case 19:  /* ALARM_INPUT2_LOW_FLAG */
        iHistory = 23; break;

    case 20:  /* ALARM_INPUT2_OFF_FLAG */
        iHistory = 24; break;
    case 21:  /* ALARM_OUTPUT2_OFF_FLAG      alarmOutput2OffFlagRaised */
        iHistory = 16; break;
    case 22:  /* ALARM_OUTPUT2_LOW_FLAG      alarmOutput2LowFlagRaised */
        iHistory = 17; break;
    case 23:  /* ALARM_OUTPUT2_HIGH_FLAG     alarmOutput2HighFlagRaised */
        iHistory = 18; break;

    case 24:  /* ALARM_OUTPUT2_TOO_HIGH_FLAG alarmOutput2TooHighFlagRaised */
        iHistory = 19; break;
    case 25:  /* ALARM_PUMP_FLAG             alarmPumpFlagRaised */
        iHistory = 20; break;
	//case 27:  /* ALARM_KEY_SWITCH_OFF_FLAG*/
		//iHistory = 25; break;
    default:
        iHistory = 0;
        break;
    }

    switch ( p_alarm_or_clear ) {
    case CONVERT_alarm:
        /* do not adjust */
        break;
    case CONVERT_clear:
        iHistory += MAX_ALARM_OFFSET;
        break;
    case CONVERT_event:
        iHistory += MAX_ALARM_OFFSET + MAX_ALARM_RESET_OFFSET;
        break;
    default:
        /* don't know:  do not adjust */
        break;
    }

    return iHistory;
}


int
convertAtmelAlarmToMIBAlarmType (int p_iAlarm) {
    int iAlarm;

    switch ( p_iAlarm ) {

    case  0:  /* ALARM_TEMPERATURE_FLAG      alarmTemperatureFlagRaised */
        iAlarm =  1;  break;
    case  1:  /* ALARM_FAN_FLAG              alarmFanFlagRaised */
        iAlarm =  2;  break;
    case  2:  /* ALARM_OUTPUT_TOO_HIGH_FLAG  alarmOutputTooHighFlagRaised */
        iAlarm =  3;  break;
    case  3:  /* ALARM_OUTPUT_HIGH_FLAG      alarmOutputHighFlagRaised */
        iAlarm =  4;  break;

    case  4:  /* ALARM_OUTPUT_LOW_FLAG       alarmOutputLowFlagRaised */
        iAlarm =  5;  break;
    case  5:  /* ALARM_INPUT_TOO_HIGH_FLAG   alarmInputTooHighFlagRaised */
        iAlarm =  6;  break;
    case  6:  /* ALARM_INPUT_HIGH_FLAG       alarmInputHighFlagRaised */
        iAlarm =  7;  break;
    case  7:  /* ALARM_INPUT_LOW_FLAG        alarmInputLowFlagRaised */
        iAlarm =  8;  break;

    case  8:  /* ALARM_OUTPUT_OFF_FLAG       alarmOutputOffFlagRaised */
        iAlarm =  9;  break;
    case  9:  /* ALARM_INPUT_OFF_FLAG        alarmInputOffFlagRaised */
        iAlarm = 10;  break;
    case 10:  /* ALARM_INTERLOCK_FLAG        alarmInterlockFlagRaised */
        iAlarm = 11;  break;
    case 11:  /* ALARM_PS1_FAIL_FLAG         alarmPs1FailFlagRaised */
        iAlarm = 12;  break;

    case 12:  /* ALARM_PS2_FAIL_FLAG         alarmPs2FailFlagRaised */
        iAlarm = 13;  break;
    case 13:  /* ALARM_POWER_SAFETY_FLAG     alarmPowerSafetyModeRaised */
        iAlarm = 14;  break;
    case 14:  /* ALARM_FUSE_FLAG             alarmKeyswitchFlagRaised ?? */
        iAlarm = 15;  break;
    case 15:  /* ALARM_48V_A_FLAG */
        iAlarm = 25;  break;

    case 16:  /* ALARM_48V_B_FLAG */
        iAlarm = 26;  break;
	case 17:  /* ALARM_INPUT2_TOO_HIGH_FLAG */
        iAlarm = 21;  break;
    case 18:  /* ALARM_INPUT2_HIGH_FLAG */
        iAlarm = 22;  break;
    case 19:  /* ALARM_INPUT2_LOW_FLAG */
        iAlarm = 23;  break;

    case 20:  /* ALARM_INPUT2_OFF_FLAG */
        iAlarm = 24; break;
    case 21:  /* ALARM_OUTPUT2_OFF_FLAG      alarmOutput2OffFlagRaised */
        iAlarm = 16; break;
    case 22:  /* ALARM_OUTPUT2_LOW_FLAG      alarmOutput2LowFlagRaised */
        iAlarm = 17; break;
    case 23:  /* ALARM_OUTPUT2_HIGH_FLAG     alarmOutput2HighFlagRaised */
        iAlarm = 18; break;

    case 24:  /* ALARM_OUTPUT2_TOO_HIGH_FLAG alarmOutput2TooHighFlagRaised */
        iAlarm = 19; break;
    case 25:  /* ALARM_PUMP_FLAG             alarmPumpFlagRaised */
        iAlarm = 20; break;
	case 26:  /* ALARM_BACK_REFL_TOO_HIGH */
		iAlarm = 27; break;	
	case 27:  /* ALARM_KEY_SWITCH_OFF */
		iAlarm = 28; break;


    default:
        iAlarm = 0;
        break;
    }

    return iAlarm;
}


int
convertAtmelAlarmToMIBClearType (int p_iAlarm) {
    int iClear;

    switch ( p_iAlarm ) {

    case  0:  /* ALARM_TEMPERATURE_FLAG      alarmTemperatureFlagRaised */
        iClear =  1;  break;
    case  1:  /* ALARM_FAN_FLAG              alarmFanFlagRaised */
        iClear =  2;  break;
    case  2:  /* ALARM_OUTPUT_TOO_HIGH_FLAG  alarmOutputTooHighFlagRaised */
        iClear =  3;  break;
    case  3:  /* ALARM_OUTPUT_HIGH_FLAG      alarmOutputHighFlagRaised */
        iClear =  4;  break;

    case  4:  /* ALARM_OUTPUT_LOW_FLAG       alarmOutputLowFlagRaised */
        iClear =  5;  break;
    case  5:  /* ALARM_INPUT_TOO_HIGH_FLAG   alarmInputTooHighFlagRaised */
        iClear =  6;  break;
    case  6:  /* ALARM_INPUT_HIGH_FLAG       alarmInputHighFlagRaised */
        iClear =  7;  break;
    case  7:  /* ALARM_INPUT_LOW_FLAG        alarmInputLowFlagRaised */
        iClear =  8;  break;

    case  8:  /* ALARM_OUTPUT_OFF_FLAG       alarmOutputOffFlagRaised */
        iClear =  9;  break;
    case  9:  /* ALARM_INPUT_OFF_FLAG        alarmInputOffFlagRaised */
        iClear = 10;  break;
    case 10:  /* ALARM_INTERLOCK_FLAG        alarmInterlockFlagRaised */
        iClear = 11;  break;
    case 11:  /* ALARM_PS1_FAIL_FLAG         alarmPs1FailFlagRaised */
        iClear = 12;  break;

    case 12:  /* ALARM_PS2_FAIL_FLAG         alarmPs2FailFlagRaised */
        iClear = 13;  break;
    case 13:  /* ALARM_POWER_SAFETY_FLAG     alarmPowerSafetyModeRaised */
        iClear = 14;  break;
    case 14:  /* ALARM_KEY_SWITCH_OFF_FLAG  alarmKeyswitchFlagRaised  */
        iClear = 15;  break;
    case 15:  /* ALARM_48V_A_FLAG */
        iClear = 25;  break;
    case 16:  /* ALARM_48V_B_FLAG */
        iClear = 26; break;
    case 17:  /* ALARM_INPUT2_TOO_HIGH_FLAG */
        iClear = 21; break;
    case 18:  /* ALARM_INPUT2_HIGH_FLAG */
        iClear = 22; break;
    case 19:  /* ALARM_INPUT2_LOW_FLAG */
        iClear = 23; break;
    case 20:  /* ALARM_INPUT2_OFF_FLAG */
        iClear = 24; break;
    case 21:  /* ALARM_OUTPUT2_OFF_FLAG      alarmOutput2OffFlagRaised */
        iClear = 16; break;
    case 22:  /* ALARM_OUTPUT2_LOW_FLAG      alarmOutput2LowFlagRaised */
        iClear = 17; break;
    case 23:  /* ALARM_OUTPUT2_HIGH_FLAG     alarmOutput2HighFlagRaised */
        iClear = 18; break;

    case 24:  /* ALARM_OUTPUT2_TOO_HIGH_FLAG alarmOutput2TooHighFlagRaised */
        iClear = 19; break;
    case 25:  /* ALARM_PUMP_FLAG             alarmPumpFlagRaised */
        iClear = 20; break;
	case 26:  /* ALARM_BACK_REFL_FLAG  */
		iClear = 27; break;	
	case 27:  /* ALARM_KEY_SWITCH_FLAG  */
		iClear = 28; break;	
    default:
        iClear = 0;
        break;
    }

    return iClear;
}

/* return bit number of alarm flag */
/* return -1 if not found */
int
convertMIBHistoryTypeToAtmelAlarm (int p_iHistoryType) {
    int iAlarm;

    switch  ( p_iHistoryType ) {

    case  1:  /* alarmTemperatureFlagRaised     ALARM_TEMPERATURE_FLAG */
        iAlarm =  0; break;
    case  2:  /* alarmFanFlagRaised             ALARM_FAN_FLAG */
        iAlarm =  1; break;
    case  3:  /* alarmOutputTooHighFlagRaised   ALARM_OUTPUT_TOO_HIGH_FLAG */
        iAlarm =  2; break;
    case  4:  /* alarmOutputHighFlagRaised      ALARM_OUTPUT_HIGH_FLAG */
        iAlarm =  3; break;

    case  5:  /* alarmOutputLowFlagRaised       ALARM_OUTPUT_LOW_FLAG */
        iAlarm =  4; break;
    case  6:  /* alarmInputTooHighFlagRaised    ALARM_INPUT_TOO_HIGH_FLAG */
        iAlarm =  5; break;
    case  7:  /* alarmInputHighFlagRaised       ALARM_INPUT_HIGH_FLAG */
        iAlarm =  6; break;
    case  8:  /* alarmInputLowFlagRaised        ALARM_INPUT_LOW_FLAG */
        iAlarm =  7; break;

    case  9:  /* alarmOutputOffFlagRaised       ALARM_OUTPUT_OFF_FLAG */
        iAlarm =  8; break;
    case 10:  /* alarmInputOffFlagRaised        ALARM_INPUT_OFF_FLAG */
        iAlarm =  9; break;
    case 11:  /* alarmInterlockFlagRaised       ALARM_INTERLOCK_FLAG */
        iAlarm = 10; break;
    case 12:  /* alarmPs1FailFlagRaised         ALARM_PS1_FAIL_FLAG */
        iAlarm = 11; break;

    case 13:  /* alarmPs2FailFlagRaised         ALARM_PS2_FAIL_FLAG */
        iAlarm = 12; break;
    case 14:  /* alarmPowerSafetyModeRaised     ALARM_POWER_SAFETY_FLAG */
        iAlarm = 13; break;
    case 15:  /* alarmKeyswitchFlagRaised       ALARM_KEY_SWITCH_OFF_FLAG  */
        iAlarm = 14; break;
    case 16:  /* alarmOutput2OffFlagRaised      ALARM_OUTPUT2_OFF_FLAG */
        iAlarm = 21; break;
    case 17:  /* alarmOutput2LowFlagRaised      ALARM_OUTPUT2_LOW_FLAG */
        iAlarm = 22; break;
    case 18:  /* alarmOutput2HighFlagRaised     ALARM_OUTPUT2_HIGH_FLAG */
        iAlarm = 23; break;

    case 19:  /* alarmOutput2TooHighFlagRaised  ALARM_OUTPUT2_TOO_HIGH_FLAG */
        iAlarm = 24; break;
    case 20:  /* alarmPumpFlagRaised            ALARM_PUMP_FLAG */
        iAlarm = 25; break;
	case 21:  /* alarmInput2TooHighFlagRaised 	ALARM_INPUT2_TOO_HIGH_FLAG */
        iAlarm = 17; break;
    case 22:  /* alarmInput2HighFlagRaised 		ALARM_INPUT2_HIGH_FLAG */
        iAlarm = 18; break;
    case 23:  /* alarmInput2LowFlagRaised  		ALARM_INPUT2_LOW_FLAG */
        iAlarm = 19; break;
    case 24:  /* alarmInput2OffFlagRaised  		ALARM_INPUT2_OFF_FLAG */
        iAlarm = 20; break;
	case 25:  /* alarm48VAFlagRaised       		ALARM_48V_A_FLAG ?? */
        iAlarm = 15; break;
	case 26:  /* alarm48VBFlagRaised       		ALARM_48V_B_FLAG ?? */
        iAlarm = 16; break;
	case 27:  /* alarmBackREflRaised */
		iAlarm = 26; break;
	case 28:  /* alarmKeySwitchRaised */
		iAlarm = 27; break;
    default:
        iAlarm = -1; break;
    }

    return iAlarm;
}


/* exploit the correspondence between alarm notification ids
 * and history type ids
 */
int
convertMIBAlarmTypeToAtmelAlarm (int p_iAlarmType) {
    int iAlarm;

    iAlarm = convertMIBHistoryTypeToAtmelAlarm (p_iAlarmType);

    return iAlarm;
}



/* exploit the correspondence between raise and clear notification ids */
int
convertMIBClearTypeToAtmelAlarm (int p_iClearType) {
    int iAlarm;

    iAlarm = convertMIBAlarmTypeToAtmelAlarm (p_iClearType);

    return iAlarm;
}




/* exploit the correspondence between alarm notification ids
 * and history type ids
 */
int
convertMIBAlarmTypeToMIBHistoryType (int p_iAlarmType) {
    int iHistory;

    iHistory = p_iAlarmType;

    return iHistory;
}


/* exploit the correspondence between clear notification ids and
 * history type ids
 */
int
convertMIBClearTypeToMIBHistoryType (int p_iClearType) {
    int iHistory;

    iHistory = p_iClearType + MAX_ALARM_OFFSET;

    return iHistory;
}


/* exploit the correspondence between event notification
 * ids and history type ids
 */
int
convertMIBEventTypeToMIBHistoryType (int p_iEventType) {
    int iHistory;

    iHistory = p_iEventType + MAX_ALARM_OFFSET + MAX_ALARM_RESET_OFFSET;

    return iHistory;
}


/* exploit the correspondence between clear notification ids
 * and history type ids
 */
int
convertMIBHistoryTypeToMIBAlarmType (int p_iHistoryType) {
    int iAlarm;

    iAlarm = p_iHistoryType;

    return iAlarm;
}


/* exploit the correspondence between clear notification
 * ids and history type ids
 */
int
convertMIBHistoryTypeToMIBClearType (int p_iHistoryType) {
    int iClear;

    iClear = p_iHistoryType - MAX_ALARM_OFFSET;

    return iClear;
}


/* exploit the correspondence between event notification
 * ids and history type ids
 */
int
convertMIBHistoryTypeToMIBEventType (int p_iHistoryType) {
    int iEvent;

    iEvent = p_iHistoryType - MAX_ALARM_OFFSET - MAX_ALARM_RESET_OFFSET;

    return iEvent;
}



void
setTheTrapVariables (
                     int p_history_table_idx,       /* 1..1000 */
                     int p_severity,
                     unsigned long p_history_type,  /* 1..70 */
                     char * p_time_buf,
                     struct variable_t *p_variables
) {
    char *p_name;
    char *p_location;
    
    // Note!!! Number of the arrays below MUST correspond to the NUM_OIDS_IN_TRAP value.
    unsigned long alarmTrapOid1[] =
        { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 7, 1, 1, 1, 0 };	// index
    unsigned long alarmTrapOid2[] =
        { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 7, 1, 1, 2, 0 };	// type
    unsigned long alarmTrapOid3[] =
        { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 7, 1, 1, 3, 0 };	// timestamp
    unsigned long alarmTrapOid4[] =
        { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 7, 1, 1, 4, 0 };	// severity
    unsigned long alarmTrapOid5[] =
        { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 7, 1, 1, 5, 0 };	// sysName
    unsigned long alarmTrapOid6[] =
        { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 7, 1, 1, 6, 0 };	// sysLocation

    /* They're all the same in a table; calculate once */
    int table_oid_length = 14;  /* sizeof is iffy */
    int table_row_part = table_oid_length - 1;

    // variable 1 is the index (integer) (1..1000)
    p_variables[0].oid_length = table_oid_length;
    alarmTrapOid1[table_row_part] = p_history_table_idx;
    memcpy (p_variables[0].oid_value, &alarmTrapOid1,
            table_oid_length * sizeof (unsigned int));
    memcpy (p_variables[0].value, &p_history_table_idx, sizeof (int));
    p_variables[0].value_type = ASN_INTEGER;
    p_variables[0].value_length = sizeof (int);

    // variable 2 is the history table id for alarms/clears/events (1..70)
    p_variables[1].oid_length = table_oid_length;
    alarmTrapOid2[table_row_part] = p_history_table_idx;
    memcpy (p_variables[1].oid_value, &alarmTrapOid2,
            table_oid_length * sizeof (unsigned int));
    memcpy (p_variables[1].value, &p_history_type, sizeof (unsigned long));
    p_variables[1].value_type = ASN_INTEGER;
    p_variables[1].value_length = sizeof (unsigned long);

    //  variable 3 is a 'timestamp' string
    p_variables[2].oid_length = table_oid_length;
    alarmTrapOid3[table_row_part] = p_history_table_idx;
    memcpy (p_variables[2].oid_value, &alarmTrapOid3,
            table_oid_length * sizeof (unsigned int));
    memcpy (p_variables[2].value, p_time_buf, strlen (p_time_buf));
    p_variables[2].value_type = ASN_OCTET_STR;
    p_variables[2].value_length = strlen (p_time_buf);

    //  variable 4 is a severity integer (1..3)
    p_variables[3].oid_length = table_oid_length;
    alarmTrapOid4[table_row_part] = p_history_table_idx;
    memcpy (p_variables[3].oid_value, &alarmTrapOid4,
            table_oid_length * sizeof (unsigned int));
    memcpy (p_variables[3].value, &p_severity, sizeof (int));
    p_variables[3].value_type = ASN_INTEGER;
    p_variables[3].value_length = sizeof (int);

    // Get sysName and sysLocation.
    p_name = ipgGetSysName();
    p_location = ipgGetSysLocation();

    //  variable 5 is the laser box's name string
    p_variables[4].oid_length = table_oid_length;
    alarmTrapOid5[table_row_part] = p_history_table_idx;
    memcpy (p_variables[4].oid_value, &alarmTrapOid5,
            table_oid_length * sizeof (unsigned int));
    memcpy (p_variables[4].value, p_name, strlen (p_name));
    p_variables[4].value_type = ASN_OCTET_STR;
    p_variables[4].value_length = strlen (p_name);

    //  variable 6 is the laser box's location string
    p_variables[5].oid_length = table_oid_length;
    alarmTrapOid6[table_row_part] = p_history_table_idx;
    memcpy (p_variables[5].oid_value, &alarmTrapOid6,
            table_oid_length * sizeof (unsigned int));
    memcpy (p_variables[5].value, p_location, strlen (p_location));
    p_variables[5].value_type = ASN_OCTET_STR;
    p_variables[5].value_length = strlen (p_location);
}



void
sendTheTrap (
             int p_notification_id,  /* notification id 1..20, 1..20 or 1..30 */
             int p_history_table_index,  /* 1..1000 */
             struct variable_t *p_pVariables,
             unsigned int p_iVarSize,  /* num bytes of p_pVariables array */
             int p_groupSelector /* 0=set; 1=clear; 2=event */
) {
    int status;
    int serverAddrIndex;
    devBoardParamsType app_nvParams;
    int result;
    int ccode;

    unsigned long* oid_group;
    unsigned long alarm_set[] = { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 4 };
    unsigned long alarm_clear[] = { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 5 };
    unsigned long event[] = { 1, 3, 6, 1, 4, 1, 19775, 1, 1, 6 };
    /* this works as long as the notifications are at the same level
     * in the MIB hierarchy
     */
    unsigned int oid_group_size = 10;
    unsigned long ip_addresses_[6];
    int num_ip_addresses = 0;

    switch ( p_groupSelector ) {
    case 0: oid_group = &(alarm_set[0]); break;
    case 1: oid_group = &(alarm_clear[0]); break;
    case 2: oid_group = &(event[0]); break;
    default: oid_group = &(event[0]); break;
    }

    memset (&(ip_addresses_[0]), 0, 6 * 4);

    status = semaphore_get_2 (&semaphore_nvParams_info, SEMAPHORE_TMO, 30801);
    if ( status == TX_SUCCESS ) {
		if (TrapFirstTimeIn)
		{
		   openSocket ();
		   TrapFirstTimeIn = 0;
		}
        /* send traps to the SNMP management stations specified
         * in the app_nvParams struct
         */
        customizeReadDevBoardParams (&app_nvParams);
        root_breakpoint (&app_nvParams.mgmtStations[0].ipAddress);

        /* copy ip addresses out and release the semaphore asap */
        /* so we don't have to hold semaphore while sending snmp traps */
        for ( serverAddrIndex = 0;
              serverAddrIndex < BL_COMMUNITY_STRINGS_ARRAY;
              ++serverAddrIndex ) {
            if ( 0 < app_nvParams.mgmtStations[serverAddrIndex].ipAddress ) {
                ip_addresses_[num_ip_addresses] = app_nvParams.mgmtStations[serverAddrIndex].ipAddress;
                ++num_ip_addresses;
            }
        }

        status = semaphore_put_2 (&semaphore_nvParams_info, 30802);
    }

    for (serverAddrIndex = 0;
         serverAddrIndex < num_ip_addresses;
         serverAddrIndex++) {
        g_snmp_mgmt_stn_idx = serverAddrIndex;

        if ( 0 < ip_addresses_[serverAddrIndex] ) {
            g_snmp_toggle += 1;
            result = SNMPSendTrap (SNMP_TRAP_ENTERPRISESPECIFIC,
                                   p_notification_id, // trap ref. #
                                   NUM_OIDS_IN_TRAP,	// oids in trap
                                   p_pVariables,
                                   p_iVarSize,
                                   SNMP_COMMUNITY_TYPE_PUBLIC,
                                   oid_group,
                                   oid_group_size, /* num ints in oid_group[] */
                                   ip_addresses_[serverAddrIndex]);
            if (0 != result) {
                ++g_snmp_send_trap_failed;
                g_snmp_send_trap_last_result = result;
            }
            g_snmp_toggle -= 1;
            tx_thread_sleep (g_snmp_send_trap_timeout);
        }
    }
    g_snmp_mgmt_stn_idx = -1;

    ccode = rtcWriteExtraData (p_history_table_index);
}


static char *ipgGetSysName(void)
{
    devBoardParamsType app_nvParams;

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    // give either the default or what we remember
    if (strlen(app_nvParams.krbUsername) > 0)
    {
	    strncpy(sysname, app_nvParams.krbUsername, 31);
    }
    
    return (char *) sysname;
}

static char *ipgGetSysLocation(void)
{
    devBoardParamsType app_nvParams;

    customizeReadDevBoardParams(&app_nvParams);
    root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);

    /* give either the default or what we remember */
    if (strlen(app_nvParams.krbRealmname) > 0)
    {
	    strncpy(syslocation, app_nvParams.krbRealmname, 127);
    }
    
    return (char *) syslocation;
}

