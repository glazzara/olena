#ifndef POWER_IT_HH
# define POWER_IT_HH


#include <mln/algebra/mat.hh>
#include <cmath>
#include "misc.hh"

// from num. rec. in C


namespace mln
{

  algebra::quat power_it(algebra::mat<4,4,float> A)
  {
    float    normex = 1.;

    algebra::vec<4,float> x0(literal::zero);
    algebra::vec<4,float> b(literal::zero);

    algebra::vec<4,float> x(literal::zero);
    for (int i = 0; i < 4; i++)
      x[i] = 1.;

    while (fabs(norm::l2(x) - norm::l2(x0)) > 1e-6)
      {
        x0 = x;
        normex = norm::l2(x);
        b = x / normex;
        x = A * b;
      }
    normex = norm::l2(x);
    
    algebra::quat q(x / normex);
    q.set_unit();
    return q;
  }
}

#endif /* POWER_IT_HH */
