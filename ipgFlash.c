///////////////////////////////////////////////////////////////////////////////
// $Id: ipgFlash.c,v 1.1.1.1 2006-05-04 14:38:39 eugene Exp $
//
//      IPG Photonics Corporation
//      COPYRIGHT (c) 2004 IPG Photonics Coroporation ALL RIGHTS RESERVED
//  This program may not be reproduced, in whole or in part in any
//  form or any means whatsoever without the written permission of:
//
//  IPG Photonics Corporation
//  50 Old Webster Road
//  Oxford, MA 01540, USA
//
//
//  Filename:           ipgFlash.c
//
//  Description:
//     Updates flash image on ConnectMe device.
//
// 1.00 This file is based heavily on the naftpapp_connect.c module 
//      (Copyrighted (c) by Digi International, Inc.  All Rights Reserved.),
//      which is a derivative of naftpapp.c provided with the Net+OS. 
//      provided with the Digi Developer's kit authored by James Puzzo.  
//      Functionality should remain identical to the orignal except that 
//      new functions may be added to allow images to be uploaded using 
//      an SNMP interface.
//
//
//
//
///////////////////////////////////////////////////////////////////////////////



#include <tx_api.h>
#include <naflash.h>
#include <Flash.h>
#include <sockapi.h>
#include <fservapi.h>
#include <sysAccess.h>
#include "bsp_api.h"
#include <reg_def.h>
#include "ipgFlash.h"

//#include "../../fs_intf/ftpsvrfs.h"
//#include "../../../h/fs_api.h"

#include "ftpsvrfs.h"
#include "ftpsvrfs.c"
#include "fs_api.h"
#include "ipgFiles.h"
#include "watch.h"

//............................................................................................

// Platform specific headers - for connectme_lsk

#include "../../../connect/h/bcomm.h"
#include "../../../connect/h/dgarm_fw_header.h"

//............................................................................................

// Definitions and Declarations. 

#define PRINT_SERIAL    0
#define BUFFERSIZE    128 * 1024
#define DELAY_BEFORE_RESET      (10*NABspTicksPerSecond)
#define NOT_USED        0

//............................................................................................

//  Function Prototypes

static int LocateFlashBank(int bank, int *start_sector, int *end_sector,
			   int *max_size);

static int fs_wait(NAFS_IO_REQUEST_CB * io_request);

static int ftp_flash_data_close(char *argp, unsigned long handle);
static int ftp_flash_retr(char *argp, unsigned long handle);
static int ftp_flash_run(char *argp, unsigned long handle);
static int ftp_flash_stor(char *bufferp, int buflen, char *argp,
			  unsigned long handle);

static int ftp_process_image_header(void);

void ProgramBootLoader(void);

static void reset_global_state(void);
static void reset(ULONG notUsed);

//............................................................................................

//  Global Variables

TX_TIMER resetTimer;

static char errorlog[40];
static char sectorDatap[BUFFERSIZE];

static int currentLength = 0;
static int flashFull = 0;
static int romsize;
static int sectors = 0;
static int successful = 0;

static unsigned int currentSector = 0;
static unsigned int image_type;
static unsigned int writeIndex = 0;

static unsigned long *sectorSizes;
static unsigned long *sectorOffsets;


static dgarm_eos_header_t image_header;


//............................................................................................


/*
 *
 *  Function: void reset_global_state(void)
 *
 *  Description:
 *
 *      This function is used to reset the global state variables
 *      in preparation for another download attempt.
 *
 *  Parameters:
 *
 *      none
 *
 *  Return Values:
 *
 *      none
 *
 */

static void reset_global_state(void)
{
    writeIndex = 0;
    successful = 0;
    memset(&image_header, 0, sizeof(image_header));
    image_type = 0;
}

//-----------------------------------------------------------------------------------

#if FTP_DIRECT_TO_FLASH

//-----------------------------------------------------------------------------------


