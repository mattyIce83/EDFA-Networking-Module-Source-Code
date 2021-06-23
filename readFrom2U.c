///////////////////////////////////////////////////////////////////////////////
// $Id: readFrom2U.c,v 1.1.1.1 2006-07-26 17:51:48 eugene Exp $
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
#include "MAGIC.h"
#include "ascii.h"

#define MAX_2U_RESPONSE		  40 //was 31
// #define MAX_BUF			 200

extern char pl_buffer[MAX_2U_RESPONSE];
// extern char alrmTrace[MAX_BUF];
// unsigned long int ktotal = 0;	// read2U

static void
readFrom2U_breakpoint (void) {
  int foo;
  int bar;

  foo = pl_buffer[0];
  bar = foo;
}


void readFrom2U(int * p_count)
{

  // int k = 0;
    int EOS;
    int i;
    int j;
    int SEEN_VALID;
    int rc_f;

    ++(*p_count);

    EOS = 0;
    SEEN_VALID = 0;
    j = 0;
    for (i = 0; i < MAX_2U_RESPONSE; ++i)
	pl_buffer[i] = ASCII_NULL;
    for (i = 0; i < MAX_2U_RESPONSE; ++i) {
	pl_buffer[j++] = getchar();	// read the response from the 2U
	// alrmTrace[k++] = pl_buffer[j - 1];
	// ktotal = k - 1;
	// if (k == MAX_BUF) {
	//     k = 0;
	//     ktotal = k;
	// }

	if (!(isascii(pl_buffer[j - 1]))) {
	    putchar(pl_buffer[j - 1]); /* GORD why-oh-why? */
	    return;
	}

	if (pl_buffer[j - 1] == ASCII_CR) {	// RETURN character?
	    if ((EOS == 0) && (!SEEN_VALID)) {
                for (j = 0; j < MAX_2U_RESPONSE; ++j)   // initialize the buffer[]
                  pl_buffer[j] = ASCII_NULL;  // and start over
		i = 0;  /* bjb - set loop ctl variable back to 0 ?!? */
		j = 0;
	    } else {
                pl_buffer[j - 1] = ASCII_X;  //overwrite  the \r with X
		pl_buffer[j++] = ASCII_NULL;  // follow with \r
		break;         // and we're finished
	    }
	    EOS++;
	}

	if (!(isascii(pl_buffer[j - 1]))) {
	    for (j = 0; j < MAX_2U_RESPONSE; ++j)	// initialize the buffer[]
		pl_buffer[j] = ASCII_NULL;	// and start over

	    break;
	}

	if (isdigit(pl_buffer[j - 1]))	// if the response is numeric, that's
	    SEEN_VALID = 1;	// good

	if (j == 2) {
            pl_buffer[j] = ASCII_NULL;	// required for the strcmp
	    if (!(strncmp(pl_buffer, "ZR", j)))	// reply from a ZIMR(ead) operative
		SEEN_VALID = 1;
	}
    }

    if ( 0 != (rc_f = fflush(stdin)) ) {
      readFrom2U_breakpoint ();
      /* Is errno equal to EBADF, perchance? */
      /* What does a fflush on an input stream do, anyway? */
      /* It throws away the remaining info that has been read and buffered, it seems */
      /* but, isn't this a serial interface, one character at a time??? */
    }

}

