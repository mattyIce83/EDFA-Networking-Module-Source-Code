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
 *  Waltham, MA 02452                       AmericaSales@netsilicon.com
 *
 *************************************************************************/
/*
 * Edit History
 * 09/19/03  JZW     Fixed some autodoc problems
 */

/**
 * This contains interface between HTTP server and the file system. It is 
 * used to retrieve and store file from web pages and mail service.
 *
 * HTTP server only supports file upload with ENCTYPE="multipart/form-data". 
 * The HTML file upload input entry must be defined after all HTML input entries 
 * that require to be serviced before writing a file.
 * 
 *
 * User is only required to start the HTTP server and HTTP server will initialize
 * this intereface with the file system. These APIs are not for external use.
 * The file system must be initialized before HTTP server starts.
 *
 * If this file is compiled with APP_FILE_SYSTEM defined, it will include the 
 * NETOS file system. Otherwise, user can modify this interface to support their 
 * file system. 
 * Once APP_FILE_SYSTEM is defined, HTTP server and NETOS file system libraries
 * are required. 
 * 
 * The following definitions are used when HTTP server creates or uploads a file:
 *      <list type="unordered">
 *          <item> _HTTP_POST_FUNCTION_ENABLE - Enabled by default to prevent
 *                 HTTP server to send its own error response when error occurs
 *                 during uploading a file.</item>
 *          <item> _RESTRICTED_PERMISSION - Enabled by default to restricte user
 *                  permissions to access the file system.</item>
 *          <item> _RESTRICTED_FILE_CREATION - Disabled by default that allows
 *                  a file can be rewritten. </item>
 *      </list>
 *
 *
 * @external
 * @overview HTTP Advanced Server & File System Interface
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 * @see @link _HTTP_POST_FUNCTION_ENABLE
 * @see @link _RESTRICTED_PERMISSION
 * @see @link _RESTRICTED_FILE_CREATION
 * @see @link HTTP_Advanced_Server
 */
#include "AsExtern.h"

#ifdef APP_FILE_SYSTEM
#include <stdio.h>
#include <string.h>
#include <tx_api.h>
#include <npttypes.h>
#include <sysaccess.h>
#include <fs_api.h>
#include <rss.h>
#include <fss.h>

/**
 * This _HTTP_POST_FUNCTION_ENABLE definition allows HTTP server to call a post processing 
 * function even error occurs during file upload.
 * If this definition is disabled, HTTP server will send its own error response.
 * This is only applied to file upload.
 *
 * @external
 * @since 6.0
 * @category HTTP_Advanced_Server:File_System_Interface
 *
 */
#define _HTTP_POST_FUNCTION_ENABLE	1

/**
 * This _RESTRICTED_PERMISSION definition only allows users to create & open 
 * a file with the right permissions. If a directory is created with NAFS_FS_GROUP1, 
 * user must have permission for NAFS_FS_GROUP1 to access the directory.
 *
 * @external
 * @since 6.0
 * @category HTTP_Advanced_Server:File_System_Interface
 *
 */
#define _RESTRICTED_PERMISSION     1

/**
 * This _RESTRICTED_FILE_CREATION definition only allows users to create a 
 * non-existing file. A file can only be created if the file is not existed.
 * If this definition is 0, data will over write into a file if a file is 
 * already existed.
 *
 * @external
 * @since 6.0
 * @category HTTP_Advanced_Server:File_System_Interface
 *
 */
#define _RESTRICTED_FILE_CREATION   0

/* enable printf for debugging */
#define _PRINTF_DEBUG   0

#if (_PRINTF_DEBUG > 0)
/* print out the processing state */
#define _PROCESS_NAME(a)    Print_ProcessName((a))
#else
#define _PROCESS_NAME(a)    
#endif


/**
 * Define default group-id 
 *
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 */
#define DEFAULT_GROUP_ID        NAFS_FS_GROUP1	


/**
 * Define default file extension
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 */
#define DEFAULT_FILE_EXTENSION  ".*"

/**
 * Http server uses this "FS" prefix for file system access
 *
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 * @external
 *
 * @note Do not change this prefix. HTTP server accesses the file system
 * when the request path contains this prefix.
 */
#define FILE_SYSTEM_PREFIX      "FS"  


#define _WRITE_BIT_OFFSET   16

#define IS_FILE_SYSTEM_BUSY(a)  (((int)(a) == NAFS_IO_REQUEST_CB_BUSY) || \
								((int)(a) == NAFS_FILE_CB_BUSY) || \
								((int)(a) == NAFS_DIR_PATH_CB_BUSY))
 
/**
 * File system process status
 *
 * It's used to indicate the status of the io request process on the NETOS file system.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param eFileSystemStatus_Wait        Waiting for the io request process
 * @param eFileSystemStatus_Complete    Complete the io request process
 * @param eFileSystemStatus_Error       Error occurs on the io request process
 *
 */
typedef enum {
    eFileSystemStatus_Wait,
    eFileSystemStatus_Complete,
    eFileSystemStatus_Error
} eFileSystemStatus;

/**
 * File system process actions
 *
 * It's used to indicate what process is performing on the NETOS file system.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param eFileSystemProcess_None           No process is performing
 * @param eFileSystemProcess_OpenFile       Open a file 
 * @param eFileSystemProcess_GetDir         Get a directory 
 * @param eFileSystemProcess_EntryCount     Get entry count on a directory
 * @param eFileSystemProcess_ListDir        List a directory
 * @param eFileSystemProcess_ReadFile       Read a file
 * @param eFileSystemProcess_WriteFile      Write a file
 * @param eFileSystemProcess_CreateFile     Create a file
 * @param eFileSystemProcess_CloseFile      Close a file
 * @param eFileSystemProcess_Max            Not used
 *
 */
typedef enum {
    eFileSystemProcess_None,
    eFileSystemProcess_OpenFile,
    eFileSystemProcess_GetDir,
    eFileSystemProcess_EntryCount,
    eFileSystemProcess_ListDir,
    eFileSystemProcess_ReadFile,
    eFileSystemProcess_WriteFile,
    eFileSystemProcess_CreateFile,
    eFileSystemProcess_CloseFile,
    eFileSystemProcess_Max
} eFileSystemProcess;
    
/**
 * Structure for HTTP request for the NETOS file system access
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param dir_path           Buffer that contains directory name
 * @param file_name          Buffer that contains file name
 * @param parsing_buffer     Buffer for parsing the request directory or file name
 * @param io_request         I/O request for NETOS file system 
 * @param dir_info_list      Pointer to directory list. 
 *                           This will be dynamically allocated and free.
 * @param file_info          File information
 * @param dir_info_list_items Number of items in dire_info_list   
 * @param file_handler       File handler for accessing a file 
 * @param file_offset        File offset
 * @param permissions        User permissions
 * @param error_status       NETOS file system error status
 * @param state              NETOS file system process action
 * @param process_function_cb Pointer to function for the file system process action
 *
 */
typedef struct {
    char                dir_path[NAFS_DIRECTORY_PATH_SIZE];
    char                file_name[NAFS_FILE_NAME_SIZE];
    char                parsing_buffer[NAFS_FILE_NAME_SIZE];

    NAFS_IO_REQUEST_CB  io_request;
    NAFS_DIR_ENTRY_INFO * dir_info_list;
    FILEINFO            file_info;
    unsigned int        dir_info_list_items;
    unsigned int        file_handler;
    unsigned int        file_offset;
    unsigned int        permissions;
    int                 error_status;    
    eFileSystemProcess  state;
	void 				* process_function_cb;
} RPFS_BUFFER_T;    

/**
 * Structure for defining data type of an extension type
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param ext_type  Data type
 * @param ext_name  Pointer to extension name
 *
 */
typedef struct rpfs_ext_t {
    rpDataType          ext_type;
    char                * ext_name;
}RPFS_EXT_T;

/**
 * This gSupportedExt is a global variable which contains a list of
 * extension name and its data type.
 *
 * This list can be extended.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 */
static RPFS_EXT_T gSupportedExt[] = {
    {eRpDataTypeNone, DEFAULT_FILE_EXTENSION},
    {eRpDataTypeHtml,       ".html"},   
    {eRpDataTypeHtml,       ".htm"},    
    {eRpDataTypeImageGif,   ".gif"},    
    {eRpDataTypeText,       ".txt"},    
    {eRpDataTypeImageJpeg,  ".jpg"},    
    {eRpDataTypeImageJpeg,  ".jpeg"},   
    {eRpDataTypeXml,        ".xml"},    
    {eRpDataTypeWav,        ".wav"}    
};    
    
typedef void (* LIST_DIR_FUNC_T) (RPFS_BUFFER_T * pFileReq, char ** dataName, unsigned char * dataType);
typedef void (* READ_FILE_FUNC_T) (RPFS_BUFFER_T * pFileReq, char ** bufferPtr, unsigned int * length);
typedef void (* WRITE_FILE_FUNC_T) (RPFS_BUFFER_T * pFileReq, char ** bufferPtr, unsigned int * length);

static RPFS_BUFFER_T   * gRpFSBuffer = NULL;     /* list of available file request buffers */
static unsigned int     gRpFSMaxBuffers = 0;    /* max number of file request buffers */
static unsigned			gRpFSDefaultGroupId = DEFAULT_GROUP_ID; /* default group id */

#if (_PRINTF_DEBUG > 0)
/**
 * Returns the http file system process string
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    processNum   File system process enum
 *                  
 *          <list type="unordered">
 *          <item> eFileSystemProcess_None</item>
 *          <item> eFileSystemProcess_OpenFile</item>
 *          <item> eFileSystemProcess_GetDir</item>
 *          <item> eFileSystemProcess_EntryCount</item>
 *          <item> eFileSystemProcess_ListDir</item>
 *          <item> eFileSystemProcess_ReadFile</item>
 *          <item> eFileSystemProcess_WriteFile</item>
 *          <item> eFileSystemProcess_CreateFile</item>
 *          <item> eFileSystemProcess_CloseFile</item>
 *          </list>
 *
 *                        
 * @return  "process string"   string of file system process state
 *
 * @see @link eFileSystemProcess
 * 
 */
