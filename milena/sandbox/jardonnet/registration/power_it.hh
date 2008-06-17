#ifndef POWER_IT_HH
# define POWER_IT_HH


#include <mln/algebra/mat.hh>
#include <cmath>
#include "misc.hh"

// from num. rec. in C


namespace mln
{

  template <uint n>
  algebra::vec<n,float> power_it(algebra::mat<n,n,float> A)
  {
    float    normex = 1.;

    algebra::vec<n,float> x0(literal::zero);
    algebra::vec<n,float> b(literal::zero);

    algebra::vec<n,float> x(literal::zero);
    for (int i = 0; i < n; i++)
      x[i] = 1.;

    //FIXME: infinit loop.
    while (about_equal(norm::l2(x),norm::l2(x0)))
      {
        x0 = x;
        normex = norm::l2(x);
        b = x / normex;
        x = A * b;
      }
    return x.normalize();
  }
}

#endif /* POWER_IT_HH */