/*
 *
 *  Function: int ftp_process_image_header(void)
 *
 *  Description:
 *
 *      Determine whether the image header in the global state
 *      is valid or not.  If valid, it prepares the flash for
 *      the entire image write and then writes the header.
 *
 *  Side effects:
 *
 *      This function will execute a "printf" and change the
 *      errorlog in the global state if errors are found.
 *      Will also use a printf to communicate the image type.
 *
 *      "image_type" in the global state is set appropriately.
 *
 *      Flash sectors for the image may be erase.
 *
 *      Image header may be written to flash.
 *
 *  Parameters:
 *
 *      none
 *
 *  Return Values:
 *
 *      NAFTPS_SUCCESS      - Image header is valid
 *      NAFTPS_GENERROR     - Image header format bad or flash operations
 *                            failed
 *      NAFTPS_NOMEMSNDBUF  - Image too large
 *
 */

static int ftp_process_image_header(void)
{
    int start_sector = 0;
    int end_sector = 0;
    int space_avail = 0;
    int retval = NAFTPS_SUCCESS;
    int result;
#if PRINT_SERIAL
    printf("Examining header.\n");
#endif
    if (writeIndex < sizeof(image_header)) {
#if PRINT_SERIAL
	sprintf(errorlog, "Complete header not yet received (%d < %d)\n",
		writeIndex, sizeof(image_header));
	printf("%s", errorlog);
#endif

	retval = NAFTPS_GENERROR;
//        goto done;
	return retval;
    }

    if (memcmp(image_header.image_id, "Digi", 4)) {
#if PRINT_SERIAL
	sprintf(errorlog, "Failed image ID test %02x %02x %02x %02x.\n",
		image_header.image_id[0],
		image_header.image_id[1],
		image_header.image_id[2], image_header.image_id[3]);
	printf("%s", errorlog);
#endif

	retval = NAFTPS_GENERROR;
//        goto done;
	return retval;
    }

    if (image_header.dgarm_fw_header_size < sizeof(image_header)) {
#if PRINT_SERIAL
	sprintf(errorlog, "Unrecognized header length (%d < %d)\n",
		image_header.dgarm_fw_header_size, sizeof(image_header));
	printf("%s", errorlog);
#endif

	retval = NAFTPS_GENERROR;
//        goto done;
	return retval;
    }

    image_type = image_header.image_flags & DGARM_HEADER_FWTYPE_MASK;

    if (image_type == DGARM_HEADER_POST_FLAG) {
	image_type = BCOMM_TYPE_POST;

#if PRINT_SERIAL
	printf("Appears to be a POST image... checking length.\n");
#endif

    } else if (image_type == DGARM_HEADER_EOS_FLAG) {
	image_type = BCOMM_TYPE_EOS;

#if PRINT_SERIAL
	printf("Appears to be an EOS image... checking length.\n");
#endif
    }

    else {

#if PRINT_SERIAL
	sprintf(errorlog, "Unrecognized image type 0x%x\n", image_type);
	printf("%s", errorlog);
#endif
	retval = NAFTPS_GENERROR;
//        goto done;
	return retval;
    }

//writeIndex = NAAppOffsetInFlash;

    start_sector = 0x04;	//bcomm_GetFlashFirstSectorTypeIndex (image_type);
    end_sector = 0x0b;		//bcomm_GetFlashLastSectorTypeIndex (image_type);
    space_avail = end_sector - start_sector;	//bcomm_GetFlashSectorDistance (start_sector, end_sector);

    if (space_avail < image_header.compressed_length) {

#if PRINT_SERIAL
	sprintf(errorlog, "Not enough space for image (0x%x > 0x%x)\n",
		image_header.compressed_length, space_avail);
	printf("%s", errorlog);
#endif

	retval = NAFTPS_NOMEMSNDBUF;
//        goto done;
	return retval;
    }
#if PRINT_SERIAL
    printf("Image appears to be valid...\n");
    printf("Erasing sectors %d to %d.\n", start_sector, end_sector);
#endif

    if (NAFlashErase(start_sector, end_sector) != NAFLASH_SUCCESS) {

#if PRINT_SERIAL
	sprintf(errorlog, "Sector erase failed %d - %d\n",
		start_sector, end_sector);
	printf("%s", errorlog);
#endif

	retval = NAFTPS_GENERROR;
//        goto done;
	return retval;
    }
#if PRINT_SERIAL
    printf("Storing header.\n");
#endif

    result = NAFlashWrite(start_sector, 0, sizeof(image_header),
			  (char *) &image_header, DO_NOT_ERASE);
    if (result != NAFLASH_SUCCESS) {

#if PRINT_SERIAL
	sprintf(errorlog,
		"Failure writing header to flash -- NAFlashWrite returned %d\n",
		result);
	printf("%s", errorlog);
	retval = NAFTPS_GENERROR;
//        goto done;
	return retval;
#endif

    }

  done:
    return retval;
}


