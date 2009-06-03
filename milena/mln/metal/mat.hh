// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_METAL_MAT_HH
# define MLN_METAL_MAT_HH

/*!
 * \file  mln/metal/mat.hh
 *
 * \brief Definition of a generic matrix class.
 */

# include <iostream>

# include <mln/core/concept/object.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/contract.hh>
# include <mln/trait/all.hh>
# include <mln/trait/value_.hh>
# include <mln/metal/vec.hh>


// FIXME: Document.



namespace mln
{


  // Fwd decl.
  namespace metal {
    template <unsigned n, unsigned m, typename T> class mat;
  }


  namespace trait
  {

    template <unsigned n, unsigned m, typename T>
    struct value_< metal::mat<n,m,T> >
    {
      typedef trait::value::nature::matrix nature;
      typedef trait::value::kind::data     kind;

      enum {
	nbits = n * m * mln_nbits(T),
	card  = n * m * mln_card(T)
      };
      typedef mln_value_quant_from_(card)  quant;

      typedef metal::mat<n, m, mln_sum(T)> sum;
    };

  } // end of namespace mln::trait



  namespace metal
  {

    template <unsigned n, unsigned m, typename T>
    class mat : public Object< mat<n,m,T> >
    {
    public:

      typedef T coord;
      enum { N = n,
	     M = m,
             dim = n * m };
      
      static const mat<n,m,T> Id;

      mat()
      {
      }

      template <typename U>
      mat(const mat<n,m,U>& rhs);

      /// Constructor; coordinates are set by function \p f.
      template <typename F>
      mat(const Function_v2v<F>& f);

      template <typename U>
      mat& operator=(const mat<n,m,U>& rhs);

      const T& operator()(unsigned i, unsigned j) const;

      T& operator()(unsigned i, unsigned j);

      void set_all(const T& val);

      unsigned size() const;

      static mat identity();

    private:
      T data_[n][m];
    };

  }


  namespace trait
  {

    // Unarys.

    template < template<class> class Name,
	       unsigned n, unsigned m, typename T >
    struct set_precise_unary_< Name, metal::mat<n,m,T> >
    {
      typedef metal::mat<n, m, mln_trait_unary(Name, T)> ret;
    };

    // Default for binarys; works for (+), (-), comparisons, and promote.

    template < template<class, class> class Name,
	       unsigned n, unsigned m, typename T, typename U>
    struct set_precise_binary_< Name, metal::mat<n,m,T>, metal::mat<n,m,U> >
    {
      typedef metal::mat<n, m, mln_trait_binary(Name, T, U)> ret;
    };

    // mat * mat

    template < unsigned n, unsigned o, typename T,
	       unsigned m, typename U >
    struct set_precise_binary_< op::times, metal::mat<n,o,T>, metal::mat<o,m,U> >
    {
      typedef metal::mat<n, m, mln_sum_product(T, U)> ret;
    };

    template < unsigned n, typename T, typename U >
    struct set_precise_binary_< op::times, metal::mat<n,n,T>, metal::mat<n,n,U> >
    { // Disambiguate between both previous defs.
      typedef metal::mat<n, n, mln_sum_product(T, U)> ret;
    };

    // mat * vec

    template < unsigned n, unsigned m, typename T,
	       typename U >
    struct set_precise_binary_< op::times, metal::mat<n,m,T>, metal::vec<m,U> >
    {
      typedef metal::vec<n, mln_sum_product(T, U)> ret;
    };

    // mat * s

    template < template<class, class> class Name,
	       unsigned n, unsigned m, typename T,
	       typename S >
    struct set_precise_binary_< Name, metal::mat<n,m,T>, mln::value::scalar_<S> >
    {
      typedef metal::mat<n, m, mln_trait_binary(Name, T, S)> ret;
    };

    template < template<class, class> class Name,
	       unsigned n, unsigned m, typename T,
	       typename S >
    struct set_binary_< Name,
			mln::Object, metal::mat<n,m,T>,
			mln::value::Scalar, S >
    {
      typedef metal::mat<n, m, mln_trait_binary(Name, T, S)> ret;
    };

  } // end of namespace mln::trait



  namespace metal
  {

    // ==

