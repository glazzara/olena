#ifndef MLN_UPDATE_QK_HH
# define MLN_UPDATE_QK_HH

#include "interpolation.hh"
#include "quat7.hh"

namespace mln
{

  float arc(quat7<3> dqk, quat7<3> dqk_1)
  {
    //algebra::mat
    
    return 0;
  }

  
  quat7<3> update_qk(quat7<3> qk[3],
                     float    dk[3])
  {
    static float tetak = 11;
    static quat7<3> dqk;
    
    quat7<3> dqk_1 = dqk;
             dqk   = qk[0] - qk[1];
    
    float tetak_1 = tetak;
          tetak   = arc(dqk, dqk_1);
    
    if (tetak < 10 and tetak_1 < 10) //10degrees
    {
      float vk[3];
      float dk[3];
      float coef[3];
      
      vk[0] = 0;
      vk[1] = - dqk.norm();
      vk[2] = - dqk_1.norm() + vk[1];

      float a1 = (dk[0] + dk[2]) / (vk[2] - vk[0]);
      float b1 = dk[1] - ((dk[0] + dk[2]) / (vk[2] - vk[0])) * vk[1];
      float v1 = - b1 / a1; // > 0
      
      polcoe(vk, dk, 3, coef);
      float a2 = coef[0];
      float b2 = coef[1];
      float v2 = -b2 / (2. * a2);

      float vmax = 25 * dqk.norm();

      if (0 < v2 and v2 < v1 and v1 < vmax)
        return qk[0] + (dqk / dqk.norm()) * v2;
           
      if (0 < v1 and v1 < v2 and v2 < vmax)
        return qk[0] + (dqk / dqk.norm()) * v1;
     
      if (v1 > vmax and v2 > vmax)      
        return qk[0] + (dqk / dqk.norm()) * vmax;

      
      return qk[0];
    }
  }
    
}


#endif // ! MLN_UPDATE_QK_HH