static char const * Print_ProcessName(eFileSystemProcess processNum)
{
    static char const * processName[eFileSystemProcess_Max] = 
    {
        "eFileSystemProcess_None",
        "eFileSystemProcess_OpenFile",
        "eFileSystemProcess_GetDir",
        "eFileSystemProcess_EntryCount",
        "eFileSystemProcess_ListDir",
        "eFileSystemProcess_ReadFile",
        "eFileSystemProcess_WriteFile",
        "eFileSystemProcess_CreateFile",
        "eFileSystemProcess_CloseFile"
    };

    NA_ASSERT(processNum < eFileSystemProcess_Max);

    return processName[processNum];
}
#endif

/**
 * Returns file mime type 
 *
 * This function is used to setup the mime type for HTTP server.
 * It looks at the extension in the given filename and returns the mime type for the
 * appropriate extension. A list of extension is defined in gSupportedExt.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    filename      Pointer to file name. Must include an extension. "name.ext" 
 * @param    dataType      Pointer to mime type which will be set according to
 *                         the extension of the filename.
 *                        
 * @return eRpFileNotFound   File System is not initialized for HTTP. Must complete
 *                           RpFS_OpenFileSystem call.
 * @return eRpNoError        Successfully retrieve the mime type for the given filename.
 *
 * @see @link RpErrorCode
 * @see @link rpDataType
 *
 * @note You can add more data type for an extension. gSupportedExt table contains
 *      a list of data types.
 */
static RpErrorCode Get_FileMimeType(char const * filename, rpDataType * dataType)
{
    RpErrorCode theResult = eRpFileNotFound;
    char        * ext;
    int         i;
    
    /* get the externsion for the given file */
    ext = strstr(filename, ".");
    if (ext != NULL)
    {
        for (i=0; i < asizeof(gSupportedExt); i++)
        {
            if (strcmp(ext, gSupportedExt[i].ext_name) == 0)
            {
                *dataType = gSupportedExt[i].ext_type;
                theResult = eRpNoError;
                break;
            }
        }
    }
    
    return theResult;
}
/**
 * Returns the status of file system io request
 *
 * A file system process function must be called prior this function.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    io_request  File system io request for which a previous file system API
 *                      is associated with.
 *        
 * @param    byteTransferred     Pointer to number of bytes that has been transferred.
 *
 * @param    errorStatus         Pointer to memory where file system error status 
 *                               will be returned
 *
 *                        
 * @return eFileSystemStatus_Error   File System is not initialized for HTTP. 
 * @return eFileSystemStatus_Wait    The io request is still pending. 
 * @return eFileSystemStatus_Complete  The io_request is completed.
 * @return eFileSystemStatus_Error     The io_request encounters some error.
 *
 * @see @link NAFS_IO_REQUEST_CB
 * @see @link file_sys_errors
 *
 */
static eFileSystemStatus Get_IORequestStatus(NAFS_IO_REQUEST_CB * io_request, unsigned int * byteTransferred, int * errorStatus)
{
    eFileSystemStatus   ret_status = eFileSystemStatus_Error;
    unsigned int        requestType, status;
    int                 rc;
    
    /* inquire file system io request */    
    rc = NAFSio_request_cb_status(io_request, &requestType, &status, errorStatus, 
                                 byteTransferred); 
    if (rc == NAFS_SUCCESS)
    {
        if (status == NAFS_IO_REQUEST_PENDING || status == NAFS_IO_REQUEST_PROCESSING)
            ret_status = eFileSystemStatus_Wait;
        else if (status == NAFS_IO_REQUEST_COMPLETED ||
		*errorStatus == NAFS_END_OF_FILE)
            ret_status = eFileSystemStatus_Complete;

    }
    
    if (*errorStatus != NAFS_SUCCESS)
	{
		if (ret_status == eFileSystemStatus_Wait)
			*errorStatus = NAFS_SUCCESS;
#if (_PRINTF_DEBUG > 0)
		else
	        printf("io_request status %d error %d\n", status, *errorStatus);    
#endif        
	}
    return ret_status;
}

/**
 * Returns Http file system structure buffer 
 *
 * This structure buffer is used to handle file transfer.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    theFileNumber  the file id
 *
 *                        
 * @return  NULL            The file index is not present
 *          RPFS_BUFFER_T   Pointer to http file system buffer for the specified file id
 *
 */
static RPFS_BUFFER_T * Get_FileSystemBuffer(int theFileNumber)
{
    RPFS_BUFFER_T * pFileReq = NULL;
    
    if (theFileNumber < gRpFSMaxBuffers)
        pFileReq = &gRpFSBuffer[theFileNumber];
        
    return pFileReq;
}

/**
 *  It parses the fullNamePath for the directory path and the file
 *  name. The fullNamePath must contains / (slash) to separate
 *  each directory and filename. If no slash is specified in the
 *  fullNamePath, only filename (no directory path) is specified.
 *  RpFS_Setcurrentdirectory() must be used prior this call to 
 *  set the directory path which a file will be created or
 *  opened from. 
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    fullNamePath       Pointer to a full file name path which will be
 *                              parsed the directory path and file name.
 *
 * @param    dirName            Pointer to the directory path of a file 
 *                              given in fullNamePath.
 * @param    dirLen             Size of the dirName
 *
 * @param    fileName           Pointer to the file name given in fullNamePath.
 *
 * @param    fileLen            Size of the fileName
 *
 *                        
 * @return  NAFS_SUCCESS        Successfully parse directory path and file name.
 * @return  NAFS_INVALID_VOLUME_NAME    No Volume name is specified in the fullNamePath.
 * @return  NAFS_INVALID_FILE_NAME    No file name is specified in the fullNamePath.
 * @return  NAFS_INVALID_VOLUME_NAME_LENGTH    Invalid volume name length
 * @return  NAFS_INVALID_DIR_PATH_LENGTH    Invalid file name length
 *
 * @see @link file_sys_errors
 *
 */
static int Parse_FileNamePath(char * fullNamePath, char * dirName, unsigned dirLen, char * fileName, unsigned fileLen)
{

#define _IS_VALID_PATH(a, b)    (((a) != NULL) && (((a)+1) != b))
#define _ASCII_SLASH            "/" /* 0x2F */
#define _NO_SLASH_INDEX         1

    int     rc = NAFS_SUCCESS;
    char    *filePtr, *pathPtr, *volumePtr, *dirPtr;
    char    *theNamePtr;
    int     volumeLen;
    
    theNamePtr = fullNamePath;
    while (strstr(theNamePtr, "\\") != NULL)
    {
        theNamePtr = strstr(theNamePtr, "\\");
        *theNamePtr = '/'; /* change it to forward slash */
        theNamePtr++;
    }
    /* must skip RomPager file system prefix */
    theNamePtr = strstr(fullNamePath, FILE_SYSTEM_PREFIX);
    
    if (theNamePtr == NULL)
		/* not prefix is specified. Still need to continue for parsing the directory */
        theNamePtr = fullNamePath;
    else
    {	/* skip the prefix */
        theNamePtr += strlen(FILE_SYSTEM_PREFIX);
    }    

    if (theNamePtr == NULL)
    {   /* no filename is specified */
        rc = NAFS_INVALID_VOLUME_NAME;
        goto _Parse_FileNamePath_ret;
    }

    pathPtr = theNamePtr;        
    filePtr = pathPtr;
    /* Try to get the file name only (skip all /path/)
     * Parse until the last slash for file name
     */
    while (pathPtr != NULL) {
        pathPtr = strstr(pathPtr, _ASCII_SLASH);
        if (pathPtr != NULL)
        {
            pathPtr++;
            filePtr = pathPtr;
        }
    }
    /* make sure we have valid file name */
    if (filePtr == NULL || fileLen < strlen(filePtr))
    {   
        rc = NAFS_INVALID_FILE_NAME;
        goto _Parse_FileNamePath_ret;
    }

    /* get a volume_name */
    pathPtr = theNamePtr;
    if ((pathPtr = strstr(theNamePtr, _ASCII_SLASH)) == theNamePtr)
    {
        volumePtr = theNamePtr+1; /* skip slash */
    }
    else
        volumePtr = theNamePtr;
    
    if (strstr(volumePtr, _ASCII_SLASH) != NULL && _IS_VALID_PATH(volumePtr, filePtr))
    {   
       
        volumeLen = 0;
                        
        /* get directory path */        
        dirPtr = strstr(volumePtr, _ASCII_SLASH);
        
        if (_IS_VALID_PATH(dirPtr, filePtr))
        {   /* must be in /vol/path/filename format */
            dirPtr++;
            volumeLen = dirPtr - volumePtr;
        }
        else
        {   /* must be in /vol/filename format */
            volumeLen = filePtr - volumePtr;
            dirPtr = NULL;
        }
    
        if (volumeLen > dirLen)
        {
            rc = NAFS_INVALID_VOLUME_NAME_LENGTH;
            goto _Parse_FileNamePath_ret;
        }
        /* volume name */
        strncpy(dirName, volumePtr, volumeLen);
        dirName[volumeLen] = '\0';
    
        /* copy the directory path if it's there */
        if (dirPtr != NULL)
        {
            /* make sure the length of the whole path is valid. */
            if ((volumeLen + (filePtr - dirPtr)) > dirLen)
            {
                rc = NAFS_INVALID_DIR_PATH_LENGTH;
                goto _Parse_FileNamePath_ret;
            }
            volumePtr = dirName + strlen(dirName);
            strncpy(volumePtr, dirPtr, (filePtr - dirPtr));
            volumePtr[(filePtr - dirPtr - _NO_SLASH_INDEX)] = '\0';  
        }
    }
           
    /* file name */            
    strcpy(fileName, filePtr);
    
_Parse_FileNamePath_ret:        
    return rc;
}        
/**
 * Gets user permission for the given group id 
 *
 * It derives permission from the given group id.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   groupId    group id 
 *          <list type="unordered">
 *              <item> NAFS_FS_GROUP1 </item> 
 *              <item> NAFS_FS_GROUP2 </item> 
 *              <item> NAFS_FS_GROUP3 </item> 
 *              <item> NAFS_FS_GROUP4 </item> 
 *              <item> NAFS_FS_GROUP5 </item> 
 *              <item> NAFS_FS_GROUP6 </item> 
 *              <item> NAFS_FS_GROUP7 </item> 
 *              <item> NAFS_FS_GROUP8 </item> 
 *          </list>
 * @param   permission  pointer to user permission which will be returned for
 *                      given group id
 *
 * @see @link securityLevels
 * @see @link group_masks
 *                        
 */