//-----------------------------------------------------------------------------------


/*
 *
 *  Function: void reset(ULONG notUsed)
 *
 *  Description:
 *
 *      This function is used to reset the system after new firmware has been
 *      written to flash.  It is schedule to run from a timer after the
 *      flash download completes.  It runs off of a timer to give the FTP
 *      server time to shut down connections gracefully.
 *
 *  Parameters:
 *
 *      notUsed     parameter passed by ThreadX which isn't used
 *
 *  Return Values:
 *
 *      does not return
 *
 */

static void reset(ULONG notUsed)
{
    NA_UNUSED_PARAM(notUsed);
    customizeReset();
}


//-----------------------------------------------------------------------------------


/*
 * ftp_flash_stor
 *
 * This routine is the handler for the FTP server STOR command.
 * This routine collects 32K worth of image in g_flashbuf and then
 * writes flash memory at the current sector.
 *
 * INPUT
 *
 *    bufferp    Contains data read by the FTP server.
 *    buflen     Specifies length of bufferp data.
 *    argp       Specifies file name being read.
 *    handle     Handle to FTP server client connection object. 
 */
static int ftp_flash_stor(char *bufferp, int buflen, char *argp,
			  unsigned long handle)
{
    int templen, k;
    int result;

    /*  our protocol is to verify they are trying to download rom.bin   */
    if ((strcmp(argp, "rom.bin") == 0) || (strcmp(argp, "romzip.bin") == 0)
	|| (strcmp(argp, "image.bin") == 0)) {
	if ((strcmp(argp, "image.bin") == 0) && (currentSector == 0)) {
	    writeIndex = NAAppOffsetInFlash;
	    for (k = currentSector; k < sectors; k++) {
		if (sectorSizes[k] <= writeIndex) {
		    writeIndex -= sectorSizes[k];
		    if (k == (sectors - 1)) {
			/* No more flash memory to write, so set flag */
			flashFull = 1;
		    }
		} else {
		    currentSector = k;
		    break;
		}
	    }
	    if (writeIndex != 0) {
		printf
		    ("_NAAppOffsetInFlash in the linker customizatino file is set incorrectly.  \n");
		printf
		    ("The application image must start on a sector boundary.\n");
		successful = 0;
		return (-2);
	    }
	}

	if (flashFull) {
	    /* If all flash memory has been written,
	       clear flag and return -2 */
	    printf
		("Image size exceeds flash memory size, and flash memory has been overwritten.\n");
	    successful = 0;
	    return (-2);
	}
	if ((currentLength + buflen) > romsize) {
	    /* If data received exceeds available flash memory,
	       clear flag and return -2 */
	    printf
		("Image size exceeds flash memory size, but flash memory has not been overwritten.\n");
	    successful = 0;
	    return (-2);
	}
	/*  copy this file into our buffer  */
	if (currentLength + buflen > BUFFERSIZE) {
	    templen = BUFFERSIZE - currentLength;
	} else {
	    templen = buflen;
	}

	memcpy((char *) &sectorDatap[currentLength], bufferp, templen);

	currentLength += templen;

	/* Write the data to Flash in 128K chunks */
	/* Smaller data chunks decrease performance but require a smaller buffer */
	while (currentLength == BUFFERSIZE) {
	    /* Write to a valid Flash sector */
	    if (currentSector < sectors) {
		result =
		    NAFlashWrite(currentSector, writeIndex, BUFFERSIZE,
				 sectorDatap, ERASE_AS_NEEDED);
		if (result) {
		    printf("ftp_flash_stor NAFlashWrite() error %d.\n",
			   result);
		    sprintf(errorlog, "FLASH WRITE ERR:%2d\n", result);
		    successful = 0;
		    return result;
		}

		/*  save the extra data not written to flash -- for the next NAFlashWrite() */
		currentLength = 0;
		if (buflen > templen) {
		    if ((buflen - templen) >= BUFFERSIZE) {
			memcpy((char *) &sectorDatap[currentLength],
			       (char *) &bufferp[templen], BUFFERSIZE);
			templen += BUFFERSIZE;
			currentLength = BUFFERSIZE;
		    } else {
			memcpy((char *) &sectorDatap[currentLength],
			       (char *) &bufferp[templen],
			       buflen - templen);
			currentLength = buflen - templen;
		    }
		}
		writeIndex += BUFFERSIZE;

		/* Compute the sector and sector offset to start writing next */
		for (k = currentSector; k < sectors; k++) {
		    if (sectorSizes[k] <= writeIndex) {
			writeIndex -= sectorSizes[k];
			if (k == (sectors - 1)) {
			    /* No more flash memory to write, so set flag */
			    flashFull = 1;
			}
		    } else {
			currentSector = k;
			break;
		    }
		}
	    }
	}
	successful = 1;
    }
    return 0;
}


