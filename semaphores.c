/*
 * $Id: semaphores.c,v 1.1.1.1 2007-01-08 23:29:17 eugene Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <tx_api.h>
#include <man_api.h>
#include <snmpapi.h>
#include <sysAccess.h>

#include "../../bsp/platforms/connectme/bsp.h"
#include "ipgFlash.h"


#include "../../bsp/platforms/connectme/gpio_def.h"

#include <sockapi.h>
#include <tservapi.h>


#include "ace_params.h"
#include "boardParams.h"

#include "ipgStrings.h"
#include "../../../h/fs.h"
#include "../../../h/rtc.h"
#include "../../../h/sysAccess.h"

#include <sysAccess.h>
#include "appconf.h"
#include "telnet.h"
#include "root.h"
#include "semaphores.h"

#include "../../bsp/platforms/connectme/dialog.h"
#include "../../../h/threadx/tx_api.h"
#include "../../../h/man_api.h"
#include "../../../h/snmp/snmp.h"

#include "ipgsnmp.h"
#include "watch.h"


TX_SEMAPHORE semaphore_0;
int semaphore_0_gotten = 0;
int semaphore_0_tries = 5;
int semaphore_0_unwedged = 0;
TX_SEMAPHORE semHistTblMutex;
TX_SEMAPHORE 	 semaphore_nvParams;
TX_SEMAPHORE 	 semaphore_rtc;
TX_SEMAPHORE 	 semaphore_sbuf;

int get_failed = 0;
int InsertSnmpRowFailed = 0;

int get_who = 0;
int put_who = 0;

semaphore_info_t semaphore_0_info = {
    &get_who, 
    &put_who,
    &semaphore_0_gotten,
    &semaphore_0_tries,
    &semaphore_0_unwedged,
    &get_failed,
    "serialSemaphore",
    &semaphore_0
};


int prm_get_who = 0;
int prm_put_who = 0;
int prm_gotten = 0;
int prm_tries = 5;
int prm_unwedged = 0;
int prm_get_failed = 0;

semaphore_info_t semaphore_nvParams_info = {
    &prm_get_who,
    &prm_put_who,
    &prm_gotten,
    &prm_tries,
    &prm_unwedged,
    &prm_get_failed,
    "nvPrmSemaphore",
    &semaphore_nvParams };


int rtc_get_who = 0;
int rtc_put_who = 0;
int rtc_gotten = 0;
int rtc_tries = 5;
int rtc_unwedged = 0;
int rtc_get_failed = 0;

semaphore_info_t semaphore_rtc_info = {
    &rtc_get_who, 
    &rtc_put_who,
    &rtc_gotten,
    &rtc_tries,
    &rtc_unwedged,
    &rtc_get_failed,
    "rtcSemaphore",
    &semaphore_rtc
};


int hist_get_who = 0;
int hist_put_who = 0;
int hist_gotten = 0;
int hist_tries = 5;
int hist_unwedged = 0;
int hist_get_failed = 0;

semaphore_info_t semaphore_history_table_info = {
    &hist_get_who,
    &hist_put_who,
    &hist_gotten,
    &hist_tries,
    &hist_unwedged,
    &hist_get_failed,
    "HistoryTable",
    &semHistTblMutex };


int sbuf_get_who = 0;
int sbuf_put_who = 0;
int sbuf_gotten = 0;
int sbuf_tries = 5;
int sbuf_unwedged = 0;
int sbuf_get_failed = 0;

semaphore_info_t semaphore_sbuf_info = {
    &sbuf_get_who, 
    &sbuf_put_who,
    &sbuf_gotten,
    &sbuf_tries,
    &sbuf_unwedged,
    &sbuf_get_failed,
    "sbufSemaphore",
    &semaphore_sbuf
};



int
semaphore_init_2 (semaphore_info_t * p_sem) {
    int ccode;

    ccode = tx_semaphore_create (p_sem->sem, p_sem->name_, 1);
    /* prep for the day when the initialization will be done here */
    *(p_sem->gotten) = 0;

    return ccode;
}


unsigned int
semaphore_get(TX_SEMAPHORE * pSem, unsigned long wait, int who)
{
    unsigned int status = ~TX_SUCCESS;
    int tries = 5;

    while ( (TX_SUCCESS != status) && (0 < tries) ) {
        status = tx_semaphore_get(pSem, wait);
        --tries;
    }

    if (status == TX_SUCCESS) {
	get_who = who;
    } else {
	get_failed++;
    }

    return status;
}

unsigned int
semaphore_put(TX_SEMAPHORE * pSem, int who)
{
    unsigned int status;

    if (pSem->tx_semaphore_count > 0) {
	/* woops--we didn't have the semaphore in the first place */
	return TX_SUCCESS;
    } else {
	status = tx_semaphore_put(pSem);
	put_who = who;
	return status;
    }
}


unsigned int
semaphore_get_2 (semaphore_info_t * p_sem_info, unsigned long wait, int who) {

    unsigned int status = ~TX_SUCCESS;
    int tries = *(p_sem_info->tries);

    while ( (TX_SUCCESS != status) && (0 < tries) ) {
        status = tx_semaphore_get (p_sem_info->sem, wait);
        --tries;
    }

    if ( TX_SUCCESS == status ) {
        *(p_sem_info->last_get) = who;
        ++(*(p_sem_info->gotten));
    } else {
        ++(*(p_sem_info->get_failed));
    }

    return status;
}

unsigned int
semaphore_put_2 (semaphore_info_t * p_sem_info, int who) {
    unsigned int status;

    if ( 0 < p_sem_info->sem->tx_semaphore_count ) {
        /* whoops -- we didn't have the semaphore in the first place */
        status = TX_SUCCESS;
    } else {
        status = tx_semaphore_put (p_sem_info->sem);
        *(p_sem_info->last_put) = who;
    }

    return status;
}
