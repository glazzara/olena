// Copyright (C) 2006, 2007, 2008, 2009, 2010 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_METAL_VEC_HH
# define MLN_METAL_VEC_HH

/// \file
///
/// Definition of a generic vector class.

# include <iostream>
# include <cmath>

# include <mln/core/concept/object.hh>
# include <mln/algebra/h_vec.hh>

# include <mln/trait/all.hh>
# include <mln/trait/value_.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/debug/format.hh>

# include <mln/value/ops.hh>


// FIXME: Document.


namespace mln
{

  // Fwd decls.
  namespace metal {
    template <unsigned n, typename T> class vec;
  }
  namespace literal {
    struct zero_t;
  }
  template <unsigned d, typename C> class h_vec;



  namespace trait
  {

    template <unsigned n, typename T>
    struct value_< mln::metal::vec<n,T> >
    {
      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::data        kind;

      enum {
	nbits = n * mln_nbits(T),
	card  = n * mln_card(T)
      };
      typedef mln_value_quant_from_(card)     quant;

      typedef metal::vec<n, mln_sum(T)> sum;
    };

  } // end of namespace mln::trait



  namespace metal
  {

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

      /// \{ Constructors/assignments with literal zero.
      vec(const literal::zero_t&);
      vec& operator=(const literal::zero_t&);
      /// \}

      vec(const vec<n, T>& rhs);

      template <typename U>
      vec(const vec<n, U>& rhs);

      template <typename U>
      vec& operator=(const vec<n, U>& rhs);


      // Immersion of the vector into its homogeneous space.
      algebra::h_vec<n, T> to_h_vec() const;


      const T& operator[](unsigned i) const;

      T& operator[](unsigned i);

      void set_all(const T& val);

      unsigned size() const;

      const vec<n, T>& normalize();

      /// Constructor; coordinates are set by function \p f.
      template <typename F>
      vec(const Function_v2v<F>& f);

      /// Zero value.
      static const vec<n, T> zero;