static void Get_GroupIdToPermission(rpAccess groupId, unsigned int * permission)
{
    #define _READ_BIT_OFFSET    2

    rpAccess        accessCode = groupId;
    
    if (groupId == kRpPageAccess_Unprotected)
        accessCode = gRpFSDefaultGroupId;

    *permission = (accessCode << _READ_BIT_OFFSET) | (accessCode << _WRITE_BIT_OFFSET);
}    

/**
 * Validates the given group id 
 *
 * If the give group id is invalid, it will be set to the default group id.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    groupId   Pointer to group id which will be validated
 *
 *                        
 * @return  0   Given groupId is a valid group  id
 * @return  -1  Given groupId is not a valid group id and has been set to default group_id.
 *
 * @see @link group_masks
 *
 */
static int Set_ValidGroupId(rpAccess * groupId)
{
    int ret = -1;

	switch (*groupId)
	{
	    case NAFS_FS_GROUP1:
	    case NAFS_FS_GROUP2:
	    case NAFS_FS_GROUP3:
	    case NAFS_FS_GROUP4:
	    case NAFS_FS_GROUP5:
	    case NAFS_FS_GROUP6:
	    case NAFS_FS_GROUP7:
	    case NAFS_FS_GROUP8:
            ret = 0;
	        break;
	    default:
	        *groupId = gRpFSDefaultGroupId;
	        break;
	}
    return ret;
}
/**
 * Returns the current group id and permission 
 *
 * This is called when http is processing file system. The group id & permission 
 * are returned based on the username and password (realm) that http server has 
 * authenticated. If no authentication, a default group id will be returned and 
 * permission will not be set.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param    groupId    pointer to group id that will be returned
 *          <list type="unordered">
 *          <item> NAFS_FS_GROUP1 </item> 
 *          <item> NAFS_FS_GROUP2 </item> 
 *          <item> NAFS_FS_GROUP3 </item> 
 *          <item> NAFS_FS_GROUP4 </item> 
 *          <item> NAFS_FS_GROUP5 </item> 
 *          <item> NAFS_FS_GROUP6 </item> 
 *          <item> NAFS_FS_GROUP7 </item> 
 *          <item> NAFS_FS_GROUP8 </item> 
 *          </list>
 *
 * @param    permissions    pointer to memory where user login permissions will be returned.
 *                        
 * @return  0      Successfully get a group id
 * @return  -1     Multiple realms. Unable to return a specific group id.
 *
 * @see @link group_masks
 * @see @link securityLevels
 *
 */
static int Get_LoginGroupIdnPermissions(rpAccess * groupId, unsigned int * permissions)
{
    void            * httpServerDataPtr;
    Unsigned16      timeout;
    Unsigned32      ipAddress;
    Unsigned32	    remoteIp;
    char            * username, * password, * curUserName;
    int             userCount;
    rpAccess        thisAccess;
    int             ret_code = -1;
    
    extern unsigned long NAIpAddress (void);
    extern void *getTheAllegroTaskDataPtr (void);  

    if ((ret_code = Set_ValidGroupId(groupId)) != 0)
    {
        /* get the current login user */
        httpServerDataPtr = getTheAllegroTaskDataPtr();
        remoteIp = RpGetRemoteIpAddress(httpServerDataPtr);
                
        curUserName = RpGetCurrentUserName(httpServerDataPtr);
        userCount = 0;
	    *groupId = gRpFSDefaultGroupId;

	    if (curUserName != NULL)
	    {
	        do {    
	            RpGetUserAttributes(httpServerDataPtr, userCount, &username,
	                        &password, &thisAccess, &ipAddress,
	                        &timeout);
	            userCount++;
                if (ipAddress != 0 && ipAddress != remoteIp)
                    continue;

	        } while (username != NULL && strcmp(username, curUserName) != 0);

            if (username != NULL && strcmp(username, curUserName) == 0 && 
                (ipAddress == 0 || ipAddress == remoteIp))
            {

                char    accountPassword[NASYSACC_STRLEN_PASSWORD + 4];

                *permissions = NAgetSysAccess(username, accountPassword, (unsigned int *)&ipAddress);

	            /* user must be logged in one of the following realm (group id) 
	             * If no realm is used, default realm will be set.
	             * But if multiple realms are set, return error and let the caller
	             * decide the group id
	             */
	            switch (thisAccess)
	            {
	                case kRpPageAccess_Unprotected:
	                    *groupId = gRpFSDefaultGroupId;
	                    ret_code = 0;
	                    break;
	                case NAFS_FS_GROUP1:
	                case NAFS_FS_GROUP2:
	                case NAFS_FS_GROUP3:
	                case NAFS_FS_GROUP4:
	                case NAFS_FS_GROUP5:
	                case NAFS_FS_GROUP6:
	                case NAFS_FS_GROUP7:
	                case NAFS_FS_GROUP8:
                        *groupId = thisAccess;
	                    ret_code = 0;
	                    break;
	                default:
	                    break;
	            }

            }
	    }
    }
    else
    {
        Get_GroupIdToPermission(*groupId, permissions);
    }
    return ret_code;
}    

#if (_RESTRICTED_PERMISSION == 0) 

/**
 * Gets group_id from the given write-permission 
 *
 * It derives group-id from the given write-permission.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   permission  Pointer to user permission which will be returned for
 *                      given group id
 *
 * @param   group_id    group id 
 *          <list type="unordered">
 *          <item> NAFS_FS_GROUP1 </item> 
 *          <item> NAFS_FS_GROUP2 </item> 
 *          <item> NAFS_FS_GROUP3 </item> 
 *          <item> NAFS_FS_GROUP4 </item> 
 *          <item> NAFS_FS_GROUP5 </item> 
 *          <item> NAFS_FS_GROUP6 </item> 
 *          <item> NAFS_FS_GROUP7 </item> 
 *          <item> NAFS_FS_GROUP8 </item> 
 *          </list>
 *                        
 * @return  True    group-id is successfully obtained from the given write-permission
 * @return  False   Unable to obtain the group-id from the given write-permission
 *
 * @see @link securityLevels
 * @see @link group_masks
 *
 */
static Boolean Get_WritePermissionToGroupId(unsigned int permission, rpAccess * group_id)
{
    Boolean         rc = False;
    unsigned int    write_group;
    int             i;

    /* derive group id from permission */
    write_group = permission >> _WRITE_BIT_OFFSET;

    for (i=kRpPageAccess_Realm1; i <= kRpPageAccess_Realm8; i = i << 1)
    {
        if ((write_group & i) != 0)
        {
            *group_id = i;
            rc = True;
            break;
        }
    }
    
    return rc;
}
#endif

/**
 * This function is used to see whether the given permission allows the given
 * group-id for write access. 
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   groupId    group id 
 *          <list type="unordered">
 *          <item> NAFS_FS_GROUP1 </item> 
 *          <item> NAFS_FS_GROUP2 </item> 
 *          <item> NAFS_FS_GROUP3 </item> 
 *          <item> NAFS_FS_GROUP4 </item> 
 *          <item> NAFS_FS_GROUP5 </item> 
 *          <item> NAFS_FS_GROUP6 </item> 
 *          <item> NAFS_FS_GROUP7 </item> 
 *          <item> NAFS_FS_GROUP8 </item> 
 *          </list>
 *
 * @param   permission  Permission to see whether the given group id is granted with
 *                      write access
 *
 * @return  True    Write permission is allowed for the given group-id 
 * @return  False   No write permission for the given group-id
 *
 * @see @link securityLevels
 * @see @link group_masks
 *
 */
static Boolean Is_WritePermission(rpAccess groupId, unsigned int permission)
{
    Boolean     isAllowed = False;
    Unsigned32  wPermission = groupId << _WRITE_BIT_OFFSET;

    if ((wPermission & permission) != 0)
        isAllowed = True;

    return isAllowed;
}    

/**
 * Handles appropriate file system process function for the given file request
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   pFileReq    pointer to file request indicating which file system 
 *                      process function will be executed.
 *
 * @param   funcPtr     pointer to a function which is called to retrieve data
 *                      to handle file system process function.
 *
 *                        
 * @return  NAFS_SUCCESS    Successfully perform file system process function.
 * @return  <0              Error on accessing the file system 
 *
 * @see @link file_sys_errors
 * @see @link RPFS_BUFFER_T
 *
 */
