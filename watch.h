#ifndef __WATCH_H
#define __WATCH_H

/*
 * Make a stack that starts with a guard,
 * is filled with marks, and ends with a guard.
 * The way to do this is to malloc( + STACKGUARD_SIZE), but bump
 * the pointer just before calling the tx_thread_create.
 */

#define STACKGUARD_SIZE (4)
#define STACKGUARD (0xDEADBEEF) /* per threadx which seems to remark stack */
#define STACKMARK (0xEF) /* per threadx TX_STACK_FILL */
#define STACKCHUNK 4

extern unsigned char *snmpStackBegin;
extern int snmpStackSize;

extern unsigned char *telnetStackBegin;
extern int telnetStackSize;

extern unsigned char *alarmStackBegin;
extern int alarmStackSize;

extern unsigned char *serialStackBegin;
extern int serialStackSize;

extern unsigned char *collectionStackBegin;
extern int collectionStackSize;

#endif /* __WATCH_H */