    template <unsigned n, unsigned m, typename T, typename U>
    bool
    operator==(mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // +

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_plus(T,U)>
    operator+(mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // -

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_minus(T,U)>
    operator-(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // - (unary)

    template <unsigned n, unsigned m, typename T>
    mat<n, m, mln_trait_op_uminus(T)>
    operator-(const mat<n,m,T>& lhs);

    // mat * mat

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    mat<n, m, mln_sum_product(T,U)>
    operator*(const mat<n,o,T>& lhs, const mat<o,m,U>& rhs);

    // mat * vec

    template <unsigned n, unsigned m, typename T,
	      typename U>
    vec<n, mln_sum_product(T,U)>
    operator*(const mat<n,m,T>& lhs, const vec<m,U>& rhs);

    // mat * s

    template <unsigned n, unsigned m, typename T,
	      typename S>
    mat<n, m, mln_trait_op_times(T,S)>
    operator*(const mat<n,m,T>& lhs, const value::scalar_<S>& s);

    // mat / s

    template <unsigned n, unsigned m, typename T, typename S>
    mat<n, m, mln_trait_op_div(T,S)>
    operator/(const mat<n,m,T>& lhs, const value::scalar_<S>& s);

    // <<
    
    template <unsigned n, unsigned m, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,T>& v);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, unsigned m, typename T>
    const mat<n,m,T> mat<n,m,T>::Id = mat<n,m,T>::identity();

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T> mat<n,m,T>::identity()
    {
      static mat<n,m,T> id_;
      static bool flower = true;
      if (flower)
	{
	  for (unsigned i = 0; i < n; ++i)
	    for (unsigned j = 0; j < m; ++j)
	      id_(i, j) = (i == j);
	  flower = false;
	}
      return id_;
    }

    template <unsigned n, unsigned m, typename T>
    template <typename U>
    inline
    mat<n,m,T>::mat(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = rhs(i, j);
    }
    
    template <unsigned n, unsigned m, typename T>
    template <typename F>
    inline
    mat<n,m,T>::mat(const Function_v2v<F>& f_)
    {
      mlc_converts_to(mln_result(F), T)::check();
      const F& f = exact(f_);
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = f(i * n + j);
    }

    template <unsigned n, unsigned m, typename T>
    template <typename U>
    inline
    mat<n,m,T>&
    mat<n,m,T>::operator=(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = rhs(i, j);
      return *this;
    }
    
    template <unsigned n, unsigned m, typename T>
    inline
    const T&
    mat<n,m,T>::operator()(unsigned i, unsigned j) const
    {
      mln_precondition(i < n && j < m);
      return data_[i][j];
    }
    
    template <unsigned n, unsigned m, typename T>
    inline
    T&
    mat<n,m,T>::operator()(unsigned i, unsigned j)
    {
      mln_precondition(i < n && j < m);
      return data_[i][j];
    }
    
    template <unsigned n, unsigned m, typename T>
    inline
    void mat<n,m,T>::set_all(const T& val)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = val;
    }
    
    template <unsigned n, unsigned m, typename T>
    inline
    unsigned mat<n,m,T>::size() const
    {
      return n * m;
    }
    

    // Operators.


    template <unsigned n, unsigned m, typename T, typename U>
    inline
    bool
    operator==(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  if (lhs(i, j) != rhs(i, j))
	    return false;
      return true;
    }

    template <unsigned n, unsigned m, typename T, typename U>
    inline
    mat<n, m, mln_trait_op_plus(T,U)>
    operator+(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n, m, mln_trait_op_plus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) + rhs(i, j);
      return tmp;
    }

    template <unsigned n, unsigned m, typename T, typename U>
    inline
    mat<n,m, mln_trait_op_minus(T,U)>
    operator-(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n,m, mln_trait_op_minus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) - rhs(i, j);
      return tmp;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m, mln_trait_op_uminus(T)>
    operator-(const mat<n,m,T>& rhs)
    {
      mat<n,m, mln_trait_op_uminus(T)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; i < m; ++i)
	  tmp(i, j) = - rhs(i, j);
      return tmp;
    }

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    inline
    mat<n, m, mln_sum_product(T,U)>
    operator*(const mat<n,o,T>& lhs, const mat<o,m,U>& rhs)
    {
      mat<n,m, mln_sum_product(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  {
	    tmp(i, j) = literal::zero;
	    for (unsigned k = 0; k < o; ++k)
	      tmp(i, j) += lhs(i, k) * rhs(k, j);
	  }
      return tmp;
    }

    template <unsigned n, unsigned m, typename T,
	      typename U>
    inline
    vec<n, mln_sum_product(T,U)>
    operator*(const mat<n,m,T>& lhs, const vec<m,U>& rhs)
    {
      vec<n, mln_sum_product(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	{
	  mln_sum_product(T,U) sum(literal::zero);
	  for (unsigned j = 0; j < m; ++j)
	    sum += lhs(i, j) * rhs[j];
	  tmp[i] = sum;
	}
      return tmp;
    }

    template <unsigned n, unsigned m, typename T, typename S>
    inline
    mat<n, m, mln_trait_op_times(T,S)>
    operator*(const mat<n,m,T>& lhs, const value::scalar_<S>& s_)
    {
      S s = s_.to_equiv();
      mat<n, m, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) * s;
      return tmp;
    }

    template <unsigned n, unsigned m, typename T, typename S>
    inline
    mat<n,m, mln_trait_op_div(T,S)>
    operator/(const mat<n,m,T>& lhs, const value::scalar_<S>& s_)
    {
      S s = s_.to_equiv();
      mat<n,m, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) / s;
      return tmp;
    }

    // <<
    
    template <unsigned n, unsigned m, typename T>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,T>& v)
    {
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << '[';
	  for (unsigned j = 0; j < m; ++j)
	    ostr << debug::format(v(i, j)) << (j == m - 1 ? "]" : ", ");
	  ostr << std::endl;
	}
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY
    
  } // end of namespace mln::metal

} // end of namespace mln

# include <mln/make/mat.hh>

#endif // ! MLN_METAL_MAT_HH
