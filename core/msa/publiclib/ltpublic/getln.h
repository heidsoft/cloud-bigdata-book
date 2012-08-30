#ifndef GETLN_H
#define GETLN_H

#include "buffer.h"
#include "stralloc.h"

extern int getln(buffer *,stralloc *,int *,int);
extern int getln2(buffer *,stralloc *,char **,unsigned int *,int);

#endif
