// Copyright (C) 2006, 2008, 2009, 2011, 2012 EPITA Research and
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

#ifndef MLN_ALGEBRA_VEC_HH
# define MLN_ALGEBRA_VEC_HH

/// \file
///
/// Definition of a generic vector class.

# include <iostream>
# include <cmath>

# include <mln/core/concept/object.hh>

# include <mln/literal/zero.hh>
# include <mln/literal/origin.hh>
# include <mln/norm/l2.hh>
# include <mln/trait/all.hh>
# include <mln/trait/value_.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/debug/format.hh>

# include <mln/value/ops.hh>


// FIXME: Document.


namespace mln
{

  // Forward declarations.
  namespace algebra {
    template <unsigned n, typename T> class vec;
    template <unsigned d, typename C> class h_vec;
    template <unsigned n, unsigned m, typename T> class mat;
  }

  namespace literal {
    struct zero_t;
  }

  namespace norm {
    template <unsigned n, typename C>
    mln_sum_product(C,C) l2(const algebra::vec<n,C>& vec);
  }

  namespace value {
    template <unsigned n> struct rgb;
  }

  template <typename G, typename C> struct point;
  template <typename E> struct Gpoint;
  // end of forward declarations


  namespace trait
  {

    template <unsigned n, typename T>
    struct value_< mln::algebra::vec<n,T> >
    {
      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::data        kind;

      enum {
	dim   = n,
	nbits = n * mln_nbits(T),
	card  = n * mln_card(T)
      };
      typedef T comp;
      typedef mln_value_quant_from_(card) quant;
      typedef algebra::vec<n, mln_sum(T)> sum;
    };

    template <unsigned n, typename T>
    struct set_precise_unary_< op::ord, mln::algebra::vec<n,T> >
    {
      typedef mln::internal::ord_vec< mln::algebra::vec<n,T> > ret;
    };

  } // end of namespace mln::trait



  namespace algebra
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


    } // end of namespace mln::algebra::internal


    /*! \brief N-dimensional vector.
      \ingroup mlnalgebratypes
     */
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

      /// \{ Constructors/assignments with literal::zero.
      vec(const literal::zero_t&);
      vec& operator=(const literal::zero_t&);
      /// \}

      /// \{ Constructors/assignments with literal::origin.
      vec(const literal::origin_t&);
      vec& operator=(const literal::origin_t&);
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

      /* FIXME: What if the vector is null?  Even if we choose not to
	 handle this case, we should *state* in the documentation the
	 behavior of this method.

	 I (Roland) have added an assertion to detect ``erroneous''
	 cases, but we might want something different.
	 ``Implementation defined'' or ``undefined behavior'' is fine
	 by me, as long as the documentation mentions it.

	 FWIW, Trimesh's developers chose to set all the coordinates
	 of a vector being normalized to 1, when it norm is equal or
	 lower (sic) to zero.  */
      const vec<n, T>& normalize();

      /// Transposition.
      mat<1, n, T> t() const;

      /// Constructor; coordinates are set by function \p f.
      template <typename F>
      vec(const Function_v2v<F>& f);

      /// Zero value.
      static const vec<n, T> zero;

      /// Origin value.
      static const vec<n, T> origin;


      /// Conversion to a matrix.
      template <typename U>
      operator mat<n, 1, U>() const;

      /// Construction from a matrix.
      template <typename U>
      vec(const mat<n, 1, U>& rhs);

      /// Assignment from a matrix.
      template <typename U>
      vec& operator=(const mat<n, 1, U>& rhs);
    };


    /// \internal Conversion: vec -> rgb.
    template <typename T, unsigned m>
    void from_to_(const vec<3,T>& from, value::rgb<m>& to_);

    /// \internal Conversion: vec<1,C1> -> point<G,C2>
    template <typename C1, typename G, typename C2>
    inline
    void
    from_to_(const vec<1,C1>& from, point<G,C2>& to);

    /// \internal Conversion: vec<n,C1> -> point<G,C2>
    template <unsigned n, typename C1, typename G, typename C2>
    void
    from_to_(const vec<n,C1>& from, point<G,C2>& to);

    /// \internal Conversion: vec<1,C1> -> point<G,C1>
    template <typename C1, typename G>
    inline
    void
    from_to_(const vec<1,C1>& from, point<G,C1>& to);

    /// \internal Conversion: vec<n,C1> -> point<G,C1>
    template <unsigned n, typename C1, typename G>
    void
    from_to_(const vec<n,C1>& from, point<G,C1>& to);

    /// \internal Conversion: vec -> gpoint
    template <unsigned n, typename T, typename P>
    void
    from_to_(const vec<n,T>& from, Gpoint<P>& to);

  } // end of namespace mln::algebra



  namespace trait
  {

    // For unary traits.


    template < template <class> class Name,
	       unsigned n, typename T >
    struct set_precise_unary_< Name, algebra::vec<n, T> >
    {
      typedef mln_trait_unary(Name, T) V;
      typedef algebra::vec<n, V> ret;
    };


    // For binary traits.


    // vec + vec

    template < unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< op::plus,
				algebra::vec<n, T>, algebra::vec<n, U> >
    {
      typedef mln_trait_op_plus(T, U) V;
      typedef algebra::vec<n, V> ret;
    };

    // - vec

    template < unsigned n, typename T >
    struct set_precise_unary_< op::uminus,
			       algebra::vec<n, T> >
    {
      typedef mln_trait_op_uminus(T) V;
      typedef algebra::vec<n, V> ret;
    };

    // vec - vec

    template < unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< op::minus,
				algebra::vec<n, T>, algebra::vec<n, U> >
    {
      typedef mln_trait_op_minus(T, U) V;
      typedef algebra::vec<n, V> ret;
    };

    // vec * vec

    template < unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				algebra::vec<n, T>, algebra::vec<n, U> >
    {
      typedef mln_sum_product(T,U) ret;
    };

    // vec * s

    template < unsigned n, typename T,
	       typename S >
    struct set_precise_binary_< op::times,
				algebra::vec<n, T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_times(T, S) V;
      typedef algebra::vec<n, V> ret;
    };

    // vec / s

    template < unsigned n, typename T,
	       typename S >
    struct set_precise_binary_< op::div,
				algebra::vec<n, T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_div(T, S) V;
      typedef algebra::vec<n, V> ret;
    };

