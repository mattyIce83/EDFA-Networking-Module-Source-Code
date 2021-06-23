/* $Id: ring_buffer.c,v 1.1.1.1 2006-12-22 23:19:25 eugene Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "ring_buffer.h"


#ifndef RING_BUFFER_SEMAPHORE
#define RING_BUFFER_SEMAPHORE 0
#endif

#if (1 == RING_BUFFER_SEMAPHORE)
#include "semaphore.h"
#endif /* RING_BUFFER_SEMAPHORE */


#ifndef RING_BUFFER_TEST
#define RING_BUFFER_TEST 0
#endif

#define CHARS_IN_ALPHABET 26
#define ASCII_A 'A'
#define ASCII_H 'H'
#define ASCII_T 'T'
#define ASCII_NULL '\0'
#define ASCII_NEWLINE '\n'

#define NOT_SHARED_BY_PROCESS 0

struct ring_buffer_s {
  char * buffer;
  int buffer_size;
  int high_water_mark;
  int number_overflows;
  int number_used;
  char * cursor_read;
  char * cursor_write;
#if ( 1 == RING_BUFFER_SEMAPHORE )
  sem_t semaphore;
#endif  /* RING_BUFFER_SEMAPHORE */
};



/* blocking - won't return until the semaphore is got */
static void
l_ring_buffer_get_semaphore (ring_buffer_t * p_buffer) {

#if ( 1 == RING_BUFFER_SEMAPHORE )
  if ( p_buffer ) {
    sem_wait (&(p_buffer->semaphore));
  }
#endif  /* RING_BUFFER_SEMAPHORE */

  return;
}

static void
l_ring_buffer_put_semaphore (ring_buffer_t * p_buffer) {

#if ( 1 == RING_BUFFER_SEMAPHORE )
  if ( p_buffer ) {
    sem_post (&(p_buffer->semaphore));
  }
#endif  /* RING_BUFFER_SEMAPHORE */

  return;
}

static int
l_ring_buffer_pointer_valid (ring_buffer_t * p_buffer, char * p_cursor) {
  int answer = 0;

  if ( (p_cursor >= &(p_buffer->buffer[0])) &&
       (p_cursor < (&(p_buffer->buffer[0]) + p_buffer->buffer_size)) ) {
    answer = 1;
  }

  return answer;
}

static char *
l_ring_buffer_increment_pointer (ring_buffer_t * p_buffer, char * p_cursor) {
  char * pointer = p_cursor;

  if ( l_ring_buffer_pointer_valid (p_buffer, pointer) ) {
    ++pointer;
    if ( ! l_ring_buffer_pointer_valid (p_buffer, pointer) ) {
      pointer = &(p_buffer->buffer[0]);
    }
  }

  return pointer;
}


/* allocate a ring_buffer_s structure and initialize to empty;
 * return NULL for failure
 */
ring_buffer_t *
ring_buffer_create (int p_buffer_size) {
  ring_buffer_t *buffer = NULL;
  int ii;
#if ( 1 == RING_BUFFER_SEMAPHORE )
  int sem_rc;
#endif

  if ( 0 < p_buffer_size ) {
    buffer = malloc (sizeof (unsigned long));
    if ( 0 != buffer ) {
      buffer->buffer = malloc (p_buffer_size);
      if ( 0 != buffer->buffer ) {
        memset (&(buffer->buffer[0]), ASCII_NULL, p_buffer_size);
        buffer->cursor_read = &(buffer->buffer[0]);
        buffer->cursor_write = &(buffer->buffer[0]);
        buffer->high_water_mark = 0;
        buffer->number_overflows = 0;
        buffer->number_used = 0;
        buffer->buffer_size = p_buffer_size;
#if ( 1 == RING_BUFFER_SEMAPHORE )
        sem_rc = sem_init (&(buffer->semaphore), NOT_SHARED_BY_PROCESS, 1);
        if ( -1 == sem_rc ) {
          free (buffer->buffer);
          free (buffer);
          buffer = 0;
        }
#endif
      } else {
        free (buffer);
        buffer = 0;
      }
    }
  }

  return buffer;
}

