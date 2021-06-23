/*
 *
 * $Id: trap.h,v 1.2 2010-03-08 20:09:11 eugene Exp $
 *
 * $Log: trap.h,v $
 * Revision 1.2  2010-03-08 20:09:11  eugene
 * Bug_908: Added NUM_OIDS_IN_TRAP define.
 *
 *
 */

#ifndef TRAP_H__
#define TRAP_H__

#include "../../../h/snmpapi.h"


// Number of IPG specific OID variables sent by the SNMP trap.
#define NUM_OIDS_IN_TRAP   6


void sendTheTrap
( int p_notification_id,     /* 1..20, 1..20 or 1..30 */
  int p_history_table_index, /* 1..1000 */
  struct variable_t *p_pVariables,
  unsigned int p_iVarSize,  /* num bytes of p_pVariables array */
  int p_groupSelector  /* 0 = set, 1 = clear, 2 = event */
  );

void setTheTrapVariables
( int p_history_table_idx,  /* 1..1000 */
  int p_severity,
  unsigned long p_type,     /* 1..70 */
  char * p_time_buf,
  struct variable_t *p_variables);

#endif
