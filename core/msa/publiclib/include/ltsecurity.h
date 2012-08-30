#ifndef _LTSECURITY_
#define _LTSECURITY_

/*md501.c*/
unsigned long ltMd5Code(char *pInput,int iLen,char *pKey);
char  *ltMd5Encrypto(unsigned char  *pInput, int iInLen,unsigned char  *pOutput);

/*b64.c*/
int             ltEncode64(const char *_in, unsigned inlen,
                        char *_out, unsigned outmax, unsigned *outlen);
int             ltDecode64(const char *in, unsigned inlen, char *out, unsigned *outlen);

/*des01.c*/
int ltDesEncrypt(char *in,int iLen,char *out,char *key);
int ltDesDecrypt(char *in,int in_len,char *out,char *key);
int ltDesEncryptA(unsigned char *in,unsigned char *out,unsigned char *key);
int ltDesDecryptA(unsigned char *in,unsigned char *out,unsigned char *key);

/*des02.c*/
int ltDesEncryptFile(char *pInFile,char *pOutFile,char *pKey);
int ltDesDencryptFile(char *pInFile,char *pOutFile,char *pKey);


/*sha1.c*/
void ltSha1(const void *msg, unsigned len,unsigned char  *output);
void ltSha1A(const char *msg, unsigned char  *output);

#endif
