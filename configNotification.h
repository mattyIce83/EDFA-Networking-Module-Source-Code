#ifndef __configNotifiation_h
#define __configNotifiation_h


extern int g_snmp_toggle;
extern int g_snmp_mgmt_stn_idx;

void configErrorNotification(struct varBind *, char *);
void configChangeNotification(struct varBind *);
void configNotification(char *, char *);


#endif /* __configNotifiation_h */
