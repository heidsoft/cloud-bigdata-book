#include "case.h"

int case_starts(register char *s,register char *t)
{
  register unsigned char x;
  register unsigned char y;

  for (;;) {
    x = *s++ - 'A';
    if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = *t++ - 'A';
    if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if (!y) return 1;
    if (x != y) return 0;
  }
}


void case_lowers(char *s)
{
  unsigned char x;
  while (x = *s) {
    x -= 'A';
    if (x <= 'Z' - 'A') *s = x + 'a';
    ++s;
  }
}

void case_lowerb(char *s,unsigned int len)
{
  unsigned char x;
  while (len > 0) {
    --len;
    x = *s - 'A';
    if (x <= 'Z' - 'A') *s = x + 'a';
    ++s;
  }
}

int case_diffs(register char *s,register char *t)
{
  register unsigned char x;
  register unsigned char y;

  for (;;) {
    x = *s++ - 'A';
    if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = *t++ - 'A';
    if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if (x != y) break;
    if (!x) break;
  }
  return ((int)(unsigned int) x) - ((int)(unsigned int) y);
}

int case_diffb(register char *s,unsigned int len,register char *t)
{
  register unsigned char x;
  register unsigned char y;

  while (len > 0) {
    --len;
    x = *s++ - 'A';
    if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = *t++ - 'A';
    if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if (x != y)
      return ((int)(unsigned int) x) - ((int)(unsigned int) y);
  }
  return 0;
}

