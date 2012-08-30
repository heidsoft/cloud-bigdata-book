/*
** Copyright 2001 Double Precision, Inc.
** See COPYING for distribution information.
*/

#define	SHA1_INTERNAL
#include	"sha1.h"
void ltSha1(const void *msg, unsigned len,unsigned char  *output);
void ltSha1A(const char *msg, unsigned char  *output);


#define	K0 0x5A827999
#define	K1 0x6ED9EBA1
#define	K2 0x8F1BBCDC
#define	K3 0XCA62C1D6

#define	K20(x)	x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x

static SHA1_WORD K[80] = { K20(K0), K20(K1), K20(K2), K20(K3) };

void sha1_context_init(struct SHA1_CONTEXT *c)
{
	if (sizeof(SHA1_WORD) != 4)
		abort();

	c->H[0] = 0x67452301;
	c->H[1] = 0xEFCDAB89;
	c->H[2] = 0x98BADCFE;
	c->H[3] = 0x10325476;
	c->H[4] = 0xC3D2E1F0;
	c->blk_ptr=0;
}

void sha1_context_hash(struct SHA1_CONTEXT *c,
		const unsigned char blk[SHA1_BLOCK_SIZE])
{
SHA1_WORD	A,B,C,D,E;
SHA1_WORD	TEMP;
SHA1_WORD	W[80];
unsigned	i, t;

#define f(t,B,C,D)	( \
	(t) < 20 ? ( (B) & (C) ) | ( (~(B)) & (D) ) : \
	(t) >= 40 && (t) < 60 ? ( (B) & (C) ) | ( (B) & (D) ) | ( (C) & (D) ):\
		(B) ^ (C) ^ (D) )

#define S(a,b) ( ((SHA1_WORD)(a) << (b)) | ((SHA1_WORD)(a) >> (32 - (b))))

	for (i=t=0; t<16; t++)
	{
		W[t]= blk[i]; i++;
		W[t] = (W[t] << 8) | blk[i]; i++;
		W[t] = (W[t] << 8) | blk[i]; i++;
		W[t] = (W[t] << 8) | blk[i]; i++;
	}

	for (t=16; t<80; t++)
	{
		TEMP= W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16];
		W[t]= S(TEMP, 1);
	}

	A=c->H[0];
	B=c->H[1];
	C=c->H[2];
	D=c->H[3];
	E=c->H[4];

	for (t=0; t<80; t++)
	{
		TEMP = S(A,5);
		TEMP += f(t, B, C, D);
		TEMP += E;
		TEMP += W[t];
		TEMP += K[t];

		E=D;
		D=C;
		C= S(B, 30);
		B=A;
		A=TEMP;
	}

	c->H[0] += A;
	c->H[1] += B;
	c->H[2] += C;
	c->H[3] += D;
	c->H[4] += E;
}

void sha1_context_hashstream(struct SHA1_CONTEXT *c, const void *p, unsigned l)
{
const unsigned char *cp=(const unsigned char *)p;
unsigned ll;

	while (l)
	{
		if (c->blk_ptr == 0 && l >= SHA1_BLOCK_SIZE)
		{
			sha1_context_hash(c, cp);
			cp += SHA1_BLOCK_SIZE;
			l -= SHA1_BLOCK_SIZE;
			continue;
		}

		ll=l;
		if (ll > SHA1_BLOCK_SIZE - c->blk_ptr)
			ll=SHA1_BLOCK_SIZE - c->blk_ptr;
		memcpy(c->blk + c->blk_ptr, cp, ll);
		c->blk_ptr += ll;
		cp += ll;
		l -= ll;
		if (c->blk_ptr >= SHA1_BLOCK_SIZE)
		{
			sha1_context_hash(c, c->blk);
			c->blk_ptr=0;
		}
	}
}

void sha1_context_endstream(struct SHA1_CONTEXT *c, unsigned long l)
{
unsigned char buf[8];
static unsigned char zero[SHA1_BLOCK_SIZE-8];

	buf[0]=0x80;
	sha1_context_hashstream(c, &buf, 1);
	while (c->blk_ptr != SHA1_BLOCK_SIZE-8)
	{
		if (c->blk_ptr > SHA1_BLOCK_SIZE-8)
		{
			sha1_context_hashstream(c, zero,
				SHA1_BLOCK_SIZE - c->blk_ptr);
			continue;
		}
		sha1_context_hashstream(c, zero,
			SHA1_BLOCK_SIZE-8-c->blk_ptr);
	}

	l *= 8;
	buf[7] = l;
	buf[6] = (l >>= 8);
	buf[5] = (l >>= 8);
	buf[4] = (l >> 8);
	buf[3]=buf[2]=buf[1]=buf[0]=0;

	sha1_context_hashstream(c, buf, 8);
}

void sha1_context_digest(struct SHA1_CONTEXT *c, SHA1_DIGEST d)
{
unsigned char *dp=d + SHA1_DIGEST_SIZE;
unsigned i;

	for ( i=5; i; )
	{
	SHA1_WORD	w=c->H[--i];

		*--dp=w; w >>= 8;
		*--dp=w; w >>= 8;
		*--dp=w; w >>= 8;
		*--dp=w;
	}
}

void sha1_context_restore(struct SHA1_CONTEXT *c, const SHA1_DIGEST d)
{
const unsigned char *dp=d;
unsigned i;

	for (i=0; i<5; i++)
	{
	SHA1_WORD	w= *dp++;

		w=(w << 8) | *dp++;
		w=(w << 8) | *dp++;
		w=(w << 8) | *dp++;
		c->H[i]=w;
	}
	c->blk_ptr=0;
}

void sha1_digest(const void *msg, unsigned len, SHA1_DIGEST d)
{
struct SHA1_CONTEXT c;

	sha1_context_init( &c );
	sha1_context_hashstream(&c, msg, len);
	sha1_context_endstream(&c, len);
	sha1_context_digest( &c, d );
}

void ltSha1(const void *msg, unsigned len,unsigned char  *output){
	char caOut[32],caOut1[32];
	SHA1_DIGEST d;
	sha1_digest(msg, len, d);
	strncpy(caOut,d,20);
	ltDesCvtAsc(caOut,20,caOut1);
	caOut1[20]=0;
    	strcpy((char *)output,(char *)caOut1);
	return;
}

void ltSha1A(const char *msg, unsigned char  *output){
	char caOut[32],caOut1[32];
	SHA1_DIGEST d;
	sha1_digest((void *)msg, strlen(msg), d);
	strncpy(caOut,d,20);
        ltDesCvtAsc(caOut,20,caOut1);
        caOut1[20]=0;
        strcpy((char *)output,(char *)caOut1);
        return;

}



