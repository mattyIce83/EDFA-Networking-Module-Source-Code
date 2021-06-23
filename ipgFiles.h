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
#ifndef _IPGFILES_H
#define _IPGFILES_H

// Enable printf
#define DEBUG_PRINTF   0

#define FS_VOLUME1_SIZE   		(3000 * 1024)
#define FS_VOLUME1 				"RAM0"
#define FS_VOLUME1_DIR   		FS_VOLUME1 "/"
#define FS_VOLUME1_SUB_DIR0   		FS_VOLUME1 "/DIR0"
#define FS_VOLUME1_SUB_DIR0_1   	FS_VOLUME1_SUB_DIR0 "/DIR00"
#define FS_VOLUME1_SUB_DIR1   		FS_VOLUME1 "/DIR1"

#define FS_VOLUME2_SIZE   		(2000 * 1024)
#define FS_VOLUME2 				"FLASH0"
#define FS_VOLUME2_DIR			FS_VOLUME2 "/"
#define FS_VOLUME2_SUB_DIR0   		FS_VOLUME2 "/DIR0"
#define FS_VOLUME2_SUB_DIR0_1   	FS_VOLUME2_SUB_DIR0 "/DIR00"
#define FS_VOLUME2_SUB_DIR1   		FS_VOLUME2 "/DIR1"


// these bit offsets are used to calculate access bit (in sysAccess.h) 
// from a group-id (group_id << READ_BIT_OFFSET)

#define READ_BIT_OFFSET    		2 
#define WRITE_BIT_OFFSET   		16

#define FILE_TYPE_FILE          	1    
#define FILE_TYPE_DIR         		2    
#define IS_FILE_TYPE(a)        		((a) == FILE_TYPE_FILE)   
#define IS_DIRECTORY_TYPE(a)   		((a) == FILE_TYPE_DIR)   


unsigned FileSystem_GetNumberOfDirUser (void);
unsigned FileSystem_GetDirUserList (char *name_table, unsigned table_size);
unsigned FileSystem_GetNumberOfVolumes (void);
unsigned FileSystem_GetVolumeList (char *volume_table, unsigned table_size);
int FileSystem_CreateRamVolume (char *volume_name, char *volume_area, unsigned volume_size);
int FileSystem_CreateDirectory (char *dir_path, unsigned int group_id, 
							char *username, char *password);
int FileSystem_CreateTestFile (char *dir_path, unsigned int group_id, 
                              char **test_file_name, unsigned long *file_size);

#endif 								//  _IPGFILES_H
