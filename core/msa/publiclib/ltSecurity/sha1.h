#ifndef	sha1_h
#define	sha1_h


static const char sha1_h_rcsid[]="$Id: sha1.h,v 1.2 2001/04/19 01:20:46 mrsam Exp $";

#if	HAVE_CONFIG_H
#include	"config.h"
#endif

#if	HAVE_SYS_TYPES_H
#include	<sys/types.h>
#endif
#if	HAVE_SYNCH_H
#include	<synch.h>
#endif

#ifdef	HAVE_U_INT32_T
#define	SHA1_WORD	u_int32_t
#else
#ifdef	HAVE_UINT32_T
#define	SHA1_WORD	uint32_t
#else
#define	SHA1_WORD	unsigned long
#endif
#endif

#define	SHA1_DIGEST_SIZE	20
#define	SHA1_BLOCK_SIZE		64

#ifdef	__cplusplus
extern "C" {
#endif

typedef	unsigned char SHA1_DIGEST[20];

#ifdef	SHA1_INTERNAL

struct SHA1_CONTEXT {

	SHA1_WORD	H[5];

	unsigned char blk[SHA1_BLOCK_SIZE];
	unsigned blk_ptr;
	} ;

void sha1_context_init(struct SHA1_CONTEXT *);
void sha1_context_hash(struct SHA1_CONTEXT *,
		const unsigned char[SHA1_BLOCK_SIZE]);
void sha1_context_hashstream(struct SHA1_CONTEXT *, const void *, unsigned);
void sha1_context_endstream(struct SHA1_CONTEXT *, unsigned long);
void sha1_context_digest(struct SHA1_CONTEXT *, SHA1_DIGEST);
void sha1_context_restore(struct SHA1_CONTEXT *, const SHA1_DIGEST);

#endif

void sha1_digest(const void *, unsigned, SHA1_DIGEST);
const char *sha1_hash(const char *);

#ifdef	__cplusplus
 } ;
#endif

#endif
