
#ifndef QUAT7_HH
# define QUAT7_HH

# include <assert.h>
# include <algorithm>

# include <mln/algebra/mat.hh>
# include <mln/core/p_array.hh>

# include "rotation.hh"
# include "jacobi.hh"

# include <mln/norm/l2.hh>

# include <mln/trait/all.hh>

# include <mln/make/vec.hh>

# include "tools.hh"

namespace mln
{

  template <unsigned n>
  struct quat7
  {
    algebra::quat _qR;
    algebra::vec<n,float> _qT;
    
    quat7()
    {
    }
    
    quat7(const algebra::quat& qR, const algebra::vec<n,float>& qT) :
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
    
    algebra::vec<n,float> operator()(const algebra::vec<n,float>& v) const
    {
      return rotate(_qR, v) + _qT;
    }

    template <typename P>
    void apply_on(const p_array<P>& input, p_array<P>& output) const
    {
      assert(input.npoints() == output.npoints());
      assert(_qR.is_unit());

      for (size_t i = 0; i < input.npoints(); i++)
        output.hook_()[i] = algebra::to_point<P>((*this)(input[i]));
    }
    
    friend std::ostream& operator << (std::ostream& os, quat7& q)
    {
      std::cout << q._qR << std::endl;
      std::cout << q._qT << std::endl;
      return os;
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


  template <typename P>
  quat7<P::dim> match(const p_array<P>& C,
                      const algebra::vec<P::dim,float>& mu_C,
                      const p_array<P>& Xk,
                      const algebra::vec<P::dim,float>& mu_Xk)
  {
    assert(C.npoints() == Xk.npoints());
    
    // qR

    algebra::mat<P::dim,P::dim,float> Mk;
    for (size_t i = 0; i < C.npoints(); ++i)
      {
        algebra::vec<P::dim,float> Ci = C[i];
        algebra::vec<P::dim,float> Xki = Xk[i];
        Mk += make::mat(Ci - mu_C) * trans(make::mat(Xki - mu_Xk));
      }
    Mk /= C.npoints();
    
    const algebra::mat<P::dim,P::dim,float> Ak = Mk - trans(Mk);

    const float v[3] = {Ak(1,2), Ak(2,0), Ak(0,1)};
    const algebra::mat<3,1,float> D = make::mat<3,1,3,float>(v);

    
    algebra::mat<4,4,float> Qk;
    Qk(0,0) = tr(Mk);
    put(trans(D), 0,1, Qk);
    put(D, 1,0, Qk);

    put(Mk + trans(Mk) - algebra::mat<P::dim,P::dim,float>::identity() * tr(Mk), 1,1, Qk);
    
    algebra::quat qR;
    jacobi(Qk, qR);
    
    // qT
    
    const algebra::vec<P::dim,float> qT = mu_Xk - rotate(qR, mu_C);
    
    return quat7<P::dim>(qR, qT);
  }
  
} //end of namespace mln

#endif // ndef QUAT7_HH
