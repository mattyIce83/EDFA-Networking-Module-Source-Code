/*
 *            Copyright (c) 1998-2000 by NETsilicon Inc.
 *
 *  This software is copyrighted by and is the sole property of 
 *  NETsilicon.  All rights, title, ownership, or other interests
 *  in the software remain the property of NETsilicon.  This
 *  software may only be used in accordance with the corresponding
 *  license agreement.  Any unauthorized use, duplication, transmission,  
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice may not be removed or modified without prior
 *  written consent of NETsilicon.
 *
 *  NETsilicon, reserves the right to modify this software
 *  without notice.
 *
 *  NETsilicon
 *  411 Waverley Oaks Road                  USA 781.647.1234
 *  Suite 227                               http://www.netsilicon.com
 *  Waltham, MA 02452                       AmericaSales@netsilicon.com
 *
 *************************************************************************
 *
 *     Module Name: cgi.c
 *         Version: 1.00
 *   Original Date: 27-Jul-2000 
 *          Author: 
 *        Language: Ansi C
 * Compile Options: 
 * Compile defines: 
 *       Libraries: 
 *    Link Options: 
 *
 *    Entry Points: 
 *
 * Description.
 * =======================================================================
 * Contains the function stubs for CGI functionality.
 *                                                                        
 *                                                                        
 * Edit Date/Ver   Edit Description
 * ==============  ===================================================
 * SW   09/19/01   RpExtern.h --> AsExtern.h; void * to RpUserExitInit()
 * SW   10/01/01   Added include rpcgi.h
 * PMJ  10/04/01   Updated the prototype.
 *
 */

#include "AsExtern.h"
#include "RpCgi.h"

int RpUserExitInit(void *theTaskDataPtr) {

    return 0;
}

int RpUserExitDeInit() {

    return 0;
}

void RpExternalCgi(void *theDataPtr, rpCgiPtr theCgiPtr) {

    return;
}


