#ifndef FMT_H
#define FMT_H

#define FMT_ULONG 40 /* enough space to hold 2^128 - 1 in decimal, plus \0 */
#define FMT_LEN ((char *) 0) /* convenient abbreviation */

extern unsigned int fmt_uint(register char *s,register unsigned int u);
extern unsigned int fmt_uint0(char *s,unsigned int u,unsigned int n);
extern unsigned int fmt_xint();
extern unsigned int fmt_nbbint();
extern unsigned int fmt_ushort();
extern unsigned int fmt_xshort();
extern unsigned int fmt_nbbshort();
extern unsigned int fmt_ulong(register char *s,register unsigned long u);
extern unsigned int fmt_xlong();
extern unsigned int fmt_nbblong();

extern unsigned int fmt_plusminus();
extern unsigned int fmt_minus();
extern unsigned int fmt_0x();

extern unsigned int fmt_str(register char *s,register char *t);
extern unsigned int fmt_strn(register char *s,register char *t,unsigned int n);

#endif

