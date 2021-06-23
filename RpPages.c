/* Created with PageBuilder version 4.04 on Wed Feb 18 10:38:55 2015 */

#include "AsExtern.h"

#if RomPagerServer

extern rpObjectDescription PgEDFAWelcomePage;
extern rpObjectDescription PgEDFAGeneralSettings;
extern rpObjectDescription PgEDFAChassisManager;
extern rpObjectDescription PgEDFAChassisManager_Form_1;
extern rpObjectDescription PgEDFANetworkAdministration;
extern rpObjectDescription PgEDFAThresholdSettings;
extern rpObjectDescription PgEDFAAlarmsStatus;
extern rpObjectDescription PgEDFA_stylesheet;
extern rpObjectDescription PgEDFA_Utils;

rpObjectDescriptionPtr gRpObjectList[] = {
	&PgEDFAWelcomePage,
	&PgEDFAGeneralSettings,
	&PgEDFAChassisManager,
	&PgEDFAChassisManager_Form_1,
	&PgEDFANetworkAdministration,
	&PgEDFAThresholdSettings,
	&PgEDFAAlarmsStatus,
	&PgEDFA_stylesheet,
	&PgEDFA_Utils,
	(rpObjectDescriptionPtr) 0
};

rpObjectDescPtrPtr gRpMasterObjectList[] = {
	(rpObjectDescriptionPtr *) gRpObjectList,
	(rpObjectDescPtrPtr) 0
};


#endif	/* RomPagerServer */
