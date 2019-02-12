#include "ref10_ge.h"

/*
r = p
*/

static const REF10_fe d2 = {
#include "ref10_d2.h"
} ;

extern void REF10_ge_p3_to_cached(REF10_ge_cached *r,const REF10_ge_p3 *p)
{
  REF10_fe_add(r->YplusX,p->Y,p->X);
  REF10_fe_sub(r->YminusX,p->Y,p->X);
  REF10_fe_copy(r->Z,p->Z);
  REF10_fe_mul(r->T2d,p->T,d2);
}