static int Process_FileState(RPFS_BUFFER_T * pFileReq, void * funcPtr)
{
    int             rc = NAFS_SUCCESS;

	NA_ASSERT((int)pFileReq);

    switch (pFileReq->state) {
        case eFileSystemProcess_OpenFile:
            rc = NAFSopen_file(pFileReq->dir_path, pFileReq->file_name, &pFileReq->file_handler, 
                              pFileReq->permissions, &pFileReq->io_request);
            break;        
        case eFileSystemProcess_GetDir:
        {
            char          *pathPtr;
            unsigned char dataType;
            
            NA_ASSERT((int)funcPtr);
            /* call file system list-dir to get info on the current pathPtr */            
            (*(LIST_DIR_FUNC_T) funcPtr)(pFileReq, &pathPtr, &dataType);
            
            pFileReq->dir_info_list_items++; /* at least 1 entry */
            
            pFileReq->dir_info_list = (NAFS_DIR_ENTRY_INFO *)calloc(pFileReq->dir_info_list_items, sizeof(NAFS_DIR_ENTRY_INFO));
    
            if (pFileReq->dir_info_list == NULL)
            {
                rc = NAFS_MALLOC_FAILED;
            }
            else
            {
                memset(pFileReq->dir_info_list, 0, (pFileReq->dir_info_list_items * sizeof(NAFS_DIR_ENTRY_INFO)));

                rc = NAFSlist_dir(pathPtr, pFileReq->permissions, 
                                pFileReq->dir_info_list, pFileReq->dir_info_list_items,
                                &pFileReq->io_request);
            }
            break;
        }
        case eFileSystemProcess_EntryCount:
        {
            char          *dataPtr;
            unsigned char dataType;
            
            NA_ASSERT((int)funcPtr);
            /* Setup the dir info buffer for number of entries in the directory. */
            (*(LIST_DIR_FUNC_T) funcPtr)(pFileReq, &dataPtr, &dataType);
            rc = NAFSdir_entry_count(dataPtr, pFileReq->permissions, 
                                &pFileReq->dir_info_list_items, &pFileReq->io_request);
            break;
        }
        case eFileSystemProcess_ListDir:
        {   
            char          *dataPtr;
            unsigned char dataType;
            int           i;
            
            NA_ASSERT((int)funcPtr);
            /* list-dir is successfully requested so let's get the info */
            (*(LIST_DIR_FUNC_T)funcPtr)(pFileReq, &dataPtr, &dataType);
            for (i=0; i < pFileReq->dir_info_list_items; i++)
            {
                if (pFileReq->dir_info_list[i].entry_type ==  dataType && 
                    strcmp(pFileReq->dir_info_list[i].entry_name, dataPtr) == 0)
                {
					/* We should have group-id, file-date & file-size from get-dir.
					 * Let's setup file access=group-id. This will invoke user
					 * to supply a username/password in order to download this
					 * file.
					 * But if user overrides the file access for all users,
					 * we don't want to use the group_id as the file access. 
					 * We simply set this file without any file access 
					 * (no username/password requeired).
					*/
                    /* pFileReq->permissions = NASYSACC_FS_SUPER_USER;*/
                    pFileReq->file_info.FileAccess = pFileReq->dir_info_list[i].group_id;
                    pFileReq->file_info.FileDate =  pFileReq->dir_info_list[i].last_modified_time;
                    pFileReq->file_info.FileSize =  pFileReq->dir_info_list[i].file_size;
                    if (Get_FileMimeType(pFileReq->file_name, &pFileReq->file_info.FileType) != eRpNoError)
                    {
                        Get_FileMimeType(DEFAULT_FILE_EXTENSION, &pFileReq->file_info.FileType);
                    }
                    break;
                }
            }
            break;
        }
        case eFileSystemProcess_ReadFile:
        {
            char         *readPtr;
            unsigned int readLength;
            
            NA_ASSERT((int)funcPtr);
            (*(READ_FILE_FUNC_T)funcPtr)(pFileReq, &readPtr, &readLength);
            rc = NAFSread_file(pFileReq->file_handler, pFileReq->permissions, 
                      readLength, pFileReq->file_offset, 
                      readPtr, &pFileReq->io_request);
            break;
        }
        case eFileSystemProcess_WriteFile:
        {
            char            *writePtr;
            unsigned int    writeLength;
            
            NA_ASSERT((int)funcPtr);
            (*(READ_FILE_FUNC_T)funcPtr)(pFileReq, &writePtr, &writeLength);
            rc = NAFSwrite_file(pFileReq->file_handler, pFileReq->permissions, 
                      writeLength, pFileReq->file_offset, 
                      writePtr, &pFileReq->io_request, NAFS_ZERO_OFFSET_ERASE);
            break;
        }
        case eFileSystemProcess_CreateFile:
            rc = NAFScreate_file(pFileReq->dir_path, pFileReq->file_name, pFileReq->permissions,
                              pFileReq->file_info.FileAccess, &pFileReq->io_request);
            break;
        case eFileSystemProcess_CloseFile:
            rc = NAFSclose_file(pFileReq->file_handler, pFileReq->permissions, 
                      &pFileReq->io_request);
            break;
        default:
			break;
    }
    
    #if (_PRINTF_DEBUG > 0)
    printf("process [%s] - handler 0x%x error %d\n", _PROCESS_NAME(pFileReq->state), 
                    pFileReq->file_handler, rc);
    #endif
    return rc;
}    

/**
 * Gets the last directory name 
 *
 * This function is used to pass into Process_FileState function to 
 * retrieve data for list-dir process.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   pFileReq    pointer to RPFS_BUFFER_T which will be parsed to get the 
 *                       last directory name
 * @param   dataName   pointer to an address that contains last directory name.
 *
 * @param   dataType   pointer to directory data type
 *          <list type="unordered">
 *          <item> NAFS_FILE_TYPE_DIR. directory data type </item>
 *          </list>
 *
 */
static void Get_LastDirectoryName(RPFS_BUFFER_T * pFileReq, char ** dataName, unsigned char * dataType)
{

    char * pathPtr;
    char * dirPtr;
    
    NA_ASSERT((strlen(pFileReq->dir_path) > 0));
    
    pathPtr = pFileReq->dir_path;
    
    /* get the last / for the directory path */
    while (strlen(pathPtr) > 1 && (dirPtr = strstr(pathPtr, "/")) != NULL)
    {
        dirPtr++;
        pathPtr = dirPtr;
    }
    
    *dataType = NAFS_FILE_TYPE_DIR;
    *dataName = pathPtr;
}

/**
 * Removes the last directory name 
 *
 * This function is used to pass into Process_FileState function to retrieve
 * data for entry-count & get-dir processes for the last directory.
 * These processes must be passed with the full directory path without the inquire 
 * directory (last directory).
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   pFileReq   pointer to RPFS_BUFFER_T which will be parsed the full directory
 *                      path with the last directory removed. 
 * @param   dataName  pointer to an address that contains directory path with last
 *                      directory excluded.
 *
 * @param   dataType   pointer to directory data type
 *          <list type="unordered">
 *          <item> NAFS_FILE_TYPE_DIR </item>
 *          </list>
 *
 */
static void Remove_LastDirectoryName(RPFS_BUFFER_T * pFileReq, char ** dataName, unsigned char * dataType)
{

    char * pathPtr;
    char * dirPtr;
    
    NA_ASSERT((strlen(pFileReq->dir_path) > 0));
    
	/* This will remove the last directory and copy into the parsing_buffer */
    pathPtr = pFileReq->dir_path;

	/* search for last slash "/" in the full directory path */    
    while ((dirPtr = strstr(pathPtr, "/")) != NULL)
    {
        pathPtr = dirPtr+1;
    }
    strncpy(pFileReq->parsing_buffer, pFileReq->dir_path, (pathPtr - pFileReq->dir_path));
    pFileReq->parsing_buffer[(pathPtr - pFileReq->dir_path)] = '\0';
    
    *dataType = NAFS_FILE_TYPE_DIR;
    *dataName = pFileReq->parsing_buffer;
}

/**
 * Gets file name 
 *
 * This function is used to pass into Process_FileState function to 
 * retrieve data for list-dir process to get info about a file.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   pFileReq   Pointer to RPFS_BUFFER_T which contains the file name
 * @param   dataName  Pointer to an address that contains the file name
 *
 * @param   dataType   Pointer to memory where directory data type will be returned
 *          <list type="unordered">
 *          <item> NAFS_FILE_TYPE_FILE </item>
 *          </list>
 *
 */
static void Get_FileName(RPFS_BUFFER_T * pFileReq, char ** dataName, unsigned char * dataType)
{
    
    * dataType = NAFS_FILE_TYPE_FILE;
    * dataName = pFileReq->file_name;
}
/**
 * Get the full directory path of a file 
 *
 * This function is used to pass into Process_FileState function to 
 * retrieve data for entry-count & get-dir processes to get file info.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   pFileReq   pointer to RPFS_BUFFER_T which contains the directory path of a file
 * @param   dataName  pointer to an address that contains the directory path
 *
 * @param   dataType   pointer to directory data type
 *          <list type="unordered">
 *          <item> NAFS_FILE_TYPE_DIR. directory data type </item>
 *          </list>
 *
 */
static void Get_FileNameDirectory(RPFS_BUFFER_T * pFileReq, char ** dataName, unsigned char * dataType)
{

    * dataType = NAFS_FILE_TYPE_DIR;
    * dataName = pFileReq->dir_path;
}

/**
 * Sees whether the given path is root directory only
 *
 * The path should contain only a single slash for root directory.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0
 *
 * @param   path  pointer to the directory path
 *
 * @return  True    is only root directory
 * @return  False   is not root directory
 *
 */
static Boolean Is_RootDirectory(char * path)
{
    char * dir;
    Boolean isRoot = False;
    
    dir = strstr(path, "/");
    
    if ((strlen(path) > 0 && dir == NULL) || strlen(dir) == 1)
        isRoot = True;
        
    return isRoot;
}    
#endif /* APP_FILE_SYSTEM */

