/* port.c */
/* code to help port the vax pascal into linux c */

#include "imoria.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void * safe_malloc(int blocksize, char *message)
{
  void * new_pointer;

  if((new_pointer = (void *)malloc(blocksize)) == NULL) {
    printf("\n\r\n\rMemory error (%d bytes)! Ref: %s.\n\r\n\r",blocksize,message);
    printf("malloc calls: %ld\tmalloc bytes: %ld\n\r",malloc_calls,malloc_bytes);
    printf("free   calls: %ld\tfree   bytes: %ld\n\r",free_calls,free_bytes);
    printf("\n\rdelta  calls: %ld\ndelta  bytes: %ld\n\r",
	   malloc_calls-free_calls,malloc_bytes-free_bytes);
    printf("\n\r\n\r");
    exit_game();
  }

  malloc_calls++;
  malloc_bytes += blocksize;

  return new_pointer;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void dispose(void *ptr, int size, char *message)
{
  free_calls++;
  free_bytes += size;

  free(ptr);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

extern  char * chomp(char *input_line)
{
  /* remove \n from the end of a string if there is one */
  integer x;

  x = strlen(input_line);
  if (x && (input_line[x-1] == '\n'))
    {
      input_line[x-1] = 0;
    }

  return input_line;
  
}; /* end chomp */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

integer min3(integer i1, integer i2, integer i3)
{
  if (i1 < i2) {
    return (i1 < i3) ? i1 : i3;
  } else {
    return (i2 < i3) ? i2 : i3;
  }
};

//////////////////////////////////////////////////////////////////////

void ignore_signals()
{
};
void restore_signals()
{
};
void default_signals()
{
};

/* Something happens to disturb the player.		-CJS-
   The first arg indicates a major disturbance, which affects search.
   The second arg indicates a light change. */
/* see moria1.c from umoria */
void disturb(s, l)
int s, l;
{
#if 0
  command_count = 0;
  if (s && search_flag)
    search_off();
  if (py.flags.rest > 0)
    rest_off();
  if (l || find_flag)
    {
      find_flag = FALSE;
      check_view();
    }
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* END FILE  port.c */
