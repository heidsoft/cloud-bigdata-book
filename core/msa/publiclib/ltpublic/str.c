#include "ltstr.h"

unsigned int str_chr(register char *s,int c)
{
  register char ch;
  register char *t;

  ch = c;
  t = s;
  for (;;) {
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
  }
  return t - s;
}

unsigned int str_copy(register char *s,register char *t)
{
  register int len;

  len = 0;
  for (;;) {
    if (!(*s = *t)) return len; ++s; ++t; ++len;
    if (!(*s = *t)) return len; ++s; ++t; ++len;
    if (!(*s = *t)) return len; ++s; ++t; ++len;
    if (!(*s = *t)) return len; ++s; ++t; ++len;
  }
}

int str_diff(register char *s,register char *t)
{
  register char x;

  for (;;) {
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((int)(unsigned int)(unsigned char) x)
       - ((int)(unsigned int)(unsigned char) *t);
}

int str_diffn(register char *s,register char *t,unsigned int len)
{
  register char x;

  for (;;) {
    if (!len--) return 0; x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    if (!len--) return 0; x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    if (!len--) return 0; x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    if (!len--) return 0; x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((int)(unsigned int)(unsigned char) x)
       - ((int)(unsigned int)(unsigned char) *t);
}


unsigned int str_len(char *s)
{
  register char *t;

  t = s;
  for (;;) {
    if (!*t) return t - s; ++t;
    if (!*t) return t - s; ++t;
    if (!*t) return t - s; ++t;
    if (!*t) return t - s; ++t;
  }
}


unsigned int str_rchr(s,c)
register char *s;
int c;
{
  register char ch;
  register char *t;
  register char *u;

  ch = c;
  t = s;
  u = 0;
  for (;;) {
    if (!*t) break; if (*t == ch) u = t; ++t;
    if (!*t) break; if (*t == ch) u = t; ++t;
    if (!*t) break; if (*t == ch) u = t; ++t;
    if (!*t) break; if (*t == ch) u = t; ++t;
  }
  if (!u) u = t;
  return u - s;
}


int str_start(register char *s,register char *t)
{
  register char x;

  for (;;) {
    x = *t++; if (!x) return 1; if (x != *s++) return 0;
    x = *t++; if (!x) return 1; if (x != *s++) return 0;
    x = *t++; if (!x) return 1; if (x != *s++) return 0;
    x = *t++; if (!x) return 1; if (x != *s++) return 0;
  }
}