/**
 * The RpHSOpenFileSystem routine is called when the web server starts up.
 *
 * This function initializes the interface between the HTTP server and the file
 * system.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theOpenFilesCount  Number of files will be opened simultaneous
 *
 *                        
 * @return  eRpNoError      File system interface was successfully initialized
 * @return  eRpOutOfMemory  No memory
 *
 * @see @link RpErrorCode
 *
 */
RpErrorCode RpHSOpenFileSystem(int theOpenFilesCount) {    

#ifdef APP_FILE_SYSTEM
    RpErrorCode theResult = eRpOutOfMemory;
    int         i;
    
    #if (_PRINTF_DEBUG > 0)
    printf("------   RpFS_OpenFileSystem: %d -----\n", theOpenFilesCount);
    #endif

    /* allocate memory for file system handlers */    
    gRpFSBuffer = (RPFS_BUFFER_T *)calloc(theOpenFilesCount, sizeof(RPFS_BUFFER_T)); 
    if (gRpFSBuffer != NULL)
    {
        gRpFSMaxBuffers = theOpenFilesCount;
        for (i=0; i < theOpenFilesCount; i++)
        {
            gRpFSBuffer[i].dir_path[0] = '\0';
            gRpFSBuffer[i].file_name[0] = '\0';
            gRpFSBuffer[i].dir_info_list = NULL;
            gRpFSBuffer[i].state = eFileSystemProcess_None;
			gRpFSBuffer[i].process_function_cb = NULL;
        }

        theResult = eRpNoError;
    }
    return theResult;
#else /* APP_FILE_SYSTEM */

    return eRpNoError;

#endif
        
}

/**
 *  The RpHSCloseFileSystem routine is called once at when the web server finishes
 *	so that the file system can deinitialize any internal variables and processes.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @return  eRpNoError              The file system interface was successfully closed.
 * @return  eRpFileSystemNotOpen    File system is not initialized.
 *
 * @see @link RpErrorCode
 * @see @link RpHSOpenFileSystem
 *
 */
RpErrorCode RpHSCloseFileSystem(void) {

#ifdef APP_FILE_SYSTEM
    RpErrorCode theResult = eRpFileSystemNotOpen;
    
    #if (_PRINTF_DEBUG > 0)
    printf("------    RpFS_CloseFileSystem    ------\n");
    #endif
        
    if (gRpFSBuffer != NULL) {
        free (gRpFSBuffer);
        gRpFSBuffer = NULL;
        theResult = eRpNoError;
    }    
    return theResult;
#else /* APP_FILE_SYSTEM */

    return eRpNoError;
#endif    
}

/**
 * The RpHSCreateFile routine is called to start creating a new file on the file system.
 * The RpHSCreateFileStatus call is used to check for completion.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber   File id 
 * @param   theFullNamePtr  Name of file to be created
 * @param   theFileInfoPtr  Pointer to a file info block
 *
 *                        
 * @return  eRpNoError              The process of creating a file was successfully started.
 * @return  eRpFileSystemNotOpen    Unable to create a file or File system is not initialized
 *
 * @see @link FILEINFOPTR
 * @see @link RpErrorCode
 * @see @link RpHSCreateFileStatus
 *
 */
RpErrorCode RpHSCreateFile(int theFileNumber, char * theFullNamePtr, FILEINFOPTR theFileInfoPtr)
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode      theResult = eRpFileSystemNotOpen;
    RPFS_BUFFER_T    * pFileReq = NULL;

    pFileReq = Get_FileSystemBuffer(theFileNumber);

    if (pFileReq != NULL)
    {   
        theResult = eRpNoError;

        /* parse the dir name and file name */                
        pFileReq->error_status = Parse_FileNamePath(theFullNamePtr, pFileReq->dir_path, 
                                            NAFS_DIRECTORY_PATH_SIZE, pFileReq->file_name, 
                                            NAFS_FILE_NAME_SIZE);
                              
        if (pFileReq->error_status != NAFS_SUCCESS)
            goto _createFile_ret;
        if (strlen(pFileReq->dir_path) == 0)
        {
            pFileReq->error_status = NAFS_INVALID_VOLUME_NAME;
            goto _createFile_ret;
        }
            
        /* if user did login with multiple access,
         * get the permissions from the multiple access and make
         * sure we have valid group id.
         */
        pFileReq->error_status        = NAFS_SUCCESS;
        pFileReq->permissions         = NASYSACC_FS_SUPER_USER; 
        pFileReq->file_info.FileDate = theFileInfoPtr->FileDate;
        pFileReq->file_info.FileSize = theFileInfoPtr->FileSize;
        pFileReq->file_info.FileType = theFileInfoPtr->FileType;
        pFileReq->file_info.FileAccess = theFileInfoPtr->FileAccess;


        pFileReq->dir_info_list = NULL;
        pFileReq->file_offset = 0;
        
        /* initialize file system io request control */        
        pFileReq->error_status = NAFSinit_io_request_cb(&pFileReq->io_request, NULL, 0);
        if (pFileReq->error_status != NAFS_SUCCESS)
            goto _createFile_ret;

        /* We need to setup group-id.
         * We get the group-id (realm) & permission by query the current login username.
         *
         * If the user is setup for multiple accesses, we must get & use the
         * group-id from the directory where a file is going to be created.
         *
         */
         
        /* get the current request access code for the group id. 
         * only return single access bit.
         */

        if (Get_LoginGroupIdnPermissions(&pFileReq->file_info.FileAccess, &pFileReq->permissions) != 0)
        {   

            if (Is_RootDirectory(pFileReq->dir_path))
            {
                pFileReq->file_info.FileAccess = gRpFSDefaultGroupId;
                pFileReq->state = eFileSystemProcess_CreateFile;
	    		pFileReq->process_function_cb = NULL;

                /* make sure we have any write permission. If so, use super-user
                 * permission.
                 */
                if (Is_WritePermission(kRpPageAccess_AllRealms, pFileReq->permissions) == True)
                    pFileReq->permissions         = NASYSACC_FS_SUPER_USER; 

            }
            else
            {
            /* Multiple group-id is setup so we must use the same group id as the  
             * directory if the same access code (user's permission) is used to login.
             *
             * If the user log in with different access code than the directory's group-id,
             * the lowest group-id of the user's login access code will be used.
             *
             * In order to get the current directroy group-id, we need 
             * to get dir on previous directory so it will return the current 
             * directory info.
             * 1. Get the number of items in the previous directory.
             * 2. Allocate the number of items in the directory.
             * 3. Get the directory listing.
             * 4. Setup the group-id for the current directory
             * 5. Create a file.
             * 6. Open a file.
             * 7. Write data into a file.
             * 8. Repeat 7 until no more data.
             * 9. Close a file and free memory.
             */

                pFileReq->permissions         = NASYSACC_FS_SUPER_USER;
                pFileReq->state = eFileSystemProcess_EntryCount;
	    		pFileReq->process_function_cb = Remove_LastDirectoryName;
            }
        }
        else
        {   /* group-id is ok */

#if (_RESTRICTED_PERMISSION == 0)
            /* we must make sure we have write permission set on the same group-id 
             * which a file will be created with. Use super-user so that it still allows
             * a file to be created with different group-id than directory's group-id.
            */
            if (Is_WritePermission(pFileReq->file_info.FileAccess, pFileReq->permissions) == True)
                pFileReq->permissions         = NASYSACC_FS_SUPER_USER;
#endif  
            pFileReq->state = eFileSystemProcess_CreateFile;
			pFileReq->process_function_cb = NULL;
        }
        
        pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);

    	#if (_PRINTF_DEBUG > 0)
        printf("\n\nRpFS_CreateFile: >>>>> index %d path = %s file = %s error = %d\n", 
                        theFileNumber, pFileReq->dir_path, pFileReq->file_name, 
                        pFileReq->error_status);
        #endif
        
    }
_createFile_ret:
    return theResult;
#else /* APP_FILE_SYSTEM */

    return eRpNoError;
#endif
}

/**
 * The RpHSCreateFileStatus routine is called to determine whether the RpHSCreateFile
 * call has completed.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber          File id
 * @param   theCompletionStatusPtr Pointer to the create status which will be set
 *                                 indicating whether the file has been successfully
 *                                 created
 *          <list type="unordered">
 *          <item> ACTIONCOMPLETED - Finish creating file action </item>
 *          <item> ACTIONPENDING   - Waiting for completion of a file creation </item>
 *          </list>
 *
 *                        
 * @return  eRpNoError              No error when creating a file
 * @return  eRpFileSystemNotOpen    Unable to create a file or File system is not initialized
 * @return  eRpFileCreateError      Unable to create a file
 *
 * @see @link HttpFileAction
 * @see @link RpErrorCode
 * @see @link RpHSCreateFile
 *
 */
