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

namespace mln
{

  namespace registration
  {
  
    template <typename P>
    P center(const p_array<P>& a, size_t length)
    {
      algebra::vec<P::dim,float> c(literal::zero);
      for (size_t i = 0; i < length; ++i)
        {
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
              M& map,
              const size_t length)
    {
      assert(length <= a1.npoints());
     
      float f = 0.f;
      for (size_t i = 0; i < length; ++i)
        {
          algebra::vec<3,float> a1f = a1[i];
          algebra::vec<3,float> a2f = map(a1[i]);
          f += norm::l2(a1f - a2f);
        }
      return f / a1.npoints();
    }

  } // end of namespace registration
} // end of namespace mln

#endif // ndef CLOUD_HH
