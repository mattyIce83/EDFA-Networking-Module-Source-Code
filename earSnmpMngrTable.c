/*
 * $Id: earSnmpMngrTable.c,v 1.1.1.1 2007-01-08 15:46:32 eugene Exp $
 *
 *  Function: int earSnmpMngrTableWrite (int actionCode,  
 *                   struct varBind *info)
 *
 *  Description:
 *
 *      Set the value of a columnar object in the table
 *      earSnmpMngrTable.
 *
 *      The table of SNMP network managers.
 *
 *      The Fusion agent breaks up the set operation into
 *      four phases.  During each phase, this routine is
 *      called once for each columnar object in the row.
 *      So this routine will be called 40 times to write
 *      a new row that contains 10 columnar objects.  The
 *      current phase is indicated by the value of
 *      actionCode.  It can have these values:
 *
 *      SNMP_SET_RESERVE 1   reserve any buffer space or other
 *                          resources needed for the write.
 *
 *      SNMP_SET_COMMIT  2   copy the new value for the object
 *                          into the local buffer.
 *
 *      SNMP_SET_ACTION     write the value to the variable.
 *
 *      SNMP_SET_FREE       release the buffer or other 
 *                          resources previously allocated.
 *
 *      SNMP_SET_UNDO       restore the object's original value.
 *
 *      info->setTo will contain the new value for the field 
 *      currently being written.
 *      info->val will contain the original value of the field
 *      currently being written.
 *
 *  Parameters:
 *
 *      actionCode  indicates which phase of the write operation
 *                  is being requested.  
 *      info        pointer to structure with information about
 *                  the variable and the value it is being set
 *                  to.  The structure is defined in SNMPIMPL.H.
 *
 *  Variables:
 *
 *      snmpIndices     loaded with indices decoded from OID value
 *                      passed to this function
 *
 *      result          used to store function result
 *
 *      manInfo         pointer to management info about variable
 *
 *      oldIndex        index of row before set
 *
 *      newIndex        index of row after set
 *
 *      row             pointer to row buffer
 *
 *      isInsert        set if there is no row at oldIndex
 *
 *      didWrite        set once the new row has been written
 *
 *      startedUndo     set once we start the undo process
 *
 *      fieldCode       identifies which columnar object in the 
 *                      table is being read
 *
 *      lastActionCode  stores actionCode of previous call
 *
 *      fieldsCommitted used to track which columnar objects in the
 *                      table are being set.  Each bit represents one
 *                      columnar object
 *
 *      fieldsUndone    used to track which fields have been undone
 *
 *      ccode           used to hold return values from management API
 *
 *
 *  Return Values:
 *
 *      SNMP_ERR_NOERROR            
 *      SNMP_ERR_TOOBIG             
 *      SNMP_ERR_NOSUCHNAME         
 *      SNMP_ERR_BADVALUE           
 *      SNMP_ERR_READONLY           
 *      SNMP_ERR_GENERR             
 *      SNMP_ERR_NOACCESS           
 *      SNMP_ERR_WRONGTYPE          
 *      SNMP_ERR_WRONGLENGTH        
 *      SNMP_ERR_WRONGENCODING      
 *      SNMP_ERR_WRONGVALUE         
 *      SNMP_ERR_NOCREATION         
 *      SNMP_ERR_INCONSISTENTVALUE  
 *      SNMP_ERR_RESOURCEUNAVAILABLE
 *      SNMP_ERR_COMMITFAILED       
 *      SNMP_ERR_UNDOFAILED         
 *      SNMP_ERR_AUTHORIZATIONERROR 
 *      SNMP_ERR_NOTWRITABLE        
 *      SNMP_ERR_INCONSISTENTNAME   
 *
 */


#define WANT_MIBMAN

#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "tx_api.h"
#if 1
#include "reg_def.h"
#endif
#include "man_api.h"
#include "asn1.h"
#include "sockapi.h"
#include "tservapi.h"

#include "rtc.h"
#include "../../bsp/platforms/connectme/gpio_def.h"
#include "../../bsp/platforms/connectme/bsp.h"
#include "fbanks.h"

#include "snmpapi.h"
#include "snmpvars.h"

#include "boardParams.h"
#include "man_agnt.h"
#include "MibMan.h"
#include "EARMIB.h"
#include "ipgStrings.h"
#include "telnet.h"
#include "ipgsnmp.h"
#include "MAX_ALARM.h"
#include "semaphores.h"
#include "MAGIC.h"


