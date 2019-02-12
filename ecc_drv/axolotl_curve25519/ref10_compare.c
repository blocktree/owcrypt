#include <string.h>
#include "ref10_compare.h"

/* Const-time comparison from SUPERCOP, but here it's only used for 
   signature verification, so doesn't need to be const-time.  But
   copied the nacl version anyways. */
int REF10_crypto_verify_32_ref(const unsigned char *x, const unsigned char *y)
{
  unsigned int differentbits = 0;
#define REF10_F(i) differentbits |= x[i] ^ y[i];
  REF10_F(0)
  REF10_F(1)
  REF10_F(2)
  REF10_F(3)
  REF10_F(4)
  REF10_F(5)
  REF10_F(6)
  REF10_F(7)
  REF10_F(8)
  REF10_F(9)
  REF10_F(10)
  REF10_F(11)
  REF10_F(12)
  REF10_F(13)
  REF10_F(14)
  REF10_F(15)
  REF10_F(16)
  REF10_F(17)
  REF10_F(18)
  REF10_F(19)
  REF10_F(20)
  REF10_F(21)
  REF10_F(22)
  REF10_F(23)
  REF10_F(24)
  REF10_F(25)
  REF10_F(26)
  REF10_F(27)
  REF10_F(28)
  REF10_F(29)
  REF10_F(30)
  REF10_F(31)
  return (1 & ((differentbits - 1) >> 8)) - 1;
}
