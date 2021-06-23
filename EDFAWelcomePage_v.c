/* Created with PageBuilder version 4.04 on Thu Mar 26 12:11:04 2015 */

#include "AsExtern.h"
#include "boardparams.h"
#include <stdlib.h>
#include <asn1.h>
#include <snmpimpl.h>
#include <man_api.h>
#include <snmp.h>
#include <snmpvars.h>
#include "man_agnt.h"
#include "MibMan.h"
#include "EARMIB.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tx_api.h>
#if 1
#include <reg_def.h>
#endif
#include <sockapi.h>
#include <tservapi.h>
#include <rtc.h>
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include <fbanks.h>

#include "boardParams.h"
#include "ipgStrings.h"
#include "telnet.h"
#include "ipgsnmp.h"

#include "semaphores.h"
#include "STACK_SIZE.h"
#include "ascii.h"
#include "readFrom2U2.h"
#include "root.h"
#include "ipgFlash.h"

#include "atmel.h"

#if RomPagerServer



/* ******************************************* */
/* *    Built from "EDFAWelcomePage.html"    * */
/* ******************************************* */



/* *********************************************** */
/* *    Built from "EDFAGeneralSettings.html"    * */
/* *********************************************** */

unsigned char earModelNumber_temp[MAX_2U_RESPONSE];

unsigned char earSerialNumber_temp[MAX_2U_RESPONSE];

unsigned char earDateOfManufacture_temp[MAX_2U_RESPONSE];

unsigned char earFirmwareRevision_temp[MAX_2U_RESPONSE];

unsigned char earControlMode_temp[MAX_2U_RESPONSE];

extern char *getEARRtcTime_temp(struct variable *vp, oid * name, int *length,
		     int isGet, int *varLen, setMethod * setVar);
char *getEARRtcTime_temp(struct variable *vp, oid * name, int *length,
		     int isGet, int *varLen, setMethod * setVar){

    #define RTC_BUFFER_LENGTH 40
    char rtc_buffer[RTC_BUFFER_LENGTH];
    rtcDataType timedate;
    void *resultBuffer = NULL;

            memcpy (&timedate, &g_timedate, sizeof (rtcDataType));

			sprintf(rtc_buffer, "%04d-%02d-%02d %02d:%02d:%02d",
                    timedate.year + 2000,
					timedate.month,
                    timedate.day,
                    timedate.hours, 
                    timedate.minutes, 
                    timedate.seconds);		

            resultBuffer = (void *) &(rtc_buffer[0]);

    return resultBuffer;
}

unsigned char earDigiFirmwareRev_temp[MAX_2U_RESPONSE];

unsigned char earDigiActiveFB_temp[MAX_2U_RESPONSE];

unsigned char earDigiActiveFBConfirmed_temp[MAX_2U_RESPONSE];

extern char *getearDigiTelnetTimeout_temp(void);
char *getearDigiTelnetTimeout_temp(void){

    char telnetBuffer[MAX_2U_RESPONSE];
    int timeoutS = 0;
    void *resultBuffer2 = NULL;
    devBoardParamsType App_nvParams;

    customizeReadDevBoardParams(&App_nvParams);
    timeoutS = App_nvParams.telnetTimeoutS;
    sprintf(telnetBuffer, "%4d", timeoutS);
    resultBuffer2 = (void *) &(telnetBuffer[0]);

    return resultBuffer2;
}

signed char earMeasuredGainValue_temp[MAX_2U_RESPONSE];

signed char earMinimumGainSetPoint_temp[MAX_2U_RESPONSE];

signed char earMaximumGainSetPoint_temp[MAX_2U_RESPONSE];

signed char earTotalOpticalOutputPower_temp[MAX_2U_RESPONSE];

signed char earTotalOpticalOutputPower2_temp[MAX_2U_RESPONSE];

signed char earMaximumPowerSetpoint_temp[MAX_2U_RESPONSE];

unsigned long earMaximumCurrentSetpoint_temp = 0;

unsigned long earPumpLaserCurrent_temp = 0;

unsigned long earPumpLaserCurrent2_temp = 0;

signed char earAmbientTemperature_temp[MAX_2U_RESPONSE];

signed char earTotalOpticalInputPower_temp[MAX_2U_RESPONSE];

signed char earTotalOpticalInput2Power_temp[MAX_2U_RESPONSE];

signed char earGainControlSetpoint_temp[MAX_2U_RESPONSE];

signed char earGainControlSetpoint2_temp[MAX_2U_RESPONSE];

unsigned long earCurrentSetpointValue_temp = 0;

signed char earPowerSafetyMode_temp[MAX_2U_RESPONSE];

/* ********************************************** */
/* *    Built from "EDFAChassisManager.html"    * */
/* ********************************************** */

char * whichSubmitButton;
float powerSetpoint;
float setpoint;