manTableIndexType MTWnewIndex;
earSnmpMngrTableType *MTWrow = NULL;
snmpIndexType *MTWsnmpIndex = NULL;
manTableIndexType MTWoldIndex;
int MTWisInsert = 0;
int MTWdidWrite = 0;
int MTWstartedUndo = 0;
int MTWlastActionCode = -1;
WORD32 MTWfieldsCommitted[1];
WORD32 MTWfieldsUndone[1];

void root_breakpoint(long unsigned int *);

int earSnmpMngrTableWrite(int actionCode, struct varBind *info)
{
    int result;
    manVarType *manInfo;
    int fieldCode;
    MAN_ERROR_TYPE ccode;
    int status;
    int row_being_looked_at;	/* we can set these every time we're called */
    int col_being_looked_at;	/* we can set this every time we're called */
    int per_row;
    unsigned long mgmt_ip_addr;
    unsigned long hostmask;
    devBoardParamsType nvParams;

    fieldCode = snmpGetFieldCode(info->vp);
    manInfo = snmpGetVariableInfo(info->vp);

    result = SNMP_ERR_NOERROR;

    /*
     * earSnmpMngrTable(1.3.6.1.4.1.19775.1.1.1.21)
     */
    switch (actionCode) {
    case SNMP_SET_RESERVE:
	/* allocate space */
	if (MTWsnmpIndex == NULL) {
	    /* Figure out which index is being referred to. */
	    MTWsnmpIndex =
		snmpExtractIndices(info->vp, info->oid, info->oidLen,
				   IS_WRITE, 13);

	    /* why are we making a copy of this? */
	    MTWoldIndex.snmpIndex =
		(void *) &MTWsnmpIndex->index->value.intValue;

	    MTWoldIndex.wantExact = 1;	/* always use exact index for writes */
	}
	if (MTWrow == NULL) {
	    /* a new row */
	    memset(MTWfieldsCommitted, 0, sizeof(MTWfieldsCommitted));
	    memset(MTWfieldsUndone, 0, sizeof(MTWfieldsUndone));
	    MTWisInsert = 0;
	    MTWdidWrite = 0;
	    MTWstartedUndo = 0;
	    /* a new row  that we plan to insert at the old index */
	    result =
		snmpInitRow(manInfo, &MTWoldIndex,
			    sizeof(earSnmpMngrTableType), &MTWisInsert,
			    (void **) &MTWrow);
	}
	if (result == SNMP_ERR_NOERROR) {
	    /* resize the field part of the row so it is big enough to hold a value */
	    result = snmpAllocateFieldBuffer(actionCode, info, MTWrow);
	}
	/* At this point, we have:
	 * - MTWsnmpIndex alloccated and containing an index to the row we're changing;
	 * - MTWrow allocated and big enough to contain a new value
	 */
	break;
    case SNMP_SET_COMMIT:

	/* 
	 * Now is the time to check for the range of the setTo variable.
	 * Check for stuff like 
	 * - duplicated IP addresses
	 * - ip address with a 0 or 255 in the last octet (hang-on, should't we use the netmask?)
	 * - ip address > 223 and < 2 in the first octet
	 * - if we are changing the status of an already 0 ip address to inService?
	 */
	row_being_looked_at = MTWsnmpIndex->index->value.intValue;
	col_being_looked_at = fieldCode;
	/* now copy the value in info->set into the row buffer */
	result = snmpSetField(actionCode, info, MTWrow);

	customizeReadDevBoardParams(&nvParams);

	/* if this is an ipaddress, then we have a set of tests for it */

	if (col_being_looked_at == earSnmpMngrIpAddrHint) {
            /*
            * Wrinkle: apparently the customer wants to be able to set the 
            * address to 0.0.0.0. We risk this being a duplicate, of course,
            * it is by definition an illegal address, so it will fail those
            * test(s). We don't care. 
            * There seems to be a compiler quirk whereby the condition code
            * isn't generated if info->setTo.intVal = 0 apriori.
            * Wow. This is harder than I thought. 
            */
	    if ((result == SNMP_ERR_NOERROR) && (info->setTo.intVal > 0) ) {

		/* look for duplicates if it is different, then set it */
		hostmask = info->setTo.intVal; /* temp for debugging */
		for (per_row = 0; per_row < BL_COMMUNITY_STRINGS_ARRAY;
		     per_row++) {
		        mgmt_ip_addr = nvParams.mgmtStations[per_row].ipAddress;

                        if (hostmask == mgmt_ip_addr) /* dupe: quit now */ {
				result = SNMP_ERR_WRONGVALUE ;
				break;
			}
		}
	    }
	    /* if not a dupe, then is it sensible? */
	    if ((result == SNMP_ERR_NOERROR) && (info->setTo.intVal > 0) ) {
		/* look for absurd values in the MSB */
		if ((MTWrow->earSnmpMngrIpAddr[0] > 223) ||	/* multicast address */
		    (MTWrow->earSnmpMngrIpAddr[0] < 2) ||	/* silly class A address */
		    (MTWrow->earSnmpMngrIpAddr[0] == 127)) {	/* loopback addresses */
		    result = SNMP_ERR_INCONSISTENTVALUE ;
		}
	    }
            /* if not a dupe, and sensible, is it legal for this network */
	    if ((result == SNMP_ERR_NOERROR) && (info->setTo.intVal > 0) ) {
                /* as long as we only have interface[0] to worry about. */
		hostmask =
		    nvParams.aceConfig.ace_interface_info[0].static_config.
		    subnet_mask;
		hostmask ^= (-1L);
		mgmt_ip_addr = info->setTo.intVal;

                /* check to see if host address bits are all zero, "network" */
		mgmt_ip_addr &= hostmask;
		if (mgmt_ip_addr == 0)  {
		    result = SNMP_ERR_GENERR;
                } else {
                    /* see if host address bits are all one, "broadcast" */
		    hostmask &= (-1L);
		    if (mgmt_ip_addr == hostmask) {
		        result = SNMP_ERR_GENERR;
                    }
                }
	    }
	}

	if (col_being_looked_at == earSnmpMngrRowStatusHint) {
	    /* we can only really change this if the ip address is non-zero */
	    if ((nvParams.mgmtStations[row_being_looked_at - 1].
		 ipAddress == 0)
		&& (MTWrow->earSnmpMngrRowStatus ==
		    /*rfc1903 InService */ 1)) {
		result = SNMP_ERR_GENERR;
	   }
	}

	if (result == SNMP_ERR_NOERROR) {
	    MTWfieldsCommitted[fieldCode / 32] |= 1 << (fieldCode & 0x1f);
	}
	break;
    case SNMP_SET_ACTION:
	row_being_looked_at = MTWsnmpIndex->index->value.intValue;
	col_being_looked_at = fieldCode;

	if (MTWlastActionCode == SNMP_SET_COMMIT) {
	    /*
	     * Time to jam our MTWrow into the database
	     */
	    if (MTWisInsert) {
		/* create MTWrow at the location of MTWoldIndex into the mib */
		memcpy(&MTWnewIndex, &MTWoldIndex, sizeof(MTWnewIndex));
		ccode =
		    manInsertSnmpRow(manInfo->id, &MTWnewIndex, MTWrow,
				     SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */
				     );
		result = snmpErrorLookup[ccode];
	    } else {
		/* overwrite the old place in the mib */
		MTWnewIndex.wantExact = 1;
		/* patch up the row to account for inservice and ipaddress affecting one another */
		if (col_being_looked_at == earSnmpMngrIpAddrHint) {
		    mgmt_ip_addr = MTWrow->earSnmpMngrIpAddr[0];
		    mgmt_ip_addr <<= 8;
		    mgmt_ip_addr |= MTWrow->earSnmpMngrIpAddr[1];
		    mgmt_ip_addr <<= 8;
		    mgmt_ip_addr |= MTWrow->earSnmpMngrIpAddr[2];
		    mgmt_ip_addr <<= 8;
		    mgmt_ip_addr |= MTWrow->earSnmpMngrIpAddr[3];
		    if (mgmt_ip_addr != 0) {
			MTWrow->earSnmpMngrRowStatus = 1;
		    } else {
			MTWrow->earSnmpMngrRowStatus =
			    /* rfc1903 notInService */ 2;
		    }
		}
		if (col_being_looked_at == earSnmpMngrRowStatusHint) {
		    /* if it set to NotInService, then we can zero out the ip address */
		    if (MTWrow->earSnmpMngrRowStatus ==
			/* rfc1903 notInService */ 2) {
			mgmt_ip_addr = 0;
			MTWrow->earSnmpMngrIpAddr[0] = 0;
			MTWrow->earSnmpMngrIpAddr[1] = 0;
			MTWrow->earSnmpMngrIpAddr[2] = 0;
			MTWrow->earSnmpMngrIpAddr[3] = 0;
			/* when we get the community strings going, we are going to have to delete strings */
		    } else {
			mgmt_ip_addr =
			    nvParams.mgmtStations[row_being_looked_at -
						  1].ipAddress;
		    }
		}

		/* man_id_type, manTableIndexType row-to-be-set, manTableIndexType rows-new-position, row, timeout */
		ccode =
		    manSetSnmpRow(manInfo->id, &MTWoldIndex, &MTWoldIndex,
				  MTWrow,
				  SEMAPHORE_TMO /*MAN_TIMEOUT_FOREVER */ );
		if (ccode == MAN_SUCCESS) {
		    MTWdidWrite = TRUE;
		    /* okay, lets push the change through to the nvParms */

		    if (col_being_looked_at > earSnmpMngrIndexHint) {
			/* then there may be reason to write it */
			customizeReadDevBoardParams(&nvParams);
			/* if it is different, then set it */
			if (nvParams.mgmtStations[row_being_looked_at - 1].
			    ipAddress != mgmt_ip_addr) {
			    nvParams.mgmtStations[row_being_looked_at -
						  1].ipAddress =
				mgmt_ip_addr;
			    root_breakpoint(&nvParams.mgmtStations[0].
					    ipAddress);
			    nvParams.telnetMagic = MAGIC;	/* why? */
			    customizeWriteDevBoardParams(&nvParams);

			    /* force it back into sync */
			    SyncSnmpFromNvram(&nvParams);

			    /* Heuristic for trap: if 0, we send "remove" else "add" */
			    if (mgmt_ip_addr == 0) {
				configNotification
				    (SNMPMgmtStationRemoveEvent, "none");
			    } else {
				configNotification(SNMPMgmtStationAddEvent,
						   "none");
			    }

			}
		    }
		}
	    }
	}
	break;
    case SNMP_SET_FREE:
	if (MTWrow != NULL) {
	    snmpFreeOctetStringBuffers(manInfo, MTWrow);
	    free(MTWrow);
	    MTWrow = NULL;
	}
	if (MTWsnmpIndex != NULL) {
	    snmpFreeIndices(MTWsnmpIndex);
	    MTWsnmpIndex = NULL;
	}
	break;
    case SNMP_SET_UNDO:
	if ((MTWrow != NULL) && (!MTWstartedUndo)) {
	    /* we never did commit */
	    snmpFreeOctetStringBuffers(manInfo, MTWrow);
	    free(MTWrow);
	    MTWrow = NULL;
	}
	if (MTWdidWrite) {
	    if (MTWisInsert) {
		/* we inserted a new row. back it out */
		ccode =
		    manDeleteSnmpRow(manInfo->id, &MTWnewIndex,
				     10 /*MAN_TIMEOUT_FOREVER */ );
		if (ccode != MAN_SUCCESS) {
		    result = SNMP_ERR_UNDOFAILED;
		}
		MTWdidWrite = FALSE;
	    } else {
		/* we overwrote the old row, so now we over write it again, but with the saved value */
		if (!MTWstartedUndo) {
		    /* create a row to hold a copy of old value */
		    result =
			snmpInitRow(manInfo, &MTWnewIndex,
				    sizeof(earSnmpMngrTableType), NULL,
				    (void **) &MTWrow);
		    MTWstartedUndo = 1;
		}
		if (result == SNMP_ERR_NOERROR) {
		    /* stretch the row as needed */
		    result =
			snmpAllocateFieldBuffer(actionCode, info, MTWrow);
		}
		if (result != SNMP_ERR_NOERROR) {
		    snmpFreeOctetStringBuffers(manInfo, MTWrow);
		    free(MTWrow);
		    MTWrow = NULL;
		    result = SNMP_ERR_UNDOFAILED;
		    break;
		}
		/* extract the old value of the row */
		snmpSetField(actionCode, info, MTWrow);
		MTWfieldsUndone[fieldCode / 32] |= 1 << (fieldCode & 0x1f);
		if (memcmp
		    (MTWfieldsCommitted, MTWfieldsUndone,
		     sizeof(MTWfieldsUndone)) == 0) {
		    /* and overwrite the row thereby undoing our change */
		    ccode =
			manSetSnmpRow(manInfo->id, &MTWnewIndex,
				      &MTWoldIndex, MTWrow,
				      MAN_TIMEOUT_FOREVER);
		    if (ccode != MAN_SUCCESS) {
			result = SNMP_ERR_UNDOFAILED;
		    }
		    snmpFreeOctetStringBuffers(manInfo, MTWrow);
		    free(MTWrow);
		    MTWrow = NULL;
		    MTWstartedUndo = 0;
		    MTWdidWrite = 0;
		}
	    }
	}
	break;
    }
    MTWlastActionCode = actionCode;

    return result;
}