RpErrorCode RpHSCreateFileStatus(int theFileNumber, int * theCompletionStatusPtr) 
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode     theResult = eRpFileSystemNotOpen;
    RPFS_BUFFER_T   * pFileReq = NULL;
    unsigned int    byteTransferred;
    unsigned int	status;
            
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL)
    {
        theResult = eRpNoError;
    
        if (pFileReq->error_status == NAFS_SUCCESS)
        {
            /* get the file system io status for the previous file system process */    
            status = Get_IORequestStatus(&pFileReq->io_request, &byteTransferred, 
                                     &pFileReq->error_status);
        }
		else if (IS_FILE_SYSTEM_BUSY(pFileReq->error_status))
		{
			/* try to previous API call again */
            pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
			status = eFileSystemStatus_Wait;
		}
        else
        {   /* error occurs so skip any processing.
             * But no error is returned since http server sends its own
             * error message back to client instead of calling the
             * post-processing function (the web page initiates the upload)
             * to handle any error status.
             */
            status = eFileSystemStatus_Error;
        }
        
        *theCompletionStatusPtr = ACTIONPENDING;   
    
        /* if successfully created a file or even createFile returns error because,
         * the file is already exist, we want to continue 
         */
        if (status == eFileSystemStatus_Complete 
#if (_RESTRICTED_FILE_CREATION == 0)
            /* if a file is already existed */
            || (status != eFileSystemStatus_Wait && 
            pFileReq->state == eFileSystemProcess_CreateFile &&
            pFileReq->error_status == NAFS_DUPLICATE_DIR_ENTRY) 
#endif
            )
        {   
            switch (pFileReq->state) {
                case eFileSystemProcess_EntryCount:
                    /* We now know number of entry in the previous directory.
                     * let's allocate memory and get the directory info
                     */
                    pFileReq->state = eFileSystemProcess_GetDir;
					pFileReq->process_function_cb = Remove_LastDirectoryName;
                    break;            
                case eFileSystemProcess_GetDir:
                    /* let's get the current directory info. */
                    pFileReq->state = eFileSystemProcess_ListDir;
					pFileReq->process_function_cb = Get_LastDirectoryName;
                    break;
                case eFileSystemProcess_ListDir:
                    /* let's create a file since we already get group-id for the directory
                     * where a file will be created.
                     */
                    {
                    rpAccess groupId = 0;
                    /* we must make sure we have write permission set on the same group-id 
                     * where a file will be created to. 
                     *
                     * If the given permission doesn't contain the group-id access for the 
                     * directory, we must derive the group-id from the user login permission
                     * and use it to create a file.
                     *
                     * Use super-user permission so that it still allows
                     * a file to be created with different group-id than directory's group-id.
                     * 
                     */

                    /* get the user login permissions */
                    Get_LoginGroupIdnPermissions(&groupId, &pFileReq->permissions);

                #if (_RESTRICTED_PERMISSION == 0)
                    /* the login permission has the write access permission for directory */
                    if (Is_WritePermission(pFileReq->file_info.FileAccess, pFileReq->permissions) == True)
                        pFileReq->permissions         = NASYSACC_FS_SUPER_USER;
                    /* derive group-id from the login write permission */
                    else if (Get_WritePermissionToGroupId(pFileReq->permissions, &pFileReq->file_info.FileAccess) == True)
                        pFileReq->permissions         = NASYSACC_FS_SUPER_USER;
                #endif  
                    }

                    pFileReq->state = eFileSystemProcess_CreateFile;
					pFileReq->process_function_cb = NULL;
                    break;
                case eFileSystemProcess_CreateFile:
                    /* file is created. 
                     * let's open the file
                     */
                    pFileReq->state = eFileSystemProcess_OpenFile;
					pFileReq->process_function_cb = NULL;
                    break;
                 default: 
                    /* file has been successfully created/opened  */
                    pFileReq->error_status = NAFS_SUCCESS;
                    *theCompletionStatusPtr = ACTIONCOMPLETED;
                    break;
            }
        	if (*theCompletionStatusPtr == ACTIONPENDING)
	            pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);

        }
        else if (status == eFileSystemStatus_Error)
        {
#if (_RESTRICTED_FILE_CREATION == 0)
            /* if the file is being opened, we need to wait and try again */
            if (pFileReq->state == eFileSystemProcess_OpenFile &&
                pFileReq->error_status == NAFS_FILE_IS_OPEN)
            {
	            pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
            }
            else
#endif
            {
                /*
                 *  Although error occurs, let's tell http server create action is done 
                 *  and continues the write proccess.
                 */
                *theCompletionStatusPtr = ACTIONCOMPLETED;

			    #if (_HTTP_POST_FUNCTION_ENABLE == 0)
	            theResult = eRpFileCreateError;
			    #endif
            }
        }
    } 
    
    return theResult;
#else /* APP_FILE_SYSTEM */
    *theCompletionStatusPtr = ACTIONCOMPLETED;   
    
    return eRpNoError;
#endif
}


/**
 * The RpHSCloseFile routine is used to start closing a file.
 * The RpHSCloseFileStatus call is used to detect the completion of the close.  
 * theCompleteFlag is used to signal whether all data was received from browser.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber   File id
 * @param   theCompleteFlag Complete flag indicator
 *          <list type="unordered">
 *          <item>  1      - File is completed </item>
 *          <item>  0      - File is incompleted </item>
 *          </list>
 *
 *                        
 * @return  eRpNoError              The process of closing a file was successfully started.
 * @return  eRpFileSystemNotOpen    File system is not initialized.
 *
 * @see @link RpErrorCode
 * @see @link RpHSCloseFileStatus
 *
 */
RpErrorCode RpHSCloseFile(int theFileNumber, int theCompleteFlag)
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode     theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T   * pFileReq;
    int             rc;
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    if (pFileReq != NULL) 
    {
        pFileReq->state = eFileSystemProcess_CloseFile;
		pFileReq->process_function_cb = NULL;
        rc = Process_FileState(pFileReq, pFileReq->process_function_cb);

		/* temporary save the close status. So that it can be closed */
        pFileReq->file_offset = rc;
        
        theResult = eRpNoError; 
            
    }

    return theResult;
#else /* APP_FILE_SYSTEM */
    return eRpNoError;
#endif
}

/**
 * The RpHSCloseFileStatus routine is called to determine whether a file has
 * been closed.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber          File id
 * @param   theCompletionStatusPtr Pointer to memory which will be set
 *                                 indicating whether the file has been successfully
 *                                 closed
 *          <list type="unordered">
 *          <item> ACTIONCOMPLETED - File was closed </item>
 *          <item> ACTIONPENDING   - Waiting for completion to close a file </item>
 *          </list>
 *
 *                        
 * @return  eRpNoError              No error on closing a file
 * @return  eRpFileSystemNotOpen    Unable to close a file or File system is not initialized
 *
 * @see @link RpErrorCode
 * @see @link HttpFileAction
 * @see @link RpHSCloseFile
 *
 */
RpErrorCode RpHSCloseFileStatus(int theFileNumber,int * theCompletionStatusPtr) 
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode     theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T   * pFileReq;
    unsigned int    byteTransferred, status;
    int             error_status = NAFS_SUCCESS;
    
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL)
    {
        theResult = eRpNoError;
        *theCompletionStatusPtr = ACTIONCOMPLETED;
		if (IS_FILE_SYSTEM_BUSY(pFileReq->file_offset))
		{
			/* try to previous API call again */
            error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
		    if (IS_FILE_SYSTEM_BUSY(error_status))
            {
                pFileReq->file_offset = error_status;
            }
            else
            {
                pFileReq->file_offset = 0;
            }
			status = eFileSystemStatus_Wait;
		}
        else
		{
	        /* get file system io request status */
    	    status = Get_IORequestStatus(&pFileReq->io_request, &byteTransferred, 
                                     &error_status);
        }

        if (status == eFileSystemStatus_Wait)
            *theCompletionStatusPtr = ACTIONPENDING;
        else
        {
	        #if (_PRINTF_DEBUG > 0)
            printf("RpFS_CloseFileStatus: index %d io status %d error status %d<<<<<\n\n", 
                            theFileNumber, status, error_status);
            #endif
            if (pFileReq->error_status == NAFS_SUCCESS && error_status != NAFS_SUCCESS)
            {
                /* Can't close a file */
                pFileReq->error_status = error_status;
                printf("RpHSCloseFileStatus: index %d error %d\n", theFileNumber, error_status);
            
            }
        
            if (pFileReq->dir_info_list != NULL) 
            {
                free(pFileReq->dir_info_list);
            }
            
		    pFileReq->process_function_cb = NULL;
            pFileReq->dir_info_list = NULL;
            pFileReq->state = eFileSystemProcess_None;
            pFileReq->file_info.FileAccess = 0;
            pFileReq->parsing_buffer[0] = '\0';
            pFileReq->dir_path[0] = '\0';
        }
    }
    return theResult;
#else /* APP_FILE_SYSTEM */

   *theCompletionStatusPtr = ACTIONCOMPLETED;
    return eRpNoError;
#endif
}


/**
 * The RpHSWriteFile routine is called to start a write from the buffer provided
 * for the number of bytes in the count. This call is asynchronous and completes 
 * when the write has been started.  The RpHSWriteStatus call is used to detect 
 * the completion of the write.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber   File id
 * @param   theWritePtr     Pointer to the write buffer 
 * @param   theByteCount    Number of bytes to write
 *
 *                        
 * @return  eRpNoError              The process of writing a file has been successfully started.
 * @return  eRpFileSystemNotOpen    Unable to write data to a file or 
 *                                  File system is not initialized
 *
 * @see @link RpErrorCode
 * @see @link RpHSWriteFileStatus
 *
 */
RpErrorCode RpHSWriteFile(int theFileNumber,char * theWritePtr, unsigned long theByteCount)
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode     theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T   * pFileReq;
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL) 
    {
	    theResult = eRpNoError;
        pFileReq->state = eFileSystemProcess_WriteFile;
        /* if no error in file system, writes to it. Otherwise, just flush it */
        if (pFileReq->error_status == NAFS_SUCCESS)
            pFileReq->error_status = NAFSwrite_file(pFileReq->file_handler, pFileReq->permissions, 
                      theByteCount, pFileReq->file_offset, theWritePtr,
		      &pFileReq->io_request, NAFS_ZERO_OFFSET_ERASE);
        
    }

    return theResult;
#else /* APP_FILE_SYSTEM */

    return eRpNoError;
#endif
}


