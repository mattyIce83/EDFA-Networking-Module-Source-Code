/*
 *  Copyright (c) 1996-2007 Digi International Inc., All Rights Reserved
 *
 *  This software contains proprietary and confidential information of Digi
 *  International Inc.  By accepting transfer of this copy, Recipient agrees
 *  to retain this software in confidence, to prevent disclosure to others,
 *  and to make no use of this software other than that for which it was
 *  delivered.  This is an unpublished copyrighted work of Digi International
 *  Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 *  prohibited.
 *
 *  Restricted Rights Legend
 *
 *  Use, duplication, or disclosure by the Government is subject to
 *  restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 *  Technical Data and Computer Software clause at DFARS 252.227-7031 or
 *  subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 *  Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 *  Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 *
 */

#ifndef _FILE_H
#define _FILE_H

#ifdef APP_FILE_SYSTEM

#ifdef __cplusplus
extern "C"
{
#endif

#include "asextern.h"

RpErrorCode RpHSSetCurrentDirectory(int theFileNumber, char *dir_name);
RpErrorCode RpHSGetErrorStatus(int theFileNumber, int *errorStatus);
void RpHSSetDefaultGroupId(unsigned group_id);
unsigned RpHSGetDefaultGroupId(void);
#endif /* APP_FILE_SYSTEM */

#ifdef __cplusplus
}
#endif

#endif /* _FILE_H */