//-----------------------------------------------------------------------------------


/* 
 * ftp_flash_retr
 *
 * This routine is for client to get the ID of the ftp server,
 * or the error log.
 *
 * INPUTS
 *
 *    argp       Points to string containing file to be created at client.
 *    handle     Used to access FTP server client connection object. 
 *
 */
static int ftp_flash_retr(char *argp, unsigned long handle)
{
    char buffer[64];


    successful = 0;

    if (strcmp(argp, "id") == 0) {
	errorlog[0] = '\0';

	if (romsize == 0x100000) {
	    sprintf(buffer, "NETARM-FTPDL-V100");
	} else if (romsize == 0x200000) {
	    sprintf(buffer, "NETARM-FTPDL-V200");
	} else {
	    sprintf(buffer, "NETARM-FTPDL-V400");
	}
    } else if (strcmp(argp, "errmsg") == 0) {
	sprintf(buffer, errorlog);
    }

    send(FSHandleToSocket(handle), buffer, strlen(buffer), 0);

    return 0;
}


//-----------------------------------------------------------------------------------


/* 
 * ftp_flash_run
 *
 * This routine is used run the rom image after a successful download
 *
 * INPUTS
 *
 *    argp       Points to string containing file to be created at client.
 *    handle     Used to access FTP server client connection object. 
 *
 */
