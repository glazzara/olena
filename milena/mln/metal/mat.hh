// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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

# include <iostream>

# include <mln/core/concept/object.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/contract.hh>
# include <mln/trait/all.hh>
# include <mln/value/props.hh>
# include <mln/value/concept/all.hh>
# include <mln/metal/vec.hh>


// FIXME: Document.



namespace mln
{

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
      mat(const Function_i2v<F>& f);

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

    // promote

    template <unsigned n, unsigned m, typename T, typename U>
    struct set_precise_binary_<promote, metal::mat<n,m, T>, metal::mat<n,m, U> >
    {
      typedef metal::mat<n,m, mln_trait_promote(T, U)> ret;
    };


    // mat + mat

    template <unsigned n, unsigned m, typename T,
	      typename U>
    struct set_precise_binary_<op::plus, metal::mat<n, m, T>, metal::mat<n, m, U> >
    {
      typedef metal::mat<n, m, mln_trait_op_plus(T, U)> ret;
    };

    // FIXME: + mat

    // mat - mat

    template <unsigned n, unsigned m, typename T,
	      typename U>
    struct set_precise_binary_<op::minus, metal::mat<n, m, T>, metal::mat<n, m, U> >
    {
      typedef metal::mat<n, m, mln_trait_op_minus(T, U)> ret;
    };

    // - mat

    template <unsigned n, unsigned m, typename T>
    struct set_precise_unary_<op::uminus, metal::mat<n, m, T> >
    {
      typedef metal::mat<n, m, mln_trait_op_uminus(T)> ret;
    };

    // mat * mat

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    struct set_precise_binary_<op::times, metal::mat<n,o,T>, metal::mat<o,m,U> >
    {
      typedef metal::mat< n, m, mln_trait_op_times(T,U) > ret;
    };

    // mat * vec

    template <unsigned n, unsigned m, typename T,
	      typename U>
    struct set_precise_binary_<op::times, metal::mat<n,m,T>, metal::vec<m,U> >
    {
      typedef mln_trait_op_times(T,U) TxU;
      typedef metal::vec< m, mln_sum(TxU) > ret;
    };

    // mat * s

