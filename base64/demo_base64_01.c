#include <stdio.h>

#define CHAR_BIT 8
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))

// Returns the length of the base64 string
int base64_encode(unsigned char str[], unsigned char base64[], int len)
{
   int idx=0,idx2=0,base64_chars=0;
   unsigned char charset[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"},t=0;
   
   // Initialize the base64 array
   memset(base64,0,(len+1) * 1.334);
   // Continue converting while the bit limit for the str array has not been reached
   while (idx < len * CHAR_BIT) {
      // Pick off bits in chunks of 6
      for (idx2=0; idx2 < 6; idx++,idx2++) {
         if (BITTEST(str,idx))
            BITSET(&t,idx2);
      }
      // Map the binary value to its ASCII representation
      t = charset[t];
      // Save the new converted char to the base64 array, increment the base64 len counter,
      // and re-initialize the temp var
      base64[base64_chars] = t;
      base64_chars++;
      t = 0;
   }   
   // Return the number of Base64 characters that were used
   return(base64_chars);
}

// Returns the length of the plain text string
int base64_decode(unsigned char base64_t[], unsigned char str[], int len)
{
   int idx=0,idx2=0,idx3,str_chars=0;
   unsigned char charset[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"},t=0,base64[1000];
   
   // Initialize the str array
   memset(str,0,(len+1) * .75);
   // Un-map the ASCII characters to their binary representations
   for (idx=0; idx < len; idx++) {
      for (idx3=0; 1; idx3++) {
         if (charset[idx3] == base64_t[idx]) {
            base64[idx] = idx3;
            break;
         }   
      }
   }   
   // Combine the significant 6-bit blocks from each byes of the base64 array
   for (idx=0,idx2=0; idx < len * CHAR_BIT; idx++,idx2++) {
      // Skip two "lame" bits ever six
      if (!(idx2 % 6) && idx2)
         idx += 2;
      // Log how many full 8-bit chars have been filled
      if (!(idx2 % 8) && idx2)
         str_chars++;
      if (BITTEST(base64,idx))
         BITSET(str,idx2);
   } 
   // Return the length of the text array
   return(str_chars);
}


int main()
{
   unsigned char str[100]={"sadfymkf7 6756"},base[100],t;
   
   t = base64_encode(str,base,strlen(str));
   printf("\n");
   t = base64_decode(base,str,t);
   
   getchar();
}
