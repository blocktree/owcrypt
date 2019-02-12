#include "ref10_ge.h"
#include "ref10_crypto_uint32.h"

static unsigned char REF10_equal(signed char b,signed char c)
{
  unsigned char ub = b;
  unsigned char uc = c;
  unsigned char x = ub ^ uc; /* 0: yes; 1..255: no */
  REF10_crypto_uint32 y = x; /* 0: yes; 1..255: no */
  y -= 1; /* 4294967295: yes; 0..254: no */
  y >>= 31; /* 1: yes; 0: no */
  return y;
}

static unsigned char REF10_negative(signed char b)
{
  unsigned long long x = b; /* 18446744073709551361..18446744073709551615: yes; 0..255: no */
  x >>= 63; /* 1: yes; 0: no */
  return x;
}

static void REF10_cmov(REF10_ge_precomp *t,REF10_ge_precomp *u,unsigned char b)
{
  REF10_fe_cmov(t->yplusx,u->yplusx,b);
  REF10_fe_cmov(t->yminusx,u->yminusx,b);
  REF10_fe_cmov(t->xy2d,u->xy2d,b);
}

/* base[i][j] = (j+1)*256^i*B */
static REF10_ge_precomp base[32][8] = {
#include "ref10_base.h"
} ;

static void REF10_select(REF10_ge_precomp *t,int pos,signed char b)
{
  REF10_ge_precomp minust;
  unsigned char bnegative = REF10_negative(b);
  unsigned char babs = b - (((-bnegative) & b) << 1);

  REF10_ge_precomp_0(t);
  REF10_cmov(t,&base[pos][0],REF10_equal(babs,1));
  REF10_cmov(t,&base[pos][1],REF10_equal(babs,2));
  REF10_cmov(t,&base[pos][2],REF10_equal(babs,3));
  REF10_cmov(t,&base[pos][3],REF10_equal(babs,4));
  REF10_cmov(t,&base[pos][4],REF10_equal(babs,5));
  REF10_cmov(t,&base[pos][5],REF10_equal(babs,6));
  REF10_cmov(t,&base[pos][6],REF10_equal(babs,7));
  REF10_cmov(t,&base[pos][7],REF10_equal(babs,8));
  REF10_fe_copy(minust.yplusx,t->yminusx);
  REF10_fe_copy(minust.yminusx,t->yplusx);
  REF10_fe_neg(minust.xy2d,t->xy2d);
  REF10_cmov(t,&minust,bnegative);
}

/*
h = a * B
where a = a[0]+256*a[1]+...+256^31 a[31]
B is the Ed25519 base point (x,4/5) with x positive.

Preconditions:
  a[31] <= 127
*/

void REF10_ge_scalarmult_base(REF10_ge_p3 *h,const unsigned char *a)
{
  signed char e[64];
  signed char carry;
  REF10_ge_p1p1 r;
  REF10_ge_p2 s;
  REF10_ge_precomp t;
  int i;

  for (i = 0;i < 32;++i) {
    e[2 * i + 0] = (a[i] >> 0) & 15;
    e[2 * i + 1] = (a[i] >> 4) & 15;
  }
  /* each e[i] is between 0 and 15 */
  /* e[63] is between 0 and 7 */

  carry = 0;
  for (i = 0;i < 63;++i) {
    e[i] += carry;
    carry = e[i] + 8;
    carry >>= 4;
    e[i] -= carry << 4;
  }
  e[63] += carry;
  /* each e[i] is between -8 and 8 */

  REF10_ge_p3_0(h);
  for (i = 1;i < 64;i += 2) {
    REF10_select(&t,i / 2,e[i]);
    REF10_ge_madd(&r,h,&t); REF10_ge_p1p1_to_p3(h,&r);
  }

  REF10_ge_p3_dbl(&r,h);  REF10_ge_p1p1_to_p2(&s,&r);
  REF10_ge_p2_dbl(&r,&s); REF10_ge_p1p1_to_p2(&s,&r);
  REF10_ge_p2_dbl(&r,&s); REF10_ge_p1p1_to_p2(&s,&r);
  REF10_ge_p2_dbl(&r,&s); REF10_ge_p1p1_to_p3(h,&r);

  for (i = 0;i < 64;i += 2) {
    REF10_select(&t,i / 2,e[i]);
    REF10_ge_madd(&r,h,&t); REF10_ge_p1p1_to_p3(h,&r);
  }
}
