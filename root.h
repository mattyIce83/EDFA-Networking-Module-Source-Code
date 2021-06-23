#ifndef ROOT_H__
#define ROOT_H__

#ifndef FALSE
#define	FALSE		        0
#define	TRUE		        1
/* #else */
/* all the places that this warning comes up, do not depend on TRUE being
   a particular value, as far as I an tell.
   The places that depend on TRUE being exactly 1 do not seem to trigger
   this warning.  So, turn it off for now.
*/
/* #warning "FALSE (and presumably TRUE) already defined" */
#endif

#define POWER_SAFETY_FLAG	14

#define alarmThreadTMO	       25		// 15		// 12 19 05 - 25	// 3000		11 08 05
/* #define snmpThreadTMO	     1500		// 15000	12 14 05 */
#define snmpThreadTMO	     15000		// 15000	12 14 05  // as actually used in EARMIBAction.c, 2006/06/07

#define	unsigned32MAX		4026531839	// 10

extern rtcDataType g_timedate;

#define SERIAL_READ_SUCCESS 0
#define SERIAL_READ_EMPTY -1
#define SERIAL_READ_NO_SEMAPHORE -2
#define SERIAL_READ_TRUNCATE 1
#define SERIAL_READ_FAILED_READ -3

int flushBufferToLastCR (void);
int getLineFromSerialBuffer (unsigned char *, unsigned int *);

#endif
