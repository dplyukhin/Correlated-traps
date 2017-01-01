#include "rng.h"
#include <stdio.h>
#include <stdlib.h>

RandState* initRandState(long seed) {
  RandState *s = (RandState *) malloc(sizeof(RandState));
  if (s == NULL)
    fprintf(stderr, "Could not initialize RandState");

  s->idum = seed;
  s->idum2 = 123456789;
  s->iy = 0;

  return s;
}

/* gaussian random number generator using ran2  */
/* Box-Muller method from the Numerical Recipeis                  */
double ran2(RandState *s)
{
  int j;
  long k;
  float temp;

  if (s->idum <= 0) {
    if (-(s->idum) < 1) s->idum=1;
    else s->idum = -(s->idum);
    s->idum2=(s->idum);
    for (j=NTAB+7;j>=0;j--) {
      k=(s->idum)/IQ1;
      s->idum=IA1*(s->idum-k*IQ1)-k*IR1;
      if (s->idum < 0) s->idum += IM1;
      if (j < NTAB) s->iv[j] = s->idum;
    }
    s->iy=s->iv[0];
  }
  k=(s->idum)/IQ1;
  s->idum=IA1*(s->idum-k*IQ1)-k*IR1;
  if (s->idum < 0) s->idum += IM1;
  k=s->idum2/IQ2;
  s->idum2=IA2*(s->idum2-k*IQ2)-k*IR2;
  if (s->idum2 < 0) s->idum2 += IM2;
  j=s->iy/NDIV;
  s->iy=s->iv[j]-s->idum2;
  s->iv[j] = s->idum;
  if (s->iy < 1) s->iy += IMM1;
  if ((temp=AM*s->iy) > RNMX) return RNMX;
  else return temp;
}


