#ifndef ALLOC_H
#define ALLOC_H

extern /*@null@*//*@out@*/char *alloc();
extern char *alloc(unsigned int n);
extern int alloc_re(char **x,unsigned int m,unsigned int n);

#endif
