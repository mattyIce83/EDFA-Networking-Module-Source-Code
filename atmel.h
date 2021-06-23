
#ifndef ATMEL_H__
#define ATMEL_H__


/* 
 * each of these:
 *   gets the serial semaphore
 *   writes the command to the Atmel across the serial line
 *   reads the serial line until it gets the answer to the question it asked
 *    (or until "retries" attempts have been done)
 *   puts the serial semaphore
 */

#define ATMEL_CONTROLMODE_POWER ASCII_P
#define ATMEL_CONTROLMODE_CURRENT ASCII_C
#define ATMEL_CONTROLMODE_GAIN ASCII_G

#define ATMEL_DEVICETYPE_LEGACY ASCII_L
#define ATMEL_DEVICETYPE_MASTER ASCII_M
#define ATMEL_DEVICETYPE_SLAVE ASCII_S

#define ATMEL_DATAMODE_OFF ASCII_O
#define ATMEL_DATAMODE_ALS ASCII_A
#define ATMEL_DATAMODE_NOMINAL ASCII_N


void *
askAtmelForUnsignedValue (char * p_command,
                          char * p_confirm,
                          readFrom2U_adjust_e p_adjust_flag,
                          readFrom2U_checksum_e p_checksum_flag,
                          int p_retries,
                          unsigned long * p_answer,
                          int p_fid);


void*
askAtmelForStringValue (char * p_command,
                        char * p_confirm,
                        readFrom2U_adjust_e p_adjust_flag,
                        readFrom2U_checksum_e p_checksum_flag,
                        int p_retries,
                        unsigned char * p_answer,
                        unsigned long *p_pAnswerLength,
                        int p_fid);



/* adjust, checksum not implemented - don't use them */
void*
askAtmelForIPAddressValue (char * p_command,
                           readFrom2U_adjust_e p_adjust_flag,
                           readFrom2U_checksum_e p_checksum_flag,
                           int p_retries,
                           unsigned long * p_ipAddress,
                           int p_fid);

#endif
