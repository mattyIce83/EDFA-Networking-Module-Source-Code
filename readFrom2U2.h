#ifndef READFROM2U2_H__
#define READFROM2U2_H__


#define MAX_2U_RESPONSE 40 

typedef enum {
  E_READ_FROM_2U_NO_CHECKSUM,
  E_READ_FROM_2U_DO_CHECKSUM
} readFrom2U_checksum_e;

typedef enum {
  E_READ_FROM_2U_NO_ADJUST,
  E_READ_FROM_2U_DO_ADJUST
} readFrom2U_adjust_e;

/*
 * args:
 *   in:
 *     p_buffer        -> caller-allocated space we can use
 *     p_buffer_length -> length of buffer we can use
 *   out:
 *     p_buffer        -> buffer filled with stuff read from 2U
 *     p_buffer_length -> amount of characters read (including NULL at end)
 */
void
readFrom2U2(char * p_buffer, int * p_buffer_length);


/*
 * args:
 *   in:
 *     p_buffer         -> caller-allocated space we can use
 *     *p_buffer_length -> length of buffer we can use
 *   out:
 *     p_buffer         -> buffer filled with stuff read from 2U
 *     *p_buffer_length -> amount of characters read (including NULL at end)
 * return:
 *   *p_buffer_length
 */
int
readLineFrom2U (char * p_buffer, int * p_buffer_length);

/*
 * args:
 *   in:
 *     p_buffer         -> caller-allocated space we can use
 *     p_buffer_length  -> length of buffer we can use
 * return:
 *   true==1 (valid)   false==0 (invalid)
 * does not change args
 */
int
validateLineFrom2U (char * p_buffer, int p_buffer_length);


/* 
 * args:
 *   in:
 *     p_buffer         -> caller-allocated space we can use
 *     p_buffer_length  -> length of buffer used so far
 *                         (we assume there is one more byte available)
 *   out:
 *     p_buffer_length  -> new length of buffer (one more than input)
 * return:
 *   *p_buffer_length
 */
int
adjustLineFrom2U (char * p_buffer, int * p_buffer_length);



/* Try to get an answer from the Atmel via the serial link
 *
 */
int
getUnsignedAnswerFromAtmel (char * p_command,
                            readFrom2U_adjust_e p_do_adjust_flag,
                            readFrom2U_checksum_e p_with_checksum_flag,
                            int p_retries,
                            unsigned long * p_the_number);

/* Try to get an answer from the Atmel via the serial link
 *
 */
int
getStringAnswerFromAtmel (char * p_command,
                          readFrom2U_adjust_e p_do_adjust_flag,
                          readFrom2U_checksum_e p_with_checksum_flag,
                          int p_retries,
                          char * p_the_string,
                          unsigned long * p_string_length);


int
getIPAddressAnswerFromAtmel (readFrom2U_adjust_e p_do_adjust_flag,
                             readFrom2U_checksum_e p_with_checksum_flag,
                             int p_retries,
                             unsigned long * p_ipAddress);

#endif /* READFROM2U2_H__ */

