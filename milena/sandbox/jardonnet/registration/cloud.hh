#ifndef CLOUD_HH
# define CLOUD_HH

# include <assert.h>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>

# include <mln/algebra/vec.hh>

namespace mln
{

  namespace registration
  {
  
    typedef algebra::vec<3, float> vec3f;

    
    vec3f center(const std::vector< vec3f >& vecs)
    {
      vec3f c;
      for (size_t i = 0; i < vecs.size(); ++i)
        c += vecs[i];
      return c / vecs.size();
    }

    
    // FIXME : move
    float sqr_norm(const vec3f& v)
    {
      return v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
    }

    float rms(const std::vector< vec3f >& vecs1,
              const std::vector< vec3f >& vecs2)
    {
      assert(vecs1.size() == vecs2.size());
      float f = 0.f;
      for (size_t i = 0; i < vecs1.size(); ++i)
        f += sqr_norm(vecs1[i] - vecs2[i]);
      return f / vecs1.size();
    }

  }
}

#endif // ndef CLOUD_HH
