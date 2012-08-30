#include <errno.h>
#include "alloc.h"
#include "error.h"
#include "byte.h"

#define ALIGNMENT 16 /* XXX: assuming that this alignment is enough */
#define SPACE 4096 /* must be multiple of ALIGNMENT */

typedef union { char irrelevant[ALIGNMENT]; double d; } aligned;
static aligned realspace[SPACE / ALIGNMENT];
#define space ((char *) realspace)
static unsigned int avail = SPACE; /* multiple of ALIGNMENT; 0<=avail<=SPACE */

/*@null@*//*@out@*/char *alloc(unsigned int n)
{
  char *x;
  n = ALIGNMENT + n - (n & (ALIGNMENT - 1)); /* XXX: could overflow */
  if (n <= avail) { avail -= n; return space + avail; }
  x = malloc(n);
  if (!x) errno = error_nomem;
  return x;
}

void alloc_free(char *x)
{
  if (x >= space)
    if (x < space + SPACE)
      return; /* XXX: assuming that pointers are flat */
  free(x);
}

int alloc_re(char **x,unsigned int m,unsigned int n)
{
  char *y;
 
  y = alloc(n);
  if (!y) return 0;
  byte_copy(y,m,*x);
  alloc_free(*x);
  *x = y;
  return 1;
}
