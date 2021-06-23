/* 
 * $Id: find_highwater.c,v 1.1.1.1 2006-05-04 14:38:39 eugene Exp $
 * A stack is marked from front guard to end guard. 
 * Search for markers in a sliding window row size from the top to bottom.
 * Window moves one sizeof(*spot) at a time and we need all of the
 * contents to equal MARK simultaneously.
 * Optimisations:
 *  - use a case to deal with the smallest unit first
 *    then iterate over the balance of the array using large units
 *  - pre-compute the value and use type punning so we can compare more bits
 * Note: high-water started just before we found the full marked chunk.
 */
#include "find_highwater.h"
#include "watch.h"


int find_highwater(unsigned char *stackbase, unsigned char mark, int size)
{
    int waterline;
    unsigned char *spot;
    unsigned char val[STACKCHUNK];
    int each;

    /* minimum order */
    if (size < STACKCHUNK) {
	return size;
    }

    spot = stackbase + size;
    /* threadx seems to leave the first bit unused */
    spot -= STACKCHUNK;

    /* preload the values for the first compare */
    for (each = 0; each < STACKCHUNK; each++) {
	val[each] = *(--spot);
    }

    /* start comparing over the window one at a time */
    for (waterline = STACKCHUNK; waterline < size; waterline++) {
	/* check to see if any of val is not a MARK */
	for (each = 0; each < STACKCHUNK; each++) {
	    if (val[each] != mark)
		break;
	}
	if (each == STACKCHUNK) {
	    /* all of them were MARK--stop now */
	    break;
	} else {
	    /* get another value in window for checking */
	    val[waterline % STACKCHUNK] = *spot--;
	}
    }

    /* now we found the unused chunk beginning, used is back */
    waterline--;
    return waterline;
}
