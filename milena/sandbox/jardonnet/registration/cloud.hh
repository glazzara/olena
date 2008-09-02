#ifndef CLOUD_HH
# define CLOUD_HH

# include <assert.h>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>

# include <mln/algebra/vec.hh>
# include <mln/core/p_array.hh>
# include <mln/norm/l2.hh>

#include "quat7.hh"

namespace mln
{

  namespace geom
  {
    template <typename P>
    P center(const p_array<P>& a)
    {
      if (a.npoints() == 0)
        return P();
      
      algebra::vec<P::dim,float> c(literal::zero);
      for (unsigned i = 0; i < a.npoints(); ++i)
        {
          // FIXME : Ugly.
          algebra::vec<P::dim,float> ai = a[i];
          c += ai;
        }
      
      return algebra::to_point<P>(c / a.npoints());
    }
  }
  
  namespace registration
  {
  
    template <typename P>
    P center(const p_array<P>& a, size_t length)
    {
      algebra::vec<P::dim,float> c(literal::zero);
      for (unsigned i = 0; i < length; ++i)
        {
          // FIXME : Ugly.
          algebra::vec<P::dim,float> ai = a[i];
          c += ai;
        }
      
      return algebra::to_point<P>(c / length);
    }

    
    // FIXME : move //exist for P?
    template <typename P>
    float sqr_norm(const P& v)
    {
      float tmp = 0;
      for (unsigned i = 0; i < P::dim; i++)
        tmp += v[i] * v[i];
      return tmp;
    }

    template <typename P, typename M>
    float rms(const p_array<P>& a1,
              quat7<P::dim>& qk,
              const M& map,
              const size_t length)
    {
      assert(length <= a1.npoints());
     
      float f = 0.f;
      for (size_t i = 0; i < length; ++i)
        {
          algebra::vec<3,float> a1f = qk(a1[i]);
          algebra::vec<3,float> a2f = map(algebra::to_point<P>(qk(a1[i])));
          f += norm::l2(a1f - a2f);
        }
      return f / length;
    }

    template <typename P, typename M>
    float rms(const p_array<P>& a1,
              M& map,
              const size_t length,
              quat7<P::dim>& q1,
              quat7<P::dim>& q2)
    {
      assert(length <= a1.npoints());
     
      float f = 0.f;
      for (size_t i = 0; i < length; ++i)
        {
          algebra::vec<3,float> a2f = q2(a1[i]);
          algebra::vec<3,float> a1f = map(algebra::to_point<P>(q1(a1[i])));
          f += norm::l2(a1f - a2f);
        }
      return f / length;
    }

  } // end of namespace registration
} // end of namespace mln

#endif // ndef CLOUD_HH
