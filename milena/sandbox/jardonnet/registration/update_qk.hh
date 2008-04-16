#ifndef MLN_UPDATE_QK_HH
# define MLN_UPDATE_QK_HH

#include "interpolation.hh"
#include "quat7.hh"

namespace mln
{

  float arc(quat7<3> dqk, quat7<3> dqk_1)
  {
    float res = 0.0;
    for (int i = 0; i < 7; i++)
      res += dqk[i] * dqk_1[i];
    res = res / (dqk.sqr_norm() * dqk_1.sqr_norm());
    return acos(res) * 180.0 / 3.14159265;
  }

  
  quat7<3> update_qk(quat7<3> qk[4],
                     float    dk[3])
  {
    quat7<3> dqk_2 = qk[2] - qk[3];
    quat7<3> dqk_1 = qk[1] - qk[2];
    quat7<3> dqk   = qk[0] - qk[1];
    
    float tetak_1 = arc(dqk_1, dqk_2);
    float tetak   = arc(dqk,   dqk_1);

    if (tetak < 10 and tetak_1 < 10) //10 degrees
    {
      float vk[3];
      float dk[3];
      float coef[3];

      vk[0] = 0;
      vk[1] = - dqk.sqr_norm();
      vk[2] = - dqk_1.sqr_norm() + vk[1];

      float a1 = (dk[2] + dk[0]) / (vk[2] - vk[0]);
      float b1 = dk[1] - ((dk[2] + dk[0]) / (vk[2] - vk[0])) * vk[1];
      float v1 = math::abs(- b1 / a1); // > 0
     
      polcoe(vk, dk, 2, coef);
      float a2 = coef[0];
      float b2 = coef[1];
      float v2 = - b2 / (2. * a2);

      float vmax = 25 * dqk.sqr_norm();
      
      if ((0 < v2 and v2 < v1 and v1 < vmax) or
          (0 < v2 and v2 < vmax and vmax < v1))
        return qk[0] + (dqk / dqk.sqr_norm()) * v2;

      if ((0 < v1 and v1 < v2 and v2 < vmax) or
          (0 < v1 and v1 < vmax and vmax < v2) or
          (v2 < 0 and 0 < v1 and v1 < vmax))
        return qk[0] + (dqk / dqk.sqr_norm()) * v1;

      if (v1 > vmax and v2 > vmax)
        return qk[0] + (dqk / dqk.sqr_norm()) * vmax;

      std::cout << "echec" << std::endl;
    }
    
    return qk[0];
  }
    
}


#endif // ! MLN_UPDATE_QK_HH
