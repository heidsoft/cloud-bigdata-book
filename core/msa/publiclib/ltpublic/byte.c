#include "byte.h"

unsigned int byte_chr(char *s,register unsigned int n,int c)
{
  register char ch;
  register char *t;

  ch = c;
  t = s;
  for (;;) {
    if (!n) break; if (*t == ch) break; ++t; --n;
    if (!n) break; if (*t == ch) break; ++t; --n;
    if (!n) break; if (*t == ch) break; ++t; --n;
    if (!n) break; if (*t == ch) break; ++t; --n;
  }
  return t - s;
}

void byte_copy(register char *to,register unsigned int n,register char *from)
{
  for (;;) {
    if (!n) return; *to++ = *from++; --n;
    if (!n) return; *to++ = *from++; --n;
    if (!n) return; *to++ = *from++; --n;
    if (!n) return; *to++ = *from++; --n;
  }
}


void byte_copyr(register char *to,register unsigned int n,register char *from)
{
  to += n;
  from += n;
  for (;;) {
    if (!n) return; *--to = *--from; --n;
    if (!n) return; *--to = *--from; --n;
    if (!n) return; *--to = *--from; --n;
    if (!n) return; *--to = *--from; --n;
  }
}


int byte_diff(register char *s,register unsigned int n,register char *t)
{
  for (;;) {
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
  }
  return ((int)(unsigned int)(unsigned char) *s)
       - ((int)(unsigned int)(unsigned char) *t);
}


unsigned int byte_rchr(char *s,register unsigned int n,int c)
{
  register char ch;
  register char *t;
  register char *u;

  ch = c;
  t = s;
  u = 0;
  for (;;) {
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
  }
  if (!u) u = t;
  return u - s;
}


void byte_zero(char *s,register unsigned int n)
{
  for (;;) {
    if (!n) break; *s++ = 0; --n;
    if (!n) break; *s++ = 0; --n;
    if (!n) break; *s++ = 0; --n;
    if (!n) break; *s++ = 0; --n;
  }
}


