///////////////////////////////////////////////////////////////////////////////
// $Id: readFrom2U2.c,v 1.1.1.1 2007-01-29 22:00:14 eugene Exp $
//
//      IPG Photonics Corporation
//      COPYRIGHT (c) 2001 IPG Photonics Coroporation ALL RIGHTS RESERVED
//  This program may not be reproduced, in whole or in part in any
//  form or any means whatsoever without the written permission of:
//
//  IPG Photonics Corporation
//  50 Old Webster Road 
//  Oxford, MA 01540, USA 
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <tx_api.h>
#include <man_api.h>
#include <snmpapi.h>
#include <sysAccess.h>
#include <termios.h>

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

#include "../../bsp/platforms/connectme/dialog.h"
#include "../../../h/threadx/tx_api.h"
#include "../../../h/man_api.h"
#include "../../../h/snmp/snmp.h"

#include "ipgsnmp.h"
#include "watch.h"
#include "ascii.h"
#include "readFrom2U2.h"

// #define MAX_2U_RESPONSE		  31
#define SERIAL_FD_COM0  0x102
#define SERIAL_FD_COM1  0x103

/* extern char pl_buffer[MAX_2U_RESPONSE]; */

unsigned int g_readFrom2U2_cant_happen = 0;
unsigned int g_readFrom2U2_garbage_in_buffer = 0;
unsigned int g_readFrom2U2_reset_control_variable = 0;
unsigned int g_readFrom2U2_getchar_calls = 0;
unsigned int g_readFrom2U2_returns = 0;
unsigned int g_readFrom2U2_EOS = 0;
unsigned int g_readFrom2U2_mark = 0;
unsigned int g_readFrom2U2_2U = 0;



/* return false (0) if it doesn' match, return true (1) if it matches
 *
 * match regex:  [0-9]+[.][0-9]+[.][0-9]+[.][0-9]
 *
 * matches for decimal dotted quad representation of ip address
 * doesn't match for hex dotted quad representation
 */