      /// Origin value.
      static const vec<n, T> origin;
    };

  } // end of namespace mln::metal


  namespace trait
  {

    // For unary traits.

    template < template <class> class Name,
	       unsigned n, typename T >
    struct set_precise_unary_< Name, metal::vec<n, T> >
    {
      typedef mln_trait_unary(Name, T) V;
      typedef metal::vec<n, V> ret;
    };

    // For binary traits.

    template < template <class, class> class Name,
	       unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< Name,
				metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef mln_trait_binary(Name, T, U) V;
      typedef metal::vec<n, V> ret;
    };

    template < unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef mln_sum_product(T,U) ret;
    };

    template < template <class, class> class Name,
	       unsigned n, typename T,
	       typename S >
    struct set_precise_binary_< Name,
				metal::vec<n, T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_binary(Name, T, S) V;
      typedef metal::vec<n, V> ret;
    };

    template < template<class, class> class Name,
	       unsigned n, typename T,
	       typename S >
    struct set_binary_< Name,
			mln::Object, metal::vec<n, T>,
			mln::value::Scalar, S >
    {
      typedef mln_trait_binary(Name, T, S) V;
      typedef metal::vec<n, V> ret;
    };

  } // end of namespace mln::trait



  namespace metal
  {

    // eq

    template <unsigned n, typename T, typename U>
    bool operator==(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // +

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_plus(T,U)>
    operator+(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // -

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_minus(T,U)>
    operator-(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // vec * vec

    template <unsigned n, typename T, typename U>
    mln_sum_product(T,U)
    operator*(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // vec * s

    template <unsigned n, typename T, typename S>
    vec<n, mln_trait_op_times(T, S)>
    operator*(const vec<n,T>& lhs, const mln::value::scalar_<S>& s);

    // vec / s

    template <unsigned n, typename T, typename S>
    vec<n, mln_trait_op_div(T, S)>
    operator/(const vec<n,T>& lhs, const mln::value::scalar_<S>& s);

    // <<

    template <unsigned n, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,T>& v);

    // vprod // FIXME: Generalize...

    template <typename T, typename U>
    vec<3, mln_trait_op_times(T,U)> // FIXME: Sum of product...
    vprod(const vec<3, T>& lhs, const vec<3, U>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename T>
    inline
    vec<n,T>::vec()
    {
    }

    template <unsigned n, typename T>
    inline
    vec<n,T>::vec(const literal::zero_t&)
    {
      this->set_all(0);
    }

    template <unsigned n, typename T>
    inline
    vec<n,T>&
    vec<n,T>::operator=(const literal::zero_t&)
    {
      this->set_all(0);
      return *this;
    }

    template <unsigned n, typename T>
    inline
    vec<n,T>::vec(const vec<n,T>& rhs)
      : super_()
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
    }

    template <unsigned n, typename T>
    template <typename U>
    inline
    vec<n,T>::vec(const vec<n, U>& rhs)
      : super_()
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
    }

    template <unsigned n, typename T>
    template <typename U>
    inline
    vec<n,T>& vec<n,T>::operator=(const vec<n, U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs[i];
      return *this;
    }

    template <unsigned n, typename T>
    inline
    const T& vec<n,T>::operator[](unsigned i) const
    {
      mln_precondition(i < dim);
      return data_[i];
    }

    template <unsigned n, typename T>
    inline
    T& vec<n,T>::operator[](unsigned i)
    {
      mln_precondition(i < dim);
      return data_[i];
    }

    template <unsigned n, typename T>
    inline
    void vec<n,T>::set_all(const T& val)
    {
      for (unsigned i = 0; i < n; ++i)
	data_[i] = val;
    }

    template <unsigned n, typename T>
    inline
    unsigned vec<n,T>::size() const
    {
      return n;
    }

    template <unsigned n, typename T>
    inline
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
    inline
    vec<n, T>::vec(const Function_v2v<F>& f_)
    {
      mlc_converts_to(mln_result(F), T)::check();
      const F& f = exact(f_);
      for (unsigned i = 0; i < n; ++i)
	data_[i] = f(i);
    }


    template <unsigned n, typename T>
    const vec<n, T> vec<n, T>::zero = all_to(0);

    template <unsigned n, typename T>
    const vec<n, T> vec<n, T>::origin = all_to(0);


    // Operators.


    template <unsigned n, typename T, typename U>
    inline
    bool operator==(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	if (lhs[i] != rhs[i])
	  return false;
      return true;
    }


    template <unsigned n, typename T, typename U>
    inline
    vec<n, mln_trait_op_plus(T,U)>
    operator+(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      vec<n, mln_trait_op_plus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] + rhs[i];
      return tmp;
    }

    template <unsigned n, typename T, typename U>
    inline
    vec<n, mln_trait_op_minus(T,U)>
    operator-(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      vec<n, mln_trait_op_minus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] - rhs[i];
      return tmp;
    }

    template <unsigned n, typename T, typename U>
    inline
    mln_sum_product(T,U)
    operator*(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      mln_sum_product(T,U) tmp(literal::zero);
      for (unsigned i = 0; i < n; ++i)
	tmp += lhs[i] * rhs[i];
      return tmp;
    }

    template <unsigned n, typename T, typename S>
    inline
    vec<n, mln_trait_op_times(T, S)>
    operator*(const vec<n,T>& lhs, const mln::value::scalar_<S>& s)
    {
      // FIXME: We made a choice here but is it correct?
      // FIXME: We "un-scalar" s so that the scalar status do not propagate.

      // Think of the case: vec<mat> v * scalar(vec w) s
      // It gives: for all i, v[i] * w  so  the i-th mat * vec w -> vec
      // The result is a vec<vec>

      // If we really want to propage the "scalar" status then
      // we shall allow for scalar(scalar(..)) !!!  => FIXME

      vec<n, mln_trait_op_times(T, S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] * s.to_equiv();
      return tmp;
    }

    template <unsigned n, typename T, typename S>
    inline
    vec<n, mln_trait_op_div(T, S)>
    operator/(const vec<n,T>& lhs, const mln::value::scalar_<S>& s)
    {
      mln_precondition(value::equiv(s) != literal::zero);
      vec<n, mln_trait_op_div(T, S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] / s.to_equiv();
      return tmp;
    }


    template <unsigned n, typename T>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const vec<n,T>& v)
    {
      ostr << '(';
      for (unsigned i = 0; i < n; ++i)
	ostr << debug::format(v[i]) << (i == n - 1 ? ")" : ", ");
      return ostr;
    }

    // vprod

    template <typename T, typename U>
    inline
    vec<3, mln_trait_op_times(T,U)> // FIXME: typename binary_arith_trait<T, U>::ret>
    vprod(const vec<3, T>& lhs, const vec<3, U>& rhs)
    {
      vec<3, mln_trait_op_times(T,U)> tmp; // FIXME: Likewise.
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
