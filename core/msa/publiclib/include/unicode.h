#ifndef	unicode_h
#define	unicode_h

/*
** Copyright 2000 Double Precision, Inc.
** See COPYING for distribution information.
**
** $Id: unicode.h,v 1.5 2000/12/24 03:26:43 mrsam Exp $
*/

#ifdef	__cplusplus
extern "C" {
#endif

#include	"config.h"

#include	<stdlib.h>

#if HAVE_WCHAR_H
#include	<wchar.h>
#endif

#if HAVE_STDDEF_H
#include	<stddef.h>
#endif

typedef wchar_t unicode_char;

struct unicode_info {
	const char *chset;		/* Official character set */
	unicode_char *(*c2u)(const char *, int *);
		/* Convert character string in this charset to unicode */

	char *(*u2c)(const unicode_char *, int *);
		/* TODO: Convert unicode to character string in this charset */

	/* Convert the string in this character set to upper/lower/titlecase */

	char *(*toupper_func)(const char *, int *);
	char *(*tolower_func)(const char *, int *);
	char *(*totitle_func)(const char *, int *);
	} ;

extern const struct unicode_info unicode_ISO8859_1;
extern const struct unicode_info unicode_UTF8;

extern char *unicode_iso8859_u2c(const unicode_char *, int *,
	const unicode_char *);

/* ISO8859 charsets all share the same functions */

extern unicode_char *unicode_iso8859_c2u(const char *, int *,
					const unicode_char *);

extern char *unicode_iso8859_convert(const char *, int *,
					const char *);

/* IBM864 charset has some funkiness */

unicode_char *unicode_ibm864_c2u(const char *, int *,
				 const unicode_char *);

char *unicode_ibm864_u2c(const unicode_char *, int *,
			 const unicode_char *);


struct unicode_chsetlist_s {
	const char *chsetname;
	const struct unicode_info *ptr;
	} ;

extern const struct unicode_chsetlist_s unicode_chsetlist[];
extern const char *unicode_default_chset();
extern const struct unicode_info *unicode_find(const char *);

/*
** UTF8 functions
*/

	/* Convert Unicode to/from UTF-8 */

extern char *unicode_toutf8(const unicode_char *);
extern unicode_char *unicode_fromutf8(const char *);

	/* Unicode upper/lower/title case conversion functions */

extern unicode_char unicode_uc(unicode_char);
extern unicode_char unicode_lc(unicode_char);
extern unicode_char unicode_tc(unicode_char);

	/* Convert charsets to/from UTF-8 */

extern char *unicode_ctoutf8(const struct unicode_info *, const char *,
			     int *);
extern char *unicode_cfromutf8(const struct unicode_info *, const char *,
			       int *);

	/* Internal functions: */

extern unicode_char *unicode_utf8_tou(const char *, int *);
extern char *unicode_utf8_fromu(const unicode_char *, int *);

	/* Convert between different character sets */

extern char *unicode_convert(const char *txt,
			     const struct unicode_info *from,
			     const struct unicode_info *to);
	/* errno=EINVAL if conversion could not be performed */

extern char *unicode_xconvert(const char *txt,
			      const struct unicode_info *from,
			      const struct unicode_info *to);
	/* Like unicode_convert(), except unconvertable chars are replaced
	** by periods (or something similar), instead of aborting with EINVAL
	*/


extern char *unicode_convert_fromchset(const char *txt,
				    const char *from,
				    const struct unicode_info *to);
	/* Like, unicode_convert, except that we search for a character set
	** from a list of chsets we support.
	** errno=EINVAL if 'to' character set does not exist.
	*/

	/*
	** Convert between unicode and modified-UTF7 encoding used for
	** IMAP folder names.
	*/

unicode_char *unicode_modutf7touc(const char *s, int *err);

	/* err < 0 if out of memory, else ptr to first illegal modutf7-char */
	/* This can be used to test if string is a valid mod-utf7 string */

char *unicode_uctomodutf7(const unicode_char *);

char *gb2312_to_utf8(const char *cp, int *err);

int ltCvtGbToBig5(unsigned char *inStr,int iLen);
int ltCvtGbToBig5Head(unsigned char *inStr,int iLen);

#ifdef	__cplusplus
}
#endif

#endif
