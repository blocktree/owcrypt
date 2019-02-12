#include "ref10_ge.h"

static void slide(signed char *r,const unsigned char *a)
{
  int i;
  int b;
  int k;

  for (i = 0;i < 256;++i)
    r[i] = 1 & (a[i >> 3] >> (i & 7));

  for (i = 0;i < 256;++i)
    if (r[i]) {
      for (b = 1;b <= 6 && i + b < 256;++b) {
        if (r[i + b]) {
          if (r[i] + (r[i + b] << b) <= 15) {
            r[i] += r[i + b] << b; r[i + b] = 0;
          } else if (r[i] - (r[i + b] << b) >= -15) {
            r[i] -= r[i + b] << b;
            for (k = i + b;k < 256;++k) {
              if (!r[k]) {
                r[k] = 1;
                break;
              }
              r[k] = 0;
            }
          } else
            break;
        }
      }
    }

}

static REF10_ge_precomp Bi[8] = {
#include "ref10_base2.h"
} ;

/*
r = a * A + b * B
where a = a[0]+256*a[1]+...+256^31 a[31].
and b = b[0]+256*b[1]+...+256^31 b[31].
B is the Ed25519 base point (x,4/5) with x positive.
*/

void REF10_ge_double_scalarmult_vartime(REF10_ge_p2 *r,const unsigned char *a,const REF10_ge_p3 *A,const unsigned char *b)
{
  signed char aslide[256];
  signed char bslide[256];
  REF10_ge_cached Ai[8]; /* A,3A,5A,7A,9A,11A,13A,15A */
  REF10_ge_p1p1 t;
  REF10_ge_p3 u;
  REF10_ge_p3 A2;
  int i;

  slide(aslide,a);
  slide(bslide,b);

  REF10_ge_p3_to_cached(&Ai[0],A);
  REF10_ge_p3_dbl(&t,A); REF10_ge_p1p1_to_p3(&A2,&t);
  REF10_ge_add(&t,&A2,&Ai[0]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[1],&u);
  REF10_ge_add(&t,&A2,&Ai[1]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[2],&u);
  REF10_ge_add(&t,&A2,&Ai[2]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[3],&u);
  REF10_ge_add(&t,&A2,&Ai[3]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[4],&u);
  REF10_ge_add(&t,&A2,&Ai[4]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[5],&u);
  REF10_ge_add(&t,&A2,&Ai[5]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[6],&u);
  REF10_ge_add(&t,&A2,&Ai[6]); REF10_ge_p1p1_to_p3(&u,&t); REF10_ge_p3_to_cached(&Ai[7],&u);

  REF10_ge_p2_0(r);

  for (i = 255;i >= 0;--i) {
    if (aslide[i] || bslide[i]) break;
  }

  for (;i >= 0;--i) {
    REF10_ge_p2_dbl(&t,r);

    if (aslide[i] > 0) {
      REF10_ge_p1p1_to_p3(&u,&t);
      REF10_ge_add(&t,&u,&Ai[aslide[i]/2]);
    } else if (aslide[i] < 0) {
      REF10_ge_p1p1_to_p3(&u,&t);
      REF10_ge_sub(&t,&u,&Ai[(-aslide[i])/2]);
    }

    if (bslide[i] > 0) {
      REF10_ge_p1p1_to_p3(&u,&t);
      REF10_ge_madd(&t,&u,&Bi[bslide[i]/2]);
    } else if (bslide[i] < 0) {
      REF10_ge_p1p1_to_p3(&u,&t);
      REF10_ge_msub(&t,&u,&Bi[(-bslide[i])/2]);
    }

    REF10_ge_p1p1_to_p2(r,&t);
  }
}
