
#ifndef QUAT7_HH
# define QUAT7_HH

# include <assert.h>
# include <algorithm>

# include <mln/algebra/mat.hh>

# include "quat/all.hh"
# include "jacobi.hh"

# include <mln/norm/l2.hh>

# include <mln/trait/all.hh>


// FIXME : move elsewhere
namespace mln
{
  namespace algebra
  {
    
    template<unsigned n, unsigned m, typename T>
    mat<m,n,T>
    trans(const mat<n,m,T>& matrice)
    {
      mat<m,n,T> tmp;
      for (unsigned i = 0; i < n; ++i)
        for (unsigned j = 0; j < m; ++j)
          tmp(j,i) = matrice(i,j);
      return tmp;
    }
    
    // trace
    
    template<unsigned n, typename T> inline
    float tr(const mat<n,n,T>& m)
    {
      float f = 0.f;
      for (unsigned i = 0; i < n; ++i)
        f += m(i,i);
      return f;
    }
    
  }
}

namespace mln
{

  struct quat7
  {
    value::quat _qR;
    vec3f _qT;
    
    quat7()
    {
    }
    
    quat7(const value::quat& qR, const vec3f& qT) :
      _qR(qR),
      _qT(qT)
    {
    }
    
    float sqr_norm() const
    {
      return norm::l2(_qR.to_vec()) + norm::l2(_qT);
    }
    
    quat7 operator-(const quat7& rhs) const
    {
      return quat7(_qR - rhs._qR, _qT - rhs._qT);
    }
    
    // quat7 is an object-function
    
    vec3f operator()(const vec3f& v) const
    {
      return rotate(_qR, v) + _qT;
    }
    
    void apply_on(const std::vector< vec3f >& input, std::vector< vec3f >& output) const
    {
      assert(input.size() == output.size());
      assert(_qR.is_unit());
      
      std::transform(input.begin(), input.end(),
                     output.begin(),
                     *this);
    }
  };
  

  // very usefull routine
  
  template <unsigned p, unsigned q, unsigned n, unsigned m>
  void put(const algebra::mat<p,q,float>& in, // a matrix to put into...
           unsigned row, unsigned col,        // top-left location
           algebra::mat<n,m,float>& inout)    // ...a matrix to modify
  {
    assert(row + p <= n && col + q <= m);
    for (unsigned i = 0; i < p; ++i)
      for (unsigned j = 0; j < q; ++j)
        inout(row + i, col + j) = in(i,j);
  }


  quat7 match(const vecs_t& P,
              const vec3f& mu_P,
              const vecs_t& Xk,
              const vec3f& mu_Xk)
  {
    assert(P.size() == Xk.size());
    
    // qR
    
    algebra::mat<3,3,float> Ck;
    for (unsigned i = 0; i < P.size(); ++i)
      Ck += make::mat(P[i] - mu_P) * trans(make::mat(Xk[i] - mu_Xk));
    Ck /= P.size();
    
    const algebra::mat<3,3,float> Ak = Ck - trans(Ck);

    const float v[3] = {Ak(1,2), Ak(2,0), Ak(0,1)};
    const algebra::mat<3,1,float> D = make::mat<3,1,3,float>(v); // FIXME why <...>
    
    algebra::mat<4,4,float> Qk;
    Qk(0,0) = tr(Ck);
    put(trans(D), 0,1, Qk);
    put(D, 1,0, Qk);

    put(Ck + trans(Ck) - algebra::mat<3,3,float>::identity() * tr(Ck), 1,1, Qk);
    
    value::quat qR;
    jacobi(Qk, qR);
    
    // qT
    
    const vec3f qT = mu_Xk - rotate(qR, mu_P);
    
    return quat7(qR, qT);
  }
  
}

#endif // ndef QUAT7_HH
