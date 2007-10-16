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

#ifndef MLN_METAL_VEC_HH
# define MLN_METAL_VEC_HH

/*!
 * \file  mln/metal/vec.hh
 *
 * \brief Definition of a generic vector class.
 */

# include <iostream>
# include <cmath>

# include <mln/core/concept/object.hh>
# include <mln/trait/all.hh>
# include <mln/value/props.hh>
# include <mln/fun/i2v/all.hh>
# include <mln/value/concept/all.hh>


// FIXME: Document.


namespace mln
{

  // Fwd decls.
  namespace literal { struct zero_t; }
  template <unsigned d, typename C> struct h_vec;



  namespace metal
  {

    // Fwd decl.
    template <unsigned n, typename T> class vec;


    namespace internal
    {

      template <unsigned n, typename T>
      class vec_base_ : public Object< vec<n,T> >
      {
      protected:
	T data_[n];
      };

      template <typename T>
      class vec_base_ <1, T> : public Object< vec<1,T> >
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
      class vec_base_ <2, T> : public Object< vec<2,T> >
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
      class vec_base_ <3, T> : public Object< vec<3,T> >
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

      template <typename T>
      class vec_base_ <4, T> : public Object< vec<4,T> >
      {
      public:
	void set(const T& val0, const T& val1, const T& val2, const T& val3)
	{
	  data_[0] = val0;
	  data_[1] = val1;
	  data_[2] = val2;
	  data_[3] = val3;
	}
      protected:
	T data_[4];
      };


    } // end of namespace mln::metal::internal



    template <unsigned n, typename T>
    class vec : public internal::vec_base_<n, T>
    {
      typedef internal::vec_base_<n, T> super_;

    protected:
      using super_::data_;

    public:

      typedef T equiv[n];
      typedef T enc[n];

      typedef T coord;
      enum { dim = n };

      vec();

      vec(const literal::zero_t&);

      vec(const vec<n, T>& rhs);

      template <typename U>
      vec(const vec<n, U>& rhs);

      template <typename U>
      vec& operator=(const vec<n, U>& rhs);


      // Immersion of the vector into its homogeneous space.
      h_vec<n, T> to_h_vec() const;


      const T& operator[](unsigned i) const;

      T& operator[](unsigned i);

      void set_all(const T& val);

      T sprod(const vec<n, T>& rhs) const;

      unsigned size() const;

      const vec<n, T>& normalize();

      /// Constructor; coordinates are set by function \p f.
      template <typename F>
      vec(const Function_i2v<F>& f);

      /// Zero value.
      static const vec<n, T> zero;

      /// Origin value.
      static const vec<n, T> origin;
    };

  } // end of namespace mln::metal


  namespace trait
  {

    // promote

    template <unsigned n, typename T, typename U>
    struct set_precise_binary_<promote, metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef metal::vec<n, mln_trait_promote(T, U)> ret;
    };


    // vec + vec