static int
looks_like_ip_address (char * p_buffer) {
    int answer = 0; /* by default, it doesn't look like an ip address */
    char * pCh = &(p_buffer[0]);

    /* skip leading spaces, if any */
    while ( (ASCII_NULL != *pCh) &&
            ((ASCII_SPACE == *pCh) && (ASCII_TAB == *pCh)) ) {
        ++pCh;
    }

    /* check first octet */
    if ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
        while ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
            ++pCh;
        }

        /* check first dot */
        if ( (ASCII_PERIOD == *pCh) ) {
            ++pCh;

            /* check second octet */
            if ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
                while ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
                    ++pCh;
                }

                /* check second dot */
                if ( (ASCII_PERIOD == *pCh) ) {
                    ++pCh;

                    /* check third octet */
                    if ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
                        while ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
                            ++pCh;
                        }

                        /* check third dot */
                        if ( (ASCII_PERIOD == *pCh) ) {
                            ++pCh;

                            /* check fourth octet */
                            if ( (ASCII_ZERO <= *pCh) && (ASCII_NINE >= *pCh) ) {
                                answer = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    return answer;
}


/* 
 * bjb - Put in the ability to check for buffer overruns.
 * Maybe one day we'll use it.
 * And we can pass back the number of characters in the
 * buffer.
 * Maybe I'll just stick to adding the parameter for now,
 * and try to use it later.
 */
void readFrom2U2(char * p_buffer, int * p_buffer_length)
{
#define NEW_GET_LINE 1
#if 1 == NEW_GET_LINE
    readLineFrom2U (p_buffer, p_buffer_length);
#else
    int EOS = 0;  /* bjb:  what is this for? */
    int i;
    int j = 0;
    int SEEN_VALID = 0;
    int rc_f;

    /* flag from caller:  we're pretending to be readFrom2U */
    int with_x = (p_buffer[0] == ASCII_CARET);
    p_buffer[0] = ASCII_NULL;
    if ( with_x ) {
        ++g_readFrom2U2_2U;
    }

    memset (p_buffer, ASCII_NULL, MAX_2U_RESPONSE);
    for (i = 0; i < MAX_2U_RESPONSE; ++i) {
        ++g_readFrom2U2_getchar_calls;
	p_buffer[j++] = getchar();	// read the response from the 2U

	if (!(isascii(p_buffer[j - 1]))) {
            // putchar(p_buffer[j - 1]); /* GORD why-oh-why? */
            *p_buffer_length = 0; /* garbage in buffer */
            ++g_readFrom2U2_garbage_in_buffer;
	    return;
	}

	if (p_buffer[j - 1] == ASCII_CR) {	// RETURN character?
            ++g_readFrom2U2_returns;
            if ( /* (2 == j) && */ (ASCII_EXCLAMATION == p_buffer[0]) ) {
#if 0
no special treatment for marks
              j = 0;
              p_buffer[0] = ASCII_NULL;
#endif
              ++g_readFrom2U2_mark;
              break;
            }
	    if ((EOS == 0) && (!SEEN_VALID)) {
                memset (p_buffer, ASCII_NULL, MAX_2U_RESPONSE);
		j = 0;
                if ( with_x ) {
                    ++g_readFrom2U2_reset_control_variable;
                    i = 0;
                }
	    } else {
                if ( with_x ) {
                    p_buffer[j - 1] = ASCII_X;  //overwrite  the \r with X
		    p_buffer[j++] = ASCII_NULL;  // follow with NULL
		    break;         // and we're finished
                } else {
		    p_buffer[j - 1] = ASCII_NULL;	// overwrite the \r with NULL
		    break;		// and we're finished
                }
	    }
	    EOS++;
            ++g_readFrom2U2_EOS;
	}

	if (!(isascii(p_buffer[j - 1]))) {
            ++g_readFrom2U2_cant_happen;
            /* this never happens, because if the latest character
             *  is !isascii(), we don't get here
             */
            memset (p_buffer, ASCII_NULL, MAX_2U_RESPONSE);
            /* bjb:  heh, the for loop has the side effect of
             * setting j to MAX_2U_RESPONSE.  Hmm.
             */
            j = MAX_2U_RESPONSE;
	    break;
	}

	if (isdigit(p_buffer[j - 1]))	// if the response is numeric, that's
	    SEEN_VALID = 1;	// good

	if (j == 2) {
            p_buffer[j] = ASCII_NULL;	// required for the strcmp
	    if (!(strncmp(p_buffer, "ZR", j)))	// reply from a ZIMR(ead) operative
		SEEN_VALID = 1;
	}

	if (j == 4) {
	    p_buffer[j] = ASCII_NULL;	// required for the strcmp
	    if ((!(strncmp(p_buffer, "SET ", j))) ||	// factory default value
		(!(strncmp(p_buffer, "POWE", j))) ||	// acceptable control mode value
		(!(strncmp(p_buffer, "CURR", j))) ||	// acceptable control mode value
		(!(strncmp(p_buffer, "GAIN", j))))	// acceptable control mode value
		SEEN_VALID = 1;
	} /* if ( j == 4 ) */
    }  /* for (i = 0; i < MAX_2U_RESPONSE; ++i ) */

    if ( 0 != (rc_f = tcflush (SERIAL_FD_COM0, TCIFLUSH)) ) {
    }

    if ( 0 != (rc_f = fflush(stdin)) ) {
      /* What does a fflush on an input stream do, anyway?
       * It throws away the remaining info that has been read and buffered,
       * it seems
       * but, isn't this a serial interface, one character at a time???
       */
    }

    *p_buffer_length = j;
#endif
}


int
readLineFrom2U (char * p_buffer, int * p_buffer_length) {
#if 1 == NEW_GET_LINE
    int status;

    status = getLineFromSerialBuffer (p_buffer, p_buffer_length);
    /* old function returns length, not that the old callers check it */
    if ( 0 == status ) {
        status = *p_buffer_length;
    }

    return status;
#else
    int ii;
    int gotChar;

    memset (p_buffer, ASCII_NULL, *p_buffer_length);

    for ( ii = 0; ii < *p_buffer_length; ++ii ) {
        /* blocking getchar - won't return till a character is got */
        gotChar = getchar ();
        if ( 0 <= gotChar ) {
            p_buffer[ii] = gotChar;
            if ( ASCII_CR == p_buffer[ii] ) {
                p_buffer[ii] = ASCII_NULL;
                *p_buffer_length = ii;
                break; 
            }
        } else {
            /* error getting character:  end the line here and return */
            p_buffer[ii] = ASCII_NULL;
            *p_buffer_length = ii;
            break;
        }
    }

    return *p_buffer_length;
#endif
}


int
validateLineFrom2U (char * p_buffer, int p_buffer_length) {
    int ii;
    int valid = 0;

    if ( 0 < p_buffer_length ) {

        /* if there are any digits in the response, it's valid */
        for ( ii = 0; ii < p_buffer_length; ++ii ) {
            if ( isdigit (p_buffer[ii]) ) {
                valid = 1;
            }
        }

        /* otherwise, check for certain kinds of response */
        if ( ! valid ) {
            if ( 2 <= p_buffer_length ) {
                if ( ! strncmp (p_buffer, "ZR", 2) ) {
                    valid = 1;
                }
            } else {
                if ( 4 <= p_buffer_length ) {
                    if ( (!(strncmp (p_buffer, "SET ", 4))) ||
                         (!(strncmp (p_buffer, "POWE", 4))) ||
                         (!(strncmp (p_buffer, "CURR", 4))) ||
                         (!(strncmp (p_buffer, "GAIN", 4)))) {
                        valid = 1;
                    } 
                }
            }
        }
    }

    return valid;
}


/* Put an X on the end of the buffer
 * The caller has to check if there is space
 * Maybe we can avoid this step
 */
int
adjustLineFrom2U (char * p_buffer, int * p_buffer_length) {
    /* put an X on the NULL */
    p_buffer[*p_buffer_length] = ASCII_X;

    ++(*p_buffer_length);

    /* put the NULL one character further out */
    p_buffer[*p_buffer_length] = ASCII_NULL;
}


static int
do_checksum (char * p_buffer, int p_buffer_length) {
    int ii;
    char * pc = p_buffer;
    int chksum = 0;

    if ( 0 == pc ) {
        chksum = 0;
    } else {
        while ( ! isspace (*pc) ) {
            /* chksum += (*pc - ASCII_ZERO); */
            chksum += *pc;
            ++pc;
        }
    }

    chksum &= 0xFF;

    return chksum;
}

/*
 * Note, there are no re-requests to the Atmel in this function,
 *       so retries should be a very small number
 * Args:
 *   in:
 *      p_command:  ptr to NULL-terminated buffer containing the command we
 *                    want to match
 *      p_do_adjust_flag:  1 if we want to put an X on the end
 *      p_with_checksum_flag:  1 if we want to verify the checksum
 *      p_retries:     the number of read retries to do before giving up
 *
 *   out:
 *      p_the_number:  the value that the other side sent (if success)
 *                     undefined (if failed)
 * 
 * Return:
 *   true(1):  got value
 *   false(0):  didn't get value
 */
int
getUnsignedAnswerFromAtmel (char * p_command,
                            readFrom2U_adjust_e p_do_adjust_flag,
                            readFrom2U_checksum_e p_with_checksum_flag,
                            int p_retries,
                            unsigned long * p_the_number) {
    char buffer [MAX_2U_RESPONSE];
    unsigned int buffer_length;
    int answer = 0;  /* invalid answer by default */
    int ii;
    int chksum;
    int calc_chksum;
    int passed_number;
    char * vp;  /* value pointer */

    while ( 0 < p_retries ) {
        buffer_length = MAX_2U_RESPONSE;
        readLineFrom2U (&(buffer[0]), &buffer_length);
        if ( validateLineFrom2U (&(buffer[0]), buffer_length) ) {
#if 0
            if ( (E_READ_FROM_2U_DO_ADJUST == p_do_adjust_flag) &&
                 (MAX_2U_RESPONSE > buffer_length) ) {
                adjustLineFrom2U (&(buffer[0]), &buffer_length);
            }
#endif
            /* check that this is an answer to the p_command */
            /* I'm not bothering to check the backwards command */
            if ( ! strncmp
                 (&(buffer[0]), p_command, strlen (p_command) ) ) {
                /* dig out the integer */
                ii = 0;
                while ( p_command[ii] == buffer[ii] ) {
                    ++ii;
                }
                while ( (buffer_length > ii) && isspace (buffer[ii]) ) {
                    ++ii; /* skip space(s) between command and value */
                }
                vp = &(buffer[ii]);
                passed_number = strtol (&(buffer[ii]), (char **) 0, 0);
                while ( (buffer_length > ii) &&
                        (! isspace (buffer[ii])) &&
                        (ASCII_NULL != buffer[ii]) ) {
                    ++ii; /* skip number */
                }
                while ( (buffer_length > ii) && isspace (buffer[ii]) ) {
                    ++ii; /* skip space(s) between number and checksum */
                }
                if ( (buffer_length > ii) &&
                     (E_READ_FROM_2U_DO_CHECKSUM == p_with_checksum_flag) ) {
                    chksum = strtol (&(buffer[ii]), (char **) 0, 0);
                    /* calculate checksum on value */
                    calc_chksum = do_checksum (vp, buffer_length - ii);
                }

                /* there should still be chars left in the buffer -
                   if not, this probably is a bad line */
                if ( (buffer_length > ii) &&
                     ((E_READ_FROM_2U_DO_CHECKSUM != p_with_checksum_flag) ||
                      ((E_READ_FROM_2U_DO_CHECKSUM == p_with_checksum_flag) &&
                       (calc_chksum == chksum))) ) {
                    answer = 1;
                    p_retries = 1;
                }
            }
        }

        --p_retries;
    }

    if ( answer ) {
        *p_the_number = passed_number;
    }

    return answer;
  
}



/*
 * Note, there are no re-requests to the Atmel in this function,
 *       so retries should be a very small number
 * Args:
 *   in:
 *      p_command:  ptr to NULL-terminated buffer containing the command we
 *                    want to match
 *      p_do_adjust_flag:  1 if we want to put an X on the end
 *      p_with_checksum_flag:  1 if we want to verify the checksum
 *      p_retries:     the number of read retries to do before giving up
 *
 *   out:
 *      p_the_number:  the value that the other side sent (if success)
 *                     undefined (if failed)
 * 
 * Return:
 *   true(1):  got value
 *   false(0):  didn't get value
 */
int
getStringAnswerFromAtmel (char * p_command,
                          readFrom2U_adjust_e p_do_adjust_flag,
                          readFrom2U_checksum_e p_with_checksum_flag,
                          int p_retries,
                          char * p_the_string,
                          unsigned long * p_string_length) {
    char buffer [MAX_2U_RESPONSE];
    unsigned int buffer_length;
    int answer = 0;  /* invalid answer by default */
    int ii;
    int chksum;
    int calc_chksum;
    char * vp;  /* value pointer */
    int jj;

    while ( 0 < p_retries ) {
        buffer_length = MAX_2U_RESPONSE;
        readLineFrom2U (&(buffer[0]), &buffer_length);
        if ( validateLineFrom2U (&(buffer[0]), buffer_length) ) {
#if 0
            if ( (E_READ_FROM_2U_DO_ADJUST == p_do_adjust_flag) &&
                 (MAX_2U_RESPONSE > buffer_length) ) {
                adjustLineFrom2U (&(buffer[0]), &buffer_length);
            }
#endif
            /* check that this is an answer to the p_command */
            /* I'm not bothering to check the backwards command */
            if ( ! strncmp
                 (&(buffer[0]), p_command, strlen (p_command) ) ) {
                /* dig out the string */
                ii = 0;
                while ( p_command[ii] == buffer[ii] ) {
                    ++ii;
                }
                while ( (buffer_length > ii) && isspace (buffer[ii]) ) {
                    ++ii; /* skip space(s) between command and value */
                }
                vp = &(buffer[ii]);
                while ( (buffer_length > ii) &&
                        ! isspace (buffer[ii]) &&
                        (ASCII_NULL != buffer[ii]) ) {
                    ++ii; /* skip string */
                }
                while ( (buffer_length > ii) && isspace (buffer[ii]) ) {
                    ++ii; /* skip space(s) between string and checksum */
                }

                if ( (buffer_length > ii) &&
                     (E_READ_FROM_2U_DO_CHECKSUM == p_with_checksum_flag) ) {
                    chksum = strtol (&(buffer[ii]), (char **) 0, 0);
                    /* calculate checksum on value */
                    calc_chksum = do_checksum (vp, buffer_length - ii);
                }

                /* there should still be chars left in the buffer -
                   if not, this probably is a bad line */
                if ( (buffer_length > ii) &&
                     ((E_READ_FROM_2U_DO_CHECKSUM != p_with_checksum_flag) ||
                      ((E_READ_FROM_2U_DO_CHECKSUM == p_with_checksum_flag) &&
                       (calc_chksum == chksum))) ) {
                    /* copy the string to the answer buffer */
                    jj = 0;
                    while ( (*p_string_length > jj) &&
                            ! (isspace(*vp)) &&
                            (ASCII_NULL != *vp) ) {
                        p_the_string[jj] = *vp;
                        ++vp;
                        ++jj;
                    }
                    if ( *p_string_length > jj) {
                        p_the_string[jj] = ASCII_NULL;
                        ++jj;
                        *p_string_length = jj;
                        answer = 1;
                        p_retries = 1;
                    }
#if 0
                    else {
                        /* probably should throw it away,
                           but let's try truncating for now */
                        --jj;
                        *vp = ASCII_NULL;
                        ++jj;
                        *p_string_length = jj;
                        answer = 1;
                        p_retries = 1;
                    }
#endif
                }
                break;
            }
        }

        --p_retries;
    }

    return answer;
}




/*
 * Note, there are no re-requests to the Atmel in this function,
 *       so retries should be a very small number
 * Args:
 *   in:
 *      p_do_adjust_flag:  1 if we want to put an X on the end
 *      p_with_checksum_flag:  1 if we want to verify the checksum
 *      p_retries:     the number of read retries to do before giving up
 *
 *   out:
 *      p_ipAddress:  the value that the other side sent (if success)
 *                    undefined (if failed)
 * 
 * Return:
 *   true(1):   got value
 *   false(0):  didn't get value
 */
int
getIPAddressAnswerFromAtmel (readFrom2U_adjust_e p_do_adjust_flag,
                             readFrom2U_checksum_e p_with_checksum_flag,
                             int p_retries,
                             unsigned long * p_ipAddress) {
    char buffer [MAX_2U_RESPONSE];
    unsigned int buffer_length;
    int answer = 0;  /* invalid answer by default */
    int octet;
    unsigned long uctet;
    char * vp;  /* value pointer */
    char * beginvp;
    char * endvp;

    while ( 0 < p_retries ) {
        buffer_length = MAX_2U_RESPONSE;
        readLineFrom2U (&(buffer[0]), &buffer_length);
        if ( validateLineFrom2U (&(buffer[0]), buffer_length) ) {
            if ( (E_READ_FROM_2U_DO_ADJUST == p_do_adjust_flag) &&
                 (MAX_2U_RESPONSE > buffer_length) ) {
                adjustLineFrom2U (&(buffer[0]), &buffer_length);
            }

            if ( looks_like_ip_address (&(buffer[0])) ) {
                beginvp = vp = &(buffer[0]);
                /* we already know it looks like an ip address */
                /* first (most significant) octet */
                while ( *vp != ASCII_PERIOD ) { ++vp; }
                *vp = ASCII_NULL;
                octet = strtol (beginvp, &endvp, 0);
                uctet = (unsigned long) octet;
                vp = endvp;
                beginvp = ++vp;
                *p_ipAddress = 0xFF000000 & (uctet << 24);

                /* second octet */
                while ( *vp != ASCII_PERIOD ) { ++vp; }
                *vp = ASCII_NULL;
                octet = strtol (beginvp, &endvp, 0);
                uctet = (unsigned long) octet;
                vp = endvp;
                beginvp = ++vp;
                *p_ipAddress |= 0x00FF0000 & (uctet << 16);

                /* third octet */
                while ( *vp != ASCII_PERIOD ) { ++vp; }
                *vp = ASCII_NULL;
                octet = strtol (beginvp, &endvp, 0);
                uctet = (unsigned long) octet;
                vp = endvp;
                beginvp = ++vp;
                *p_ipAddress |= 0x0000FF00 & (uctet << 8);

                /* fourth (least significant) octet */
                octet = strtol (beginvp, &endvp, 0);
                uctet = (unsigned long) octet;
                *p_ipAddress |= 0x000000FF & uctet;

                answer = 1;
                p_retries = 1;
                break;
            }
        }

        --p_retries;
    }

    return answer;
}