// This API is called on the clicking of one of the submit buttons
extern void postProcessTheForm(void *theTaskDataPtr, Signed16Ptr theIndexValuesPtr);
void postProcessTheForm(void *theTaskDataPtr, Signed16Ptr theIndexValuesPtr) {

	char buffer[MAX_FIELD_LEN];
    float setpointF;
    float floatVal;
    float floatVal2;

	printf("In postProcessTheForm\n");
	
	// which button was pressed?
	// get the value on the submit button presses
	if(theTaskDataPtr != NULL)
	{
		whichSubmitButton = RpGetSubmitButtonValue(theTaskDataPtr);
	}
	else
	{
		// it is unlikely this will ever be called. It is for defensive programming only
		printf("danger, AWS not running!\n");
		return;
	}
	printf("The %s submit button was pressed\n", whichSubmitButton);
	
	// If Set Output Power 1 submit button was pressed send command to update Ouput Power Setpoint in Atmel via UART
if(strcmp("SetOP1", whichSubmitButton) == 0)
	{
		floatVal = (float) powerSetpoint;
		//floatVal2 = (floatVal / 10.0f);
		sprintf(buffer, "ZIMSOP %1.1f\r", floatVal);
		printf("%s", buffer);
		fflush(stdout); 
		
		// copyTempToPerm(theTempDataCache, thePermDataCache); // Old code from example
	}
	
	// If Set Output Power 2 submit button was pressed send command to update Ouput Power 2 Setpoint in Atmel via UART
	if(strcmp("SetOP2", whichSubmitButton) == 0)
	{
		setpointF = (float) setpoint;
		//setpointF /= 10.0f;
		sprintf (buffer, "ZIMSSP2 %1.1f\r", setpointF);
		printf ("%s", buffer);
		fflush (stdout);
		
	}
	
	//If Pumps Off submit button was pressed send command to turn off pumps
	if(strcmp("Off", whichSubmitButton) == 0)
	{
		// Code doesn't exist at the moment, below is from earPumpsOff.c but Hazem said it s incorrect
        printf("ZIMSLASEROFF\r");
        fflush(stdout);
	}
	
	//If Reboot submit button was pressed ...
	if(strcmp("Reboot", whichSubmitButton) == 0)
	{
		// Code to Reboot Chassis
		
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
		
		 // Initialize GPIO PORTC5 to Output and set it low for reset
		narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, mode, 
				narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, mode) & ~gpio[5].bit);
		narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, dir,
                narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, dir) | gpio[5].bit);
		narm_write_reg (NARM_PORTX_REG, gpio[5].regaddr, reserved,
                narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, reserved) & ~gpio[5].bit);
		narm_write_reg(NARM_PORTX_REG, gpio[5].regaddr, data, 
				narm_read_reg (NARM_PORTX_REG, gpio[5].regaddr, data) & ~gpio[5].bit);
				
	}

	return;
}

unsigned char earOutputPowerSetpointValue_temp[MAX_2U_RESPONSE];

extern void SetOutputPowerSetpointValue(char *theValue);
void SetOutputPowerSetpointValue(char *theValue) {

	powerSetpoint = atof(theValue);
	return;
}

unsigned char earOutputPower2SetpointValue_temp[MAX_2U_RESPONSE];

extern void SetOutputPower2SetpointValue(char *theValue);
void SetOutputPower2SetpointValue(char *theValue) {

	setpoint = atof(theValue);
	return;
}


/* ***************************************************** */
/* *    Built from "EDFANetworkAdministration.html"    * */
/* ***************************************************** */

unsigned char earIpAddress_temp[16];

unsigned char earSubnetMask_temp[16];

unsigned char earGateway_temp[16];

extern unsigned char *getIPManager1(void);
unsigned char *getIPManager1(void) {
	devBoardParamsType app_nvParams;
	unsigned long  theResult;
	static unsigned char ipTmpBuf[16];
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[0].ipAddress);
	theResult = app_nvParams.mgmtStations[0].ipAddress;
	sprintf(ipTmpBuf, "\t\t\t\t%ld.%ld.%ld.%ld\n\r",
		    (theResult >> 24) & 0xff,
		    (theResult >> 16) & 0xff,
		    (theResult >> 8) & 0xff,
		    theResult & 0xff);
	return ipTmpBuf;
}

extern unsigned char *getIPManager2(void);
unsigned char *getIPManager2(void) {
	devBoardParamsType app_nvParams;
	unsigned long  theResult;
	static unsigned char ipTmpBuf[16];
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[1].ipAddress);
	theResult = app_nvParams.mgmtStations[1].ipAddress;
	sprintf(ipTmpBuf, "\t\t\t\t%ld.%ld.%ld.%ld\n\r",
		    (theResult >> 24) & 0xff,
		    (theResult >> 26) & 0xff,
		    (theResult >> 8) & 0xff,
		    theResult & 0xff);
	return ipTmpBuf;
}

