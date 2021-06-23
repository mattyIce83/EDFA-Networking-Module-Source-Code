#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <tx_api.h>

#include "readFrom2U2.h"
#include "semaphores.h"
#include "atmel.h"



/*
 * askAtmelForUnsignedValue
 *  get the serial semaphore
 *  ask atmel for info
 *  collect info and decode
 *  return info
 *
 * parameters:
 *  p_command - command to send to atmel
 *  p_confirm - string to expect back from atmel
 *  p_adjust_flag - whether to put an X on the end
 *  p_checksum_flag - whether to verify the checksum
 *  p_retries - how many times to read lines before sending a new command
 *  p_answer - pointer to place to put the answer
 *  p_fid - function id - for semaphore stats (last get and last set)
 * 
 *  return NULL if error
 *  return pointer to caller-supplied space for answer (p_answer) if success
 *
 */

void *
askAtmelForUnsignedValue (char * p_command,
                          char * p_confirm,
                          readFrom2U_adjust_e p_adjust_flag,
                          readFrom2U_checksum_e p_checksum_flag,
                          int p_retries,
                          unsigned long * p_answer,
                          int p_fid)
{
    void *resultBuffer = NULL;
    int status;
    int done = 0;

    status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, p_fid);
    if (status == TX_SUCCESS) {
        //while ( ! done ) {
            flushBuffer ();
            printf ("%s\r", p_command);
            fflush (stdout);
            tx_thread_sleep (12);  /* delay to allow reply to return */
            if ( done = getUnsignedAnswerFromAtmel (p_confirm,
                                                    p_adjust_flag,
                                                    p_checksum_flag,
                                                    p_retries,
                                                    p_answer) ) {
                resultBuffer = (void*) p_answer;
            }
        //}
        status = semaphore_put_2(&semaphore_0_info, p_fid);
    }

    return resultBuffer;
}



void *
askAtmelForStringValue (char * p_command,
                        char * p_confirm,
                        readFrom2U_adjust_e p_adjust_flag,
                        readFrom2U_checksum_e p_checksum_flag,
                        int p_retries,
                        unsigned char * p_answer,
                        unsigned long *p_pAnswerLength,
                        int p_fid) {
    void *resultBuffer = NULL;
    int status;
    int done = 0;

    status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, p_fid);
    if (status == TX_SUCCESS) {
        //while ( ! done ) {
            flushBuffer ();
            printf ("%s\r", p_command);
            fflush (stdout);
            tx_thread_sleep (13);  /* delay to allow reply to return */
            if ( done = getStringAnswerFromAtmel (p_confirm,
                                                  p_adjust_flag,
                                                  p_checksum_flag,
                                                  p_retries,
                                                  p_answer,
                                                  p_pAnswerLength) ) {
                resultBuffer = (void*) p_answer;
            }
        //}
        status = semaphore_put_2 (&semaphore_0_info, p_fid);
    }

    return resultBuffer;
}



void *
askAtmelForIPAddressValue (char * p_command,
                           readFrom2U_adjust_e p_adjust_flag,
                           readFrom2U_checksum_e p_checksum_flag,
                           int p_retries,
                           unsigned long * p_ipAddress,
                           int p_fid) {
    void *resultBuffer = NULL;
    int status;
    int done = 0;

    status = semaphore_get_2 (&semaphore_0_info, SEMAPHORE_TMO, p_fid);
    if (status == TX_SUCCESS) {
        //while ( ! done ) {
            flushBuffer ();
            printf ("%s\r", p_command);
            fflush (stdout);
            tx_thread_sleep (12);  /* delay to allow reply to return */
            if ( done = getIPAddressAnswerFromAtmel (p_adjust_flag,
                                                     p_checksum_flag,
                                                     p_retries,
                                                     p_ipAddress) ) {
                resultBuffer = (void*) p_ipAddress;
            }
        //}
        status = semaphore_put_2 (&semaphore_0_info, p_fid);
    }

    return resultBuffer;
}