/* unallocate a ring_buffer_s structure */
void
ring_buffer_destroy (ring_buffer_t ** p_buffer) {
#if ( 1 == RING_BUFFER_SEMAPHORE )
  int sem_rc;
  int count_destroy = 0;
#endif
  if ( 0 != *p_buffer ) {
#if ( 1 == RING_BUFFER_SEMAPHORE )

    /* TODO BJB is this really worth while?
     * sem_destroy will unlock semaphore,
     * maybe not atomically with the "destroy" operation
     */
    l_ring_buffer_get_semaphore (*p_buffer);

    sem_rc = sem_destroy (&((*p_buffer)->semaphore));

    if ( -1 == sem_rc ) {
      while ( (-1 == sem_rc) && (EAGAIN == errno) && (10 > count_destroy) ) {
        sem_rc = sem_destroy (&((*p_buffer)->semaphore));
        sleep (1);
        ++count_destroy;
      }

      if ( (-1 == sem_rc) && (EAGAIN == errno) && (10 == count_destroy) ) {
        fprintf (stderr, "ERROR!  unallocating a locked semaphore");
        printf ("ERROR!  unallocating a locked semaphore");
      }
    }
#endif
    free ((*p_buffer)->buffer);
    free (*p_buffer);
    *p_buffer = 0;
  }
}


static int
l_ring_buffer_clear (ring_buffer_t * p_buffer ) {
  int rc = 1;

  p_buffer->cursor_read = &(p_buffer->buffer[0]);
  p_buffer->cursor_write = &(p_buffer->buffer[0]);
  p_buffer->number_used = 0;

  return rc;
}

int
ring_buffer_clear (ring_buffer_t * p_buffer) {
  int rc = 0;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    rc = l_ring_buffer_clear (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }
  
  return rc;
}

/* [=1234567]  read == write, empty, 0-6 available for writing */
/* [rw234567]  0 used, 1-6 available, 7 unavailable */
/* [r123456w]  0-6 used, 7 unavailable:  full */
/* [0r23456w]  1-6 used, 0 unavailable, 7 available */
/* [0123r56w]  456 used, 7012 available, 3 unavailable */
/* [0w23r567]  45670 used, 12 available, 3 unavailable */
/* [01w3r567]  456701 used, 2 available, 3 unavailable */
/* [012wr567]  4567012 used, 3 unavailable:  full */
/* [012w456r]  7012 used, 345 available, 6 unavailable */
/* [r12w4567]  012 used, 3456 available, 7 unavailable */
/* [0r2w4567]  12 used, 34567 available, 0 unavailable */
/* [01rw4567]  2 used, 345670 available, 1 unavailable */
/* [012=4567]  3456701 available, 2 unavailable:  empty */


static int
l_ring_buffer_full (ring_buffer_t * p_buffer ) {
  int rc = 0;

  if ( p_buffer->number_used == (p_buffer->buffer_size - 1) ) {
    rc = 1;
  }

  return rc;
}

