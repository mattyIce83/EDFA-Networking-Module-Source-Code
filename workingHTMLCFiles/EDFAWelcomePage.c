/* Created with PageBuilder version 4.04 on Thu Mar 26 12:11:04 2015 */

#include "AsExtern.h"

#if RomPagerServer



/* ******************************************* */
/* *    Built from "EDFAWelcomePage.html"    * */
/* ******************************************* */

extern rpObjectDescription PgEDFAWelcomePage;

static char PgEDFAWelcomePage_Item_1[] =
	"<!DOCTYPE html>\n"
	C_oHTML C_oHEAD "\n"
	"        " C_oMETA " http-equiv=\"Content-Type\"" C_CONTENT
	"\"text/html; charset=UTF-8\">\n"
	"        <style>\n"
	"\n"
	"            /* CSS Document */\n"
	"            /* ---------- GENERAL ---------- */\n"
	"\n"
	"            body {\n"
	"                background-color: #C0C0C0;\n"
	"                color: #000;\n"
	"                font-family: \"Varela Round\", Arial, Helvetica, "
	"sans-serif;\n"
	"                font-size: 16px;\n"
	"                line-height: 1.5em;\n"
	"            }\n"
	"\n"
	"            input {\n";

static char PgEDFAWelcomePage_Item_2[] =
	"                border: none;\n"
	"                font-family: inherit;\n"
	"                font-size: inherit;\n"
	"                font-weight: inherit;\n"
	"                line-height: inherit;\n"
	"                -webkit-appearance: none;\n"
	"            }\n"
	"\n"
	"            /* ---------- LOGIN ---------- */\n"
	"\n"
	"            #login {\n"
	"                margin: 50px auto;\n"
	"                width: 600px;\n"
	"            }\n"
	"\n"
	"            #login h2 {\n"
	"                background-color: #f95252;\n"
	"                border-radius: 20px 20px 0 0;\n";

static char PgEDFAWelcomePage_Item_3[] =
	"                color: #fff;\n"
	"                font-size: 28px;\n"
	"                padding: 20px 26px;\n"
	"            }\n"
	"\n"
	"            #login fieldset {\n"
	"                background-color: #fff;\n"
	"                -webkit-border-radius: 0 0 20px 20px;\n"
	"                -moz-border-radius: 0 0 20px 20px;\n"
	"                border-radius: 0 0 20px 20px;\n"
	"                padding:  30px 30px;\n"
	"            }\n"
	"\n"
	"            #login fieldset p {\n"
	"                color: #777;\n"
	"                margin-bottom: 14px;\n"
	"            }\n";

static char PgEDFAWelcomePage_Item_4[] =
	"\n"
	"\n"
	"        </style>\n"
	"        " C_oTITLE "\n"
	"            EDFA Welcome\n"
	"        " C_xTITLE "<link rel=\"stylesheet\" "
	"href=\"/open-iconic/font/css/open-iconic-bootstrap.css\" >\n"
	"\n"
	"        <!--[if lt IE 9]>\n"
	"                <script "
	"src=\"http://html5shiv.googlecode.com/svn/trunk/html5.js\"></script>\n"
	"        <![endif]-->\n"
	"\n"
	"    " C_xHEAD "<!-- PAGE CONTENT -->\n"
	"\n"
	"    " C_oDIV_ " id=\"login\">\n"
	"        \n"
	"        " C_oH2 "\n"
	"            EDFA Login\n"
	"        " C_xH2 "<fieldset>\n"
	"            " C_oIMG_SRC;

static char PgEDFAWelcomePage_Item_5[] =
	"https://useiconic.com/open-iconic/svg/lock-locked.svg\"" C_ALT
	"padlock icon\" style=\"float:left;width:30px;height:30px\">\n"
	"            <nav  id = \"navigation\">\n"
	"                " C_oANCHOR_HREF "EDFAGeneralSettings.html\">General "
	"Settings" C_xANCHOR "|\n"
	"                " C_oANCHOR_HREF "EDFAChassisManager.html\">EDFA "
	"Chassis Manager" C_xANCHOR "|\n"
	"                " C_oANCHOR_HREF "EDFANetworkAdministration.html\">"
	"Network Administration" C_xANCHOR "|\n"
	"                " C_oANCHOR_HREF "EDFAThresholdSettings.html\">"
	"Threshold" C_xANCHOR "|\n";

static char PgEDFAWelcomePage_Item_6[] =
	"                " C_oANCHOR_HREF "EDFAAlarmsStatus.html\">Alarms "
	"Status" C_xANCHOR "</nav>\n"
	"            " C_oP C_xP "<!-- inserting blank space on page -->\n"
	"            " C_oP "Welcome to EDFA Remote Access Site" C_xP C_oP
	"Upon selection of destination page you will be \n"
	"                prompted for a username and password by the server.\n"
	"            " C_xP C_oP C_oIMG_SRC
	"http://www.ipgphotonics.com/Collateral/Images/English-US/2RU%20NE%20on"
	"%20white.jpg\"" C_ALT "EDFA picture\" "
	"style=\"float:left;width:326px;height:154px\">\n";

static char PgEDFAWelcomePage_Item_7[] =
	"            " C_xP "</fieldset>\n"
	"\n"
	"    " C_xDIV "<!-- end login message-->\n"
	"\n"
	"    " C_oP "&#169 IPG Photonics 2014" C_xP "<!-- Copyright message -->"
	"\n"
	"    \n"
	C_xBODY_xHTML;


static rpItem PgEDFAWelcomePage_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_4 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_5 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_6 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAWelcomePage_Item_7 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFAWelcomePage = {
	"/EDFAWelcomePage.html",
	PgEDFAWelcomePage_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Unprotected,
	eRpDataTypeHtml,
	eRpObjectTypeDynamic
};


/* *********************************************** */
/* *    Built from "EDFAGeneralSettings.html"    * */
/* *********************************************** */

extern rpObjectDescription PgEDFAGeneralSettings;

static char PgEDFAGeneralSettings_Item_1[] =
	"<!DOCTYPE html>\n"
	C_oHTML C_oTITLE "\n"
	"\tEDFA General Settings\n"
	"    " C_xTITLE C_oMETA " http-equiv=\"Content-Type\"" C_CONTENT
	"\"text/html; charset=UTF-8\">\n"
	"    " C_oMETA " http-equiv=\"Refresh\"" C_CONTENT "\"5\">\n"
	"    <link rel=\"stylesheet\" href=\"EDFA_stylesheet.css\" "
	"type=\"text/css\">\n"
	"    <script src=\"EDFA_Utils.js\" language=\"JavaScript\"></script>\n"
	"    <!-- <script src=\"errhandler.js\" language=\"JavaScript\">"
	"</script>\n"
	"    <script src=\"validation.js\" language=\"JavaScript\"></script> "
	"-->\n"
	"\n"
	"    <style>\n"
	"        \n"
	"            #wrapper {\n";

static char PgEDFAGeneralSettings_Item_2[] =
	"                width: 100%;\n"
	"                border: 1px solid #000;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838; \n"
	"                border-radius: 10px 10px 0 0;}\n"
	"\n"
	"            .headerDiv {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 24px;\n"
	"                FONT-WEIGHT: bold;\n"
	"            }\n"
	"\n"
	"            .headerDiv2 {\n";

static char PgEDFAGeneralSettings_Item_3[] =
	"                float: right;\n"
	"                text-align: center;}\n"
	"\n"
	"            .headerDiv3 {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 18px;\n"
	"                FONT-WEIGHT: bold;}\n"
	"\n"
	"            #navigation{\n"
	"                width: 100%;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838;\n"
	"                border: 1px solid #000;\n";

static char PgEDFAGeneralSettings_Item_4[] =
	"                border-radius: 0 0 10px 10px;\n"
	"                font-size: 14px;}\n"
	"\n"
	"\n"
	"            #header {\n"
	"                padding: 20px;\n"
	"\n"
	"            }\n"
	"        \n"
	"    </style> \n"
	"\n"
	"    " C_oDIV_ " id=\"wrapper\">\n"
	"        " C_oDIV_ " id=\"header\">\n"
	"            " C_oDIV_ " class=\"headerDiv\">\n"
	"                EDFA General Settings\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv\">\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv2\">\n"
	"                ";

// extern rpObjectDescription PgLogo;

static char PgEDFAGeneralSettings_Item_6[] =
	"            " C_xDIV C_xDIV C_xDIV "<nav  id = \"navigation\">\n"
	"        <strong>General Settings</strong> |\n"
	"        " C_oANCHOR_HREF "EDFAChassisManager.html\">EDFA Chassis "
	"Manager" C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFANetworkAdministration.html\">Network "
	"Administration" C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAThresholdSettings.html\">Threshold"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAAlarmsStatus.html\">Alarms Status"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAWelcomePage.html\">Home" C_xANCHOR
	"</nav>\n"
	"    <!-- General table -->\n"
	"\n";

static char PgEDFAGeneralSettings_Item_7[] =
	"    <table " C_WIDTH "\"70%\"  style=\"border:1; border-color: "
	"#383838; border-radius: 0 0 10px 10px;\">\n"
	"        " C_oTR "\n"
	"            <th class=\"tableBorderTextCentered\" style=\"width:20%\">"
	"<span class=\"tdbold\">Data</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">Units</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">Current</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span ";

static char PgEDFAGeneralSettings_Item_8[] =
	"class=\"tdbold\">Help</span>" C_xTH C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\" style=\"width:30%\">"
	"Model Number" C_xTD "<td class=\"withBorderTdDis\" style=\"width:5%\">"
	C_xTD "<td class=\"tableBorderTextCentered\" style=\"width:15%\">\n"
	"                ";

