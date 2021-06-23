/*
 * $Id: telnet.h,v 1.1.1.1 2006-05-04 14:38:39 eugene Exp $
 */

#ifndef _telnet_h
#define _telnet_h

#define TS_TEXT_LEN		   64
#define TS_PORT_BASE		10000
#define MAX_ENTRIES		    1
//#define MAX_ENTRIES		    2
#define MAX_SESSIONS		(2 * MAX_ENTRIES)
//#define MAX_SESSIONS		(10 * MAX_ENTRIES)

#define	MAX_STATIONS		6			// 06 13 05 - pwl
#define	MAX_ACE_INTERFACES	3			// 06 27 05 - pwl


typedef struct
{
	char	username[TS_TEXT_LEN];
	char 	password[TS_TEXT_LEN];
} TS_VALIDATE_TYPE;


typedef struct
{
	int				inuse;
	int				sockfd;
	int				menu;
	int				state;
	int				uid;
	char			buf[256];
	TX_TIMER	    t_timeout;
} TS_SESSION_TYPE;

#define MIN_TELNET_TIMEOUT 30
#define MAX_TELNET_TIMEOUT 3600

extern TS_SESSION_TYPE	TS_session_tab[];
extern int TimeoutS;

#endif /* _telnet_h */