    template <unsigned n, typename T, typename U>
    struct set_precise_binary_<op::plus, metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef metal::vec<n, mln_trait_op_plus(T, U)> ret;
    };

    // FIXME: + vec !

    // vec - vec

    template <unsigned n, typename T, typename U>
    struct set_precise_binary_<op::minus, metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef metal::vec<n, mln_trait_op_minus(T, U)> ret;
    };

    // - vec

    template <unsigned n, typename T>
    struct set_precise_unary_<op::uminus, metal::vec<n, T> >
    {
      typedef metal::vec<n, mln_trait_op_uminus(T)> ret;
    };

    // vec * s

    template <unsigned n, typename T, typename S>
    struct set_precise_binary_<op::times, metal::vec<n, T>, S >
    {
      typedef metal::vec<n, mln_trait_op_times(T, S)> ret;
    };

    // FIXME: vec / s

  } // end of namespace mln::trait



  namespace value
  {

    template <unsigned n, typename T>
    struct props< metal::vec<n,T> >
    {
      typedef trait::value::kind::data kind;
      static const std::size_t card_ = n * mln_card_(T);
      typedef metal::vec<n, mln_value_sum(T)> sum;
    };

  } // end of namespace mln::value



  namespace metal
  {

    // eq

    template <unsigned n, typename T, typename U>
    bool operator==(const vec<n,T>& lhs, const vec<n,U>& rhs);

    template <unsigned n, typename T, typename U>
    bool operator!=(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // +

    template <unsigned n, typename T, typename U>
    vec<n,T>&
    operator+=(vec<n,T>& lhs, const vec<n,U>& rhs);

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_plus(T,U)>
    operator+(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // -

    template <unsigned n, typename T, typename U>
    vec<n,T>&
    operator-=(vec<n,T>& lhs, const vec<n,U>& rhs);

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_minus(T,U)>
    operator-(const vec<n,T>& lhs, const vec<n,U>& rhs);

    template <unsigned n, typename T>
    vec<n, mln_trait_op_uminus(T)>
    operator-(const vec<n,T>& lhs);

    // *

    template <unsigned n, typename T, typename S>
    vec<n,T>&
    operator*=(vec<n,T>& lhs, const S& s);

    template <unsigned n, typename T, typename S>
    vec<n, mln_trait_op_times(T,S)>
    operator*(const vec<n,T>& lhs, const S& s);

    // /

    template <unsigned n, typename T, typename S>
    vec<n,T>&
    operator/=(vec<n,T>& lhs, const S& s);

    template <unsigned n, typename T, typename S>
    vec<n, mln_trait_op_times(T,S)> // FIXME: Use div instead!
    operator/(const vec<n,T>& lhs, const S& s);

    // <<

    template <unsigned n, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,T>& v);

    template <unsigned n>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,unsigned char>& v);

    template <unsigned n>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,signed char>& v);

    // vprod // FIXME: Generalize...

    template <typename T, typename U>
    vec<3, mln_trait_op_times(T,U)> // FIXME: Sum of product...
    vprod(const vec<3, T>& lhs, const vec<3, U>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename T>
    vec<n,T>::vec()
    {
    }

    template <unsigned n, typename T>
    vec<n,T>::vec(const literal::zero_t&)
    {
      this->set_all(0);
    }

    template <unsigned n, typename T>
    vec<n,T>::vec(const vec<n,T>& rhs)
      : super_()
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
    }

    template <unsigned n, typename T>
    template <typename U>
    vec<n,T>::vec(const vec<n, U>& rhs)
      : super_()
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
    }

    template <unsigned n, typename T>
    template <typename U>
    vec<n,T>& vec<n,T>::operator=(const vec<n, U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
      return *this;
    }

    template <unsigned n, typename T>
    const T& vec<n,T>::operator[](unsigned i) const
    {
      mln_precondition(i < dim);
      return data_[i];
    }

    template <unsigned n, typename T>
    T& vec<n,T>::operator[](unsigned i)
    {
      mln_precondition(i < dim);
      return data_[i];
    }

    template <unsigned n, typename T>
    void vec<n,T>::set_all(const T& val)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = val;
    }

    template <unsigned n, typename T>
    T vec<n,T>::sprod(const vec<n, T>& rhs) const
    {
      T tmp = 0;

      for (unsigned i = 0; i < n; ++i)
	tmp += data_[i] * rhs.data_[i];
      return tmp;
    }

    template <unsigned n, typename T>
    unsigned vec<n,T>::size() const
    {
      return n;
    }

    template <unsigned n, typename T>
    const vec<n, T>& vec<n, T>::normalize()
    {
      float n_l2 = 0;
      for (unsigned i = 0; i < n; ++i)
	n_l2 += data_[i] * data_[i];
      n_l2 = sqrt(n_l2);
      for (unsigned i = 0; i < n; ++i)
	data_[i] = T(data_[i] / n_l2);
      return *this;
    }


    template <unsigned n, typename T>
    template <typename F>
    vec<n, T>::vec(const Function_i2v<F>& f_)
    {
      mlc_converts_to(mln_result(F), T)::check();
      const F& f = exact(f_);
      for (unsigned i = 0; i < n; ++i)
	data_[i] = f(i);
    }

    template <unsigned n, typename T>
    const vec<n, T> vec<n, T>::zero = all(0);

    template <unsigned n, typename T>
    const vec<n, T> vec<n, T>::origin = all(0);


    // eq

    template <unsigned n, typename T, typename U>
    bool operator==(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	if (lhs[i] != rhs[i])
	  return false;
      return true;
    }

    template <unsigned n, typename T, typename U>
    bool operator!=(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      return not (lhs == rhs);
    }


    // +

    template <unsigned n, typename T, typename U>
    vec<n,T>&
    operator+=(vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	lhs[i] += rhs[i];
      return lhs;
    }

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_plus(T,U)>
    operator+(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      vec<n, mln_trait_op_plus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] + rhs[i];
      return tmp;
    }


    // -

    template <unsigned n, typename T, typename U>
    vec<n,T>&
    operator-=(vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	lhs[i] -= rhs[i];
      return lhs;
    }

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_minus(T,U)>
    operator-(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      vec<n, mln_trait_op_minus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] - rhs[i];
      return tmp;
    }

    template <unsigned n, typename T>
    vec<n, mln_trait_op_uminus(T)>
    operator-(const vec<n,T>& lhs)
    {
      vec<n, mln_trait_op_uminus(T)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = - lhs[i];
      return tmp;
    }


    // *

    template <unsigned n, typename T, typename S>
    vec<n,T>&
    operator*=(vec<n,T>& lhs, const S& s)
    {
      for (unsigned i = 0; i < n; ++i)
	lhs[i] *= s;
      return lhs;
    }

    template <unsigned n, typename T, typename S>
    vec<n, mln_trait_op_times(T,S)>
    operator*(const vec<n,T>& lhs, const S& s)
    {
      vec<n, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] * s;
      return tmp;
    }


    // /

    template <unsigned n, typename T, typename S>
    vec<n,T>&
    operator/=(vec<n,T>& lhs, const S& s)
    {
      mln_precondition(s != 0);
      for (unsigned i = 0; i < n; ++i)
	lhs[i] /= s;
      return lhs;
    }

    template <unsigned n, typename T, typename S>
    vec<n, mln_trait_op_times(T,S)> // FIXME: Use div.
    operator/(const vec<n,T>& lhs, const S& s)
    {
      mln_precondition(s != 0);
      vec<n, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] / s;
      return tmp;
    }


    // <<

    template <unsigned n, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,T>& v)
    {
      ostr << '(';
      for (unsigned i = 0; i < n; ++i)
	ostr << v[i] << (i == n - 1 ? ")" : ", ");
      return ostr;
    }

    template <unsigned n>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,unsigned char>& v)
    {
      ostr << '(';
      for (unsigned i = 0; i < n; ++i)
	ostr << (unsigned int)(v[i]) << (i == n - 1 ? ")" : ", ");
      return ostr;
    }

    template <unsigned n>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,signed char>& v)
    {
      ostr << '(';
      for (unsigned i = 0; i < n; ++i)
	ostr << (signed int)(v[i]) << (i == n - 1 ? ")" : ", ");
      return ostr;
    }

    // vprod

    template <typename T, typename U>
    vec<3, mln_trait_op_times(T,U)> // typename binary_arith_trait<T, U>::ret>
    vprod(const vec<3, T>& lhs, const vec<3, U>& rhs)
    {
      vec<3, T> tmp; // FIXME typename binary_arith_trait<T, U>::ret> tmp;
      tmp[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
      tmp[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
      tmp[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
      return tmp;
    }



# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::metal

} // end of namespace mln

# include <mln/make/vec.hh>

#endif // ! MLN_METAL_VEC_HH
