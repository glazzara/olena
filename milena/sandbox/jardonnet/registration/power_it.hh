#ifndef POWER_IT_HH
# define POWER_IT_HH


#include <mln/algebra/mat.hh>
#include <cmath>
#include "misc.hh"

// from num. rec. in C


namespace mln
{

  /**
   * Return the biggest eigen vector.
   */
  template <uint n>
  algebra::vec<n,float> power_it(algebra::mat<n,n,float>& A)
  {
    algebra::vec<n,float> b0(literal::zero);
    algebra::vec<n,float> bk(literal::zero);
    for (unsigned i = 0; i < n; i++)
      bk[i] = 0.1;
    bk[0] = 1;
    bk[1] = 1;

    //FIXME: infinit loop
    while (!about_equal(norm::l2(bk),norm::l2(b0)))
      {
        b0 = bk;
        bk = A * bk;
        bk.normalize();
      }
    return bk.normalize();;
  }
}

#endif /* POWER_IT_HH */
