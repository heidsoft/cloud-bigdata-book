#include "byte.h"
#include "getln.h"

int getln2(buffer *ss,stralloc *sa,char **cont,unsigned int *clen,int sep)
{
  register char *x;
  register unsigned int i;
  int n;
 
  if (!stralloc_ready(sa,0)) return -1;
  sa->len = 0;
 
  for (;;) {
    n = buffer_feed(ss);
    if (n < 0) return -1;
    if (n == 0) { *clen = 0; return 0; }
    x = buffer_PEEK(ss);
    i = byte_chr(x,n,sep);
    if (i < n) { buffer_SEEK(ss,*clen = i + 1); *cont = x; return 0; }
    if (!stralloc_readyplus(sa,n)) return -1;
    i = sa->len;
    sa->len = i + buffer_get(ss,sa->s + i,n);
  }
}

int getln(buffer *ss,stralloc *sa,int *match,int sep)
{
  char *cont;
  unsigned int clen;

  if (getln2(ss,sa,&cont,&clen,sep) == -1) return -1;
  if (!clen) { *match = 0; return 0; }
  if (!stralloc_catb(sa,cont,clen)) return -1;
  *match = 1;
  return 0;
}


