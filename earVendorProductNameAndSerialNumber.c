
/*
 *
 *  Function: void *earVendorProductNameAndSerialNumerRead (struct variable *vp,  
 *              oid *name, int *length, int exact, int *varLen, 
 *              setMethod *setVar)
 *
 *  Description:
 *
 *      Get the value of earVendorProductNameAndSerialNumer.
 *
 *      Vendor product name and serial number
 *
 *  Parameters:
 *
 *      vp      pointer to information about the object 
 *              maintained by the agent.
 *      name    OID for the object supplied by the console.
 *      length  length of OID.
 *      exact   not used
 *      varLen  must be set by this function to the length
 *              of the value returned.
 *      setVar  must be set by this function to the address
 *              of the write function for this object, or
 *              to NULL if this object is read only.
 *
 *
 *  Return Values:
 *
 *      NULL    unable to read object
 *      !NULL   must point to a persistent buffer that 
 *              contains the value of the object.
 *
 */
char char_buffer[MAX_2U_RESPONSE];
void *earVendorProductNameAndSerialNumerRead(struct variable *vp,
					     oid * name, int *length,
					     int isGet, int *varLen,
					     setMethod * setVar)
{
    void *resultBuffer = char_buffer;
    int status;

    /*
     * earVendorProductNameAndSerialNumber(1.3.6.1.4.1.19775.1.1.1.18)
     */

    if (!scalar_handler(vp, name, length, isGet, varLen)) {
	return NULL;
    }

    strcpy(char_buffer, MOT_PROD_NAME);
    *varLen = strlen(char_buffer);
    *setVar = (setMethod) vp->writeFn;

    return resultBuffer;
}