static int ftp_flash_run(char *argp, unsigned long handle)
{
    int result;


    if (successful) {		/* the whole rom.bin is downloaded successfully */
	if (currentLength) {
	    /* Write to a valid Flash sector */
	    if (currentSector < sectors) {
		result =
		    NAFlashWrite(currentSector, writeIndex, currentLength,
				 sectorDatap, ERASE_AS_NEEDED);
		if (result) {
		    printf("ftp_flash_run NAFlashWrite() error %d.\n",
			   result);
		    sprintf(errorlog, "FLASH WRITE ERR:%2d\n", result);

		    /* Reset variables to allow downloading again */
		    currentSector = 0;
		    writeIndex = 0;
		    currentLength = 0;
		    flashFull = 0;
		    successful = 0;
		    return result;
		}
	    }
	}

	printf("FTP: Flash download complete.\n");
	if (tx_timer_create
	    (&resetTimer, "Reset Timer", reset, NOT_USED,
	     DELAY_BEFORE_RESET, NOT_USED,
	     TX_AUTO_ACTIVATE) == TX_SUCCESS) {
	    printf("Resetting system in %d seconds.\n",
		   DELAY_BEFORE_RESET / NABspTicksPerSecond);
	} else {
	    printf("Unable to automatically reset system.\n");
	}

    }
    // Reset variables to allow downloading again 
    currentSector = 0;
    writeIndex = 0;
    currentLength = 0;
    flashFull = 0;
    successful = 0;

    return 0;
}


//-----------------------------------------------------------------------------------


/* 
 * ftp_flash_data_close
 *
 * This routine is used to determine success or failure after
 * an attempted download.
 *
 * INPUTS
 *
 *    argp       Points to string containing file to be created at client.
 *    handle     Used to access FTP server client connection object. 
 *
 */
static int ftp_flash_data_close(char *argp, unsigned long handle)
{
    NA_UNUSED_PARAM(argp);
    NA_UNUSED_PARAM(handle);

// SEMLEK: There is currently no mechanism for providing details of 
//         flash download.  Perhaps status bit or other mechanism 
//         will be introduced later.
#if PRINT_SERIAL
    if (successful) {		/* the whole image is downloaded successfully */
	printf("FTP: Flash download complete.\n");
    } else {
	printf("FTP: Flash download failed.\n");
    }
#endif

    return 0;
}


//-----------------------------------------------------------------------------------


/*
 *
 *  Function: void ftpFlashDownloadTask (void)
 *
 *  Description:
 *
 *      This routine setups up the RAM based FTP Server.
 *
 *  Parameters:
 *
 *      none
 *
 *  Return Values:
 *
 *      none
 *
 */
//void ftp_init_flash_download (void)

void ftpFlashDownloadTask(void)
{
    /*
     * Initialize the FTP server internal data structures. The following
     * allocates control data structures for two concurrent users.
     */
    FSInitialize(2);
    FSRegisterSTOR(ftp_flash_stor);
    FSRegisterRETR(ftp_flash_retr);
    FSRegisterControlClose(ftp_flash_run);

    /*  Add Username (none) with no password to the System Access Database   */
    NAsetSysAccess(NASYSACC_ADD, "(none)", "", NASYSACC_LEVEL_RW, NULL);

    /* determine rom size here, via Mask */
    /*romsize = physical_size ((*(unsigned long *)0xffc00014 & 0xfffff000)); */
    romsize = customizeGetCSSize(NARM_CSAR0_ADDR, NARM_CSOR0_ADDR);

    /* determine the flash physical sectors and sector sizes */
    sectors = NAFlashSectors();
    sectorSizes =
	(unsigned long *) malloc(sizeof(unsigned long) * sectors);
    if (NAFlashSectorSizes(sectorSizes)) {
	printf("FTP Server not started\n");
	printf("Unable to get Flash sector size\n");
	printf("Check if flash is enabled\n");
	return;
    }


    /*  Actually start the FTP server.  */
    FSStartServer();

    printf("RAM based FTP Server ready.\n");

    NALedBlinkGreen(2, 1);
}

    /*  Add Username (none) with no password to the System Access Database   */
//    NAsetSysAccess (NASYSACC_ADD, "(none)", "", NASYSACC_LEVEL_RW, NULL);



//-----------------------------------------------------------------------------------

#else				// not FTP_DIRECT_TO_FLASH

//#ifdef FTP_DIRECT_TO_FLASH

//-----------------------------------------------------------------------------------

/*
 *
 *  Function: void ftp_server_app_init (void)
 *
 *  Description:
 *
 *      This routine starts the File system and the FTP server.
 *
 *  Parameters:
 *
 *      none
 *
 *  Return Values:
 *
 *      none
 *
 */


