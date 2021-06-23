///////////////////////////////////////////////////////////////////////////////
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
//  Filename:           ipgFlash.h 
//
//  Description:  Functions for flashing ConnectMe.
//
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
//      $Log: ipgFlash.h,v $
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


#define FTP_DIRECT_TO_FLASH	0

//............................................................................................


// Function declarations

int ProgramAppToFlash( int bank, char *path, char *filename );
void ftpFlashDownloadTask (void);


