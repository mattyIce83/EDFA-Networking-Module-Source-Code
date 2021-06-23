#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

/* $Id: ring_buffer.h,v 1.1.1.1 2006-05-16 20:33:35 eugene Exp $ */

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef octet
#define octet unsigned char
#endif

typedef struct ring_buffer_s ring_buffer_t;

/* allocate a ring_buffer_s structure and initialize to empty;
 * return NULL for failure
 */
ring_buffer_t *ring_buffer_create (int p_buffer_size);

/* unallocate a ring_buffer_s structure */
void ring_buffer_destroy (ring_buffer_t ** p_buffer);

/* add character to ring buffer -
 * return 1 for success, EOF for failure (full)
 */
int ring_buffer_add_char (ring_buffer_t * p_buffer, int p_char);

/* remove character from ring buffer return character,
 * or EOF (-1?) for failure (empty)
 */
int ring_buffer_remove_char (ring_buffer_t * p_buffer);

/* add series of characters to ring buffer;
 * return number chars successfully added
 */
int ring_buffer_add_chars (ring_buffer_t * p_buffer,
			   octet *p_chars, int p_length);

/* remove a number of characters from ring buffer,
 * put in supplied buffer;
 * return number of characters actually removed
 */
int ring_buffer_remove_chars (ring_buffer_t * p_buffer,
			      int p_buffer_size, octet *p_out_buffer);



/* empty out the ring buffer and throw away any existing contents */
/* return 0 if p_buffer was null */
int ring_buffer_clear (ring_buffer_t * p_buffer);

/* look through the contents (if any) of the ring buffer
 * and return the offset (from read pointer) of the character which matches
 * the given one.  Do not change the ring buffer.  Return -1 if there is
 * no match
 */
int ring_buffer_char_match (ring_buffer_t * p_buffer, octet p_to_match);


/* return amount of space available for character storage */
int ring_buffer_space_available (ring_buffer_t * p_buffer);

/* return number of characters currently stored (implemented in terms of
 * ring_buffer_space_available ())
 */
int ring_buffer_space_used (ring_buffer_t * p_buffer);

/* return -1 if p_buffer is invalid;
 * otherwise return p_buffer->high_water_mark
 */
int ring_buffer_high_water_mark_get (ring_buffer_t * p_buffer);

/* return -1 if p_buffer is invalid;
 * otherwise return p_buffer->buffer_size
 */
int ring_buffer_buffer_size_get (ring_buffer_t * p_buffer);

/* return -1 if p_buffer is invalid;
 * otherwise return p_buffer->number_overflows
 */
int ring_buffer_number_overflow_get (ring_buffer_t * p_buffer);

void ring_buffer_dump (ring_buffer_t * p_buffer);
void ring_buffer_dump_verbose (ring_buffer_t *p_buffer);

#ifdef __cplusplus
}
#endif

#endif  /* _RING_BUFFER_H_ */
