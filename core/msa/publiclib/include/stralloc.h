#ifndef STRALLOC_H
#define STRALLOC_H


typedef struct stralloc { char *s; unsigned int len; unsigned int a; } stralloc;


extern int stralloc_ready(stralloc *,unsigned int);
extern int stralloc_readyplus(stralloc *,unsigned int);
extern int stralloc_copy(stralloc *,stralloc *);
extern int stralloc_cat(stralloc *,stralloc *);
extern int stralloc_copys(stralloc *,char *);
extern int stralloc_cats(stralloc *,char *);
extern int stralloc_copyb(stralloc *,char *,unsigned int);
extern int stralloc_catb(stralloc *,char *,unsigned int);
extern int stralloc_append(stralloc *,char *); /* beware: this takes a pointer to 1 char */
extern int stralloc_starts(stralloc *,char *);

#define stralloc_0(sa) stralloc_append(sa,"")

extern int stralloc_catulong0(stralloc *,unsigned long,unsigned int);
extern int stralloc_catlong0(stralloc *,long,unsigned int);

#define stralloc_catlong(sa,l) (stralloc_catlong0((sa),(l),0))
#define stralloc_catuint0(sa,i,n) (stralloc_catulong0((sa),(i),(n)))
#define stralloc_catint0(sa,i,n) (stralloc_catlong0((sa),(i),(n)))
#define stralloc_catint(sa,i) (stralloc_catlong0((sa),(i),0))

#endif
