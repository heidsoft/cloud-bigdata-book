#include "fmt.h"


unsigned int fmt_str(register char *s,register char *t)
{
  register unsigned int len;
  char ch;
  len = 0;
  if (s) { while (ch = t[len]) s[len++] = ch; }
  else while (t[len]) len++;
  return len;
}

unsigned int fmt_strn(register char *s,register char *t,unsigned int n)
{
  register unsigned int len;
  char ch;
  len = 0;
  if (s) { while (n-- && (ch = t[len])) s[len++] = ch; }
  else while (n-- && t[len]) len++;
  return len;
}


unsigned int fmt_ulong(register char *s,register unsigned long u)
{
  register unsigned int len; register unsigned long q;
  len = 1; q = u;
  while (q > 9) { ++len; q /= 10; }
  if (s) {
    s += len;
    do { *--s = '0' + (u % 10); u /= 10; } while(u); /* handles u == 0 */
  }
  return len;
}


unsigned int fmt_uint(register char *s,register unsigned int u)
{
  register unsigned long l; l = u; return fmt_ulong(s,l);
}


unsigned int fmt_uint0(char *s,unsigned int u,unsigned int n)
{
  unsigned int len;
  len = fmt_uint(FMT_LEN,u);
  while (len < n) { if (s) *s++ = '0'; ++len; }
  if (s) fmt_uint(s,u);
  return len;
}
