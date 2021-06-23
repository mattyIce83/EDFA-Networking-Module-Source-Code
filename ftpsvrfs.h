/*
 *            Copyright (c) 1998-2003 by NETsilicon Inc.
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
 *  Suite 304                               http://www.netsilicon.com
 *  Waltham, MA 02452                       AmericaSalenetsilicon.com
 *
 *************************************************************************/


#ifndef _FTPSVR_FTPFILE_H
#define _FTPSVR_FTPFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

int FSInstallFileSystem(char * root_directory);
int FS_StoreFile (char *bufferp, int buflen, char *argp, unsigned long handle);
int FS_RetrieveFile (char * argp, unsigned long handle);
int FS_DeleteFile (char * argp, unsigned long handle);
int FS_RenameFrom(char * argp, unsigned long handle);
int FS_Pwd(char * argp, unsigned long handle, char * buffer);
int FS_RootDirectory(char * argp, unsigned long handle);
int FS_ChangeDirectory(char * argp, unsigned long handle);
int FS_ListDir(char * argp, unsigned long handle);
int FS_nListDir(char * argp, unsigned long handle);
int FS_MakeDir (char * argp, unsigned long handle);
int FS_RemoveDir (char * argp, unsigned long handle);
int FS_SessionClosed(char * argp, unsigned long handle);
int FS_DataSessionClosed(char * argp, unsigned long handle);

#ifdef __cplusplus
}
#endif
#endif /* _FTPSVR_FTPFILE_H */