/**
 *  The RpHSWriteStatus routine is called to determine whether the write
 *  has finished.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber              File id
 * @param   theCompletionStatusPtr     Pointer to the call status
 *          <list type="unordered">
 *          <item> ACTIONCOMPLETED     Write action is completed </item>
 *          <item> ACTIONPENDING       Waiting for write completion </item>
 *          </list>
 * @param   theBytesWrittenPtr         Pointer to the number of bytes that has been 
 *                                     written to
 *
 *                        
 * @return  eRpNoError              No error when writing file
 * @return  eRpFileSystemNotOpen    Unable to write a file or 
 *                                  File system is not initialized
 * @return  eRpFileCreateError      Unable to write a file
 *
 * @see @link HttpFileAction
 * @see @link RpErrorCode
 * @see @link RpHSWriteFile
 *
 */
RpErrorCode RpHSWriteFileStatus(int theFileNumber, int * theCompletionStatusPtr, unsigned long * theBytesWrittenPtr)
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode      theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T    * pFileReq = NULL;
    unsigned int     byteTransferred;
    unsigned int	 status;
    

    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL)
    {
	    theResult = eRpNoError;

        *theCompletionStatusPtr = ACTIONCOMPLETED;
        *theBytesWrittenPtr = 0;
        
		if (IS_FILE_SYSTEM_BUSY(pFileReq->error_status))
		{
			/* make byteTransferred=0 so it pretents we didn't read anything.
			 * RpHSWriteFile should be called again 
			 */
			status = eFileSystemStatus_Complete;
			byteTransferred = 0;
		}
        else if (pFileReq->error_status == NAFS_SUCCESS)
        {
            /* get file system io request status */
            status = Get_IORequestStatus(&pFileReq->io_request, &byteTransferred, 
                                         &pFileReq->error_status);
		}
		        
        if (status == eFileSystemStatus_Complete)
        {   /* data is successfully written so setup for next write */
            pFileReq->file_info.FileSize += byteTransferred;
            pFileReq->file_offset += byteTransferred;
            *theBytesWrittenPtr = byteTransferred;
        
        }
        else if (status == eFileSystemStatus_Wait)
            *theCompletionStatusPtr = ACTIONPENDING;
		else 
		{

            pFileReq->file_info.FileSize += byteTransferred;
            pFileReq->file_offset += byteTransferred;
            *theBytesWrittenPtr = byteTransferred;

		#if (_HTTP_POST_FUNCTION_ENABLE == 0)
	        if (pFileReq->error_status != NAFS_SUCCESS)
		        theResult = eRpFileCreateError;
		#endif
        }        
	    #if (_PRINTF_DEBUG > 0)
        printf("RpFS_WriteFileStatus: index %d bytes transferred %d io status %d error status %d\n", 
                            theFileNumber, byteTransferred, status, pFileReq->error_status);
        #endif
    }                        
    return theResult;
#else /* APP_FILE_SYSTEM */
    *theBytesWrittenPtr = 0;
    *theCompletionStatusPtr = ACTIONCOMPLETED;

    return eRpNoError;
#endif
}

/**
 *  The RpHSOpenFile routine is called to open an individual file.  The file byte
 *  position is set to 0.  The open file call is responsible for all directory
 *  positioning, since the full file name from the URL will be passed in.  This
 *  call is asynchronous and completes when the open has been started.  The
 *  RpHSOpenFileStatus call is used to check for completion. An example file name
 *  is: /FS/MyFirstDirectory/TheSecondDirectory/MyFile.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber      File id
 * @param   theFullNamePtr     Pointer to full URL object name
 *
 * @return  eRpNoError  The process of opening a file has been successfully started.
 * @return  eRpFileSystemNotOpen    File system is not initialized
 *
 * @see @link RpErrorCode
 * @see @link RpHSOpenFileStatus
 *
 */
RpErrorCode RpHSOpenFile(int theFileNumber, char * theFullNamePtr)
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode      theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T    * pFileReq = NULL;
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);

    if (pFileReq != NULL)
    {    
	    theResult = eRpNoError;

        /* parse the directory path & file name */        
        pFileReq->error_status = Parse_FileNamePath(theFullNamePtr, pFileReq->dir_path, 
                              NAFS_DIRECTORY_PATH_SIZE, pFileReq->file_name, 
                              NAFS_FILE_NAME_SIZE);
                              
        if (pFileReq->error_status == NAFS_SUCCESS)
        {                          
            pFileReq->permissions = NASYSACC_FS_SUPER_USER;
            pFileReq->file_offset = 0;
            /* intialize file system io request control */
            pFileReq->error_status = NAFSinit_io_request_cb(&pFileReq->io_request, NULL, 0);
            
            /* We need to get at least group-id to setup the realm (file access) and file size.
             * 1. Get the number of items in the directory.
             * 2. Allocate the number of items in the directory.
             * 3. Get the directory listing.
             * 4. Setup the realm & file size if the file is in the directory.
             * 5. Open a file.
             * 6. Read a file.
             * 7. Repeat 6 until the end of file.
             * 8. Close a file and free the allocated memory.
             */
            if (pFileReq->error_status == NAFS_SUCCESS)
            {
                pFileReq->state = eFileSystemProcess_EntryCount;
				pFileReq->process_function_cb = Get_FileNameDirectory;
                pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
            }
                              
		    #if (_PRINTF_DEBUG > 0)
            printf("RpFS_OpenFile: >>>>> path = %s file = %s error = %d\n", 
                            pFileReq->dir_path, pFileReq->file_name, pFileReq->error_status);
            #endif
        }

    }
    
    return theResult;
#else /* APP_FILE_SYSTEM */

    return eRpNoError;
#endif
}

/**
 *  The RpHSOpenFileStatus routine is called to determine when the file has
 *  been opened.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber              File id
 * @param   theCompletionStatusPtr     Pointer to the open status which will be set
 *                                     indicating whether open is completed or not.
 *          <list type="unordered">
 *          <item> ACTIONCOMPLETED     Open action is completed </item>
 *          <item> ACTIONPENDING       Waiting for open completion </item>
 *          </list>
 * @param   theFileInfoPtr              Pointer to a file info block which contains
 *                                      file info on a file that has been successfully
 *                                      opened
 * @return  eRpNoError  No error when opening a file
 * @return  <0          Unable to open a file or file system is not initialized
 *
 * @see @link HttpFileAction
 * @see @link FILEINFOPTR
 * @see @link RpErrorCode
 *
 */
RpErrorCode RpHSOpenFileStatus(int theFileNumber, int * theCompletionStatusPtr, FILEINFOPTR theFileInfoPtr)
{
#ifdef APP_FILE_SYSTEM 
    RpErrorCode     theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T   *pFileReq = NULL;
    unsigned int    byteTransferred;
    unsigned int	status;
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL)
    {
        theResult = eRpNoError;
        *theCompletionStatusPtr = ACTIONPENDING;
        
        if (pFileReq->error_status == NAFS_SUCCESS)
        {
            /* check file system io request status */        
            status = Get_IORequestStatus(&pFileReq->io_request, &byteTransferred, 
                                         &pFileReq->error_status);
        }
		else if (IS_FILE_SYSTEM_BUSY(pFileReq->error_status))
		{
			/* try to previous API call again */
            pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
			status = eFileSystemStatus_Wait;
		}
        else
        {   /* error occurs so skip any processing. */
            status = eFileSystemStatus_Error;
        }
    
        if (status == eFileSystemStatus_Complete)
        {   
            /* 
             * if get entry count is ok, allocate memory for it and 
             * list the dir for the file info. Then, does an open.
             */
            switch (pFileReq->state) {
                case eFileSystemProcess_EntryCount:
                    /* let's allocate memory to get directory listing */
                    pFileReq->state = eFileSystemProcess_GetDir;
					pFileReq->process_function_cb = Get_FileNameDirectory;
                    pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
                    break;
                case eFileSystemProcess_GetDir:                
                    /* let's setup group-id, file size & file date
                     * Make sure it's a data file (not a directory)
                     */
                    pFileReq->state = eFileSystemProcess_ListDir;
					pFileReq->process_function_cb = Get_FileName;
                    pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);

                    if (pFileReq->error_status == NAFS_SUCCESS)
                    {	
						/*
						 * If the file access is same as the default file access, 
						 * no need to setup realm for HTTP server.
						 */
						if ((pFileReq->file_info.FileAccess & gRpFSDefaultGroupId) != 0)
	                       	theFileInfoPtr->FileAccess =  kRpPageAccess_Unprotected;
						else
	                       	theFileInfoPtr->FileAccess =  pFileReq->file_info.FileAccess;

                        theFileInfoPtr->FileDate =    pFileReq->file_info.FileDate;
                        theFileInfoPtr->FileSize =    pFileReq->file_info.FileSize;
                        theFileInfoPtr->FileType =    pFileReq->file_info.FileType;
                    }
                    break;
                case eFileSystemProcess_ListDir:
                    /* let's open the file */
                    Get_GroupIdToPermission(pFileReq->file_info.FileAccess, &pFileReq->permissions);
                    pFileReq->state = eFileSystemProcess_OpenFile;
					pFileReq->process_function_cb = NULL;
                    pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
                    break;
                default:
                    /* a file is successfully opened */
                    *theCompletionStatusPtr = ACTIONCOMPLETED;
                    pFileReq->error_status = NAFS_SUCCESS;
                    break;
            }
        }
        else if (status == eFileSystemStatus_Error)
        {   
            /* return ok so that user will be prompted with username/password entry */
            if (pFileReq->error_status == NAFS_NO_READ_PERMISSION)
            {   
                *theCompletionStatusPtr = ACTIONCOMPLETED;   
            } 
            /* if the file is being opened, we should try again */
            else if (pFileReq->error_status == NAFS_FILE_IS_OPEN)
            {
                pFileReq->error_status = Process_FileState(pFileReq, pFileReq->process_function_cb);
            }
            else
            {
                /* returns error. */
                *theCompletionStatusPtr = ACTIONCOMPLETED;   
                theResult = eRpFileOpenError;
            }
        }
    }
    
    return theResult;
