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


#ifndef EXTENDED_VEC_HH
# define EXTENDED_VEC_HH

# include <cassert>
# include <xtd/optraits.hh>


// FIXME: Document.


namespace xtd
{

  namespace internal
  {

    template <unsigned n, typename T>
    class vec_base_
    {
    protected:
      T data_[n];
    };

    template <typename T>
    class vec_base_ <1, T>
    {
    public:
      void set(const T& val0)
      {
	data_[0] = val0;
      } 
    protected:
      T data_[1];
    };

    template <typename T>
    class vec_base_ <2, T>
    {
    public:
      void set(const T& val0, const T& val1)
      {
	data_[0] = val0;
	data_[1] = val1;
      } 
    protected:
      T data_[2];
    };

    template <typename T>
    class vec_base_ <3, T>
    {
    public:
      void set(const T& val0, const T& val1, const T& val2)
      {
	data_[0] = val0;
	data_[1] = val1;
	data_[2] = val2;
      } 
    protected:
      T data_[3];
    };


  } // end of namespace xtd::internal



  template <unsigned n, typename T>
  class vec : public internal::vec_base_<n, T>
  {
    typedef internal::vec_base_<n,T> super;
    using super::data_;

  public:

    typedef T value_type;
    enum { dim = n };

    vec()
    {
    }

    template <typename U>
    vec(const vec<n, U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
    }

    template <typename U>
    vec& operator=(const vec<n, U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
      return *this;
    }

    const T operator[](unsigned i) const
    {
      assert(i < n);
      return data_[i];
    }

    T& operator[](unsigned i)
    {
      assert(i < n);
      return data_[i];
    }

    void set_all(const T& val)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = val;
    }

    unsigned size() const
    {
      return n;
    }

    // eq

    bool operator==(const vec& rhs) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (data_[i] != rhs[i])
	  return false;
      return true;
    }

    bool operator!=(const vec& rhs) const
    {
      return not (*this == rhs);
    }

    // +

    template <typename U>
    vec& operator+=(const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] += rhs[i];
      return *this;
    }

    template <typename U>
    vec<n, xtd_op_plus_trait(T, U)>
    operator+(const vec<n,U>& rhs) const
    {
      vec<n, xtd_op_plus_trait(T, U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = data_[i] + rhs[i];
      return tmp;
    }

    // -

    template <typename U>
    vec& operator-=(const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] -= rhs[i];
      return *this;
    }

    template <typename U>
    vec<n, xtd_op_minus_trait(T, U)>
    operator-(const vec<n,U>& rhs) const
    {
      vec<n, xtd_op_plus_trait(T, U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = data_[i] - rhs[i];
      return tmp;
    }

    vec<n, xtd_op_uminus_trait(T)>
    operator-()
    {
      vec<n, xtd_op_uminus_trait(T)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = - data_[i];
      return tmp;
    }

    // *

    template <typename U> void operator*=(const vec<n,U>& rhs);
    template <typename U> void operator*(const vec<n,U>& rhs) const;

    template <typename S>
    vec& operator*=(const S& scalar)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] *= scalar;
      return *this;
    }

    template <typename S>
    vec<n, xtd_op_mult_trait(T, S)>
    operator*(const S& scalar) const
    {
      vec<n, xtd_op_mult_trait(T, S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = data_[i] * scalar;
      return tmp;
    }

    // /
    
    template <typename U> void operator/=(const vec<n,U>& rhs);
    template <typename U> void operator/(const vec<n,U>& rhs) const;

    template <typename S>
    vec& operator/=(const S& scalar)
    {
      assert(scalar != 0);
      for (unsigned i = 0; i < n; ++i)
	data_[i] /= scalar;
      return *this;
    }

    template <typename S>
    vec<n, xtd_op_div_trait(T, S)>
    operator/(const S& scalar) const
    {
      assert(scalar != 0);
      vec<n, xtd_op_div_trait(T, S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = data_[i] / scalar;
      return tmp;
    }

  };



  template <typename uop,
	    unsigned n, typename T>
  struct set_trait_< uop, vec<n,T> >
  {
    typedef vec<n, xtd_unop_trait(uop, T)> ret;
  };


  template <typename bop,
	    unsigned n, typename L, typename R>
  struct set_trait_< bop, vec<n,L>, vec<n,R> >
  {
    typedef vec<n, xtd_binop_trait(bop, L, R)> ret;
  };


} // end of namespace xtd


#endif // ! EXTENDED_VEC_HH
