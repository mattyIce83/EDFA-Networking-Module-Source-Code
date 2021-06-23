/*****************************************************************************
* Copyright (c) 2003 Digi International Inc., All Rights Reserved
* 
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
* 
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/

/*
 *  bcomm_flash.c -- provides an API to extract information about
 *                   the layout of the flash from the BIOS
 *                   communication area (i.e. bcomm). 
 */

//#include "bcomm.h"

/************************************************************************
 *  bcomm_GetFlashFirstSectorTypeAddress()
 ************************************************************************
 * This function is used to get the address of the first sector with a 
 * given sector type.
 *
 * Inputs:
 * flash_base - is the base address of flash.
 *
 * sector_type - indicates the type of sector that is being searched for.
 *
 * Returns:
 * On success this function returns the address of the first sector with 
 * the given sector type.  On failure this function returns 0.
 *
 ************************************************************************/
unsigned long bcomm_GetFlashFirstSectorTypeAddress(unsigned long
						   flash_base,
						   unsigned char
						   sector_type)
{
    unsigned int sector_index = 0;
    unsigned int bcomm_index = 0;
    unsigned int sector_count;
    unsigned int sector_size;
    unsigned long flash_address = flash_base;

    while ((sector_index < bcomm.bc_TFSectors) &&
	   (bcomm_index < BCOMM_NUM_FSECTORS)) {
	if (bcomm.bc_FSTypes[bcomm_index] == sector_type) {
	    return flash_address;
	}

	sector_count = bcomm.bc_FSectors[bcomm_index] & BCOMM_COUNT_MASK;
	/* assert(sector_count != 0); */
	sector_size = bcomm.bc_FSectors[bcomm_index] & ~BCOMM_COUNT_MASK;

	flash_address += (sector_count * sector_size);
	sector_index += sector_count;
	bcomm_index++;
    }

    /* sector_type not found */
    return 0;
}

/************************************************************************
 * bcomm_GetFlashFirstSectorTypeIndex()
 ************************************************************************
 * This function is used to get the index of the first sector of a given
 * sector type.
 * 
 * Inputs:
 * sector_type - indicates the type of sector that is being searched for. 
 *
 * Returns:
 * On success this function returns the first sector index of a given
 * sector type.  On failure this function returns a negative value.
 *
 ************************************************************************/
int bcomm_GetFlashFirstSectorTypeIndex(unsigned char sector_type)
{
    unsigned int sector_index = 0;
    unsigned int bcomm_index = 0;

    while ((sector_index < bcomm.bc_TFSectors) &&
	   (bcomm_index < BCOMM_NUM_FSECTORS)) {
	if (bcomm.bc_FSTypes[bcomm_index] == sector_type) {
	    return ((int) sector_index);
	}

	sector_index += bcomm.bc_FSectors[bcomm_index] & BCOMM_COUNT_MASK;

	bcomm_index++;
    }

    /* sector_type not found */
    return -1;
}

/************************************************************************
 * bcomm_GetFlashLastSectorTypeIndex()
 ************************************************************************
 * This function is used to get the index of the last sector of a given
 * sector type.
 * 
 * Inputs:
 * sector_type - indicates the type of sector that is being searched for. 
 *
 * Returns:
 * On success this function returns the last sector index of a given
 * sector type.  On failure this function returns a negative value.
 *
 ************************************************************************/
int bcomm_GetFlashLastSectorTypeIndex(unsigned int sector_type)
{
    unsigned int sector_index = 0;
    unsigned int sector_count;
    unsigned int bcomm_index = 0;
    int first_found = 0;

    while ((sector_index < bcomm.bc_TFSectors) &&
	   (bcomm_index < BCOMM_NUM_FSECTORS)) {
	if (bcomm.bc_FSTypes[bcomm_index] == sector_type) {
	    if (!first_found) {
		/* found the first sector */
		first_found = 1;
	    }
	} else if (first_found) {
	    /* found the last sector */
	    break;
	}

	sector_count = bcomm.bc_FSectors[bcomm_index] & BCOMM_COUNT_MASK;
	/* assert(sector_count != 0); */
	sector_index += sector_count;

	bcomm_index++;
    }

    if (first_found) {
	return ((int) (sector_index - 1));
    }

    /* sector_type not found */
    return -1;
}

/************************************************************************
 * bcomm_GetFlashSectorAddress 
 ************************************************************************
 * This function is used to get the flash address of a given sector index.
 *
 * Inputs:
 *
 * flash_base - is the base address of flash.
 *
 * sector_index - is the index of the sector to get the address for.
 * 		: 
 * Returns:
 * On success this function returns the address of the given sector.  0 is
 * returned on failure.
 *
 ************************************************************************/
