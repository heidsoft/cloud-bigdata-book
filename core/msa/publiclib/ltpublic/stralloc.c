#include "byte.h"
#include "ltstr.h"
#include "stralloc.h"

int stralloc_starts(sa,s)
stralloc *sa;
char *s;
{
  int len;
  len = str_len(s);
  return (sa->len >= len) && byte_equal(s,len,sa->s);
}

int stralloc_catb(stralloc *sa,char *s,unsigned int n)
{
  if (!sa->s) return stralloc_copyb(sa,s,n);
  if (!stralloc_readyplus(sa,n + 1)) return 0;
  byte_copy(sa->s + sa->len,n,s);
  sa->len += n;
  sa->s[sa->len] = 'Z'; /* ``offensive programming'' */
  return 1;
}

int stralloc_cat(stralloc *sato,stralloc *safrom)
{
  return stralloc_catb(sato,safrom->s,safrom->len);
}



int stralloc_cats(stralloc *sa,char *s)
{
  return stralloc_catb(sa,s,str_len(s));
}

int stralloc_copyb(stralloc *sa,char *s,unsigned int n)
{
  if (!stralloc_ready(sa,n + 1)) return 0;
  byte_copy(sa->s,n,s);
  sa->len = n;
  sa->s[n] = 'Z'; /* ``offensive programming'' */
  return 1;
}


int stralloc_copy(stralloc *sato,stralloc *safrom)
{
  return stralloc_copyb(sato,safrom->s,safrom->len);
}

int stralloc_copys(stralloc *sa,char *s)
{
  return stralloc_copyb(sa,s,str_len(s));
}

int stralloc_readyplus(register stralloc *x,register unsigned int n)
{ register unsigned int i;
  if (x->s) {
    i = x->a; n += x->len;
    if (n > i) {
      x->a = 30 + n + (n >> 3);
      if (alloc_re(&x->s,i * sizeof(char),x->a * sizeof(char))) return 1;
      x->a = i; return 0; }
    return 1; }
  x->len = 0;
  return !!(x->s = (char *) alloc((x->a = n) * sizeof(char)));
}



int stralloc_append(register stralloc *x,register char *i)
{
 if (!stralloc_readyplus(x,1)) return 0;
  x->s[x->len++] = *i;
  return 1;
}

int stralloc_ready(register stralloc *x,register unsigned int n)
{ register unsigned int i;
  if (x->s) {
    i = x->a;
      if (n > i) {
         x->a = 30 + n + (n >> 3);
         if (alloc_re(&x->s,i * sizeof(char),x->a * sizeof(char))) return 1;
         x->a = i; return 0;
      }
    return 1;
  }
  x->len = 0;
  return !!(x->s = (char *) alloc((x->a = n) * sizeof(char)));
}