/*
 *
 *  Function: void *earSnmpMngrTableRead (struct variable *vp,  
 *              oid *name, int *length, int isGet, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get a columnar object from earSnmpMngrTable.
 *
 *      The table of SNMP network managers.
 *
 *      The parameter name contains the OID of the object being read
 *      and a table index.  The parameter length indicates the 
 *      length of the OID and index.  If a GETNEXT operation returns
 *      an object at a different index, then the index information
 *      in name and length must be updated accordingly.
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *              The index for the table lookup is encoded
 *              after the OID of the object.  If the index
 *              of the object read is different than the
 *              index value in name must be updated
 *              accordingly on exit.
 *      length  length of OID with index.  If the index of
 *              the object read is different than what is
 *              specified on input, then the this field 
 *              should be updated with the new length.
 *      isGet   specifies whether the console wants a GET
 *              or GETNEXT.  If set, then console wants
 *              the object that exactly matches the index
 *              encoded in the OID (a GET).  If zero, then
 *              the console wants the first object with an
 *              index greater than the OID value in NAME.
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this table, or
 *              to NULL if the table is read only.
 *
 *  Variables:
 *
 *      resultBuffer    pointer to result.  The result must be
 *                      returned in a persistent buffer
 *
 *      fieldCode       indicates which columnar object in the
 *                      table is being read
 *
 *      snmpIndex       holds index extracted from name parameter
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read the row
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */

earSnmpMngrTableType *row;	/* storage for one row from table */

void *earSnmpMngrTableRead(struct variable *vp, oid * name, int *length,
			   int isGet, int *varLen, setMethod * setVar)
{
    void *resultBuffer = NULL;
    manVarType *manInfo = snmpGetVariableInfo(vp);
    MAN_ERROR_TYPE ccode;
    snmpIndexType *snmpIndex;
    int status;

    *varLen = 0;

    /*
     * earSnmpMngrTable(1.3.6.1.4.1.19775.1.1.1.21)
     */
    snmpIndex = snmpExtractIndices(vp, name, *length, IS_READ, 13);

    if (snmpIndex != NULL) {
        manTableIndexType manIndex;	/* index for management API */
        row = (earSnmpMngrTableType *) malloc(sizeof(earSnmpMngrTableType));

        if (row != NULL) {
            memset(row, 0, sizeof(earSnmpMngrTableType));

            manIndex.wantExact = isGet;
            if (snmpIndex->isNullIndex) {
                manIndex.numericIndex = 0;
                manIndex.snmpIndex = NULL;

                snmpIndex->index->value.intValue = 1;
            } else {
                /*
                 * The raw SNMP indices are stored in snmpIndex.  The
                 * algorithm for using these indices should be described
                 * somewhere in the MIB's RFC.
                 *
                 * This table uses a simple integer index.  Increment
                 * the index if this is a GET-NEXT.  Then set 
                 * oldIndex.snmpIndex to point to it.
                 */
                if (!isGet) {
                    snmpIndex->index->value.intValue++;
                }
                manIndex.snmpIndex =
                    (void *) &snmpIndex->index->value.intValue;
            }

            ccode = snmpReadRow(manInfo, &manIndex, row);

            if (ccode == MAN_SUCCESS) {
                resultBuffer = snmpExtractField(vp, row, varLen, NULL);

                /*
                 * To Do:  Update the index values in snmpIndex to reflect
                 *         the actual index the row is at.  This will be
                 *         encoded into the name parameter by the call to
                 *         snmpEncodeIndices.
                 */

                memcpy(name, vp->name, vp->namelen * sizeof(WORD32));
                *length = snmpEncodeIndices(vp, name, snmpIndex);

                if (snmpFreeBufferLater(resultBuffer) !=
                    SNMP_ERR_NOERROR) {
                    free(resultBuffer);
                    resultBuffer = NULL;
                    *varLen = 0;
                }
            } else {
                resultBuffer = NULL;
                *varLen = 0;
            }
            snmpFreeOctetStringBuffers(manInfo, row);
            free(row);
        } else {  /* if unable to allocate memory for row buffer */
            
            resultBuffer = NULL;
            *varLen = 0;
        }
        snmpFreeIndices(snmpIndex);
    }
    *setVar = (setMethod) earSnmpMngrTableWrite /*vp->writeFn */ ;

    return resultBuffer;
}