unsigned long bcomm_GetFlashSectorAddress(unsigned long flash_base,
					  unsigned int sector_index)
{
    unsigned int i = 0;
    unsigned int bcomm_index = 0;
    unsigned int sector_count;
    unsigned int sector_size;
    unsigned long flash_address = flash_base;

    while ((i < bcomm.bc_TFSectors) && (bcomm_index < BCOMM_NUM_FSECTORS)) {
	sector_count = bcomm.bc_FSectors[bcomm_index] & BCOMM_COUNT_MASK;
	/*assert(sector_count != 0); */

	sector_size = bcomm.bc_FSectors[bcomm_index] & ~BCOMM_COUNT_MASK;

	if ((i + sector_count) > sector_index) {
	    while (i < sector_index) {
		flash_address += sector_size;
		i++;
	    }
	    /* sector address found */
	    return flash_address;
	}

	flash_address += (sector_count * sector_size);
	i += sector_count;
	bcomm_index++;
    }

    /* sector address not found */
    return 0;
}

/************************************************************************
 * bcomm_GetFlashSectorLength() 
 ************************************************************************
 * This function is used to get the length of a particular sector.
 *
 * Inputs:
 *
 * sector_index - is the index of the sector to get the length for.
 *
 * Returns:
 * On success this function returns the length of the given sector.  0 is
 * returned on failure.
 *
 ************************************************************************/
unsigned long bcomm_GetFlashSectorLength(unsigned int sector_index)
{
    unsigned int i = 0;
    unsigned int bcomm_index = 0;
    unsigned int sector_count;

    while ((i < bcomm.bc_TFSectors) && (bcomm_index < BCOMM_NUM_FSECTORS)) {
	sector_count = bcomm.bc_FSectors[bcomm_index] & BCOMM_COUNT_MASK;
	/*assert(sector_count != 0); */

	if ((i + sector_count) > sector_index) {
	    return bcomm.bc_FSectors[bcomm_index] & ~BCOMM_COUNT_MASK;
	}

	i += sector_count;
	bcomm_index++;
    }

    /* length not found */
    return 0;
}

/************************************************************************
 * bcomm_GetFlashAddressSectorAndOffset()
 ************************************************************************
 * This function is used to get the sector and offset into the sector for
 * a given flash address.
 *
 * Inputs:
 *
 * flash_base - is the base address of flash.
 *
 * flash_address - the flash address at which the sector and offset will
 * be determined from.
 *
 * Outputs:
 *
 * sector_index - is the index of the sector that the flash_address 
 * resides in.
 *
 * offset - is the offset from the start of the sector.
 * 
 * Returns:
 * On success this function returns 0 and fills in the sector_index and
 * offset parameters.  On failure this function returns a negative value.
 *
 ************************************************************************/
int bcomm_GetFlashAddressSectorAndOffset(unsigned long flash_base,
					 unsigned long flash_address,
					 unsigned int *sector_index,
					 unsigned int *offset)
{
    unsigned long base_offset;
    unsigned int i = 0;
    unsigned int bcomm_index = 0;
    unsigned int sector_count;
    unsigned int sector_size;
    unsigned int total_length = 0;
    unsigned int length;

    /* assert(flash_base < flash_address); */

    base_offset = flash_address - flash_base;

    /* assert(base_offset < (bcomm.bc_flash_size * 1024)); */

    while ((i < bcomm.bc_TFSectors) && (bcomm_index < BCOMM_NUM_FSECTORS)) {
	sector_count = bcomm.bc_FSectors[bcomm_index] & BCOMM_COUNT_MASK;
	/*assert(sector_count != 0); */

	sector_size = bcomm.bc_FSectors[bcomm_index] & ~BCOMM_COUNT_MASK;
	/*assert(sector_size != 0); */

	length = (sector_count * sector_size);

	if ((total_length + length) > base_offset) {
	    /* found the address area */
	    while ((total_length + sector_size) <= base_offset) {
		total_length += sector_size;
		i++;
	    }

	    /* return the sector_index and offset */
	    *sector_index = i;
	    *offset = base_offset - total_length;

	    return 0;
	}

	i += sector_count;
	total_length += length;
	bcomm_index++;
    }

    /* invalid address */
    return -1;
}


/************************************************************************
 * bcomm_GetFlashSectorDistance() 
 ************************************************************************
 * This function is used to get the number of bytes from the starting sector
 * specified to the ending sector specified.  Note the bytes returned includes
 * the bytes of the ending sector.
 *
 * Inputs:
 *
 * start_index - is the index of the starting sector.
 *
 * end_index - is the index of the ending sector.
 * 
 * Returns:
 * On success this function returns the length in bytes between the starting
 * and ending sectors including the ending sector.  0 is returned on failure.
 *
 ************************************************************************/
unsigned int bcomm_GetFlashSectorDistance(unsigned int start_index,
					  unsigned int end_index)
{
    unsigned int start_address;
    unsigned int end_address;
    unsigned int end_length;

    if ((start_index > end_index) ||
	(start_index >= bcomm.bc_TFSectors) ||
	(end_index >= bcomm.bc_TFSectors)) {
	return 0;
    }

    start_address = bcomm_GetFlashSectorAddress(0, start_index);
    end_address = bcomm_GetFlashSectorAddress(0, end_index);
    end_length = bcomm_GetFlashSectorLength(end_index);

    /* assert(start_address <= end_address); */

    return ((end_address - start_address) + end_length);
}