int
ring_buffer_full (ring_buffer_t * p_buffer) {
  int rc = 0;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    rc = l_ring_buffer_full (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return rc;
}


static int
l_ring_buffer_empty (ring_buffer_t * p_buffer) {
  int rc = 0;

  if ( 0 == p_buffer->number_used ) {
    rc = 1;
  }

  return rc;
}

int
ring_buffer_empty (ring_buffer_t * p_buffer) {
  int rc = 0;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    rc = l_ring_buffer_empty (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return rc;
}

static int
l_ring_buffer_space_used (ring_buffer_t * p_buffer) {
  int used = 0;

  used = p_buffer->number_used;

  return used;
}

int
ring_buffer_space_used (ring_buffer_t * p_buffer) {
  int used = 0;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    used = l_ring_buffer_space_used (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return used;
}

static int
l_ring_buffer_space_available (ring_buffer_t * p_buffer) {
  int available = 0;

  available = p_buffer->buffer_size - p_buffer->number_used - 1;

  return available;
}

int
ring_buffer_space_available (ring_buffer_t * p_buffer) {
  int available = 0;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    available = l_ring_buffer_space_available (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return available;
}

static int
l_ring_buffer_add_char (ring_buffer_t * p_buffer, int p_char) {
  int rc = -1;

  if ( ! l_ring_buffer_full (p_buffer) ) {
    *(p_buffer->cursor_write) = (char) p_char;
    p_buffer->cursor_write = l_ring_buffer_increment_pointer
      (p_buffer, p_buffer->cursor_write);
    rc = 1;
    ++p_buffer->number_used;
    if ( p_buffer->high_water_mark < p_buffer->number_used ) {
      p_buffer->high_water_mark = p_buffer->number_used;
    }
  } else {
    /* add 1 every time we try to overflow */
    ++p_buffer->number_overflows;
  }

  return rc;
}

/* add character to ring buffer -
 * return 1 for success, EOF for failure (full)
 */
int
ring_buffer_add_char (ring_buffer_t * p_buffer, int p_char) {
  int rc = -1;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    rc = l_ring_buffer_add_char (p_buffer, p_char);
    l_ring_buffer_put_semaphore (p_buffer);
  }
  
  return rc;
}


static int
l_ring_buffer_remove_char (ring_buffer_t * p_buffer) {
  int thechar = -1;

  if ( ! l_ring_buffer_empty (p_buffer) ) {
    thechar = *(p_buffer->cursor_read);
    p_buffer->cursor_read = l_ring_buffer_increment_pointer
      (p_buffer, p_buffer->cursor_read);
    --p_buffer->number_used;
  }

  return thechar;
}

/* remove character from ring buffer & return character,
 * or EOF (-1?) for failure (empty)
 */
int
ring_buffer_remove_char (ring_buffer_t * p_buffer) {
  int thechar = -1;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    thechar = l_ring_buffer_remove_char (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return thechar;
}

static int
l_ring_buffer_add_chars (ring_buffer_t * p_buffer,
		       octet *p_chars, int p_length) {
  int ii = 0;

  while ( (ii < p_length) &&
          (1 == l_ring_buffer_add_char (p_buffer, p_chars[ii])) ) {
    ++ii;
  }
  
  return ii;
}

/* add series of octets to ring buffer;
 * return number octets successfully added
 */
int
ring_buffer_add_chars (ring_buffer_t * p_buffer,
		       octet *p_chars, int p_length) {
  int ii = 0;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    ii = l_ring_buffer_add_chars (p_buffer, p_chars, p_length);
    l_ring_buffer_put_semaphore (p_buffer);
  }
  
  return ii;
}

static int
l_ring_buffer_remove_chars (ring_buffer_t * p_buffer,
			  int p_buffer_size, octet *p_out_buffer) {
  int num_out = -1;
  int ii = 0;
  int oldchar;

  if ( p_out_buffer ) {
    while ( (ii < p_buffer_size) &&
            (-1 != (oldchar = l_ring_buffer_remove_char (p_buffer))) ) {
      p_out_buffer[ii] = (octet) oldchar;
      ++ii;
    }
    num_out = ii;
  }
  l_ring_buffer_put_semaphore (p_buffer);

  return num_out;
}

/* remove a number of characters from ring buffer,
 * put in supplied buffer;
 * return number of characters actually removed
 */
int
ring_buffer_remove_chars (ring_buffer_t * p_buffer,
			  int p_buffer_size, octet *p_out_buffer) {
  int num_out = -1;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    num_out = l_ring_buffer_remove_chars (p_buffer,
                                          p_buffer_size, p_out_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return num_out;
}



/* look through the contents (if any) of the ring buffer
 * and return the offset (from read pointer) of the character which matches
 * the given one.  Do not change the ring buffer.  Return -1 if there is
 * no match
 */
int
ring_buffer_char_match (ring_buffer_t * p_buffer, octet p_to_match) {
  int answer = -1;
  int offset = 0;
  char * cp;  /* current ptr */

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    cp = p_buffer->cursor_read;
    if ( p_buffer && (0 < l_ring_buffer_space_used (p_buffer)) ) {
      while ( (cp != p_buffer->cursor_write) && (p_to_match != *cp) ) {
        cp = l_ring_buffer_increment_pointer (p_buffer, cp);
        ++offset;
      }
      if ( (cp != p_buffer->cursor_write) && (p_to_match == *cp) ) {
        answer = offset;
      }
    }
    l_ring_buffer_put_semaphore (p_buffer);
  }
  return answer;
}


static int
l_ring_buffer_high_water_mark_get (ring_buffer_t * p_buffer) {
  int hwm = -1;

  hwm = p_buffer->high_water_mark;

  return hwm;
}

int
ring_buffer_high_water_mark_get (ring_buffer_t * p_buffer) {
  int hwm = -1;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    hwm = l_ring_buffer_high_water_mark_get (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return hwm;
}

static int
l_ring_buffer_size_get (ring_buffer_t * p_buffer) {
  int hwm = -1;

  hwm = p_buffer->buffer_size;

  return hwm;
}

int
ring_buffer_size_get (ring_buffer_t * p_buffer) {
  int hwm = -1;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    hwm = l_ring_buffer_size_get (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return hwm;
}

static int
l_ring_buffer_number_overflow_get (ring_buffer_t * p_buffer) {
  int ovf = -1;

  ovf = p_buffer->number_overflows;

  return ovf;
}
 
int
ring_buffer_number_overflow_get (ring_buffer_t * p_buffer) {
  int ovf = -1;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    ovf = l_ring_buffer_number_overflow_get (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  }

  return ovf;
}

static void
l_ring_buffer_dump (ring_buffer_t * p_buffer) {
  printf ("ring_buffer:\n");
  printf ("  buffer addr:  [%p]\n", p_buffer->buffer);
  printf ("  buffer size:  [%d]\n", l_ring_buffer_size_get (p_buffer));
  printf ("  buffer hwm:   [%d]\n", l_ring_buffer_high_water_mark_get (p_buffer));
  printf ("  buffer num_ovf: [%d]\n", l_ring_buffer_number_overflow_get (p_buffer));
  printf ("  buffer num_use: [%d]\n", l_ring_buffer_space_used (p_buffer));
  printf ("  read[%p] write[%p]\n", p_buffer->cursor_read,
          p_buffer->cursor_write);
}
 
void
ring_buffer_dump (ring_buffer_t * p_buffer) {
  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    l_ring_buffer_dump (p_buffer);
    l_ring_buffer_put_semaphore (p_buffer);
  } else {
    printf ("ring_buffer:  void pointer\n");
  }
}

void
ring_buffer_dump_verbose (ring_buffer_t *p_buffer) {
  int ii;
  int bufsize;

  if ( p_buffer ) {
    l_ring_buffer_get_semaphore (p_buffer);
    l_ring_buffer_dump (p_buffer);
    bufsize = l_ring_buffer_size_get (p_buffer);
    for ( ii = 0; ii < bufsize; ++ii ) {
      if ( ! (ii % 16) ) {
        printf ("\n%3x  ", ii);
      }
      printf ("%02d ", p_buffer->buffer[ii]);
    }
    printf ("\n");
    l_ring_buffer_put_semaphore (p_buffer);
  }
}
 
#if ( RING_BUFFER_TEST == 1 )

int
do_test_create_destroy () {
  int tr = 0;  /* "test return" */
  int ii;
  ring_buffer_t * buf;

  printf ("do_test_create_destroy:  start\n");

  for ( ii = 0; ii < 1024; ++ii ) {
    if ( 0 == (buf = ring_buffer_create (8 * (ii+1))) ) {
      tr = 1;
      printf ("create_destroy:  fail after create [%d]th time\n", ii);
      goto do_test_create_destroy_end;
    }
    ring_buffer_destroy (&buf);
    if ( 0 != buf ) {
      tr = 1;
      printf ("create_destroy:  fail after destroy\n");
      goto do_test_create_destroy_end;
    }
  }

 do_test_create_destroy_end:
  printf ("do_test_create_destroy:  end [%d]\n", tr);
  return tr;
}

int
do_test_add_one_remove_one () {
  int tr = 0;  /* "test return" */
  int ii;
  int newchar;
  ring_buffer_t * buf;

  printf ("do_test_add_one_remove_one:  start\n");

  buf = ring_buffer_create (1024);

  if ( 0 == buf ) {
    tr = 1;
    printf ("add_one_remove_one:  fail after create\n");
    goto do_test_add_one_remove_one_end;
  }

  for ( ii = 0; ii < 1024 * 1024; ++ii ) {
    newchar = (ii % CHARS_IN_ALPHABET) + ASCII_A;
    if ( 1 != ring_buffer_add_char (buf, newchar) ) {
      tr = 1;
      printf ("add_one_remove_one:  fail in add on [%d]th try\n", ii);
      ring_buffer_dump (buf);
      goto do_test_add_one_remove_one_dealloc;
    }

    if ( newchar != ring_buffer_remove_char (buf) ) {
      tr = 1;
      printf ("add_one_remove_one:  fail in remove\n");
      goto do_test_add_one_remove_one_dealloc;
    }
  }

  ring_buffer_dump (buf);

  ring_buffer_destroy (&buf);

  if ( 0 != buf ) {
    tr = 1;
    printf ("add_one_remove_one:  fail after destroy\n");
  }

 do_test_add_one_remove_one_dealloc:
  ring_buffer_destroy (&buf);

 do_test_add_one_remove_one_end:
  printf ("do_test_add_one_remove_one:  end:  [%d]\n", tr);
  return tr;
}

int
do_test_add_lots () {
  int tr = 0;
  int ii;
  int newchar;

  ring_buffer_t * buf;

  printf ("do_test_add_lots:  start\n");

  buf = ring_buffer_create (1024);

  if ( 0 == buf ) {
    tr = 1;
    printf ("add_lots:  fail in create\n");
  }

  for ( ii = 0; ii < 1023; ++ii ) {
    newchar = (ii % CHARS_IN_ALPHABET) + ASCII_A;
    if ( 1 != ring_buffer_add_char (buf, newchar) ) {
      tr = 1;
      printf ("add_lots:  Failed in add_char on [%d]th try\n", ii);
      ring_buffer_dump (buf);
      goto do_test_add_lots_dealloc;
    }
  }

  newchar = (1024 % CHARS_IN_ALPHABET) + ASCII_A;
  if ( 1 == ring_buffer_add_char (buf, newchar) ) {
    tr = 1;
    printf ("add_lots:  Succeeded in add_char on 1023'th try - shouldn't have\n");
    ring_buffer_dump (buf);
  }

 do_test_add_lots_dealloc:
  ring_buffer_destroy (&buf);

  printf ("do_test_add_lots:  end  tr[%d]\n", tr);
  return tr;
}

int
do_test_add_full_remove_full () {
  int tr = 0;

  ring_buffer_t * buf;
  int ii;
  int jj;
  int newchar;
  int gotchar;

  printf ("do_test_add_full_remove_full:  start\n");

  buf = ring_buffer_create (1024);

  if ( 0 == buf ) {
    tr = 1;
    printf ("add_full_remove_full:  fail after create\n");
    goto do_test_add_full_remove_full_end;
  }


  for ( ii = 0; ii < 1023; ++ii ) {
    newchar = (ii % CHARS_IN_ALPHABET) + ASCII_A;
    for ( jj = 0; jj < 1023; ++jj ) {
      if ( 1 != ring_buffer_add_char (buf, newchar) ) {
        tr = 1;
        printf ("  add_full_remove_full:  error adding:  ii[%d] jj[%d]\n",
                ii, jj);
        ring_buffer_dump (buf);
        goto do_test_add_full_remove_full_dealloc;
      }
    }

    for ( jj = 0; jj < 1023; ++jj ) {
      if ( newchar != (gotchar = ring_buffer_remove_char (buf)) ) {
        tr = 1;
        printf ("  add_full_remove_full:  error removing:  ii[%d] jj[%d] newchar[%d] got[%d]\n", ii, jj, newchar, gotchar);
        ring_buffer_dump (buf);
        goto do_test_add_full_remove_full_dealloc;
      }
    }
  }

  ring_buffer_dump (buf);

 do_test_add_full_remove_full_dealloc:
  ring_buffer_destroy (&buf);

 do_test_add_full_remove_full_end:
  printf ("do_test_add_full_remove_full:  end tr[%d]\n", tr);
  return tr;
}


/* return 1 if counters are ok; return 0 if not */
static int
l_check_counters (ring_buffer_t * p_buffer,
                int p_size, int p_hwm, int p_num_used, int p_num_ovf) {
  int answer = 1;
  if ( p_buffer ) {
    if ( (p_size != p_buffer->buffer_size) ||
         (p_hwm != p_buffer->high_water_mark) ||
         (p_num_used != p_buffer->number_used) ||
         (p_num_ovf != p_buffer->number_overflows) ) {
      answer = 0;
    }
  } else {
    answer = 0;
  }

  return answer;
}


int
do_test_counters () {
  int tr = 0;

  ring_buffer_t * buf;
  int ii;

  printf ("do_test_counters:  start\n");

  buf = ring_buffer_create (1024);

  if ( 0 == buf ) {
    tr = 1;
    printf ("counters:  fail after create\n");
    goto do_test_counters_end;
  }

  if ( 0 == l_check_counters (buf, 1024, 0, 0, 0) ) {
    tr = 1;
    printf ("counters:  fail; s/b size[1024], hwm[0], ovf[0], used[0]\n");
    ring_buffer_dump (buf);
    goto do_test_counters_dealloc;
  }

  for ( ii = 0; ii < 1024 - 1; ++ii ) {
    if ( 1 != ring_buffer_add_char (buf, ASCII_A) ) {
      tr = 1;
      printf ("counters:  fail after [%d]th add_char\n", ii);
      if ( 0 == l_check_counters (buf, 1024, ii + 1, 0, ii + 1) ) {
        tr = 1;
        printf ("counters:  fail; s/b size[1024], hwm[%d], ovf[0], used[%d]\n",
                ii + 1, ii + 1);
        ring_buffer_dump (buf);
        goto do_test_counters_dealloc;
      }
    }
  }

  for ( ii = 1023; ii > 0; --ii ) {
    if ( ASCII_A != ring_buffer_remove_char (buf) ) {
      printf ("counters:  fail after [%d]th remove_char\n", ii);
      if ( 0 == l_check_counters (buf, 1024, 1023, ii, 0) ) {
        tr = 1;
        printf ("counters:  fail; s/b size[1024]; hwm[1023]; ovf[0]; used[%d]\n", ii);
        ring_buffer_dump (buf);
        goto do_test_counters_dealloc;
      }
    }
  }

 do_test_counters_dealloc:
  ring_buffer_destroy (&buf);

 do_test_counters_end:
  printf ("do_test_counters:  end tr[%d]\n", tr);
  return tr;
}


/* Only use this when you are confident that
 * the "ring_buffer_add_char" routine is bulletproof.
 */
static void
l_fill_with_test_data (ring_buffer_t * p_buffer) {
  char foo_[] = "Here's a bunch\nof ZIM codes\non a few\nlines\nZIM\nZOOM\nZUM\n";
  int ii;

  for ( ii = 0; ii < 56; ++ii ) {
    ring_buffer_add_char (p_buffer, foo_[ii]);
  }
}


int
do_test_char_match () {
  int tr = 0;

  ring_buffer_t * buf;
  int ii;
  int jj;
  int offset;
  int old_char;

  printf ("do_test_char_match:  start\n");

  buf = ring_buffer_create (1024);

  if ( 0 == buf ) {
    tr = 1;
    printf ("char_match:  fail after create\n");
    goto do_test_char_match_end;
  }

  l_fill_with_test_data (buf);

  /* look for matches */
  if ( 14 != (offset = ring_buffer_char_match (buf, ASCII_NEWLINE)) ) {
    tr = 1;
    printf ("  do_test_char_match:  FAILED char_match - should have found newline - [%d] [%c]\n", offset, buf->buffer[offset]);
    ring_buffer_dump (buf);
    goto do_test_char_match_dealloc;
  }

  if ( -1 != ring_buffer_char_match (buf, ASCII_T) ) {
    tr = 1;
    printf ("  do_test_char_match:  FAILED char_match - should not have found T\n");
    ring_buffer_dump (buf);
    goto do_test_char_match_dealloc;
  }

  /* look for match first char */
  if ( 0 != ring_buffer_char_match (buf, ASCII_H) ) {
    tr = 1;
    printf ("  do_test_char_match:  FAILED char_match - should have found first char H\n");
    ring_buffer_dump (buf);
    goto do_test_char_match_dealloc;
  }

  /* remove the first six newlines, and search for 7th (and last) one */
  for ( ii = 0; ii < 6; ++ii ) {
    offset = ring_buffer_char_match (buf, ASCII_NEWLINE);
    /* printf ("line [%d], to offset [%d]:  ", ii, offset); */
    for ( jj = 0; jj <= offset; ++jj ) {
      /* remove and throw away */
      old_char = ring_buffer_remove_char (buf);
      /* printf ("%c", old_char); */
    }
  }

  if ( 3 != (offset = ring_buffer_char_match (buf, ASCII_NEWLINE)) ) {
    tr = 1;
    printf ("  do_test_char_match:  FAILED char_match - should have found last char newline\n");
    ring_buffer_dump (buf);
    goto do_test_char_match_dealloc;
  }

 do_test_char_match_dealloc:
  ring_buffer_destroy (&buf);

 do_test_char_match_end:
  printf ("do_test_char_match:  end tr[%d]\n", tr);
  return tr;
}

int
do_test_clear () {
  int tr = 0;
  ring_buffer_t * buf;
  int ii;
  int jj;
  int old_char;
  int offset;
  char foo_[] = "ZIMRM 398\nZIMCLR88832\nZIMFOO\nZIMBAR\n";
  int hwm;
  int ovf;
  int used;
  int oldchar;

  printf ("do_test_clear:  start\n");

  buf = ring_buffer_create (1024);
  if ( 0 == buf ) {
    tr = 1;
    printf ("char_match:  fail after create\n");
    goto do_test_clear_end;
  }

  l_fill_with_test_data (buf);

  /* remove the first six newlines, then clear and re-use the buffer */
  for ( ii = 0; ii < 6; ++ii ) {
    offset = ring_buffer_char_match (buf, ASCII_NEWLINE);
    /* printf ("line [%d], to offset [%d]:  ", ii, offset); */
    for ( jj = 0; jj <= offset; ++jj ) {
      /* remove and throw away */
      old_char = ring_buffer_remove_char (buf);
      /* printf ("%c", old_char); */
    }
  }

  if ( 3 != (offset = ring_buffer_char_match (buf, ASCII_NEWLINE)) ) {
    tr = 1;
    printf ("  do_test_clear:  FAILED char_match - should have found last char newline\n");
    ring_buffer_dump (buf);
    goto do_test_clear_dealloc;
  }

  /* clear */
  if ( 0 == ring_buffer_clear (buf) ) {
    tr = 1;
    printf ("do_test_clear:  failed in ring_buffer_clear\n");
    ring_buffer_dump (buf);
    goto do_test_clear_dealloc;
  }

  if ( 0 == l_check_counters (buf, 1024, 57, 0, 0) ) {
    tr = 1;
    printf ("do_test_clear:  failed l_check_counters after ring_buffer_clear\n");
    ring_buffer_dump (buf);
    goto do_test_clear_dealloc;
  }

  /* try some more adds/removes and see if it still works */
  for ( ii = 0; ii < 36; ++ii ) {
    ring_buffer_add_char (buf, foo_[ii]);
    if ( 0 == l_check_counters (buf, 1024, 57, ii + 1, 0) ) {
      tr = 1;
      printf ("do_test_clear:  failed in l_check_counters after add_char\n");
      ring_buffer_dump (buf);
      goto do_test_clear_dealloc;
    }
  }

  /* numbers based on test data */
  hwm = 57;
  used = ii - 1;
  ovf = 0;

  offset = ring_buffer_char_match (buf, ASCII_NEWLINE);
  for ( ii = 0; ii <= offset; ++ii ) {
    if ( -1 == (oldchar = ring_buffer_remove_char (buf)) ) {
      tr = 1;
      printf ("do_test_clear:  failed in remove_char, [%d]th char[%c]\n",
              ii, oldchar);
      ring_buffer_dump (buf);
      goto do_test_clear_dealloc;
    }

    if ( 0 == l_check_counters (buf, 1024, hwm, used - ii, 0) ) {
      tr = 1;
      printf ("do_test_clear:  failed in l_check_counters after [%d]th remove_char [%c] used[%d]\n", ii, oldchar, used);
      printf ("                size[1024], hwm[%d], used[%d], ovf[%d]\n",
              hwm, used - ii, 0);
      ring_buffer_dump (buf);
      goto do_test_clear_dealloc;
    }
  }

 do_test_clear_dealloc:
  ring_buffer_destroy (&buf);

 do_test_clear_end:
  printf ("do_test_clear:  end tr[%d]\n", tr);
  return tr;
}

/* test routine template */
int
do_test_add_many_remove_many () {
  int tr = 0;
  int rc;
  ring_buffer_t * buf;
  unsigned char outbuf_[16];

  printf ("do_test_add_many_remove_many:  start\n");

  buf = ring_buffer_create (1024);
  if ( 0 == buf ) {
    tr = 1;
    printf ("  do_test_add_many_remove_many:  fail after create\n");
    goto do_test_add_many_remove_many_end;
  }

  if ( 8 != (rc = ring_buffer_add_chars (buf, (unsigned char*) "ZIM\nZEM\n", 8)) ) {
    tr = 1;
    printf ("  do_test_add_many_remove_many:  fail after add_chars\n");
    ring_buffer_dump (buf);
    goto do_test_add_many_remove_many_dealloc;
  }

  if ( 4 != (rc = ring_buffer_remove_chars (buf, 4, &(outbuf_[0]))) ) {
    tr = 1;
    printf ("do_test_add_many_remove_many:  fail after first remove_chars\n");
    printf ("                               rc = [%d]  [%c.%c.%c.%c]\n", rc,
            outbuf_[0], outbuf_[1], outbuf_[2], outbuf_[3]);
    ring_buffer_dump (buf);
    goto do_test_add_many_remove_many_dealloc;
  }

  if ( outbuf_[0] != 'Z' ) {
    tr = 1;
  }
  if ( outbuf_[1] != 'I' ) {
    tr = 1;
  }
  if ( outbuf_[2] != 'M' ) {
    tr = 1;
  }
  if ( outbuf_[3] != ASCII_NEWLINE ) {
    tr = 1;
  }

  if ( 1 == tr ) {
    printf ("  do_test_add_many_remove_many:  extracted string does not match inputted string\n");
    printf ("                                 in  [%c%c%c%c]\n"
            "                                 out [%c%c%c%c]\n",
            'Z', 'I', 'M', '\n',
            outbuf_[0], outbuf_[1], outbuf_[2], outbuf_[3]);
  }

 do_test_add_many_remove_many_dealloc:
  ring_buffer_destroy (&buf);

 do_test_add_many_remove_many_end:
  printf ("do_test_add_many_remove_many:  end tr[%d]\n", tr);
  return tr;
}

/* test routine template */
int
do_test_blah_blah () {
  int tr = 0;

  ring_buffer_t * buf;

  printf ("do_test_:  start\n");

  buf = ring_buffer_create (1024);
  if ( 0 == buf ) {
    tr = 1;
    printf ("  do_test_:  fail after create\n");
    goto do_test__end;
  }

  goto do_test__dealloc;

 do_test__dealloc:
  ring_buffer_destroy (&buf);

 do_test__end:
  printf ("do_test_:  end tr[%d]\n", tr);
  return tr;
}



int
main (int p_argc, char * p_argv[]) {

  int tr = 0;  /* "test return" */
  int rc = EXIT_SUCCESS;  /* "return code" */

  tr += do_test_create_destroy ();

  tr += do_test_add_one_remove_one ();

  tr += do_test_add_lots ();

  tr += do_test_add_full_remove_full ();

  tr += do_test_counters ();

  tr += do_test_char_match ();

  tr += do_test_clear ();

  tr += do_test_add_many_remove_many ();

  if ( 0 == tr ) {
    rc = EXIT_SUCCESS;
  } else {
    printf ("FAILURES");
    rc = EXIT_FAILURE;
  }

  return rc;

}

#endif /* RING_BUFFER_TEST */

