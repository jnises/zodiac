/*
 * @author Joel Nises
 */

// This file does nothing currently

#include "Mhvl.h"

namespace mhvl
{
    /*float InvSqrt (float x)
      {
      float xhalf = 0.5f*x;
      int i = *(reinterpret_cast<int*>(&x));
      i = 0x5f3759df - (i>>1);
      x = *(float*)&i;
      x = x*(1.5f - xhalf*x*x);
      return x;
      }*/

    /** Normalization using approximated sqrt. */
    /*void normalizeInPlaceApprox(Vector3f &vec)
      {
      float sqrtdiv=InvSqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);

      vec[0]*=sqrtdiv;
      vec[1]*=sqrtdiv;
      vec[2]*=sqrtdiv;
      }

      Vector3f normalizeApprox(const Vector3f &vec)
      {
      Vector3f tmpvec=vec;

      float sqrtdiv=InvSqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);

      tmpvec[0]*=sqrtdiv;
      tmpvec[1]*=sqrtdiv;
      tmpvec[2]*=sqrtdiv;

      return tmpvec;
      }*/

}