    template <unsigned n, unsigned m, typename T,
	      typename S>
    struct set_precise_binary_<op::times, metal::mat<n,m,T>, S >
    {
      typedef metal::mat< n, m, mln_trait_op_times(T,S) > ret;
    };

  } // end of namespace mln::trait



  namespace value
  {

    template <unsigned n, unsigned m, typename T>
    struct props< metal::mat<n,m,T> >
    {
      typedef trait::value::kind::data kind;
      static const std::size_t card_ = n * m * mln_card_(T);
      typedef metal::mat<n,m, mln_value_sum(T)> sum;
    };

  } // end of namespace mln::value



  namespace metal
  {

    // eq

    template <unsigned n, unsigned m, typename T, typename U>
    bool
    operator==(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // +=

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator+=(mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // + (binary)

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_plus(T,U)>
    operator+(mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // -=

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator-=(mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // - (binary)

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_minus(T,U)>
    operator-(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // - (unary)

    template <unsigned n, unsigned m, typename T>
    mat<n, m, mln_trait_op_uminus(T)>
    operator-(const mat<n,m,T>& lhs);

    // Operator *.

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    mat<n, m, mln_trait_op_times(T,U)>
    operator*(const mat<n,o,T>& lhs, const mat<o,m,U>& rhs); // mat * mat


//     template <unsigned n, unsigned m, typename T,
// 	      typename S>
//     mat<n, m, mln_trait_op_times(T,S)>
//     operator*(const mat<n,m,T>& lhs, const S& s); // mat * s


    // FIXME: Simplification below of the general code above:

    template <unsigned n, unsigned m, typename T>
    mat<n, m, T>
    operator*(const mat<n,m,T>& lhs, const T& s); // mat * s


    template <unsigned n, unsigned m, typename T,
	      typename U>
    typename mln::trait::op::times< mat<n,m,T>, vec<m,U> >::ret
    operator*(const mat<n,m,T>& lhs, const vec<m,U>& rhs) // mat * vec
      // FIXME: Move below...
    {
      typedef mat<n,m,T> mat_t;
      typedef vec<m,U>   vec_t;
      mln_trait_op_times(mat_t, vec_t) tmp;
      for (unsigned i = 0; i < n; ++i)
	{
	  mln_trait_op_times(T,U) sum = 0; // FIXME: Use literal::zero.
	  for (unsigned j = 0; j < m; ++j)
	    sum += lhs(i, j) * rhs[j];
	  tmp[i] = sum;
	}
      return tmp;
    }

    // *=

    template <unsigned n, typename T, typename U>
    mat<n,n,T>&
    operator*=(mat<n,n,T>& lhs, const mat<n,n,U>& rhs);

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator*=(mat<n,m,T>& lhs, const U& rhs);
    
    // Operator /.

    template <unsigned n, unsigned m, typename T, typename S>
    mat<n, m, mln_trait_op_times(T,S)> // FIXME: times instead of div...
    operator/(const mat<n,m,T>& lhs, const S& s);

    // /=

    template <unsigned n, unsigned m, typename T, typename S>
    mat<n,m,T>&
    operator/=(mat<n,m,T>& lhs, const S& s);

    // <<
    
    template <unsigned n, unsigned m, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,T>& v);
    
    template <unsigned n, unsigned m>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,unsigned char>& v);
    
    template <unsigned n, unsigned m>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,signed char>& v);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, unsigned m, typename T>
    const mat<n,m,T> mat<n,m,T>::Id = mat<n,m,T>::identity();

    template <unsigned n, unsigned m, typename T>
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
    mat<n,m,T>::mat(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = rhs(i, j);
    }
    
    template <unsigned n, unsigned m, typename T>
    template <typename F>
    mat<n,m,T>::mat(const Function_i2v<F>& f_)
    {
      mlc_converts_to(mln_result(F), T)::check();
      const F& f = exact(f_);
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = f(i * n + j);
    }

    template <unsigned n, unsigned m, typename T>
    template <typename U>
    mat<n,m,T>&
    mat<n,m,T>::operator=(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = rhs(i, j);
      return *this;
    }
    
    template <unsigned n, unsigned m, typename T>
    const T&
    mat<n,m,T>::operator()(unsigned i, unsigned j) const
    {
      mln_precondition(i < n && j < m);
      return data_[i][j];
    }
    
    template <unsigned n, unsigned m, typename T>
    T&
    mat<n,m,T>::operator()(unsigned i, unsigned j)
    {
      mln_precondition(i < n && j < m);
      return data_[i][j];
    }
    
    template <unsigned n, unsigned m, typename T>
    void mat<n,m,T>::set_all(const T& val)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = val;
    }
    
    template <unsigned n, unsigned m, typename T>
    unsigned mat<n,m,T>::size() const
    {
      return n * m;
    }
    

    // eq

    template <unsigned n, unsigned m, typename T, typename U>
    bool
    operator==(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  if (lhs(i, j) != rhs(i, j))
	    return false;
      return true;
    }

    // +=

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator+=(mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) += rhs(i, j);
      return lhs;
    }

    // Operator +.

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_plus(T,U)>
    operator+(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n, m, mln_trait_op_plus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) + rhs(i, j);
      return tmp;
    }

    // -=

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator-=(mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) -= rhs(i, j);
      return lhs;
    }

    // Operators -.

    template <unsigned n, unsigned m, typename T, typename U>
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
    mat<n,m, mln_trait_op_uminus(T)>
    operator-(const mat<n,m,T>& rhs)
    {
      mat<n,m, mln_trait_op_uminus(T)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; i < m; ++i)
	  tmp(i, j) = - rhs(i, j);
      return tmp;
    }

    // *=

    template <unsigned n, typename T, typename U>
    mat<n,n,T>&
    operator*=(mat<n,n,T>& lhs, const mat<n,n,U>& rhs)
    {
      // FIXME: Optimize!
      lhs = lhs * rhs;
      return lhs;
    }

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator*=(mat<n,m,T>& lhs, const U& s)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) *= s;
      return lhs;
    }

    // Operators *.

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    mat<n,m, mln_trait_op_times(T,U)>
    operator*(const mat<n,o,T>& lhs, const mat<o,m,U>& rhs)
    {
      mat<n,m, mln_trait_op_times(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  {
	    tmp(i, j) = 0;
	    for (unsigned k = 0; k < o; ++k)
	      tmp(i, j) += lhs(i, k) * rhs(k, j);
	  }
      return tmp;
    }

    template <unsigned n, unsigned m, typename T>
    mat<n, m, T>
    operator*(const mat<n,m,T>& lhs, const T& s) // mat * s

      // FIXME: Read above.

//     template <unsigned n, unsigned m, typename T,
// 	      typename S>
//     mat<n,m, mln_trait_op_times(T,S)>
//     operator*(const mat<n,m,T>& lhs, const S& s)
    {
      mat<n,m, T> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) * s;
      return tmp;
    }

    // /

    template <unsigned n, unsigned m, typename T, typename S>
    mat<n,m,T>&
    operator/=(mat<n,m,T>& lhs, const S& s)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) /= s;
      return lhs;
    }

    // Operator /.

    template <unsigned n, unsigned m, typename T, typename S>
    mat<n,m, mln_trait_op_times(T,S)> // FIXME: Use div, not times!
    operator/(const mat<n,m,T>& lhs, const S& s)
    {
      mat<n,m, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) / s;
      return tmp;
    }

    // <<
    
    template <unsigned n, unsigned m, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,T>& v)
    {
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << '(';
	  for (unsigned j = 0; j < m; ++j)
	    ostr << v(i, j) << (j == m - 1 ? ")" : ", ");
	  ostr << std::endl;
	}
      return ostr;
    }
    
    template <unsigned n, unsigned m>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,unsigned char>& v)
    {
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << '(';
	  for (unsigned j = 0; j < m; ++j)
	    ostr << (unsigned int)(v[i][j]) << (j == m - 1 ? ")" : ", ");
	  ostr << std::endl;
	}
      return ostr;
    }
    
    template <unsigned n, unsigned m>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,signed char>& v)
    {
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << '(';
	  for (unsigned j = 0; j < m; ++j)
	    ostr << (signed int)(v[i][j]) << (j == m - 1 ? ")" : ", ");
	  ostr << std::endl;
	}
      return ostr;
    }



# endif // ! MLN_INCLUDE_ONLY
    
  } // end of namespace mln::metal

} // end of namespace mln

# include <mln/make/mat.hh>

#endif // ! MLN_METAL_MAT_HH
