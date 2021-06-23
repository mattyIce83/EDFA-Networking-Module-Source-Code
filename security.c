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
 *     Module Name: security.c
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
 * Contains the security realms for this applications.
 *                                                                        
 *                                                                        
 * Edit Date/Ver   Edit Description
 * ==============  ===================================================
 * MJC  04/09/15   added http_security.h
 * MJC
 *
 */


#include "AsExtern.h"
#include <http_awsapi.h>
#include <http_security.h>


/*
    The RpHSInitSecurityTable routine is called once at start up to
	allow setting of security databases. The serverdata is passed and
	can be used to perform other initialization as needed.
*/
void RpHSInitSecurityTable(void *serverdata)
{
    /*  Set realm 1 to Multiple Access with MD5 Digest Authentication */
    NAHttpSetRealmSecurity (0, "IPG AWS Realm 1", NA_HTTP_SECURITY_MULTIPLE_ACCESS, NA_HTTP_SECURITY_DIGEST_AUTHENTICATION);
    /*  Set realm 2 to Single Access with MD5 Digest Authentication */
    NAHttpSetRealmSecurity (1, "IPG AWS Realm 2", NA_HTTP_SECURITY_SINGLE_ACCESS, NA_HTTP_SECURITY_DIGEST_AUTHENTICATION);
}


