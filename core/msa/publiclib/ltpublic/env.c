/* env.c, envread.c, env.h: environ library
Daniel J. Bernstein, djb@silverton.berkeley.edu.
Depends on str.h, alloc.h.
Requires environ.
19960113: rewrite. warning: interface is different.
No known patent problems.
*/

#include "ltstr.h"
#include "alloc.h"
#include "env.h"

int env_isinit = 0; /* if env_isinit: */
static int ea; /* environ is a pointer to ea+1 char*'s. */
static int en; /* the first en of those are ALLOCATED. environ[en] is 0. */

static void env_goodbye(int i)
{
 alloc_free(environ[i]);
 environ[i] = environ[--en];
 environ[en] = 0;
}

static char *null = 0;

void env_clear()
{
 if (env_isinit) while (en) env_goodbye(0);
 else environ = &null;
}

static void env_unsetlen(char *s, int len)
{
 int i;
 for (i = en - 1;i >= 0;--i)
   if (!str_diffn(s,environ[i],len))
     if (environ[i][len] == '=')
       env_goodbye(i);
}

int env_unset(char *s) 
{
 if (!env_isinit) if (!env_init()) return 0;
 env_unsetlen(s,str_len(s));
 return 1;
}

static int env_add(char *s) 
{
 char *t;
 t = env_findeq(s);
 if (t) env_unsetlen(s,t - s);
 if (en == ea)
  {
   ea += 30;
   if (!alloc_re(&environ,(en + 1) * sizeof(char *),(ea + 1) * sizeof(char *)))
    { ea = en; return 0; }
  }
 environ[en++] = s;
 environ[en] = 0;
 return 1;
}

int env_put(char *s) 
{
 char *u;
 if (!env_isinit) if (!env_init()) return 0;
 u = alloc(str_len(s) + 1);
 if (!u) return 0;
 str_copy(u,s);
 if (!env_add(u)) { alloc_free(u); return 0; }
 return 1;
}

int env_put2(char *s, char *t) 
{
 char *u;
 int slen;
 if (!env_isinit) if (!env_init()) return 0;
 slen = str_len(s);
 u = alloc(slen + str_len(t) + 2);
 if (!u) return 0;
 str_copy(u,s);
 u[slen] = '=';
 str_copy(u + slen + 1,t);
 if (!env_add(u)) { alloc_free(u); return 0; }
 return 1;
}

int env_init()
{
 char **newenviron;
 int i;
 for (en = 0;environ[en];++en) ;
 ea = en + 10;
 newenviron = (char **) alloc((ea + 1) * sizeof(char *));
 if (!newenviron) return 0;
 for (en = 0;environ[en];++en)
  {
   newenviron[en] = alloc(str_len(environ[en]) + 1);
   if (!newenviron[en])
    {
     for (i = 0;i < en;++i) alloc_free(newenviron[i]);
     alloc_free(newenviron);
     return 0;
    }
   str_copy(newenviron[en],environ[en]);
  }
 newenviron[en] = 0;
 environ = newenviron;
 env_isinit = 1;
 return 1;
}


extern /*@null@*/char *env_get(char *s)
{
  int i;
  unsigned int slen;
  char *envi;
 
  slen = str_len(s);
  for (i = 0;envi = environ[i];++i)
    if ((!str_diffn(s,envi,slen)) && (envi[slen] == '='))
      return envi + slen + 1;
  return 0;
}

extern char *env_pick()
{
  return environ[0];
}

extern char *env_findeq(char *s)
{
  for (;*s;++s)
    if (*s == '=')
      return s;
  return 0;
}