void ftp_server_app_init()
{

    if (FSInitialize(2) == NAFTPS_SUCCESS)	// initialize ftp server with number of users
    {
	if (FSInstallFileSystem("FLASH0/") == NAFTPS_SUCCESS) {	// determine flash physical sectors & sector sizes
	    sectors = NAFlashSectors();
	    sectorSizes =
		(unsigned long *) malloc(sizeof(unsigned long) * sectors);
	    sectorOffsets =
		(unsigned long *) malloc(sizeof(unsigned long) * sectors);
	    if (NAFlashSectorSizes(sectorSizes)
		|| NAFlashSectorOffsets(sectorOffsets)) {
		printf("FTP Server not started\n");
		printf("Unable to get Flash sector size\n");
		printf("Check if flash is enabled\n");
		return;
	    }
	    if (FSStartServer() == NAFTPS_SUCCESS)	// start the ftp server
	    {
		printf("File System FTP Server ready.\n");
		NALedBlinkGreen(2, 1);
	    } else {
		printf("FTP Server not started\n");
	    }
	} else {
	    printf("File system install failed\n");
	}
	FSInstallFileSystem("RAM0/");	// Neil Lefebvre 120505
    } else {
	printf("File system initialization failed\n");
    }
}


//-----------------------------------------------------------------------------------


#define MAX_BOOTLOADER_SIZE		0x10000


void WriteBootLoader(void)
{
    NAFS_IO_REQUEST_CB io_request;

    unsigned int permission, request_type, status;

    int error_code, rc;

    unsigned bytes_transferred, file_handler;

//..................................................................................


    permission = NAFS_FS_GROUP1 << READ_BIT_OFFSET;

    rc = NAFSinit_io_request_cb(&io_request, NULL, 0);	// set up io request for file system 

    if (!rc) {
	rc = NAFSopen_file("FLASH0/",
			   "rom.bin",
			   &file_handler, permission, &io_request);
	if (!rc) {
	    do {
		tx_thread_sleep(5);
		rc = NAFSio_request_cb_status(&io_request,
					      &request_type,
					      &status,
					      &error_code,
					      &bytes_transferred);
		if (rc != NAFS_SUCCESS)
		    break;

		rc = error_code;
	    } while ((status == NAFS_IO_REQUEST_PENDING)
		     || (status == NAFS_IO_REQUEST_PROCESSING));

	    if (!rc) {
		rc = NAFSread_file(file_handler,
				   permission,
				   MAX_BOOTLOADER_SIZE,
				   0x0, sectorDatap, &io_request);
		if (!rc) {
		    do {
			tx_thread_sleep(5);
			rc = NAFSio_request_cb_status(&io_request,
						      &request_type,
						      &status,
						      &error_code,
						      &bytes_transferred);
			if (rc != NAFS_SUCCESS)
			    break;

			rc = error_code;
		    } while ((status == NAFS_IO_REQUEST_PENDING)
			     || (status == NAFS_IO_REQUEST_PROCESSING));

		    if ((!rc) || (rc == NAFS_END_OF_FILE)) {
			NAFlashWrite(0, 0, MAX_BOOTLOADER_SIZE,
				     sectorDatap, ERASE_AS_NEEDED);
		    }
		}
	    }

	    rc = NAFSclose_file(file_handler, permission, &io_request);
	    do {
		tx_thread_sleep(5);
		rc = NAFSio_request_cb_status(&io_request,
					      &request_type,
					      &status,
					      &error_code,
					      &bytes_transferred);
		if (rc != NAFS_SUCCESS)
		    break;

		rc = error_code;
	    } while ((status == NAFS_IO_REQUEST_PENDING)
		     || (status == NAFS_IO_REQUEST_PROCESSING));

	}
    }
}

//-----------------------------------------------------------------------------------