extern unsigned char *getIPManager3(void);
unsigned char *getIPManager3(void) {
	devBoardParamsType app_nvParams;
	unsigned long  theResult;
	static unsigned char ipTmpBuf[16];
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[2].ipAddress);
	theResult = app_nvParams.mgmtStations[2].ipAddress;
	sprintf(ipTmpBuf, "\t\t\t\t%ld.%ld.%ld.%ld\n\r",
		    (theResult >> 24) & 0xff,
		    (theResult >> 16) & 0xff,
		    (theResult >> 8) & 0xff,
		    theResult & 0xff);
	return ipTmpBuf;
}

extern unsigned char *getIPManager4(void);
unsigned char *getIPManager4(void) {
	devBoardParamsType app_nvParams;
	unsigned long  theResult;
	static unsigned char ipTmpBuf[16];
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[3].ipAddress);
	theResult = app_nvParams.mgmtStations[3].ipAddress;
	sprintf(ipTmpBuf, "\t\t\t\t%ld.%ld.%ld.%ld\n\r",
		    (theResult >> 24) & 0xff,
		    (theResult >> 16) & 0xff,
		    (theResult >> 8) & 0xff,
		    theResult & 0xff);
	return ipTmpBuf;
}

extern unsigned char *getIPManager5(void);
unsigned char *getIPManager5(void) {
	devBoardParamsType app_nvParams;
	unsigned long  theResult;
	static unsigned char ipTmpBuf[16];
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[4].ipAddress);
	theResult = app_nvParams.mgmtStations[4].ipAddress;
	sprintf(ipTmpBuf, "\t\t\t\t%ld.%ld.%ld.%ld\n\r",
            (theResult >> 24) & 0xff,
		    (theResult >> 16) & 0xff,
		    (theResult >> 8) & 0xff,
		    theResult & 0xff);	    
	return ipTmpBuf;
}

extern unsigned char *getIPManager6(void);
unsigned char *getIPManager6(void) {
	devBoardParamsType app_nvParams;
	unsigned long  theResult;
	static unsigned char ipTmpBuf[16];
	customizeReadDevBoardParams(&app_nvParams);
	root_breakpoint(&app_nvParams.mgmtStations[5].ipAddress);
	theResult = app_nvParams.mgmtStations[5].ipAddress;
	sprintf(ipTmpBuf, "\t\t\t\t%ld.%ld.%ld.%ld\n\r",
		    (theResult >> 24) & 0xff,
		    (theResult >> 16) & 0xff,
		    (theResult >> 8) & 0xff,
		    theResult & 0xff);
	return ipTmpBuf;
}



/* ************************************************* */
/* *    Built from "EDFAThresholdSettings.html"    * */
/* ************************************************* */

signed char earTemperatureAlarmThreshold_temp[MAX_2U_RESPONSE];

signed char earInputPowerLowAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInput2PowerLowAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInputPowerHighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInput2PowerHighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInputPowerOffAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInput2PowerOffAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInputPowerTooHighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earInput2PowerTooHighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPowerLowAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPower2LowAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPowerHighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPower2HighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPowerOffAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPower2OffAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPowerTooHighAlarmLevel_temp[MAX_2U_RESPONSE];

signed char earOutputPower2TooHighAlarmLevel_temp[MAX_2U_RESPONSE];



/* ******************************************** */
/* *    Built from "EDFAAlarmsStatus.html"    * */
/* ******************************************** */

unsigned long temperatureHighAlarm = 0;

unsigned long preAmpOutTooHighAlarm = 0;

unsigned long fanFailureAlarm = 0;

unsigned long preAmpOutHighAlarm = 0;

unsigned long preAmpOutLowAlarm = 0;

unsigned long preAmpInTooHighAlarm = 0;

unsigned long preAmpInHighAlarm = 0;

unsigned long preAmpInLowAlarm = 0;

unsigned long preAmpOutOffAlarm = 0;

unsigned long preAmpInOffAlarm = 0;

unsigned long interlockAlarm = 0;

unsigned long powerSupply1Alarm = 0;

unsigned long powerSupply2Alarm = 0;

unsigned long systemFuseAlarm = 0;

unsigned long powerSafetyModeAlarm = 0;

unsigned long a48VAOffAlarm = 0;

unsigned long b48VBOffAlarm = 0;

unsigned long boosterInTooHighAlarm = 0;

unsigned long boosterInHighAlarm = 0;

unsigned long boosterInLowAlarm = 0;

unsigned long boosterInOffAlarm = 0;

unsigned long boosterOutOffAlarm = 0;

unsigned long boosterOutLowAlarm = 0;

unsigned long boosterOutHighAlarm = 0;

unsigned long boosterOutTooHighAlarm = 0;

unsigned long pumpCurrentAlarm = 0;

unsigned long backReflectorAlarm = 0;



/* ****************************************** */
/* *    Built from "EDFA_Stylesheet.css"    * */
/* ****************************************** */



/* ************************************ */
/* *    Built from "EDFA_Utils.js"    * */
/* ************************************ */


#endif	/* RomPagerServer */
