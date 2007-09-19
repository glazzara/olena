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

# include <mln/core/contract.hh>
# include <mln/metal/binary_arith_trait.hh>

// FIXME: Document.

namespace mln
{

  namespace metal
  {

    template <unsigned n, unsigned m, typename T>
    class mat
    {
      public:

	typedef T value_type;
	enum {N = n, M = m};

	mat()
	{
	}

	template <typename U>
	mat(const mat<n,m,U>& rhs);

	template <typename U>
	mat& operator=(const mat<n,m,U>& rhs);

	const T& operator()(unsigned i, unsigned j) const;

	T& operator()(unsigned i, unsigned j);

	void set_all(const T& val);

	unsigned size() const;

      private:
	T data_[n][m];
    };

# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, unsigned m, typename T>
    template <typename U>
    mat<n,m,T>::mat(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = rhs(i, j);
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

    template <unsigned n, unsigned m, typename T, typename U>
    bool
    operator!=(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      return not (lhs == rhs);
    }

    // +

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator+=(mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) += rhs(i, j);
      return lhs;
    }
    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,typename binary_arith_trait<T,U>::ret>
    operator+(mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n,m,typename binary_arith_trait<T,U>::ret> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) + rhs(i, j);
      return tmp;
    }

    // -

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator-=(mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) -= rhs(i, j);
      return lhs;
    }
    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,typename binary_arith_trait<T,U>::ret>
    operator-(mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n,m,typename binary_arith_trait<T,U>::ret> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) - rhs(i, j);
      return tmp;
    }

    template <unsigned n, unsigned m, typename T>
    mat<n,m,T>
    operator-(const mat<n,m,T>& lhs)
    {
      mat<n,m,T> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; i < m; ++i)
	  tmp(i, j) = - lhs(i, j);
      return tmp;
    }

    // *

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>&
    operator*=(mat<n,m,T>& lhs, const U& scalar)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) *= scalar;
      return lhs;
    }
    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,typename binary_arith_trait<T,U>::ret>
    operator*(mat<n,m,T>& lhs, const U& scalar)
    {
      mat<n,m,typename binary_arith_trait<T,U>::ret> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) * scalar;
      return tmp;
    }
    
    template <unsigned n, unsigned m, unsigned o, typename T, typename U>
    mat<n,m,T>&
    operator*=(mat<n,o,T>& lhs, mat<o,m,U>& rhs)
    {
      lhs = lhs * rhs;
      return lhs;
    }
    template <unsigned n, unsigned m, unsigned o, typename T, typename U>
    mat<n,m,typename binary_arith_trait<T,U>::ret>
    operator*(mat<n,o,T>& lhs, mat<o,m,U>& rhs)
    {
      mat<n,m,typename binary_arith_trait<T,U>::ret> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	{
	  tmp(i, j) = 0;
	  for (unsigned k = 0; k < o; ++k)
	    tmp(i, j) += lhs(i, k) * rhs(k, j);
	}
      return tmp;
    }

    // /

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,T>
    operator/=(mat<n,m,T>& lhs, const U& scalar)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  lhs(i, j) /= scalar;
      return lhs;
    }
    template <unsigned n, unsigned m, typename T, typename U>
    mat<n,m,typename binary_arith_trait<T,U>::ret>
    operator/(mat<n,m,T>& lhs, const U& scalar)
    {
      mat<n,m,typename binary_arith_trait<T,U>::ret> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp[i][j] = lhs(i, j) / scalar;
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
