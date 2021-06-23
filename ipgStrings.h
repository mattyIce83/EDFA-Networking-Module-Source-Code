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
//  Filename:           ipgStrings.h 
//
//  Description:
//     Header file for string/parsing functions necessary for 
//     SNMP operations.  
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
//      $Log: ipgStrings.h,v $
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

// String defines
#define MAX_FIELD_LEN 40 

#define CLEI_CODE "ABC1235678"
#define MOT_PROD_NAME "EAR SN 123456"


// Function declarations
void ipgReadLine(char * buf);
int ipgStringToDecimal(char * buf);