/*
 *
 *  Function: int fs_wait( NAFS_IO_REQUEST_CB *io_request )
 *
 *  Description:
 *
 *      Waits for the NAFS file system operation to complete
 *
 *  Return Values:
 *
 *      any error code returned from the FS call
 *
 */
static int fs_wait(NAFS_IO_REQUEST_CB * io_request)
{
    unsigned int request_type, status, bytes_transferred;
    int error_code, rc;

    do {
	tx_thread_sleep(5);
	rc = NAFSio_request_cb_status(io_request, &request_type, &status,
				      &error_code, &bytes_transferred);
	if (rc != NAFS_SUCCESS)
	    break;

	rc = error_code;
    } while ((status == NAFS_IO_REQUEST_PENDING
	      || status == NAFS_IO_REQUEST_PROCESSING));

    return rc;
}


//-----------------------------------------------------------------------------------


/*
 *
 *  Function: int ProgramBootLoader( void )
 *
 *  Description:
 *
 *      Programs the file /FLASH0/rom.bin into the bootloader area of flash
 *
 *  Note:  This shouldn't be used in the final release, in fact the boot loader 
 *         sectors should be locked so that they can never be reprogrammed.  But
 *         this will be helpful during development.
 */

void ProgramBootLoader(void)
{
    unsigned int permission;

    unsigned file_handler;

    int rc, size;

    NAFS_IO_REQUEST_CB io_request;


    permission = NAFS_FS_GROUP1 << READ_BIT_OFFSET;

    rc = NAFSinit_io_request_cb(&io_request, NULL, 0);	// set up io request for file system
    if (!rc) {
	rc = NAFSopen_file("FLASH0/", "rom.bin", &file_handler, permission,
			   &io_request);
	if (!rc)
	    rc = fs_wait(&io_request);
	if (!rc) {
	    rc = NAFSopen_file_size(file_handler, permission, &size,
				    &io_request);
	    if (!rc)
		rc = fs_wait(&io_request);
	    if (!rc && size < NABootloaderSizeInFlash) {
		rc = NAFSread_file(file_handler, permission, size, 0x0,
				   sectorDatap, &io_request);
		if (!rc)
		    rc = fs_wait(&io_request);
		if (rc == NAFS_END_OF_FILE
		    && size == io_request.bytes_transferred) {
		    if (NAFlashWrite
			(0, 0, size, sectorDatap,
			 ERASE_AS_NEEDED) == NAFLASH_SUCCESS)
			printf
			    ("rom.bin programmed into boot loader area of flash.\n");
		    else
			printf("Error: failed to write to flash.\n");
		} else
		    printf("Error: could not read rom.bin.\n");
	    } else
		printf("Error: rom.bin exceeds max size.\n");

	    rc = NAFSclose_file(file_handler, permission, &io_request);
	    rc = fs_wait(&io_request);
	} else
	    printf("Error: failed to open /FLASH0/rom.bin.\n");
    } else
	printf("File system error.\n");
}


//-----------------------------------------------------------------------------------


/*
 *  Function: int ProgramAppToFlash( int bank, char *filename )
 *
 *  Description:
 *
 *      Programs an application image to one of the two application banks in the
 *      application area of flash
 *
 */