extern char earModelNumber_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_9 = {
	"earModelNumber",
	(void *) &earModelNumber_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_10[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\" "
	"style=\"width:10%\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Serial Number"
	C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earSerialNumber_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_11 = {
	"earSerialNumber",
	(void *) &earSerialNumber_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_12[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Manufacturing Date"
	C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earDateOfManufacture_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_13 = {
	"earDateOfManufacture",
	(void *) &earDateOfManufacture_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_14[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">System Firmware "
	"Revision" C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earFirmwareRevision_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_15 = {
	"earFirmwareRevision",
	(void *) &earFirmwareRevision_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_16[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Control Mode" C_xTD
	"<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earControlMode_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_17 = {
	"earControlMode",
	(void *) &earControlMode_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_18[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">RTC Date and Time"
	C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char *getEARRtcTime_temp(void);
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_19 = {
	"earRtcTime",
	(void *) getEARRtcTime_temp,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_20[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Digi Firmware "
	"Revision" C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earDigiFirmwareRev_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_21 = {
	"earDigiFirmwareRev",
	(void *) &earDigiFirmwareRev_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_22[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Digi Flash Active "
	"Bank" C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earDigiActiveFB_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_23 = {
	"eardigiActiveFB",
	(void *) &earDigiActiveFB_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_24[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Active flash Bank "
	"Confirmed" C_xTD "<td class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earDigiActiveFBConfirmed_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_25 = {
	"earDigiActiveFBConfirmed",
	(void *) &earDigiActiveFBConfirmed_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_26[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Telnet Timeout"
	C_xTD "<td class=\"tableBorderTextCentered\">sec" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char *getearDigiTelnetTimeout_temp(void);
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_27 = {
	"earDigiTelnetTimeout",
	(void *) &getearDigiTelnetTimeout_temp,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_28[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Measured Gain value"
	C_xTD "<td class=\"tableBorderTextCentered\">dB" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned char earMeasuredGainValue_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_29 = {
	"earMeasuredGainValue",
	(void *) &earMeasuredGainValue_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_30[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Min Gain Setpoint"
	C_xTD "<td class=\"tableBorderTextCentered\">dB" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned char earMinimumGainSetPoint_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_31 = {
	"earMinimumGainSetPoint",
	(void *) &earMinimumGainSetPoint_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_32[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Max Gain Setpoint"
	C_xTD "<td class=\"tableBorderTextCentered\">dB" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned char earMaximumGainSetPoint_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_33 = {
	"earMaximumGainSetPoint",
	(void *) &earMaximumGainSetPoint_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_34[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Total Optical Output"
	" Power" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earTotalOpticalOutputPower_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_35 = {
	"earTotalOpticalOutputPower",
	(void *) &earTotalOpticalOutputPower_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_36[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Total Optical Output"
	" Power 2" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td"
	" class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earTotalOpticalOutputPower2_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_37 = {
	"earTotalOpticalOutputPower2",
	(void *) &earTotalOpticalOutputPower2_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_38[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Max Power Setpoint"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earMaximumPowerSetpoint_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_39 = {
	"earMaximumPowerSetpoint",
	(void *) &earMaximumPowerSetpoint_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_40[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Max Current Setpoint"
	C_xTD "<td class=\"tableBorderTextCentered\">mA" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned long earMaximumCurrentSetpoint_temp;
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_41 = {
	"earMaximumCurrentSetpoint",
	(void *) &earMaximumCurrentSetpoint_temp,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAGeneralSettings_Item_42[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Pump Laser Current"
	C_xTD "<td class=\"tableBorderTextCentered\">mA" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned long earPumpLaserCurrent_temp;
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_43 = {
	"earPumpLaserCurrent",
	(void *) &earPumpLaserCurrent_temp,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAGeneralSettings_Item_44[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Pump Laser Current 2"
	C_xTD "<td class=\"tableBorderTextCentered\">mA" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned long earPumpLaserCurrent2_temp;
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_45 = {
	"earPumpLaserCurrent2",
	(void *) &earPumpLaserCurrent2_temp,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAGeneralSettings_Item_46[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Ambient Temperature"
	C_xTD "<td class=\"tableBorderTextCentered\">&#176C" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earAmbientTemperature_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_47 = {
	"earAmbientTemperature",
	(void *) &earAmbientTemperature_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_48[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Total Optical Input "
	"Power" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earTotalOpticalInputPower_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_49 = {
	"earTotalOpticalInputPower",
	(void *) &earTotalOpticalInputPower_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_50[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Total Optical Input "
	"Power 2" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earTotalOpticalInput2Power_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_51 = {
	"earTotalOpticalInput2Power",
	(void *) &earTotalOpticalInput2Power_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_52[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR "<!-- General table with set button-->\n"
	"\n"
	"        " C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Gain Control "
	"Setpoint" C_xTD "<td class=\"tableBorderTextCentered\">dB" C_xTD "<td"
	" class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earGainControlSetpoint_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_53 = {
	"earGainControlSetpoint",
	(void *) &earGainControlSetpoint_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_54[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Gain Control "
	"Setpoint 2" C_xTD "<td class=\"tableBorderTextCentered\">dB" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earGainControlSetpoint2_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_55 = {
	"earGainControlSetpoint2",
	(void *) &earGainControlSetpoint2_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_56[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Current Setpoint"
	C_xTD "<td class=\"tableBorderTextCentered\">mA" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern unsigned long earCurrentSetpointValue_temp;
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_57 = {
	"earCurrentSetpointValue",
	(void *) &earCurrentSetpointValue_temp,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAGeneralSettings_Item_58[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Power Safety Mode"
	C_xTD "<td class=\"tableBorderTextCentered\">dB" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern char earPowerSafetyMode_temp[];
static rpNamedTextDisplayItem PgEDFAGeneralSettings_Item_59 = {
	"earPowerSafetyMode",
	(void *) &earPowerSafetyMode_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAGeneralSettings_Item_60[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR
	C_xTABLE;

static char PgEDFAGeneralSettings_Item_61[] =
	"\n"
	C_xBODY_xHTML;


static rpItem PgEDFAGeneralSettings_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_4 }, 
//	{ eRpItemType_ImageSource, (void *) &PgLogo }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_6 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_7 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_8 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_9 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_10 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_11 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_12 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_13 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_14 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_15 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_16 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_17 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_18 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_19 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_20 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_21 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_22 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_23 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_24 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_25 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_26 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_27 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_28 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_29 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_30 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_31 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_32 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_33 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_34 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_35 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_36 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_37 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_38 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_39 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_40 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_41 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_42 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_43 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_44 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_45 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_46 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_47 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_48 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_49 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_50 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_51 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_52 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_53 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_54 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_55 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_56 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_57 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_58 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAGeneralSettings_Item_59 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_60 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAGeneralSettings_Item_61 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFAGeneralSettings = {
	"/EDFAGeneralSettings.html",
	PgEDFAGeneralSettings_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Realm1 | kRpPageAccess_Realm2,
	eRpDataTypeHtml,
	eRpObjectTypeDynamic
};

/* ********************************************** */
/* *    Built from "EDFAChassisManager.html"    * */
/* ********************************************** */

extern rpObjectDescription PgEDFAChassisManager;

static char PgEDFAChassisManager_Item_1[] =
	"<!DOCTYPE html>\n"
	C_oHTML C_oTITLE "\n"
	"        EDFA Chassis Manager\n"
	"    " C_xTITLE C_oMETA " http-equiv=\"Content-Type\"" C_CONTENT
	"\"text/html; charset=UTF-8\">\n"
	"    <link rel=\"stylesheet\" href=\"EDFA_stylesheet.css\" "
	"type=\"text/css\">\n"
	"    <script src=\"EDFA_Utils.js\" language=\"JavaScript\"></script>\n"
	"    <!-- <script src=\"errhandler.js\" language=\"JavaScript\">"
	"</script>\n"
	"    <script src=\"validation.js\" language=\"JavaScript\"></script> "
	"-->\n"
	"\n"
	"    <style>\n"
	"\n"
	"        #wrapper {\n"
	"            width: 100%;\n";

static char PgEDFAChassisManager_Item_2[] =
	"            border: 1px solid #000;\n"
	"            float: left;\n"
	"            background-color: #C0C0C0;\n"
	"            border-style: solid;\n"
	"            border-color: #383838; \n"
	"            border-radius: 10px 10px 0 0;}\n"
	"\n"
	"        .headerDiv {\n"
	"            float: left;\n"
	"            width: 33%;\n"
	"            text-align: left;\n"
	"            font-size: 24px;\n"
	"            FONT-WEIGHT: bold;\n"
	"        }\n"
	"\n"
	"        .headerDiv2 {\n"
	"            float: right;\n"
	"            text-align: center;}\n"
	"\n"
	"        .headerDiv3 {\n";

static char PgEDFAChassisManager_Item_3[] =
	"            float: left;\n"
	"            width: 33%;\n"
	"            text-align: left;\n"
	"            font-size: 18px;\n"
	"            FONT-WEIGHT: bold;}\n"
	"\n"
	"        #navigation{\n"
	"            width: 100%;\n"
	"            float: left;\n"
	"            background-color: #C0C0C0;\n"
	"            border-style: solid;\n"
	"            border-color: #383838;\n"
	"            border: 1px solid #000;\n"
	"            border-radius: 0 0 10px 10px;\n"
	"            font-size: 14px;}\n"
	"\n"
	"\n"
	"        #header {\n"
	"            padding: 20px;\n"
	"\n"
	"        }\n"
	"\n";

static char PgEDFAChassisManager_Item_4[] =
	"    </style> \n"
	"\n"
	"    " C_oDIV_ " id=\"wrapper\">\n"
	"        " C_oDIV_ " id=\"header\">\n"
	"            " C_oDIV_ " class=\"headerDiv\">\n"
	"                EDFA Chassis Manager\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv\">\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv2\">\n"
	"                ";

// extern rpObjectDescription PgLogo;

static char PgEDFAChassisManager_Item_6[] =
	"            " C_xDIV C_xDIV C_xDIV "<nav  id = \"navigation\">\n"
	"        " C_oANCHOR_HREF "EDFAGeneralSettings.html\">General Settings"
	C_xANCHOR "|\n"
	"        <strong>EDFA Chassis Manager</strong> |\n"
	"        " C_oANCHOR_HREF "EDFANetworkAdministration.html\">Network "
	"Administration" C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAThresholdSettings.html\">Threshold"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAAlarmsStatus.html\">Alarms Status"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAWelcomePage.html\">Home" C_xANCHOR
	"</nav>\n"
	"\n"
	"    <!-- Form for User to input data to EDFA, \"SOP, SSP2, PumpsOff ";

static char PgEDFAChassisManager_Item_7[] =
	"and ChassisReboot\n"
	"    Stuff Below is just a basic cut and past from old GeneralRp.htm "
	"NOT functional-->\n"
	"    ";

static char PgEDFAChassisManager_Item_9[] =
	"        <table " C_WIDTH "\"80%\"  style=\"border:1; border-color: "
	"#383838; border-radius: 0 0 10px 10px;\">\n"
	"            " C_oTR "\n"
	"                <th class=\"tableBorderTextCentered\" "
	"style=\"width:20%\"><span class=\"tdbold\">Data</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">Units</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">Current</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span ";

static char PgEDFAChassisManager_Item_10[] =
	"class=\"tdbold\">Update</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">Help</span>" C_xTH C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Output Power"
	"Setpoint" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td"
	" class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long earOutputPowerSetpointValue_temp;
static rpNamedTextDisplayItem PgEDFAChassisManager_Item_11 = {
	"earOutputPowerSetpointValue",
	(void *) &earOutputPowerSetpointValue_temp,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAChassisManager_Item_12[] =
	"                " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long earOutputPowerSetpointValue_temp;
extern void SetOutputPowerSetpointValue(int theValue);
static rpTextFormItem PgEDFAChassisManager_Item_13 = {
	"OutputPowerSetpointValueText",
	(void *) &earOutputPowerSetpointValue_temp,
	(void *) SetOutputPowerSetpointValue,
	eRpVarType_Direct,
	eRpVarType_Function,
	eRpTextType_Signed16,
	15,
	15,
	(char *) 0
};

static char PgEDFAChassisManager_Item_14[] =
	"                    ";

static rpButtonFormItem PgEDFAChassisManager_Item_15 = {
	"SetOP1",
	(char *) 0
};

static char PgEDFAChassisManager_Item_16[] =
	"                " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    <!-- Help Place Holder, TBD -->\n"
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Output Power "
	"Setpoint 2" C_xTD "<td class=\"tableBorderTextCentered\">dBx10" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long earOutputPower2SetpointValue_temp;
static rpNamedTextDisplayItem PgEDFAChassisManager_Item_17 = {
	"earOutputPower2SetpointValue",
	(void *) &earOutputPower2SetpointValue_temp,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAChassisManager_Item_18[] =
	"                " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long earOutputPower2SetpointValue_temp;
extern void SetOutputPower2SetpointValue(int theValue);
static rpTextFormItem PgEDFAChassisManager_Item_19 = {
	"OutputPower2SetpointValueText",
	(void *) &earOutputPower2SetpointValue_temp,
	(void *) SetOutputPower2SetpointValue,
	eRpVarType_Direct,
	eRpVarType_Function,
	eRpTextType_Signed16,
	15,
	15,
	(char *) 0
};

static rpButtonFormItem PgEDFAChassisManager_Item_20 = {
	"SetOP2",
	(char *) 0
};

static char PgEDFAChassisManager_Item_21[] =
	"                " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    <!-- Help Place Holder, TBD -->\n"
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Power Off Pumps"
	C_xTD "<td class=\"withBorderTdDis\" style=\"width:5%\">" C_xTD "<td "
	"class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

static rpButtonFormItem PgEDFAChassisManager_Item_22 = {
	"Off",
	(char *) 0
};

static char PgEDFAChassisManager_Item_23[] =
	"                " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    <!-- Help Place Holder, TBD -->\n"
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Chassis Reboot "
	C_xTD "<td class=\"withBorderTdDis\" style=\"width:5%\">" C_xTD "<td "
	"class=\"withBorderTdDis\">" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

static rpButtonFormItem PgEDFAChassisManager_Item_24 = {
	"Reboot",
	(char *) 0
};

static char PgEDFAChassisManager_Item_25[] =
	"                " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    <!-- Help Place Holder, TBD -->\n"
	"                " C_xTD C_xTR
	C_xTABLE;

static char PgEDFAChassisManager_Item_26[] =
	C_xFORM;

static char PgEDFAChassisManager_Item_27[] =
	C_xBODY_xHTML;



static rpItem PgEDFAChassisManager_Form_1_Items[] = { 
	{ eRpItemType_FormAsciiText, (void *) &PgEDFAChassisManager_Item_13 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_15 }, 
	{ eRpItemType_FormAsciiText, (void *) &PgEDFAChassisManager_Item_19 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_20 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_22 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_24 }, 
	{ eRpItemType_LastItemInList } 
};

extern void postProcessTheForm(void *theTaskDataPtr, Signed16Ptr theIndexValuesPtr);
static rpObjectExtension PgEDFAChassisManager_Form_1_ObjectExtension = {
	postProcessTheForm,
	&PgEDFAChassisManager,
	(rpObjectDescriptionPtr) 0,
	0,
	kRpObjFlags_None,
	(char *) 0
};

rpObjectDescription PgEDFAChassisManager_Form_1 = {
        "/Forms/EDFAChassisManager_1",
	PgEDFAChassisManager_Form_1_Items,
	&PgEDFAChassisManager_Form_1_ObjectExtension,
	(Unsigned32) 0,
	kRpPageAccess_Realm2,
	eRpDataTypeForm,
	eRpObjectTypeDynamic
};

static rpItem PgEDFAChassisManager_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_4 }, 
//	{ eRpItemType_ImageSource, (void *) &PgLogo }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_6 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_7 }, 
	{ eRpItemType_FormHeader, (void *) &PgEDFAChassisManager_Form_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_9 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_10 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAChassisManager_Item_11 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_12 }, 
	{ eRpItemType_FormAsciiText, (void *) &PgEDFAChassisManager_Item_13 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_14 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_15 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_16 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAChassisManager_Item_17 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_18 }, 
	{ eRpItemType_FormAsciiText, (void *) &PgEDFAChassisManager_Item_19 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_20 }, 
   	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_21 },
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_22 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_23 }, 
	{ eRpItemType_FormSubmit, (void *) &PgEDFAChassisManager_Item_24 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_25 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_26 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAChassisManager_Item_27 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFAChassisManager = {
	"/EDFAChassisManager.html",
	PgEDFAChassisManager_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Realm2,
	eRpDataTypeHtml,
	eRpObjectTypeDynamic
};

/* ***************************************************** */
/* *    Built from "EDFANetworkAdministration.html"    * */
/* ***************************************************** */

extern rpObjectDescription PgEDFANetworkAdministration;

static char PgEDFANetworkAdministration_Item_1[] =
	"<!DOCTYPE html> <!-- This html doc is for the P Builder generation -->"
	"\n"
	C_oHTML C_oTITLE "\n"
	"\tEDFA Network Administration\n"
	"    " C_xTITLE C_oMETA " http-equiv=\"Content-Type\"" C_CONTENT
	"\"text/html; charset=UTF-8\">\n"
	"    " C_oMETA " http-equiv=\"Refresh\"" C_CONTENT "\"30\">\n"
	"    <link rel=\"stylesheet\" href=\"EDFA_stylesheet.css\" "
	"type=\"text/css\">\n"
	"    <link rel=\"stylesheet\" href=\"../public_html/stylesheet.css\" "
	"type=\"text/css\"/>\n"
	"\n"
	"    <script src=\"EDFA_Utils.js\" language=\"JavaScript\"></script>\n";

static char PgEDFANetworkAdministration_Item_2[] =
	"    <!-- <link type=\"text/css\" href=\"/css/stylesheet.css\" "
	"rel=\"stylesheet\">\n"
	"    <script src=\"/scripts/errhandler.js\" language=\"JavaScript\">"
	"</script>\n"
	"    <script src=\"/scripts/validation.js\" language=\"JavaScript\">"
	"</script>\n"
	"    </script>-->\n"
	"    <style>\n"
	"        \n"
	"            #wrapper {\n"
	"                width: 100%;\n"
	"                border: 1px solid #000;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n";

static char PgEDFANetworkAdministration_Item_3[] =
	"                border-color: #383838; \n"
	"                border-radius: 10px 10px 0 0;}\n"
	"\n"
	"            .headerDiv {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 24px;\n"
	"                FONT-WEIGHT: bold;\n"
	"            }\n"
	"\n"
	"            .headerDiv2 {\n"
	"                float: right;\n"
	"                text-align: center;\n"
	"                border-style: solid;\n"
	"                border-color: #383838;\n"
	"                border: 1px solid #000;}\n"
	"\n";

static char PgEDFANetworkAdministration_Item_4[] =
	"            .headerDiv3 {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 18px;\n"
	"                FONT-WEIGHT: bold;}\n"
	"\n"
	"            #navigation{\n"
	"                width: 100%;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838;\n"
	"                border: 1px solid #000;\n"
	"                border-radius: 0 0 10px 10px;\n";

static char PgEDFANetworkAdministration_Item_5[] =
	"                font-size: 14px;}\n"
	"\n"
	"\n"
	"            #header {\n"
	"                padding: 20px;\n"
	"\n"
	"            }\n"
	"        \n"
	"    </style> \n"
	"    \n"
	"    " C_oDIV_ " id=\"wrapper\">\n"
	"        " C_oDIV_ " id=\"header\">\n"
	"            " C_oDIV_ " class=\"headerDiv\">\n"
	"                EDFA Network Administration\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv\">\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv2\">\n"
	"                ";

// extern rpObjectDescription PgLogo;

static char PgEDFANetworkAdministration_Item_7[] =
	"            " C_xDIV C_xDIV C_xDIV "<nav  id = \"navigation\">\n"
	"        " C_oANCHOR_HREF "EDFAGeneralSettings.html\">General Settings"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAChassisManager.html\">EDFA Chassis "
	"Manager" C_xANCHOR "|\n"
	"        <strong>Network Administration</strong> |\n"
	"        " C_oANCHOR_HREF "EDFAThresholdSettings.html\">Threshold"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAAlarmsStatus.html\">Alarms Status"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAWelcomePage.html\">Home" C_xANCHOR
	"</nav>\n"
	"        \n"
	"<table" C_WIDTH "\"70%\" style=\"border:1; border-color: black; ";

static char PgEDFANetworkAdministration_Item_8[] =
	"border-radius: 0 0 10px 10px;\">\n"
	"    <tr class=\"tableBorderTextCentered\">\n"
	"        <th class=\"tableBorderTextCentered\" style=\"width:25%\">\n"
	"            <span class=\"tdbold\">Network and IP Managers</span>\n"
	"        " C_xTH "<th class=\"tableBorderTextCentered\" "
	"style=\"width:22.5%\">\n"
	"            <span class=\"tdbold\">Value</span>\n"
	"        " C_xTH "<th class=\"tableBorderTextCentered\" "
	"style=\"width:22.5%\">\n"
	"            <span class=\"tdbold\">Help</span>\n"
	"        " C_xTH C_xTR C_oTR "\n";

static char PgEDFANetworkAdministration_Item_9[] =
	"        <td class=\"tableHeaderTextCentered\">IP Address" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"            ";

extern char earIpAddress_temp[];
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_10 = {
	"earIpAddress",
	(void *) &earIpAddress_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_11[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">Network Mask" C_xTD "<td"
	" class=\"tableBorderTextCentered\" >\n"
	"            ";

extern char earSubnetMask_temp[];
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_12 = {
	"earSubnetMask",
	(void *) &earSubnetMask_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_13[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\" >\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">Gateway" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"            ";

extern char earGateway_temp[];
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_14 = {
	"earGateway",
	(void *) &earGateway_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_15[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">IP Manager (1)" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"            ";

extern char *getIPManager1(void);
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_16 = {
	"longIPManager1",
	(void *) getIPManager1,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_17[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">IP Manager (2)" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"            ";

extern char *getIPManager2(void);
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_18 = {
	"longIPManager2",
	(void *) getIPManager2,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_19[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">IP Manager (3)" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"            ";

extern char *getIPManager3(void);
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_20 = {
	"longIPManager3",
	(void *) getIPManager3,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_21[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">IP Manager (4)" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"            ";

extern char *getIPManager4(void);
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_22 = {
	"longIPManager4",
	(void *) getIPManager4,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_23[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">IP Manager (5)" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"            ";

extern char *getIPManager5(void);
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_24 = {
	"longIPManager5",
	(void *) getIPManager5,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_25[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_oTR "\n"
	"        <td class=\"tableHeaderTextCentered\">IP Manager (6)" C_xTD
	"<td class=\"tableBorderTextCentered\">\n"
	"            ";

extern char *getIPManager6(void);
static rpNamedTextDisplayItem PgEDFANetworkAdministration_Item_26 = {
	"longIPManager6",
	(void *) getIPManager6,
	eRpVarType_Function,
	eRpTextType_ASCII,
	20
};

static char PgEDFANetworkAdministration_Item_27[] =
	"        " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"            <!-- Help Place Holder, TBD -->\n"
	"        " C_xTD C_xTR C_xTABLE
	C_xBODY_xHTML;


static rpItem PgEDFANetworkAdministration_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_4 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_5 }, 
//	{ eRpItemType_ImageSource, (void *) &PgLogo }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_7 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_8 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_9 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_10 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_11 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_12 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_13 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_14 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_15 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_16 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_17 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_18 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_19 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_20 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_21 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_22 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_23 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_24 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_25 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFANetworkAdministration_Item_26 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFANetworkAdministration_Item_27 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFANetworkAdministration = {
	"/EDFANetworkAdministration.html",
	PgEDFANetworkAdministration_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Realm1 | kRpPageAccess_Realm2,
	eRpDataTypeHtml,
	eRpObjectTypeDynamic
};


/* ************************************************* */
/* *    Built from "EDFAThresholdSettings.html"    * */
/* ************************************************* */

extern rpObjectDescription PgEDFAThresholdSettings;

static char PgEDFAThresholdSettings_Item_1[] =
	"<!DOCTYPE html>\n"
	C_oHTML C_oTITLE "\n"
	"\tEDFA Threshold Settings\n"
	"    " C_xTITLE C_oMETA " http-equiv=\"Content-Type\"" C_CONTENT
	"\"text/html; charset=UTF-8\">\n"
	"    " C_oMETA " http-equiv=\"Refresh\"" C_CONTENT "\"20\">\n"
	"    <link rel=\"stylesheet\" href=\"EDFA_stylesheet.css\" "
	"type=\"text/css\">\n"
	"    <script src=\"EDFA_Utils.js\" language=\"JavaScript\"></script>\n"
	"    <!-- <link type=\"text/css\" href=\"/css/stylesheet.css\" "
	"rel=\"stylesheet\">\n"
	"    <script src=\"/scripts/errhandler.js\" language=\"JavaScript\">"
	"</script>\n";

static char PgEDFAThresholdSettings_Item_2[] =
	"    <script src=\"/scripts/validation.js\" language=\"JavaScript\">"
	"</script>\n"
	"    </script>-->\n"
	"\n"
	"    <style>\n"
	"        \n"
	"            #wrapper {\n"
	"                width: 100%;\n"
	"                border: 1px solid #000;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838; \n"
	"                border-radius: 10px 10px 0 0;}\n"
	"\n"
	"            .headerDiv {\n"
	"                float: left;\n"
	"                width: 33%;\n";

static char PgEDFAThresholdSettings_Item_3[] =
	"                text-align: left;\n"
	"                font-size: 24px;\n"
	"                FONT-WEIGHT: bold;\n"
	"            }\n"
	"\n"
	"            .headerDiv2 {\n"
	"                float: right;\n"
	"                text-align: center;\n"
	"                border-style: solid;\n"
	"                border-color: #383838;\n"
	"                border: 1px solid #000;}\n"
	"\n"
	"            .headerDiv3 {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 18px;\n";

static char PgEDFAThresholdSettings_Item_4[] =
	"                FONT-WEIGHT: bold;}\n"
	"\n"
	"            #navigation{\n"
	"                width: 100%;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838;\n"
	"                border: 1px solid #000;\n"
	"                border-radius: 0 0 10px 10px;\n"
	"                font-size: 14px;}\n"
	"\n"
	"\n"
	"            #header {\n"
	"                padding: 20px;\n"
	"\n"
	"            }\n"
	"        \n"
	"    </style> \n"
	"\n"
	"    " C_oDIV_ " id=\"wrapper\">\n";

static char PgEDFAThresholdSettings_Item_5[] =
	"        " C_oDIV_ " id=\"header\">\n"
	"            " C_oDIV_ " class=\"headerDiv\">\n"
	"                EDFA Threshold Levels\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv\">\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv2\">\n"
	"                ";

// extern rpObjectDescription PgLogo;

static char PgEDFAThresholdSettings_Item_7[] =
	"            " C_xDIV C_xDIV C_xDIV "<nav  id = \"navigation\">\n"
	"        " C_oANCHOR_HREF "EDFAGeneralSettings.html\">General Settings"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAChassisManager.html\">EDFA Chassis "
	"Manager" C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFANetworkAdministration.html\">Network "
	"Administration" C_xANCHOR "|\n"
	"        <strong>Threshold</strong> |\n"
	"        " C_oANCHOR_HREF "EDFAAlarmsStatus.html\">Alarms Status"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAWelcomePage.html\">Home" C_xANCHOR
	"</nav>\n"
	"    <table" C_WIDTH "\"70%\"  style=\"border:1; border-color: black; ";

static char PgEDFAThresholdSettings_Item_8[] =
	"border-radius: 0 0 10px 10px;\" >\n"
	"        \n"
	"        <tr class=\"tableBorderTextCentered\">\n"
	"            <th class=\"tableBorderTextCentered\" style=\"width:25%\">"
	"<span class=\"tdbold\">Thresholds</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:10%\"><span "
	"class=\"tdbold\">Units</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:10%\"><span "
	"class=\"tdbold\">Current Value</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:10%\"><span "
	"class=\"tdbold\">Help</span>" C_xTH C_xTR C_oTR "\n";

static char PgEDFAThresholdSettings_Item_9[] =
	"            <td class=\"tableHeaderTextCentered\">Temperature High"
	C_xTD "<td class=\"tableBorderTextCentered\">&#176C" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earTemperatureAlarmThreshold_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_10 = {
	"earTemperatureAlarmThreshold",
	(void *) &earTemperatureAlarmThreshold_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_11[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power Low"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInputPowerLowAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_12 = {
	"earInputPowerLowAlarmLevel",
	(void *) &earInputPowerLowAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_13[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power 2 Low"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInput2PowerLowAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_14 = {
	"earInput2PowerLowAlarmLevel",
	(void *) &earInput2PowerLowAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_15[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power High"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInputPowerHighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_16 = {
	"earInputPowerHighAlarmLevel",
	(void *) &earInputPowerHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_17[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power 2 High"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInput2PowerHighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_18 = {
	"earInput2PowerHighAlarmLevel",
	(void *) &earInput2PowerHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_19[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power off"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInputPowerOffAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_20 = {
	"earInputPowerOffAlarmLevel",
	(void *) &earInputPowerOffAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_21[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power 2 off"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInput2PowerOffAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_22 = {
	"earInput2PowerOffAlarmLevel",
	(void *) &earInput2PowerOffAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_23[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power too High"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInputPowerTooHighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_24 = {
	"earInputPowerTooHighAlarmLevel",
	(void *) &earInputPowerTooHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_25[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Input Power 2 too "
	"High" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earInput2PowerTooHighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_26 = {
	"earInput2PowerTooHighAlarmLevel",
	(void *) &earInput2PowerTooHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_27[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power Low"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPowerLowAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_28 = {
	"earOutputPowerLowAlarmLevel",
	(void *) &earOutputPowerLowAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_29[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power 2 Low"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPower2LowAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_30 = {
	"earOutputPower2LowAlarmLevel",
	(void *) &earOutputPower2LowAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_31[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power High"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPowerHighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_32 = {
	"earOutputPowerHighAlarmLevel",
	(void *) &earOutputPowerHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_33[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power 2 High"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPower2HighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_34 = {
	"earOutputPower2HighAlarmLevel",
	(void *) &earOutputPower2HighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_35[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power off"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPowerOffAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_36 = {
	"earOutputPowerOffAlarmLevel",
	(void *) &earOutputPowerOffAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_37[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power 2 off"
	C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPower2OffAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_38 = {
	"earOutputPower2OffAlarmLevel",
	(void *) &earOutputPower2OffAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_39[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power Too "
	"High" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPowerTooHighAlarmLevel_temp[];
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_40 = {
	"earOutputPowerTooHighAlarmLevel",
	(void *) &earOutputPowerTooHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_41[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_oTR "\n"
	"            <td class=\"tableHeaderTextCentered\">Output Power 2 Too "
	"High" C_xTD "<td class=\"tableBorderTextCentered\">dBm" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                ";

extern signed char earOutputPower2TooHighAlarmLevel_temp;
static rpNamedTextDisplayItem PgEDFAThresholdSettings_Item_42 = {
	"earOutputPower2TooHighAlarmLevel",
	(void *) &earOutputPower2TooHighAlarmLevel_temp,
	eRpVarType_Direct,
	eRpTextType_ASCII,
	20
};

static char PgEDFAThresholdSettings_Item_43[] =
	"            " C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                <!-- Help Place Holder, TBD -->\n"
	"            " C_xTD C_xTR C_xTABLE
	C_xBODY_xHTML;


static rpItem PgEDFAThresholdSettings_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_4 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_5 }, 
//	{ eRpItemType_ImageSource, (void *) &PgLogo }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_7 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_8 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_9 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_10 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_11 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_12 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_13 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_14 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_15 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_16 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_17 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_18 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_19 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_20 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_21 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_22 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_23 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_24 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_25 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_26 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_27 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_28 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_29 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_30 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_31 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_32 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_33 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_34 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_35 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_36 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_37 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_38 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_39 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_40 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_41 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAThresholdSettings_Item_42 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAThresholdSettings_Item_43 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFAThresholdSettings = {
	"/EDFAThresholdSettings.html",
	PgEDFAThresholdSettings_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Realm1 | kRpPageAccess_Realm2,
	eRpDataTypeHtml,
	eRpObjectTypeDynamic
};


/* ******************************************** */
/* *    Built from "EDFAAlarmsStatus.html"    * */
/* ******************************************** */

extern rpObjectDescription PgEDFAAlarmsStatus;

static char PgEDFAAlarmsStatus_Item_1[] =
	"<!DOCTYPE html>\n"
	C_oHTML C_oTITLE "EDFA Alarms" C_xTITLE C_oMETA " charset=\"UTF-8\">\n"
	"        " C_oMETA C_NAME "\"viewport\"" C_CONTENT
	"\"width=device-width, initial-scale=1.0\">\n"
	"        " C_oMETA " http-equiv=\"Refresh\"" C_CONTENT "\"2\">\n"
	"        <link rel=\"stylesheet\" href=\"EDFA_stylesheet.css\" "
	"type=\"text/css\">\n"
	"        <script src=\"EDFA_Utils.js\" language=\"JavaScript\">"
	"</script>\n"
	"   \n"
	"    <style>\n"
	"        \n"
	"            #wrapper {\n"
	"                width: 100%;\n"
	"                border: 1px solid #000;\n"
	"                float: left;\n";

static char PgEDFAAlarmsStatus_Item_2[] =
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838; \n"
	"                border-radius: 10px 10px 0 0;}\n"
	"\n"
	"            .headerDiv {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 24px;\n"
	"                FONT-WEIGHT: bold;\n"
	"            }\n"
	"\n"
	"            .headerDiv2 {\n"
	"                float: right;\n"
	"                text-align: center;\n"
	"                border-style: solid;\n";

static char PgEDFAAlarmsStatus_Item_3[] =
	"                border-color: #383838;\n"
	"                border: 1px solid #000;}\n"
	"\n"
	"            .headerDiv3 {\n"
	"                float: left;\n"
	"                width: 33%;\n"
	"                text-align: left;\n"
	"                font-size: 18px;\n"
	"                FONT-WEIGHT: bold;}\n"
	"\n"
	"            #navigation{\n"
	"                width: 100%;\n"
	"                float: left;\n"
	"                background-color: #C0C0C0;\n"
	"                border-style: solid;\n"
	"                border-color: #383838;\n";

static char PgEDFAAlarmsStatus_Item_4[] =
	"                border: 1px solid #000;\n"
	"                border-radius: 0 0 10px 10px;\n"
	"                font-size: 14px;}\n"
	"\n"
	"\n"
	"            #header {\n"
	"                padding: 20px;\n"
	"\n"
	"            }\n"
	"        \n"
	"    </style>  \n"
	"\n"
	"    " C_oDIV_ " id=\"wrapper\">\n"
	"        " C_oDIV_ " id=\"header\">\n"
	"            " C_oDIV_ " class=\"headerDiv\">\n"
	"                EDFA Alarms Status\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv\">\n"
	"            " C_xDIV C_oDIV_ " class=\"headerDiv2\">\n"
	"                ";

// extern rpObjectDescription PgLogo;

static char PgEDFAAlarmsStatus_Item_6[] =
	"            " C_xDIV C_xDIV C_xDIV "<nav  id = \"navigation\">\n"
	"        " C_oANCHOR_HREF "EDFAGeneralSettings.html\">General Settings"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAChassisManager.html\">EDFA Chassis Manager"
	C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFANetworkAdministration.html\">Network "
	"Administration" C_xANCHOR "|\n"
	"        " C_oANCHOR_HREF "EDFAThresholdSettings.html\">Threshold"
	C_xANCHOR "|\n"
	"        <strong>Alarms</strong> |\n"
	"        " C_oANCHOR_HREF "EDFAWelcomePage.html\">Home" C_xANCHOR
	"</nav>\n"
	"        \n"
	"        <table id=\"alarmtable\" style=\"border: 1; border-color: ";

static char PgEDFAAlarmsStatus_Item_7[] =
	"black; border-radius: 0 0 10px 10px;\">\n"
	"            <thead class=\"tableBorderTextCentered\">\n"
	"                <th class=\"tableBorderTextCentered\" "
	"style=\"width:20%\"><span class=\"tdbold\">Alarm</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">State</span>" C_xTH C_oTH C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">Alarm</span>" C_xTH "<th "
	"class=\"tableBorderTextCentered\" style=\"width:20%\"><span "
	"class=\"tdbold\">State</span>" C_xTH "</thead>\n";

static char PgEDFAAlarmsStatus_Item_8[] =
	"            \n"
	"            <tbody>\n"
	"                \n"
	"            " C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Temperature High"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long temperatureHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_9 = {
	"TemperatureHighAlarm",
	(void *) &temperatureHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_10[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Output Too High" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpOutTooHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_11 = {
	"PreAmpOutTooHighAlarm",
	(void *) &preAmpOutTooHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_12[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Fan Failure"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long fanFailureAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_13 = {
	"FanFailureAlarm",
	(void *) &fanFailureAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_14[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Reserved" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpOutTooHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_15 = {
	"PreAmpOutTooHighAlarm",
	(void *) &preAmpOutTooHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_16[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Output High"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpOutHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_17 = {
	"PreAmpOutHighAlarm",
	(void *) &preAmpOutHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_18[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Output Low" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpOutLowAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_19 = {
	"PreAmpOutLowAlarm",
	(void *) &preAmpOutLowAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_20[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Input Too "
	"High" C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpInTooHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_21 = {
	"PreAmpInTooHighAlarm",
	(void *) &preAmpInTooHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_22[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Input High" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpInHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_23 = {
	"PreAmpInHighAlarm",
	(void *) &preAmpInHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_24[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Input Low"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpInLowAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_25 = {
	"PreAmpInLowAlarm",
	(void *) &preAmpInLowAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_26[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Output Off" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpOutOffAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_27 = {
	"PreAmpOutOffAlarm",
	(void *) &preAmpOutOffAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_28[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Input Off"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long preAmpInOffAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_29 = {
	"PreAmpInOffAlarm",
	(void *) &preAmpInOffAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_30[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Interlock" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long interlockAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_31 = {
	"InterlockAlarm",
	(void *) &interlockAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_32[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Power Supply One"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long powerSupply1Alarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_33 = {
	"PowerSupply1Alarm",
	(void *) &powerSupply1Alarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_34[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Power Supply Two" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long powerSupply2Alarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_35 = {
	"PowerSupply2Alarm",
	(void *) &powerSupply2Alarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_36[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">System Fuse"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long systemFuseAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_37 = {
	"SystemFuseAlarm",
	(void *) &systemFuseAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_38[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Power Safety Mode" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long powerSafetyModeAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_39 = {
	"PowerSafetyModeAlarmAlarm",
	(void *) &powerSafetyModeAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_40[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">48V A Off"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long a48VAOffAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_41 = {
	"48VAOffAlarm",
	(void *) &a48VAOffAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_42[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">48V B Off" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long b48VBOffAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_43 = {
	"48VBOffAlarm",
	(void *) &b48VBOffAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_44[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Input Two Too "
	"High" C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterInTooHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_45 = {
	"BoosterInTooHighAlarm",
	(void *) &boosterInTooHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_46[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Input Two High" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterInHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_47 = {
	"BoosterInHighAlarm",
	(void *) &boosterInHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_48[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Input Two Low"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterInLowAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_49 = {
	"BoosterInLowAlarm",
	(void *) &boosterInLowAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_50[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Input Two Off" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterInOffAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_51 = {
	"BoosterInOffAlarm",
	(void *) &boosterInOffAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_52[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Output Two Off"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterOutOffAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_53 = {
	"BoosterOutOffAlarm",
	(void *) &boosterOutOffAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_54[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Output Two Low" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterOutLowAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_55 = {
	"BoosterOutLowAlarm",
	(void *) &boosterOutLowAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_56[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Output Two High"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterOutHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_57 = {
	"BoosterOutHighAlarm",
	(void *) &boosterOutHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_58[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Output Two Too High" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long boosterOutTooHighAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_59 = {
	"BoosterOutTooHighAlarm",
	(void *) &boosterOutTooHighAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_60[] =
	"                " C_xTD C_xTR C_oTR "\n"
	"                <td class=\"tableHeaderTextCentered\">Pump Current"
	C_xTD "<td class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long pumpCurrentAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_61 = {
	"PumpCurrentAlarm",
	(void *) &pumpCurrentAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_62[] =
	"                " C_xTD C_oTD C_xTD "<td "
	"class=\"tableHeaderTextCentered\">Back Refl Too High" C_xTD "<td "
	"class=\"tableBorderTextCentered\">\n"
	"                    ";

extern unsigned long backReflectorAlarm;
static rpNamedTextDisplayItem PgEDFAAlarmsStatus_Item_63 = {
	"BackReflTooHighAlarm",
	(void *) &backReflectorAlarm,
	eRpVarType_Direct,
	eRpTextType_Unsigned32,
	20
};

static char PgEDFAAlarmsStatus_Item_64[] =
	"                " C_xTD C_xTR "</tbody>\n"
	"        " C_xTABLE "<script>\n"
	"            // Script to color alarm cells based on the value "
	"contained therein\n"
	"            var tds = "
	"document.getElementById(\"alarmtable\").getElementsByTagName(\"td\");\n"
	"            var num_tds = tds.length;\n"
	"            for (var i = 0; i < num_tds; i++) {\n"
	"                if (tds[i].innerText.indexOf(\"1\") > -1) {\n"
	"                    tds[i].style.backgroundColor = \"red\";\n"
	"                 }\n";

static char PgEDFAAlarmsStatus_Item_65[] =
	"                else if (tds[i].innerText.indexOf(\"0\") > -1){\n"
	"                    tds[i].style.backgroundColor = \"green\"\n"
	"                }\n"
	"            }\n"
	"              \n"
	"        </script>\n"
	"        \n"
	"    "
	C_xBODY_xHTML;


static rpItem PgEDFAAlarmsStatus_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_4 }, 
//	{ eRpItemType_ImageSource, (void *) &PgLogo }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_6 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_7 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_8 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_9 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_10 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_11 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_12 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_13 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_14 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_15 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_16 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_17 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_18 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_19 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_20 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_21 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_22 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_23 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_24 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_25 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_26 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_27 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_28 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_29 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_30 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_31 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_32 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_33 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_34 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_35 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_36 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_37 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_38 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_39 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_40 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_41 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_42 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_43 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_44 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_45 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_46 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_47 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_48 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_49 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_50 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_51 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_52 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_53 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_54 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_55 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_56 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_57 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_58 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_59 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_60 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_61 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_62 }, 
	{ eRpItemType_NamedDisplayText, (void *) &PgEDFAAlarmsStatus_Item_63 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_64 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFAAlarmsStatus_Item_65 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFAAlarmsStatus = {
	"/EDFAAlarmsStatus.html",
	PgEDFAAlarmsStatus_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Realm1 | kRpPageAccess_Realm2,
	eRpDataTypeHtml,
	eRpObjectTypeDynamic
};


/* ****************************************** */
/* *    Built from "EDFA_stylesheet.css"    * */
/* ****************************************** */

extern rpObjectDescription PgEDFA_stylesheet;

static char PgEDFA_stylesheet_Item_1[] =
	"body\n"
	"{\n"
	"    font-family: Verdana, Arial, Helvetica, sans-serif;\n"
	"    font-size: 12px;\n"
	"    background-color: #F0F0F0;\n"
	"    color: black;\n"
	"}\n"
	"\n"
	"select\n"
	"{\n"
	"    FONT-SIZE: 11px;\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif\n"
	"}\n"
	"INPUT\n"
	"{\n"
	"    FONT-SIZE: 11px;\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif\n"
	"}\n"
	"\n"
	"TABLE, TR, TD, P, BR {\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 12px;\n"
	"}\n"
	"\n"
	".pageheader {\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n";

static char PgEDFA_stylesheet_Item_2[] =
	"    font-size: 12px;\n"
	"    FONT-WEIGHT: bold;\n"
	"    COLOR: black;\n"
	"    background-color: #C0C0C0;\n"
	"    border-style: solid;\n"
	"    border-color: #383838;\n"
	"    border-radius: 10px 10px 0 0;\n"
	"}\n"
	"\n"
	".navpane{}\n"
	"\n"
	".CONTENTHEADER {\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 16px;\n"
	"    FONT-WEIGHT: bold;\n"
	"    COLOR: pink;\n"
	"    BACKGROUND-COLOR: #00008B;\n"
	"    PADDING: 10px;\n"
	"    MARGIN: 0px;\n"
	"}\n"
	".PAGETAB {\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 12px;\n";

static char PgEDFA_stylesheet_Item_3[] =
	"    FONT-WEIGHT: normal;\n"
	"    COLOR: #00008B;\n"
	"    BACKGROUND-COLOR: #C5D6FC;\n"
	"    BORDER-STYLE: solid;\n"
	"    BORDER-COLOR: #C5D6FC;\n"
	"    PADDING: 5px;\n"
	"}\n"
	".PAGETAB-SELECTED {\t\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 12px;\n"
	"    FONT-WEIGHT: bold;\n"
	"    COLOR: #00008B;\n"
	"\tBACKGROUND-COLOR: #C5D6FC;\n"
	"    BORDER-STYLE: solid;\n"
	"    BORDER-COLOR: #C5D6FC;\t\n"
	"    PADDING: 5px;\n"
	"}\n"
	".PAGECONTENT {\n"
	"    BACKGROUND-COLOR: #E8E8E8;\n"
	"}\n"
	".COPYRIGHT {\n";

static char PgEDFA_stylesheet_Item_4[] =
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 10px;\n"
	"}\n"
	".BORDERCOLOR {\n"
	"    BACKGROUND-COLOR: #00008B;\n"
	"}\n"
	".BORDER {\n"
	"    PADDING: 0px;\n"
	"    BORDER: 2px solid #00008B;\n"
	"}\n"
	"\n"
	"HR.SEPARATORLINE {\n"
	"    COLOR: #00008B;\n"
	"}\n"
	"A:LINK, A:VISITED, A:ACTIVE {\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 12px;\n"
	"    COLOR: #0033CC;\n"
	"}\n"
	"A:HOVER {\n"
	"    FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;\n"
	"    FONT-SIZE: 12px;\n"
	"    COLOR: #00008B;\n"
	"}\n"
	".ERRORFIELD {\n";

static char PgEDFA_stylesheet_Item_5[] =
	"    FONT-WEIGHT: bold;\n"
	"    BACKGROUND-COLOR: #FFDDDD;\n"
	"    BORDER-COLOR: #E80000;\n"
	"    BORDER-STYLE: solid;\n"
	"}\n"
	".STATUSBOX {\n"
	"    FONT-SIZE: 12px;\n"
	"    FONT-WEIGHT: bold;\n"
	"    COLOR: #00008B;\n"
	"    BACKGROUND-COLOR: red;\n"
	"    BORDER-COLOR: #00008B;\n"
	"    BORDER-STYLE: solid;\n"
	"    BORDER-WIDTH: thin;\n"
	"}\n"
	".ERRORBOX {\n"
	"    FONT-SIZE: 12px;\n"
	"    FONT-WEIGHT: bold;\n"
	"    COLOR: #E80000;\n"
	"    BACKGROUND-COLOR: #FFDDDD;\n"
	"    BORDER-COLOR: #E80000;\n"
	"    BORDER-STYLE: solid;\n"
	"    BORDER-WIDTH: thin;\n"
	"}\n"
	".ACTIVE {\n";

static char PgEDFA_stylesheet_Item_6[] =
	"    COLOR: #FFFFFF;\n"
	"    BACKGROUND-COLOR: #228B22;\n"
	"}\n"
	".INACTIVE {\n"
	"    COLOR: #FFFFFF;\n"
	"    BACKGROUND-COLOR: #00008B;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Page Content Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"page-content {\n"
	"    margin-top: 0px;\n"
	"    margin-left: 0px;\n"
	"    margin-right: 10px;\n"
	"    margin-bottom: 20px;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n";

static char PgEDFA_stylesheet_Item_7[] =
	" * General Page Content Body Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"page-content-body {\n"
	"    border: 1px solid #00008b;\n"
	"}\n"
	"\n"
	"page-content-body div.page-heading {\n"
	"    color: white;\n"
	"    padding: 4px 5px 4px 5px;\n"
	"    background-color: #00008b;\n"
	"}\n"
	"\n"
	"page-content-body td.page-heading-cell {\n"
	"    color: white;\n"
	"    width: 99%;\n"
	"    white-space: nowrap;\n"
	"    vertical-align: middle;\n"
	"    background-color: #00008b;\n"
	"}\n"
	"\n"
	"page-content-body div.page-heading h2 {\n";

static char PgEDFA_stylesheet_Item_8[] =
	"    margin: 0px;\n"
	"    padding: 0px;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"page-content-body div.page-heading a {\n"
	"    color: white;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"page-content-body td.page-subheading-cell {\n"
	"    color: white;\n"
	"    width: 1%;\n"
	"    white-space: nowrap;\n"
	"    text-align: right;\n"
	"    vertical-align: middle;\n"
	"    background-color: #00008b;\n"
	"}\n"
	"\n"
	"page-content-body td.page-subheading-cell a {\n"
	"    color: white;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"page-content-body td.page-subheading-cell span {\n";

static char PgEDFA_stylesheet_Item_9[] =
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"page-content-body td.page-subheading-cell img {\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"page-content-body div.page-htmlcontent {\n"
	"    margin: 5px;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Miscellaneous Page Content Styles\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"page-content-body .page-separator {\n"
	"    color: #00008b;\n"
	"    margin: 0px 0px 5px 0px;\n"
	"    padding: 3px 5px 3px 5px;\n";

static char PgEDFA_stylesheet_Item_10[] =
	"    background-color: #c5d6fc;\n"
	"}\n"
	"\n"
	"page-content-body .stats-label {\n"
	"    width: 25%;\n"
	"    white-space: nowrap;\n"
	"    text-align: right;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Page and Form Field & Input Styles\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"page-content .field-input {\n"
	"    width: 99%;\n"
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-input-" C_S_small "{\n"
	"    width: 1%;\n";

static char PgEDFA_stylesheet_Item_11[] =
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-checkbox {\n"
	"    width: 99%;\n"
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-checkbox-" C_S_small "{\n"
	"    width: 1%;\n"
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-label {\n"
	"    width: 1%;\n"
	"    white-space: nowrap;\n"
	"    text-align: right;\n"
	"}\n"
	"\n"
	"page-content .field-label-left {\n"
	"    width: 1%;\n"
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-value {\n"
	"    width: 99%;\n";

static char PgEDFA_stylesheet_Item_12[] =
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-value-" C_S_small "{\n"
	"    width: 1%;\n"
	"    white-space: nowrap;\n"
	"    text-align: left;\n"
	"}\n"
	"\n"
	"page-content .field-spacing {\n"
	"    padding-left: 10px;\n"
	"}\n"
	"\n"
	"page-content .field-expansion {\n"
	"    width: 99%;\n"
	"}\n"
	"\n"
	"page-content .field-indent {\n"
	"    padding-left: 30px;\n"
	"}\n"
	"\n"
	"page-content .field-error {\n"
	"    border-style: solid;\n"
	"    border-color: #E80000;\n"
	"    background-color: #FFDDDD;\n"
	"}\n"
	"\n"
	"page-content .field-warning {\n"
	"    border-style: solid;\n";

static char PgEDFA_stylesheet_Item_13[] =
	"    border-color: #E8E800;\n"
	"    background-color: #FFFFC0;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Page Content Table & List Styles\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	" table.page-content-table {\n"
	"    width: 98%;\n"
	"}\n"
	"\n"
	" table.page-content-table th {\n"
	"    white-space: nowrap;\n"
	"    padding: 3px 10px 3px 10px;\n"
	"}\n"
	"\n"
	" table.page-content-table td {\n"
	"    white-space: nowrap;\n"
	"    padding: 3px 10px 3px 10px;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_14[] =
	" table.page-content-table thead tr th {\n"
	"    border-bottom: 1px solid #00008b;\n"
	"}\n"
	"\n"
	" table.page-content-table .column-header {\n"
	"    width: 1%;\n"
	"}\n"
	"\n"
	" table.page-content-table .column-spacing {\n"
	"    width: 100%;\n"
	"}\n"
	"\n"
	" table.page-content-table .primary-row {\n"
	"    background-color: white;\n"
	"}\n"
	"\n"
	" table.page-content-table .secondary-row {\n"
	"    background-color: #dcdcdc;\n"
	"}\n"
	"\n"
	" table.page-content-table .group-row {\n"
	"    border-top: 1px solid #00008b;\n"
	"    background-color: white;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_15[] =
	"/*********************************************************************"
	"********\n"
	" * General Tab Content Body Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	" div.tab-heading {\n"
	"}\n"
	"\n"
	" div.tab-content {\n"
	"    margin-bottom: 5px;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-heading {\n"
	"    margin: 0px;\n"
	"    color: #00008b;\n"
	"    background-color: #c5d6fc;\n"
	"    padding: 3px 5px 3px 5px;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-heading h3 {\n"
	"    margin: 0px;\n"
	"    padding: 0px;\n";

static char PgEDFA_stylesheet_Item_16[] =
	"\tmargin-left: 15px;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-heading h3 a {\n"
	"    display: block;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-heading h3.tab-selected {\n"
	"    font-weight: bold;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-heading img {\n"
	"\tpadding-top: 3px;\n"
	"    float: left;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-body {\n"
	"    margin: 10px;\n"
	"}\n"
	"\n"
	" div.tab-content div.tab-content-body-heading {\n"
	"    padding-bottom: 10px;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Form Style\n"
	" ";

static char PgEDFA_stylesheet_Item_17[] =
	"**********************************************************************"
	"*******/\n"
	"\n"
	"form.form-content {\n"
	"    margin: 0px;\n"
	"    padding: 0px;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Form Button Bar Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"form.form-content div.form-buttons {\n"
	"    margin-top: 10px;\n"
	"    padding-top: 5px;\n"
	"    padding-bottom: 5px;\n"
	"    border-top: 1px solid #00008b;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_18[] =
	"/*********************************************************************"
	"********\n"
	" * Page Messages Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.page-content-messages {\n"
	"    margin-left: 100px;\n"
	"    margin-right: 100px;\n"
	"}\n"
	"\n"
	"div.page-content-messages div.status-message {\n"
	"    padding: 5px;\n"
	"    color: #00008B;\n"
	"    display: block;\n"
	"    font-size: 12px;\n"
	"    text-align: center;\n"
	"    border-width: thin;\n"
	"    border-style: solid;\n"
	"    border-color: #00008B;\n";

static char PgEDFA_stylesheet_Item_19[] =
	"    background-color: #E8E8E8;\n"
	"}\n"
	"\n"
	"div.page-content-messages div.error-message {\n"
	"    padding: 5px;\n"
	"    color: #E80000;\n"
	"    font-size: 12px;\n"
	"    font-weight: bold;\n"
	"    text-align: center;\n"
	"    border-width: thin;\n"
	"    border-style: solid;\n"
	"    border-color: #E80000;\n"
	"    background-color: #FFDDDD;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Help Page Header Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n";

static char PgEDFA_stylesheet_Item_20[] =
	"div.help-page-header {\n"
	"    padding: 0px;\n"
	"    margin-top: 10px;\n"
	"    margin-right: 10px;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Help Page Logo Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-header-logo {\n"
	"    float: left;\n"
	"    width: 200px;\n"
	"    height: 101px;\n"
	"    padding: 0px;\n"
	"    text-align: center;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"\n";

static char PgEDFA_stylesheet_Item_21[] =
	"/*********************************************************************"
	"********\n"
	" * Help Page Title (Heading) Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-header-title {\n"
	"    padding-top: 5px;\n"
	"    margin-left: 220px;\n"
	"}\n"
	"\n"
	"div.help-page-header-title h1 {\n"
	"    text-align: left;\n"
	"    padding-bottom: 3px;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Help Header/Content Separator Style\n"
	" ";

static char PgEDFA_stylesheet_Item_22[] =
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-header-separator {\n"
	"    clear: both;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Help Page Sub-Header Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-subheader {\n"
	"\theight: 30px;\n"
	"\tmargin-left: 10px;\n"
	"\tmargin-right: 10px;\n"
	"\tmargin-top: 30px;\n"
	"\tmargin-bottom: 30px;\n"
	"\tborder-bottom: 1px solid #00008b;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_23[] =
	"/*********************************************************************"
	"********\n"
	" * Current Help Page Title Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-subheader-title {\n"
	"\tfloat: left;\n"
	"    padding: 0px;\n"
	"    text-align: left;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"div.help-page-subheader-title h2 {\n"
	"\tfont-size: 18px;\n"
	"\tfont-weight: bold;\n"
	"    margin: 0px;\n"
	"\tpadding: 0px;\n"
	"\tvertical-align: middle;\n"
	"}\n"
	"\n"
	"\n";

static char PgEDFA_stylesheet_Item_24[] =
	"/*********************************************************************"
	"********\n"
	" * General Help Page Content & Body Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.page-content-help {\n"
	"\tmargin: 1px;\n"
	"    text-align: right;\n"
	"}\n"
	"\n"
	"div.help-page-content-body {\n"
	"\tfont-size: 12px;\n"
	"}\n"
	"\n"
	"div.help-page-content .field-spacing {\n"
	"    padding-left: 10px;\n"
	"}\n"
	"\n"
	"div.help-page-content .field-indent {\n"
	"    padding-left: 30px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body p {}\n"
	"\n";

static char PgEDFA_stylesheet_Item_25[] =
	"div.help-page-content-body div.help-section {\n"
	"\tmargin-left: 5px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-section h3 {\n"
	"\tfont-size: 14px;\n"
	"\tfont-weight: bold;\n"
	"\tmargin-left: -5px;\n"
	"\ttext-decoration: underline;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-section p {\n"
	"\tfont-size: 12px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-field {}\n"
	"\n"
	"div.help-page-content-body div.help-field h4 {\n"
	"\tfont-size: 12px;\n"
	"\tfont-weight: bold;\n"
	"\tmargin-top: 0px;\n"
	"\tpadding-top: 0px;\n"
	"\tmargin-bottom: 2px;\n"
	"\tpadding-bottom: 0px;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_26[] =
	"div.help-page-content-body div.help-field p {\n"
	"\tfont-size: 12px;\n"
	"\tmargin-top: 0px;\n"
	"\tpadding-top: 0px;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Help Page Content & Body Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-content-body table.help-content-table {\n"
	"    border-top: 1px solid black;\n"
	"    border-left: 1px solid black;\n"
	"}\n"
	"\n"
	"div.help-page-content-body table.help-content-table thead {\n";

static char PgEDFA_stylesheet_Item_27[] =
	"    background-color: #c3c3c3\n"
	"}\n"
	"\n"
	"div.help-page-content-body table.help-content-table thead tr th {\n"
	"    padding: 3px;\n"
	"    font-weight: bold;\n"
	"    text-align: center;\n"
	"    vertical-align: middle;\n"
	"    border-right: 1px solid black;\n"
	"    border-bottom: 1px solid black;\n"
	"}\n"
	"\n"
	"table.help-content-table tbody tr td {\n"
	"    padding: 2px;\n"
	"    text-align: left;\n"
	"    vertical-align: top;\n"
	"    border-right: 1px solid black;\n"
	"    border-bottom: 1px solid black;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_28[] =
	"/*********************************************************************"
	"********\n"
	" * General Help Page Sub-Header Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-subheader {\n"
	"\theight: 30px;\n"
	"\tmargin-left: 10px;\n"
	"\tmargin-right: 10px;\n"
	"\tmargin-top: 30px;\n"
	"\tmargin-bottom: 30px;\n"
	"\tborder-bottom: 1px solid #00008b;\n"
	"}\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Current Help Page Title Style\n"
	" ";

static char PgEDFA_stylesheet_Item_29[] =
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-subheader-title {\n"
	"\tfloat: left;\n"
	"    padding: 0px;\n"
	"    text-align: left;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"div.help-page-subheader-title h2 {\n"
	"\tfont-size: 18px;\n"
	"\tfont-weight: bold;\n"
	"    margin: 0px;\n"
	"\tpadding: 0px;\n"
	"\tvertical-align: middle;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Help Navigation Buttons Style\n"
	" ";

static char PgEDFA_stylesheet_Item_30[] =
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-subheader-navigation {\n"
	"\ttext-align: right;\n"
	"    vertical-align: middle;\n"
	"}\n"
	"\n"
	"div.help-page-subheader-navigation a {\n"
	"\tvertical-align: middle;\n"
	"}\n"
	"\n"
	"div.help-page-subheader-navigation img {\n"
	"    border: 0px;\n"
	"\tvertical-align: middle;\n"
	"}\n"
	"\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Help Table of Contents Style\n"
	" ";

static char PgEDFA_stylesheet_Item_31[] =
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-content-toc-help {\n"
	"\tpadding: 10px;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc ul {\n"
	"    margin: 0px;\n"
	"    padding: 0px;\n"
	"    list-style-type: none;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc li {\n"
	"    margin: 0px;\n"
	"    padding: 0px;\n"
	"    list-style-type: none;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc ul li {\n"
	"\tmargin-bottom: 10px;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc ul li.section {\n"
	"\tfont-weight: bold;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc ul li.section a {\n";

static char PgEDFA_stylesheet_Item_32[] =
	"    font-weight: normal;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc ul li.section ul {\n"
	"\tmargin-left: 20px;\n"
	"}\n"
	"\n"
	"div.help-page-content-toc ul li.section ul li {\n"
	"\tmargin-bottom: 0px;\n"
	"\tfont-weight: normal;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Help Page Content & Body Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-content{\n"
	"\tmargin: 10px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body {\n"
	"\tfont-size: 12px;\n"
	"}\n"
	"\n";

static char PgEDFA_stylesheet_Item_33[] =
	"div.help-page-content .field-spacing {\n"
	"    padding-left: 10px;\n"
	"}\n"
	"\n"
	"div.help-page-content .field-indent {\n"
	"    padding-left: 30px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body p {}\n"
	"\n"
	"div.help-page-content-body div.help-section {\n"
	"\tmargin-left: 5px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-section h3 {\n"
	"\tfont-size: 14px;\n"
	"\tfont-weight: bold;\n"
	"\tmargin-left: -5px;\n"
	"\ttext-decoration: underline;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-section p {\n"
	"\tfont-size: 12px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-field {}\n"
	"\n";

static char PgEDFA_stylesheet_Item_34[] =
	"div.help-page-content-body div.help-field h4 {\n"
	"\tfont-size: 12px;\n"
	"\tfont-weight: bold;\n"
	"\tmargin-top: 0px;\n"
	"\tpadding-top: 0px;\n"
	"\tmargin-bottom: 2px;\n"
	"\tpadding-bottom: 0px;\n"
	"}\n"
	"\n"
	"div.help-page-content-body div.help-field p {\n"
	"\tfont-size: 12px;\n"
	"\tmargin-top: 0px;\n"
	"\tpadding-top: 0px;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * General Help Page Content & Body Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n";

static char PgEDFA_stylesheet_Item_35[] =
	"div.help-page-content-body table.help-content-table {\n"
	"    border-top: 1px solid black;\n"
	"    border-left: 1px solid black;\n"
	"}\n"
	"\n"
	"div.help-page-content-body table.help-content-table thead {\n"
	"    background-color: #c3c3c3\n"
	"}\n"
	"\n"
	"div.help-page-content-body table.help-content-table thead tr th {\n"
	"    padding: 3px;\n"
	"    font-weight: bold;\n"
	"    text-align: center;\n"
	"    vertical-align: middle;\n"
	"    border-right: 1px solid black;\n"
	"    border-bottom: 1px solid black;\n"
	"}\n"
	"\n"
	"table.help-content-table tbody tr td {\n";

static char PgEDFA_stylesheet_Item_36[] =
	"    padding: 2px;\n"
	"    text-align: left;\n"
	"    vertical-align: top;\n"
	"    border-right: 1px solid black;\n"
	"    border-bottom: 1px solid black;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n"
	" * Help Header/Content Separator Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-footer-separator {\n"
	"    clear: both;\n"
	"}\n"
	"\n"
	"\n"
	"/*********************************************************************"
	"********\n";

static char PgEDFA_stylesheet_Item_37[] =
	" * Help Page Footer & Copyright Style\n"
	" "
	"**********************************************************************"
	"*******/\n"
	"\n"
	"div.help-page-footer-copyright {\n"
	"    font-size: 10px;\n"
	"    padding-top: 15px;\n"
	"    text-align: center;\n"
	"    vertical-align: bottom;\n"
	"}\n"
	"\n"
	"\n"
	"   table{\n"
	"    border:1; \n"
	"    border-color: #383838; \n"
	"    border-radius: 0 0 10px 10px;\n"
	"   }\n"
	"\n"
	"\ttd{\n"
	"\tborder-style:solid;\n"
	"\tborder:1;\n"
	"\t}\n"
	"\n"
	"\t.loginScreen{\n"
	"\ttext-align:center;\n"
	"        border-top:0;\n"
	"\tborder-left:0;\n"
	"\tborder-right:0;\n";

static char PgEDFA_stylesheet_Item_38[] =
	"\tborder-color:rgb(0,0,160);\n"
	"\t}\n"
	"\t\n"
	"\t.noBordersLogin{\n"
	"\tborder-style:none;\n"
	"\t}\n"
	"\n"
	"\t.ipgTitle{\n"
	"\tcolor:rgb(0,0,160);\n"
	"\tfont-size:30;\n"
	"\t}\n"
	"\t\n"
	"\t\n"
	"\t.userPassLogin{\n"
	"\tfont-weight:bold;\n"
	"\tcolor:rgb(0,0,160);\n"
	"\t}\n"
	"\t\n"
	"\t.btnLogincenter{\n"
	"\ttext-align:center;\n"
	"\tborder-style:none;\n"
	"\t}\n"
	"\t\n"
	"\t.tabsTitle{\n"
	"\tborder-style:solid;\n"
	"\ttext-align:center;\n"
	"\tborder-color:rgb(0,0,160);\n"
	"\tborder-collapse:collapse;\n"
	"\tbackground-color:white;\n"
	"\t}\n"
	"\t\n"
	"\t.tdTabs{\n"
	"\t border-style:none;\n"
	"\t }\n"
	" \n"
	"\t.tables{\n"
	"\t width:98%;\n"
	"\t background-color:rgb(53,64,198);\n";

static char PgEDFA_stylesheet_Item_39[] =
	"\t font-size:25px;\n"
	"\t border-color:rgb(53,64,198);\n"
	"\t color:white;\n"
	"\t size:30%;\n"
	"\t }\n"
	"\t \n"
	"\t.withBorderTdDis{\n"
	"    border-color:rgb(0,0,160);\n"
	"\tbackground-color:rgb(206,206,206); \n"
	"\t}\n"
	"\t\n"
	"   .tableBorder{\n"
	"   border-collapse:collapse;\n"
	"   border-color:rgb(0,0,160);\n"
	"   }\n"
	" \n"
	"   .tableBorderTextCentered{\n"
	"   border-collapse:collapse;\n"
	"   border-color:rgb(0,0,160);\n"
	"   text-align:center;\n"
	"   }\n"
	"   \n"
	"   .tableHeaderTextCentered{\n"
	"   border-collapse:collapse;\n"
	"   border-color:rgb(0,0,160);\n";

static char PgEDFA_stylesheet_Item_40[] =
	"   background-color:rgb(0,0,160);\n"
	"   text-align:center;\n"
	"   color:white;\n"
	"   }\n"
	"   \n"
	"   .ChassisRebootBtn{\n"
	"    width:155px;\n"
	"\tbackground-color:rgb(128,128,255);\n"
	"\tborder-color:rgb(128,128,255);\n"
	"\tcolor:rgb(0,0,128);\n"
	"\tfont-size:20px;\n"
	"\t}\n"
	"\n"
	"\t.NetworkTab{\n"
	"\tborder-style:none;\n"
	"\tmargin-top:1%;\n"
	"\t}\n"
	"\n"
	"   \n"
	"    \t\n"
	"   .tdbold{\n"
	"    \n"
	"\tfont-size:18px;\n"
	"        font-family:agency;\n"
	"\t}\n"
	"\t\n"
	"\t.NetworkTh{\n"
	"\tborder-collapse:collapse;\n"
	"\tborder-color:rgb(0,0,160);\n"
	"\tfont-size:20px;\n"
	"\tborder-style:solid;\n"
	"\t}\n"
	"\t \n"
	"\t.TextDisable{\n";

static char PgEDFA_stylesheet_Item_41[] =
	"\tborder-style:none;\n"
	"\tbackground-color:white;\n"
	"\tcolor:black;\n"
	"\ttext-align:center;\n"
	"    }\n"
	"\t\n"
	"    .helpDisable{\n"
	"    border-style:none;\n"
	"\tbackground-color:white;\n"
	"\tcolor:black;\n"
	"\t}\n"
	"\t \n"
	"\n";


static rpItem PgEDFA_stylesheet_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_4 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_5 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_6 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_7 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_8 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_9 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_10 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_11 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_12 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_13 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_14 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_15 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_16 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_17 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_18 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_19 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_20 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_21 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_22 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_23 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_24 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_25 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_26 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_27 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_28 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_29 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_30 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_31 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_32 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_33 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_34 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_35 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_36 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_37 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_38 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_39 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_40 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_stylesheet_Item_41 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFA_stylesheet = {
	"/EDFA_stylesheet.css",
	PgEDFA_stylesheet_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Unprotected,
	eRpDataTypeCss,
	eRpObjectTypeDynamic
};


/* ************************************ */
/* *    Built from "EDFA_Utils.js"    * */
/* ************************************ */

extern rpObjectDescription PgEDFA_Utils;

static char PgEDFA_Utils_Item_1[] =
	"// Open online help window.\n"
	"// The page parameter is the name of help file to load, relative to "
	"the help directory.\n"
	"// Example: openHelp(\'port_config_hlp.htm\')\n"
	"function openHelp(page) {\n"
	"    if (page === null || page.length === 0) {\n"
	"        alert(\'There is no online help for this page.\');\n"
	"        return;\n"
	"    }\n"
	"\n"
	"    var helpUrl = \'/help/\' + page;\n"
	"    var helpWindow = "
	"window.open(helpUrl,\'DeviceWebUIHelpWindow\',\'width=700,height=600,s";

static char PgEDFA_Utils_Item_2[] =
	"crollbars=yes,resizable=yes,status=yes,menubar=no,toolbar=no\');\n"
	"    helpWindow.focus();\n"
	"}\n"
	"\n"
	"// Close online help window\n"
	"function closeHelp() {\n"
	"    var helpWindow = "
	"window.open(\"/help/help_toc.htm\",\'DeviceWebUIHelpWindow\',\'width=0"
	",height=0,scrollbars=yes,resizable=yes,status=yes,menubar=no,toolbar=n"
	"o\');\n"
	"    helpWindow.close();\n"
	"}\n"
	"\n"
	"// Get an element with specified name from the specified form\n"
	"function getElement(form, name) {\n"
	"    if (form === null || form.elements === null ||" C_NAME "== null) ";

static char PgEDFA_Utils_Item_3[] =
	"return null;\n"
	"\n"
	"    // search element list\n"
	"    for (var i = 0; i < form.elements.length; i++) {\n"
	"        if (form.elements[i].name === name) return form.elements[i];\n"
	"    }\n"
	"\n"
	"    // none found\n"
	"    return null;\n"
	"}\n"
	"\n"
	"// Cross browser access to HTML elements.\n"
	"// This function gets the HTML element by name and returns an object\n"
	"// that contains properties for both the html element object and the "
	"style.\n"
	"// Usage:\n"
	"//   var x = new getObj(\'objectname\');\n"
	"//   x.obj.innerHTML = \"Howdy\";\n";

static char PgEDFA_Utils_Item_4[] =
	"//   x.style.top = \'20px\';\n"
	"function getObj(name) {\n"
	"    if (document.getElementById) { // IE 5+\n"
	"        this.obj = document.getElementById(name);\n"
	"        if (document.getElementById(name))\n"
	"            this.style = document.getElementById(name).style;\n"
	"    }\n"
	"    else if (document.all) { // IE 4+\n"
	"        this.obj = document.all[name];\n"
	"        if (document.all[name])\n"
	"            this.style = document.all[name].style;\n"
	"    }\n"
	"    else if (document.layers) { // NS 4\n";

static char PgEDFA_Utils_Item_5[] =
	"        this.obj = document.layers[name];\n"
	"        this.style = document.layers[name];\n"
	"    }\n"
	"}\n"
	"\n"
	"function GoTo(page)\n"
	"{\n"
	"\n"
	"window.localStorage.clear();\n"
	"window.location.href = page;\n"
	"alert(window.location.href);\n"
	"}\n"
	"\n"
	"function setup()\n"
	"{\n"
	"    this.addEventListener(\"mousemove\", resetTimer, false);\n"
	"    this.addEventListener(\"mousedown\", resetTimer, false);\n"
	"    this.addEventListener(\"keypress\", resetTimer, false);\n"
	"    this.addEventListener(\"DOMMouseScroll\", resetTimer, false); \n";

static char PgEDFA_Utils_Item_6[] =
	"    this.addEventListener(\"mousewheel\", resetTimer, false);\n"
	"    this.addEventListener(\"touchmove\", resetTimer, false);\n"
	"    this.addEventListener(\"MSPointerMove\", resetTimer, false);\n"
	"\n"
	"    startTimer();\n"
	"}\n"
	" \n"
	"function startTimer()\n"
	" {\n"
	"    // wait 15 mins before calling goInactive\n"
	"    TimeoutID = window.setTimeout(goInactive,60000);\n"
	"}\n"
	" \n"
	" function stopTimer() \n"
	" {\n"
	"    // wait 15 mins before calling goInactive\n"
	"    window.clearTimeout(TimeoutID);\n"
	"}\n"
	"\n"
	"function resetTimer() \n"
	"{\n";

static char PgEDFA_Utils_Item_7[] =
	"    window.clearTimeout(TimeoutID);\n"
	"    goActive();\n"
	"}\n"
	" \n"
	"function goInactive() \n"
	"{\n"
	"    // do something\n"
	"\twindow.localStorage.clear() ; //???\n"
	"\twindow.location.href = \'EDFA Login.htm\';\n"
	"}\n"
	" \n"
	"function goActive() \n"
	"{\n"
	"    // do something\n"
	"    startTimer();\n"
	"}\n"
	"\n"
	"function Logout(){\n"
	" window.localStorage.clear();\n"
	" window.location.href=\'EDFA Login.htm\';\n"
	" alert(window.location.href);\n"
	" }\n"
	" \n"
	" \n";


static rpItem PgEDFA_Utils_Items[] = { 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_1 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_2 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_3 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_4 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_5 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_6 }, 
	{ eRpItemType_DataZeroTerminated, (void *) &PgEDFA_Utils_Item_7 }, 
	{ eRpItemType_LastItemInList } 
};


rpObjectDescription PgEDFA_Utils = {
	"/EDFA_Utils.js",
	PgEDFA_Utils_Items,
	(rpObjectExtensionPtr) 0,
	(Unsigned32) 0,
	kRpPageAccess_Unprotected,
	eRpDataTypeJs,
	eRpObjectTypeDynamic
};

#endif	/* RomPagerServer */
