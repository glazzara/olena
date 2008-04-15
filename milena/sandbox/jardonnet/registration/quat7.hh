
#ifndef QUAT7_HH
# define QUAT7_HH

# include <assert.h>
# include <algorithm>

# include <mln/algebra/mat.hh>
# include <mln/core/p_array.hh>

# include "rotation.hh"
# include "jacobi.hh"
# include "power_it.hh"
# include "frankel_young.hh"

# include <mln/norm/all.hh>

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
      : _qR(1,0,0,0), _qT(literal::zero)
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

    float norm() const
    {
      return norm::l1(_qR.to_vec()) + norm::l1(_qT);
    }
    
    
    // quat7 is an object-function
    
    algebra::vec<n,float> operator()(const algebra::vec<n,float>& v) const
    {
      return rotate(_qR, v) + _qT;
    }

    template <typename P>
    void apply_on(const p_array<P>& input,
                  p_array<P>& output,
                  const size_t c_length) const
    {
      assert(c_length <= input.npoints());
      assert(c_length <= output.npoints());
      assert(_qR.is_unit());

      for (size_t i = 0; i < c_length; i++)
        output.hook_()[i] = algebra::to_point<P>((*this)(input[i]));
    }
    
    friend std::ostream& operator << (std::ostream& os, quat7& q)
    {
      std::cout << q._qR << std::endl;
      std::cout << q._qT << std::endl;
      return os;
    }

    quat7 operator*(float f)
    {
      return quat7(_qR * f, _qT * f);
    }
    
    quat7 operator/(float f)
    {
      return quat7(_qR / f, _qT / f);
    }

    quat7 operator-(const quat7& rhs) const
    {
      return quat7(_qR - rhs._qR, _qT - rhs._qT);
    }

    quat7 operator+(const quat7& rhs) const
    {
      return quat7(_qR + rhs._qR, _qT + rhs._qT);
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
  

  template <typename P, typename M>
  quat7<P::dim> match(const p_array<P>& C,
                      const algebra::vec<P::dim,float>& mu_C,
                      const p_array<P>& Ck,
                      M& map,
                      size_t c_length)
  {
    //FIXME: mix mu_Xk and qk loop
    
    
    //mu_Xk = center map(Ck)
    algebra::vec<P::dim,float> mu_Xk(literal::zero);
    for (size_t i = 0; i < c_length; ++i)
      {
        algebra::vec<P::dim,float> xki = map(Ck[i]);
        mu_Xk += xki;
      }
    mu_Xk /= c_length;
    
    // qR
    algebra::mat<P::dim,P::dim,float> Mk(literal::zero);
    for (size_t i = 0; i < c_length; ++i)
      {
        algebra::vec<P::dim,float> Ci  = C[i];
        algebra::vec<P::dim,float> Xki = map(Ck[i]);
        Mk += make::mat(Ci - mu_C) * trans(make::mat(Xki - mu_Xk));
      }
    Mk /= c_length;


    /*  
    const algebra::mat<P::dim,P::dim,float> Ak = Mk - trans(Mk);

    const float v[3] = {Ak(1,2), Ak(2,0), Ak(0,1)};
    const algebra::mat<3,1,float> D = make::mat<3,1,3,float>(v);

    
    algebra::mat<4,4,float> Qk(literal::zero);
    Qk(0,0) = tr(Mk);
    put(trans(D), 0,1, Qk);
    put(D, 1,0, Qk);
    
    put(Mk + trans(Mk) - algebra::mat<P::dim,P::dim,float>::identity() * tr(Mk), 1,1, Qk);
    */
    
    algebra::vec<3,float> a;
    a[0] = Mk(1,2) - Mk(2,1);
    a[1] = Mk(2,0) - Mk(0,2);
    a[2] = Mk(0,1) - Mk(1,0);

    algebra::mat<4,4,float> Qk(literal::zero);
    float t = tr(Mk);
    
    Qk(0,0) = t;
    for (int i = 1; i < 4; i++)
      {
        Qk(i,0) = a[i - 1];
        Qk(0,i) = a[i - 1];
        for (int j = 1; j < 4; j++)
          if (i == j)
            Qk(i,j) = 2 * Mk(i - 1,i - 1) - t;
      }
      
    Qk(1,2) = Mk(0,1) + Mk(1,0);
    Qk(2,1) = Mk(0,1) + Mk(1,0);
    
    Qk(1,3) = Mk(0,2) + Mk(2,0);
    Qk(3,1) = Mk(0,2) + Mk(2,0);
    
    Qk(2,3) = Mk(1,2) + Mk(2,1);
    Qk(3,2) = Mk(1,2) + Mk(2,1);
    
    algebra::quat qR(literal::zero);
    qR = jacobi(Qk);
    //std::cout << qR << std::endl;
    //qR = power_it(Qk);
    //std::cout << qR << std::endl;
    
    // qT
    const algebra::vec<P::dim,float> qT = mu_Xk - rotate(qR, mu_C);
    
    return quat7<P::dim>(qR, qT);
  }
  
} //end of namespace mln

#endif // ndef QUAT7_HH