#else /* APP_FILE_SYSTEM */
    *theCompletionStatusPtr = ACTIONCOMPLETED;   

    return eRpNoError;
#endif
}


/**
 *  The RpHSReadFile routine is called to start a read into the buffer provided
 *  for the number of bytes in the count.  The read takes place at the current
 *  file byte position with the file byte position being updated after the read
 *  completes.  This call is asynchronous and completes when the read has been
 *  started.  The SfsReadStatus call is used to detect the completion of the read.
 *
 * @external
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber   File id
 * @param   theReadPtr      Pointer to the read buffer 
 * @param   theByteCount    Number of bytes to read
 *
 *                        
 * @return  eRpNoError  The process of read a file has been successfully started.
 * @return  <0          Unable to open a file or file system is not initialized
 *
 * @see @link RpErrorCode
 * @see @link RpHSGetErrorStatus
 *
 */
RpErrorCode RpHSReadFile(int theFileNumber, char * theReadPtr, unsigned long theByteCount)
{
#ifdef APP_FILE_SYSTEM 
    RpErrorCode     theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T   * pFileReq;
    unsigned long   remainingFileSize;
    unsigned long   bytesToRead;
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL) 
    {
        theResult = eRpNoError; 
        pFileReq->state = eFileSystemProcess_ReadFile;
        
        /* if no error is encountered, read the file. But error occurs, skip reading */
        if (pFileReq->error_status == NAFS_SUCCESS)
        {   
            remainingFileSize = pFileReq->file_info.FileSize - pFileReq->file_offset;
            bytesToRead = (remainingFileSize > theByteCount) ? theByteCount : remainingFileSize;
            
            pFileReq->error_status = NAFSread_file(pFileReq->file_handler, pFileReq->permissions, 
                  bytesToRead, pFileReq->file_offset, 
                  theReadPtr, &pFileReq->io_request);
        }              
    }
    return theResult;
#else
    return eRpNoError;
#endif
}

/**
 *  The RpHSReadFileStatus routine is called to determine whether the read
 *  has finished.
 *
 * @external
 * @include "file.h"
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber              File id
 * @param   theCompletionStatusPtr     Pointer to the read status which will be set
 *                                      indicating whether a read is completed or not
 *          <list type="unordered">
 *          <item> READCOMPLETE        Data has been read </item>
 *          <item> ENDOFFILE           Data has been read and it's end of file </item>
 *          <item> READPENDING         The read is outstanding </item>
 *          </list>
 * @param   theBytesReadPtr            Pointer to number of bytes that actually read
 *
 *                        
 * @return  eRpNoError              No error when reading a file
 * @return  eRpFileSystemNotOpen    No File system supported
 * @return  eRpFileReadError        Cannot read a file
 *
 * @see @link RpErrorCode
 * @see @link HttpFileCompletion
 * @see @link RpHSGetErrorStatus
 *
 */
RpErrorCode RpHSReadFileStatus(int theFileNumber, int * theCompletionStatusPtr, unsigned long * theBytesReadPtr)
{
#ifdef APP_FILE_SYSTEM
    RpErrorCode      theResult = eRpFileSystemNotOpen;
    
    RPFS_BUFFER_T    * pFileReq = NULL;
    unsigned int     byteTransferred;
    unsigned int	 status;
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL)
    {
        theResult = eRpNoError;
        *theBytesReadPtr = 0;
        
        if (pFileReq->error_status == NAFS_SUCCESS)
		{
            /* check for file system io request status */
            status = Get_IORequestStatus(&pFileReq->io_request, &byteTransferred, 
                                         &pFileReq->error_status);
		}
		else if (IS_FILE_SYSTEM_BUSY(pFileReq->error_status))
		{
			/* RpHSReadFile should be called to read again  */
			status = eFileSystemStatus_Complete;
			byteTransferred = 0;
		}
        else
            /* error occurs so skip any processing. */
            status = eFileSystemStatus_Error;
        
        if (status == eFileSystemStatus_Complete)
        {   /* data is successfully obtained  */
            pFileReq->file_offset += byteTransferred;
            *theBytesReadPtr = byteTransferred;
            /* see whether we reach the eof */
            if (pFileReq->file_offset >= pFileReq->file_info.FileSize)
                *theCompletionStatusPtr = ENDOFFILE;
            else
                *theCompletionStatusPtr = READCOMPLETE;
                
		    #if (_PRINTF_DEBUG > 0)
            printf("RpFS_ReadFileStatus: index %d io status %d bytes %d\n", 
                        theFileNumber, status, byteTransferred);
            #endif        
        }
        else if (status == eFileSystemStatus_Wait)
            *theCompletionStatusPtr = READPENDING;
        else
        {
            /* no anymore data and returns error */
            *theCompletionStatusPtr = ENDOFFILE;
            theResult = eRpFileReadError;
        }
            
    }                        
    return theResult;
#else /* APP_FILE_SYSTEM */
    *theCompletionStatusPtr = ENDOFFILE;
    *theBytesReadPtr = 0;
    return eRpNoError;

#endif
}

#ifdef APP_FILE_SYSTEM
/**
 * Sets the directory path on the NETOS file system for a specified file number 
 *
 * This function allows caller to set a directory path where a file 
 * will be created or opened. It's used prior to upload or download a file.
 *
 * @external
 * @include "file.h"
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber      File id
 * @param   dir_name           Pointer to directory path
 *
 * @return  eRpNoError  			The directory was successfully set.
 * @return  eRpFileSystemNotOpen    Invalid FileNumber or HTTP File System is not initialized
 *
 * @see @link RpErrorCode
 *
 */
RpErrorCode RpHSSetCurrentDirectory(int theFileNumber, char * dir_name)
{
    RpErrorCode      theResult = eRpFileSystemNotOpen;
    RPFS_BUFFER_T    * pFileReq = NULL;

    NA_ASSERT((int)dir_name);
    NA_ASSERT((strlen(dir_name) < NAFS_DIRECTORY_PATH_SIZE));    
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    
    if (pFileReq != NULL)
    {
        strcpy(pFileReq->dir_path, dir_name);
        theResult = eRpNoError;
    }
    
    return theResult;
}

/**
 * Gets an error status for the given file number 
 *
 * Since this file system interface doesn't return any errors when error occurs 
 * in the NETOS file system, we need to keep track the error status on each file request. 
 * This function should be called in the post-processing function to indicate whether
 * download or upload is successfully completed.
 *
 * @external
 * @include "file.h"
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param   theFileNumber      File id
 * @param   errorStatus        Pointer to the memory where file system 
 *                             error will be returned
 *
 * @return  eRpNoError              The error status was returned successfully in errorStatus
 * @return  eRpFileSystemNotOpen    Invalid FileNumber or HTTP File System is not initialized
 *
 * @see @link RpErrorCode
 * @see @link file_sys_errors
 *
 */
RpErrorCode RpHSGetErrorStatus(int theFileNumber, int * errorStatus)
{
    RpErrorCode      theResult = eRpFileSystemNotOpen;
    RPFS_BUFFER_T    * pFileReq = NULL;

    NA_ASSERT((int)errorStatus);
    
    pFileReq = Get_FileSystemBuffer(theFileNumber);
    if (pFileReq != NULL)
    {
        theResult = eRpNoError;
        *errorStatus = pFileReq->error_status;
    }
    return theResult;
}    

/**
 * Sets a default group id
 *
 * This default group id is used for uploading file into the NETOS file system
 * when access permission information is not available from the logged in 
 * username and password. It's used for directory access permission and 
 * file creation group id.
 * 
 * It's also used for downloading a file from the NETOS file system. If a file 
 * can be accessed by this default group id, no authenication is required.
 *
 * @external
 * @include "file.h"
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @param    group_id       default group-id to be used 
 *		    <list type="unordered">
 *			<item>NAFS_FS_GROUP1 </item>
 *			<item>NAFS_FS_GROUP2 </item>
 *			<item>NAFS_FS_GROUP3 </item>
 *			<item>NAFS_FS_GROUP4 </item>
 *			<item>NAFS_FS_GROUP5 </item>
 *			<item>NAFS_FS_GROUP6 </item>
 *			<item>NAFS_FS_GROUP7 </item>
 *			<item>NAFS_FS_GROUP8 </item>
 *		    </list>
 *
 * @see @link group_masks
 *                        
 */
void RpHSSetDefaultGroupId(unsigned group_id)
{
	
	NA_ASSERT((group_id >= NAFS_FS_GROUP1) && (group_id <= NAFS_FS_GROUP8));

	gRpFSDefaultGroupId = group_id;
}
    
/**
 * Returns the default group id for accessing NETOS file system
 *
 * @external
 * @include "file.h"
 * @category HTTP_Advanced_Server:File_System_Interface
 * @since 6.0 
 *
 * @return   "group_id"         Default group id
 *		    <list type="unordered">
 *			<item>NAFS_FS_GROUP1 </item>
 *			<item>NAFS_FS_GROUP2 </item>
 *			<item>NAFS_FS_GROUP3 </item>
 *			<item>NAFS_FS_GROUP4 </item>
 *			<item>NAFS_FS_GROUP5 </item>
 *			<item>NAFS_FS_GROUP6 </item>
 *			<item>NAFS_FS_GROUP7 </item>
 *			<item>NAFS_FS_GROUP8 </item>
 *		    </list>
 *
 * @see @link group_masks
 *
 */
unsigned RpHSGetDefaultGroupId(void)
{

	return gRpFSDefaultGroupId;
}
#endif /* APP_FILE_SYSTEM */
