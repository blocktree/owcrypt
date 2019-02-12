#include "ref10_ge.h"

static const REF10_fe d = {
#include "ref10_d.h"
} ;

static const REF10_fe sqrtm1 = {
#include "ref10_sqrtm1.h"
} ;

int REF10_ge_frombytes_negate_vartime(REF10_ge_p3 *h,const unsigned char *s)
{
  REF10_fe u;
  REF10_fe v;
  REF10_fe v3;
  REF10_fe vxx;
  REF10_fe check;

  REF10_fe_frombytes(h->Y,s);
  REF10_fe_1(h->Z);
  REF10_fe_sq(u,h->Y);
  REF10_fe_mul(v,u,d);
  REF10_fe_sub(u,u,h->Z);       /* u = y^2-1 */
  REF10_fe_add(v,v,h->Z);       /* v = dy^2+1 */

  REF10_fe_sq(v3,v);
  REF10_fe_mul(v3,v3,v);        /* v3 = v^3 */
  REF10_fe_sq(h->X,v3);
  REF10_fe_mul(h->X,h->X,v);
  REF10_fe_mul(h->X,h->X,u);    /* x = uv^7 */

  REF10_fe_pow22523(h->X,h->X); /* x = (uv^7)^((q-5)/8) */
  REF10_fe_mul(h->X,h->X,v3);
  REF10_fe_mul(h->X,h->X,u);    /* x = uv^3(uv^7)^((q-5)/8) */

  REF10_fe_sq(vxx,h->X);
  REF10_fe_mul(vxx,vxx,v);
  REF10_fe_sub(check,vxx,u);    /* vx^2-u */
  if (REF10_fe_isnonzero(check)) {
    REF10_fe_add(check,vxx,u);  /* vx^2+u */
    if (REF10_fe_isnonzero(check)) return -1;
    REF10_fe_mul(h->X,h->X,sqrtm1);
  }

  if (REF10_fe_isnegative(h->X) == (s[31] >> 7))
    REF10_fe_neg(h->X,h->X);

  REF10_fe_mul(h->T,h->X,h->Y);
  return 0;
}