int ProgramAppToFlash(int bank, char *path, char *filename)
{
    int bytesWritten, copySize, max_size, rc, size;

    int current_sector = -1;
    int end_sector = -1;
    int start_sector = -1;

    unsigned int permission;

    unsigned file_handler;

    NAFS_IO_REQUEST_CB io_request;


    permission = NAFS_FS_GROUP1 << READ_BIT_OFFSET;


    if (bank < 1 || bank > 2)	// check for valid bank number
    {
	printf("Invalid bank number.\n");
	return -1;
    }
    // Determine flash sectors to put the image in
    rc = LocateFlashBank(bank, &start_sector, &end_sector, &max_size);
    if (rc)
	return rc;

    rc = NAFSinit_io_request_cb(&io_request, NULL, 0);	// set up io request for file system
    if (rc) {
	printf("File system error.\n");
	return rc;
    }
    // Open the image file and verify its size
//    rc = NAFSopen_file( "FLASH0/", filename, &file_handler, permission, &io_request );
    rc = NAFSopen_file(path, filename, &file_handler, permission, &io_request);	// 120505
    if (rc || fs_wait(&io_request)) {
//        printf( "Error: failed to open /FLASH0/%s.\n", filename );
	printf("Error: failed to open %s%s.\n", path, filename);	// 120505
	return rc;
    }

    rc = NAFSopen_file_size(file_handler, permission, &size, &io_request);
    if (rc || fs_wait(&io_request)) {
	printf("Error reading file size.");
	return rc;
    }

    if (size > max_size) {
	printf
	    ("Error: Application image too large; exceeds flash bank size.");
	return -1;
    }

    rc = NAFlashErase(start_sector, end_sector);	// Erase the flash sectors before programming them
    if (rc) {
	return rc;
    }


    current_sector = start_sector;	// Write the image to flash, one sector at a time
    bytesWritten = 0;
    while (bytesWritten < size) {
	/* Copy a full sector from the file to flash, unless there 
	   is less than one sector left */
	if (size - bytesWritten > sectorSizes[current_sector])
	    copySize = sectorSizes[current_sector];
	else
	    copySize = size - bytesWritten;

	/* Read from the file */
	rc = NAFSread_file(file_handler, permission, copySize,
			   bytesWritten, sectorDatap, &io_request);
	if (!rc)
	    rc = fs_wait(&io_request);
	if (rc && rc != NAFS_END_OF_FILE) {
	    printf("Error reading file.");
	    return rc;
	}

	/* Write the sector in flash */
	NAFlashWrite(current_sector, 0, copySize, sectorDatap,
		     DO_NOT_ERASE);

	/* Update counters */
	current_sector++;
	bytesWritten += copySize;
    }				/* end of while */

    /* close the file */
    rc = NAFSclose_file(file_handler, permission, &io_request);
    rc = fs_wait(&io_request);

    if (!rc)
	printf("%s programmed into application bank %d.\n", filename,
	       bank);
    else
	printf("Error closing file.");

    return (0);
}

//-----------------------------------------------------------------------------------


/*
 *  Function: int LocateFlashBank
 *
 *  Description:
 *
 *      Determines the start and end sectors of flash to be used for a virtual 
 *      'bank' for an application image
 *
 */

static int LocateFlashBank(int bank, int *start_sector, int *end_sector,
			   int *max_size)
{
    int avail_sectors;
    int i;

    *start_sector = -1;
    *end_sector = -1;


    /* find flash sector at start and end of application banks area */
    for (i = 0; i < sectors; i++) {
	if (*start_sector == -1) {
	    if (sectorOffsets[i] >= NAAppOffsetInFlash) {
		*start_sector = i;
	    }
	}

	if (*end_sector == -1) {
	    if (sectorOffsets[i] ==
		(NAAppOffsetInFlash + NAAppMaxSizeInFlash)) {
		*end_sector = i - 1;
	    } else if (sectorOffsets[i] >
		       (NAAppOffsetInFlash + NAAppMaxSizeInFlash)) {
		*end_sector = i - 2;
	    }
	}
    }

    if (*start_sector == -1 || *end_sector == -1) {
	printf("Error locating flash sectors.\n");
	return -1;
    }

    /* Select the apprpriate half of the application area for this bank */
    avail_sectors = *end_sector - *start_sector + 1;

    if (bank == 1)
	*end_sector = *end_sector - avail_sectors / 2;
    else if (bank == 2)
	*start_sector = *start_sector + avail_sectors / 2;

    *max_size =
	sectorOffsets[*end_sector + 1] - sectorOffsets[*start_sector];

    return 0;
}


//-----------------------------------------------------------------------------------

#endif				// FTP_DIRECT_TO_FLASH