//     template < template<class, class> class Name,
// 	       unsigned n, typename T,
// 	       typename S >
//     struct set_binary_< Name,
// 			mln::Object, algebra::vec<n, T>,
// 			mln::value::Scalar, S >
//     {
//       typedef mln_trait_binary(Name, T, S) V;
//       typedef algebra::vec<n, V> ret;
//     };

  } // end of namespace mln::trait



  namespace algebra
  {

    // vec == vec

    template <unsigned n, typename T, typename U>
    bool
    operator==(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // vec + vec

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_plus(T,U)>
    operator+(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // - vec

    template <unsigned n, typename T>
    vec<n, mln_trait_op_uminus(T)>
    operator-(const vec<n,T>& rhs);

    // vec - vec

    template <unsigned n, typename T, typename U>
    vec<n, mln_trait_op_minus(T,U)>
    operator-(const vec<n,T>& lhs, const vec<n,U>& rhs);

    // vec * vec

    /// Scalar product (dot product).
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

    // >>

    template <unsigned n, typename T>
    std::istream&
    operator>>(std::istream& istr, vec<n,T>& v);

    // vprod // FIXME: Generalize...

    /// Vectorial product (cross product).
    template <typename T, typename U>
    vec<3, mln_trait_op_times(T,U)> // FIXME: Sum of product...
    vprod(const vec<3, T>& lhs, const vec<3, U>& rhs);

  } // end of namespace mln::algebra




# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename C, typename C2>
    inline
    C
    convert_data_(metal::bool_<false>, const C2& v)
    {
      return static_cast<C>(v);
    }

    template <typename C, typename C2>
    inline
    C
    convert_data_(metal::bool_<true>, const C2& v)
    {
      return static_cast<C>(round(v));
    }

    template <typename C, typename C2>
    inline
    C
    convert_data(const C2& v)
    {
      // If (C != float && C != double) && (C2 == float || C2 == double)
      // => We want to round the value.
      // Otherwise we can just statically cast.
      //
      return convert_data_<C>(
	typename mlc_and(
	  mlc_and(mlc_is_not(C,float),
		  mlc_is_not(C,double)),
	  mlc_or(mlc_is(C2,float),
		 mlc_is(C2, double)))::eval(), v);
    }

  } // end of namespace mln::internal



  namespace algebra
  {

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
    vec<n,T>::vec(const literal::origin_t&)
    {
      this->set_all(0);
    }

    template <unsigned n, typename T>
    inline
    vec<n,T>&
    vec<n,T>::operator=(const literal::origin_t&)
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
      mlc_converts_to(U, T)::check();
      for (unsigned i = 0; i < n; ++i)
	data_[i] = static_cast<T>(rhs[i]);
    }

    template <unsigned n, typename T>
    template <typename U>
    inline
    vec<n,T>& vec<n,T>::operator=(const vec<n, U>& rhs)
    {
      mlc_converts_to(U, T)::check();
      for (unsigned i = 0; i < n; ++i)
	data_[i] = static_cast<T>(rhs[i]);
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
      mln_sum_product(T,T) l2_norm = norm::l2(*this);
      mln_assertion(l2_norm > mln_sum_product(T,T)(0));
      for (unsigned i = 0; i < n; ++i)
	data_[i] = static_cast<T>(data_[i] / l2_norm);
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
	data_[i] = static_cast<T>(f(i));
    }


    template <unsigned n, typename T>
    const vec<n, T> vec<n, T>::zero = all_to(0);

    template <unsigned n, typename T>
    const vec<n, T> vec<n, T>::origin = all_to(0);



    // Operators.


    // vec == vec

    template <unsigned n, typename T, typename U>
    inline
    bool operator==(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	if (lhs[i] != rhs[i])
	  return false;
      return true;
    }

    // vec + vec

    template <unsigned n, typename T, typename U>
    inline
    vec<n, mln_trait_op_plus(T,U)>
    operator+(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      typedef mln_trait_op_plus(T,U) R;
      vec<n, R> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] + rhs[i];
      return tmp;
    }

    // - vec

    template <unsigned n, typename T>
    inline
    vec<n, mln_trait_op_uminus(T)>
    operator-(const vec<n,T>& rhs)
    {
      typedef mln_trait_op_uminus(T) R;
      vec<n, R> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = - rhs[i];
      return tmp;
    }

    // vec - vec

    template <unsigned n, typename T, typename U>
    inline
    vec<n, mln_trait_op_minus(T,U)>
    operator-(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      typedef mln_trait_op_minus(T,U) R;
      vec<n, R> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] - rhs[i];
      return tmp;
    }

    // vec * vec

    template <unsigned n, typename T, typename U>
    inline
    mln_sum_product(T,U)
    operator*(const vec<n,T>& lhs, const vec<n,U>& rhs)
    {
      typedef mln_sum_product(T,U) R;
      mln_sum_product(T,U) tmp(literal::zero);
      for (unsigned i = 0; i < n; ++i)
	tmp += lhs[i] * rhs[i];
      return tmp;
    }

    // vec * s

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

      typedef mln_trait_op_times(T, S) R;
      vec<n, R> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] * s.to_equiv();
      return tmp;
    }

    // vec / s

    template <unsigned n, typename T, typename S>
    inline
    vec<n, mln_trait_op_div(T, S)>
    operator/(const vec<n,T>& lhs, const mln::value::scalar_<S>& s)
    {
      mln_precondition(value::equiv(s) != (S)(literal::zero));
      typedef mln_trait_op_div(T, S) R;
      vec<n, R> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = lhs[i] / s.to_equiv();
      return tmp;
    }


    // << v

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


    // >> v

    template <unsigned n, typename T>
    inline
    std::istream&
    operator>>(std::istream& istr, vec<n,T>& v)
    {
      for (unsigned i = 0; i < n; ++i)
	istr >> v[i];
      return istr;
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


    template <typename P>
    inline
    P
    to_point(const vec<P::dim,float>& v)
    {
      P tmp;
      for (unsigned i = 0; i < P::dim; ++i)
        tmp[i] = round(v[i]);
      return tmp;
    }


    // Conversions


    template <typename T, unsigned m>
    inline
    void
    from_to_(const vec<3,T>& from, value::rgb<m>& to)
    {
      algebra::vec<3, unsigned> tmp;
      for (unsigned i = 0; i < 3; ++i)
	tmp[i] = static_cast<unsigned>(from[i]); // FIXME: Use from_to_ instead of cast.

      to = value::rgb<m>(tmp);
    }

    template <unsigned n, typename C1, typename G, typename C2>
    inline
    void
    from_to_(const mln::algebra::vec<n,C1>& from, point<G,C2>& to)
    {
      mlc_converts_to(C1, C2)::check();
      enum { dim = G::dim };
      mlc_bool(G::dim == n)::check();

      unsigned j = 0;
      for (unsigned i = dim - 2; i < dim; ++i)
	to[i]   = mln::internal::convert_data<C2>(from[j++]);
      for (unsigned i = 2; i < dim; ++i, ++j)
	to[i-j] = mln::internal::convert_data<C2>(from[j]);
    }

    template <typename C1, typename G, typename C2>
    inline
    void
    from_to_(const mln::algebra::vec<1,C1>& from, point<G,C2>& to)
    {
      mlc_converts_to(C1, C2)::check();
      enum { dim = G::dim };
      mlc_bool(G::dim == 1)::check();

      to[0] = mln::internal::convert_data<C2>(from[0]);
    }

    template <unsigned n, typename C1, typename G>
    inline
    void
    from_to_(const mln::algebra::vec<n,C1>& from, point<G,C1>& to)
    {
      enum { dim = G::dim };
      mlc_bool(G::dim == n)::check();

      unsigned j = 0;
      for (unsigned i = dim - 2; i < dim; ++i)
	to[i]   = from[j++];
      for (unsigned i = 2; i < dim; ++i, ++j)
	to[i-j] = from[j];
    }

    template <typename C1, typename G>
    inline
    void
    from_to_(const mln::algebra::vec<1,C1>& from, point<G,C1>& to)
    {
      enum { dim = G::dim };
      mlc_bool(G::dim == 1)::check();

      to[0] = from[0];
    }

    template <unsigned n, typename T, typename P>
    inline
    void
    from_to_(const vec<n,T>& from, Gpoint<P>& to_)
    {
      mlc_bool(P::dim == n)::check();
      P& to = exact(to_);
      for (unsigned i = 0; i < n; ++i)
	to[i] = static_cast< typename P::coord >(from[i]); // FIXME: cast -> best effort...
    }


  } // end of namespace mln::algebra


# endif // MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/make/vec.hh>
# include <mln/algebra/mat.hh>


#endif // ! MLN_ALGEBRA_VEC_HH
