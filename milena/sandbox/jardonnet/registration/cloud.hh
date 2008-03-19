#ifndef CLOUD_HH
# define CLOUD_HH

# include <assert.h>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>

# include <mln/algebra/vec.hh>
# include <mln/core/p_array.hh>

namespace mln
{

  namespace registration
  {
  
    template <typename P>
    P center(const p_array<P>& a)
    {
      algebra::vec<P::dim,float> c;
      for (size_t i = 0; i < a.npoints(); ++i)
        {
          algebra::vec<P::dim,float> ai = a[i];
          c += ai;
        }
      
      return algebra::to_point<P>(c / a.npoints());
    }

    
    // FIXME : move //exist for P?
    template <typename P>
    float sqr_norm(const P& v)
    {
      return v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
    }

    template <typename P>
    float rms(const p_array<P>& a1,
              const p_array<P>& a2)
    {
      assert(a1.npoints() == a2.npoints());
      float f = 0.f;
      for (size_t i = 0; i < a1.npoints(); ++i)
        f += sqr_norm(a1[i] - a2[i]);
      return f / a1.npoints();
    }

  }
}

#endif // ndef CLOUD_HH
