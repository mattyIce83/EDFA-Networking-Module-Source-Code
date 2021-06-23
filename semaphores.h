#ifndef _semaphores_h
#define _semaphores_h

/* 
 * $Id: semaphores.h,v 1.1.1.1 2007-01-08 22:37:40 eugene Exp $ 
 */
 
#include <stdio.h>
#include <stdlib.h>

#define SEMAPHORE_TMO	      100	

#define          SEMAPHORE_0_TRIES_LIMIT (10)
#define          SEMAPHORE_1_TRIES_LIMIT (10)

extern int              semaphore_0_tries;
extern int              semaphore_0_unwedged;
extern TX_SEMAPHORE 	semaphore_0;
extern TX_SEMAPHORE 	semHistTblMutex;
extern TX_SEMAPHORE 	semaphore_nvParams;
extern TX_SEMAPHORE 	semaphore_rtc;
extern TX_SEMAPHORE 	semaphore_sbuf;


extern int get_failed;
extern int InsertSnmpRowFailed;

extern int get_who;
extern int put_who;

#define MAX_SEM_NAME_LEN 64

typedef struct semaphore_info_struct {
    int * last_get;
    int * last_put;
    int * gotten;
    int * tries;
    int * unwedged;
    int * get_failed;
    unsigned char name_[MAX_SEM_NAME_LEN];
    TX_SEMAPHORE * sem;
} semaphore_info_t;


extern semaphore_info_t semaphore_0_info;
extern semaphore_info_t semaphore_nvParams_info;
extern semaphore_info_t semaphore_rtc_info;
extern semaphore_info_t semaphore_history_table_info;
extern semaphore_info_t semaphore_sbuf_info;

int semaphore_init_2 (semaphore_info_t *);

unsigned int semaphore_get(TX_SEMAPHORE *, unsigned long, int);
unsigned int semaphore_put(TX_SEMAPHORE *, int);

unsigned int semaphore_get_2 (semaphore_info_t *, unsigned long, int);
unsigned int semaphore_put_2 (semaphore_info_t *, int);

#endif /* _semaphores_h */
