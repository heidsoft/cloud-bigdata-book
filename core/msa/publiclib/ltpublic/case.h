#ifndef CASE_H
#define CASE_H

extern void case_lowers(char *s);
extern void case_lowerb(char *s,unsigned int len);
extern int case_diffs(register char *s,register char *t);
extern int case_diffb(register char *s,unsigned intlen,register char *t);
extern int case_starts(register char *s,register char *t);
extern int case_startb();

#define case_equals(s,t) (!case_diffs((s),(t)))

#endif
