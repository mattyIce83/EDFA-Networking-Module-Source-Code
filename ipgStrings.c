///////////////////////////////////////////////////////////////////////////////
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
//
//  Filename:           ipgStrings.c
//
//  Description:
//     String/parsing functions necessary for SNMP operations.
//
//  Author:             Victor Semlek
//
//  Target Processor:   ARM NS7520 55Mhz
//
//  Target Compiler:    gcc
//
//  Revision:           1.00
//
//  Revision History:
//
//      $Log: ipgStrings.c,v $
//      Revision 1.1.1.1  2006-05-04 14:38:39  eugene
//      Commit of the latest Digi source code retrieved from Fidus (ver 1.140).
//
//      Revision 1.2  2006-05-04 14:38:39  gdey
//      capture what should be post 115 and pre 116
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <tx_api.h>
#include "ipgStrings.h"
#include "appconf.h"

#include "STACK_SIZE.h"
#define MAX_TIMEOUT 500		// original value was 100

TX_THREAD get_char_ctlblk;

static int inThread = 0;


// Local prototypes
static void ipgGetChar(unsigned long buf);


// Local functions
static void ipgGetChar(unsigned long buf)
{
    *(unsigned long *) buf = getchar();
    inThread = 0;
    return;
}


// Shared functions
/*
 *
 *  Function: void ipgReadLine(char * buf)
 *
 *  Description:
 *
 *      This routine gets one line of the information input from keyboard.
 *
 *  Parameters:
 *      buf     points to an array to hold the information
 *
 *  Return Values:
 *
 *      none
 *
 */
void ipgReadLine(char *buf)
{
    char retChar;
    char temp = '\0';

    int i = 0;
    int timeout = 0;
    int rc;

    void *stack;


    stack = (void *) malloc(STACK_SIZE);
    if (stack == NULL) {
	netosFatalError("Unable to allocate thread stack.", 5, 5);
    }

    inThread = 1;		// Set inThread as we are about to enter get_char_ctlblk thread
    do {
	// At this time, only set timer on first character read because of 
	// performance hit.  It should be sufficient.  Remove "if(inThread)
	// and set inThread=1 in every loop to be more robust.   
	if (inThread == 1) {
	    rc = tx_thread_create(&get_char_ctlblk,	// control block for thread
				  "getchar Thread",	// thread name
				  ipgGetChar,	// entry function
				  (unsigned long) &retChar,	// parameter
				  stack,	// start of stack
				  STACK_SIZE,	// size of stack
				  3,	// naDefaultPriority a.k.a.
				  3,	// preemption threshold APP_DEFAULT_API_PRIORITY
				  1,	// time slice threshold
				  TX_AUTO_START);	// start immediately

	    if (rc != TX_SUCCESS) {
		// Thread already exists.
		//netosFatalError ("Unable to create thread.", 5, 5);
	    }
	    // Wait for character.  If timeout is reached then there is no character to retreive.

	    timeout = 0;
	    while ((inThread) && (timeout < MAX_TIMEOUT)) {
		tx_thread_sleep(1);
		timeout++;
	    }

	    rc = tx_thread_delete(&get_char_ctlblk);


	    if (timeout == MAX_TIMEOUT)	// return if we have timed out.  We are done here.
	    {
		buf[0] = '\0';
		return;
	    }
	} else {
	    retChar = getchar();
	}

	if (retChar == '\b') {
	    if (i > 0) {
		putchar(retChar);
		putchar(0x20);	/*space */
		putchar(retChar);
		--i;
	    }
	}

	else if (retChar != '\n') {
	    buf[i] = retChar;
	    putchar(retChar);
	    ++i;
	}
    }
    while ((retChar != '\r') && (i < MAX_FIELD_LEN));


    if (i == MAX_FIELD_LEN) {
	buf[0] = '\0';
    } else {
	buf[i - 1] = '\0';
    }
    while (getchar() == '\n');	//Clear input buffer

    return;
}


/*
 *
 *  Function: int ipgStringToDecimal(char * buf)
 *
 *  Description:
 *
 *      This routine converts strings into integers.
 *
 *  Parameters:
 *      buf     points to an array to hold the information
 *
 *  Return Values:
 *
 *      int     decimal representation of converted string
 *
 */
int ipgStringToDecimal(char *buf)
{
    unsigned long number = 0;

    number = strtoul(buf, NULL, 10);

    return (int) number;
}
