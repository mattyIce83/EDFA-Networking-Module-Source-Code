/*
 *
 *     Module Name: menu.h
 *	   Version: 1.00
 *   Original Date: 10/06/99	
 *	    Author: Dale Lee
 *	  Language: Ansi C
 * Compile Options:
 * Compile defines:
 *	 Libraries:
 *    Link Options:
 *
 *    Entry Points:
 *
 *  Copyrighted (c) by NETsilicon, Inc.  All Rights Reserved.
 *
 *  This program developed by NETsilicon, Inc. is 
 *  provided as reference material for use in implementing a product based
 *  upon the NET+ARM chipset. No other rights to reproduce, use, or disseminate
 *  NETsilicon, Inc. portions of this computer program, whether in 
 *  part or in whole are granted. 
 * 
 *  NETsilicon, Inc. makes no representation or warranties   
 *  with respect to the performance of this computer program, and     
 *  specifically disclaims any responsibility for any damages,        
 *  special or consequential, connected with the use of this program. 
 * 
 *
 * Description.
 * =======================================================================
 * This file contains definitions for telnet menus used in 
 * this application. 
 *
 *
 * Edit Date/Ver   Edit Description
 * ==============  =======================================================
 * LZ   01/06/00   ported to NET_OS from PSOS.
 *
 */
  

#ifndef MENU_H
#define MENU_H

extern int telnet_send_menu(unsigned long);

typedef enum
{
   	TS_MAIN_MENU = 0,
	TS_USER_PASSWORD_MENU,
	TS_SERIAL_NUMBER_MENU,
   	TS_IP_PARAMETERS_MENU,
	TS_IP_ADDRESS_MENU,
	TS_SUBNET_MASK_MENU,
	TS_GATEWAY_ADDRESS_MENU,
   	TS_MAX_MENU
} TELNET_MENU;

/*
 * One of these data structures for each menu item.
 * an array of these make up the telnet menu's as defined
 * in menu.c
 */
struct telnet_menu_t
{
   int (*menu_cmd_func)(char *, int, unsigned long); /* Function to call when receive a command */
   char **menu_items; /* NULL terminated array of menu strings */
   int (*current_value_func)(int, char *);  /* Function to return current value */
};

extern int g_snmp_send_trap_failed;
extern int g_snmp_send_trap_last_result;
extern unsigned long g_snmp_send_trap_timeout;


#endif /* MENU_H */
