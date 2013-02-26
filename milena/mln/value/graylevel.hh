// Copyright (C) 2006, 2007, 2008, 2009, 2013 EPITA Research and
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

#ifndef MLN_VALUE_GRAYLEVEL_HH
# define MLN_VALUE_GRAYLEVEL_HH

/// \file
///
/// Definition of the mln::value::graylevel class.

# include <iostream>

# include <mln/value/ops.hh>

# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/math/max.hh>
# include <mln/metal/bexpr.hh>
# include <mln/literal/ops.hh>

# include <mln/value/graylevel_f.hh>
# include <mln/value/int_u.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace literal
  {
    /// \{ Forward declarations.
    struct black_t;
    struct medium_gray_t;
    struct white_t;
    /// \}
  }
  namespace value
  {

    /// \{ Forward declarations.
    namespace internal
    {
      template <unsigned n> class gray_;
      struct gray_f;
      template <unsigned n_src, unsigned n_dest>
      long convert(int val);
    }
    template <unsigned n> struct graylevel;
    struct float01_f;
    /// \}
  }



  namespace trait
  {

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::plus, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::internal::gray_< mlc_max_int(n, m) > ret;
    };

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::minus, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::internal::gray_< mlc_max_int(m, n) > ret;
    };

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::times, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::internal::gray_<mlc_max_int(m, n)> ret;
    };

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::div, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template < unsigned n, typename I >
    struct set_binary_< op::times,
			mln::value::Integer, mln::value::graylevel<n>,
			mln::value::Integer, I >
    {
      typedef mln::value::internal::gray_<n> ret;
    };

    template < typename I, unsigned n >
    struct set_binary_< op::times,
			mln::value::Integer, I,
			mln::value::Integer, mln::value::graylevel<n> >
    {
      typedef mln::value::internal::gray_<n> ret;
    };


    template < unsigned n, typename F >
    struct set_binary_< op::times,
			mln::value::Integer,  mln::value::graylevel<n>,
			mln::value::Floating, F >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template < typename F, unsigned n >
    struct set_binary_< op::times,
			mln::value::Floating, F,
			mln::value::Integer,  mln::value::graylevel<n> >
    {
      typedef mln::value::internal::gray_f ret;
    };


    template < unsigned n, typename S >
    struct set_precise_binary_< op::times, mln::value::graylevel<n>, mln::value::scalar_<S> >
    {
      typedef mln_value_equiv(S) E;
      typedef mln::metal::or_< mlc_equal(E, float), mlc_equal(E, double) > is_float;
      typedef mlc_if(is_float, mln::value::internal::gray_f, mln::value::internal::gray_<n>) ret;
    };

    template < unsigned n, typename S >
    struct set_precise_binary_< op::div, mln::value::graylevel<n>, mln::value::scalar_<S> >
    {
      typedef mln::value::internal::gray_f ret;
    };

    // 'graylevel<n>' as a value.

    template <unsigned n>
    struct value_< mln::value::graylevel<n> >
    {
    private:
      typedef mln::value::graylevel<n> self_;
    public:

      enum {
	dim = 1,
	nbits = n,
	card  = mln_value_card_from_(n)
      };

      typedef trait::value::nature::integer nature; // FIXME: Or scalar?.
      typedef trait::value::kind::gray      kind;
      typedef mln_value_quant_from_(card)   quant;

      static const self_ min() { return 0; }
      static const self_ max() { return card - 1; }
      static const self_ epsilon() { return 0; }

      typedef mln::value::int_u<n> comp;

      typedef float sum;
    };

  } // end of namespace mln::trait



  namespace value
  {

    /// General gray-level class on n bits.

    // Return types of the operators +-*/.

    // Operators + - return the interoperability type to allow
    // operations like (gl8(255) + gl8(255)) / 2. + - with int and
    // float are not allowed.

    // If you really need to act with the value
    // of the gray_<n> (the encoding). use the to_enc() method.
    //

    //     |--------------------------------------------|
    //     | +  ||  gl  |  glf  |gray_n |  int  | float |
    //     |============================================|
    //     |gl  ||gray_n|gray_f |gray_n |   X   |   X   |
    //     |--------------------------------------------|
    //     |glf ||      |gray_f |gray_f |   X   |   X   |
    //     |--------------------------------------------|
    //     |gray||              |gray_n |   X   |   X   |
    //     |--------------------------------------------|

    //     |--------------------------------------------|
    //     | -  ||  gl  |  glf  |gray_n |  int  | float |
    //     |============================================|
    //     |gl  ||gray_n|gray_f |gray_n |   X   |   X   |
    //     |--------------------------------------------|
    //     |glf ||      |gray_f |gray_f |   X   |   X   |
    //     |--------------------------------------------|
    //     |gray||              |gray_n |   X   |   X   |
    //     |--------------------------------------------|

    //     |--------------------------------------------|
    //     | *  ||  gl  |  glf  |gray_n |  int  | float |
    //     |============================================|
    //     |gl  ||gray_n|gray_f |gray_n |gray_n |gray_f |
    //     |--------------------------------------------|
    //     |glf ||      |gray_f |gray_f |gray_f |gray_f |
    //     |--------------------------------------------|
    //     |gray||              |gray_n |gray_n |gray_f |
    //     |--------------------------------------------|

    //     |--------------------------------------------|
    //     | /  ||  gl  |  glf  |gray_n |  int  | float |
    //     |============================================|
    //     |gl  ||gray_f|gray_f |gray_n |gray_f |gray_f |
    //     |--------------------------------------------|
    //     |glf ||      |gray_f |gray_f |gray_f |gray_f |
    //     |--------------------------------------------|
    //     |gray||              |gray_f |gray_f |gray_f |
    //     |--------------------------------------------|

    //     Valid Conversions are :

    //     gray_f -> gray_<n>
    //     gray_f -> graylevel<n>
    //     gray_f -> graylevel_f

    //     gray_<n> -> gray_f
    //     gray_<n> -> graylevel<n>
    //     gray_<n> -> graylevel_f

    //     graylevel_f -> gray_f
    //     graylevel_f -> graylevel<n>

    //     graylevel<n> -> gray_<n>
    //     graylevel<n> -> graylevel_f


    template <unsigned n>
    struct graylevel
      :
      public Integer< graylevel<n> >,

      public internal::value_like_< int_u<n>,          // Equivalent.
				    mln_enc(int_u<n>), // Encoding.
				    internal::gray_<n>,// Interoperation.
				    graylevel<n> >     // Exact.
    {

      /// Constructor without argument.
      graylevel();
      /// Copy constructor.
      graylevel(const graylevel<n>& rhs);
      /// Assigment.
      graylevel<n>& operator=(const graylevel<n>& rhs);

      /// Constructor from int.
      graylevel(int val);
      /// Assigment with int.
      graylevel<n>& operator=(int val);

      /// Constructor from any graylevel.
      template <unsigned m>
	graylevel(const graylevel<m>& rhs);
      /// Assigment with any graylevel.
      template <unsigned m>
	graylevel<n>& operator=(const graylevel<m>& rhs);

      /// Ctors with literals.
      /// \{
      graylevel(const mln::literal::black_t&);
      graylevel(const mln::literal::medium_gray_t&);
      graylevel(const mln::literal::white_t&);
      /// \}


      /// Assigment with literals.
      /// \{
      graylevel<n>& operator=(const mln::literal::black_t&);
      graylevel<n>& operator=(const mln::literal::medium_gray_t&);
      graylevel<n>& operator=(const mln::literal::white_t&);
      /// \}


      /// Access to std type.
      unsigned value() const;

      /// Conversion to float between 0 and 1.
      float to_float() const;
    };



    namespace internal
    {
      // Fwd decl;
      template <typename T> struct convert_;

      // convert for graylevel.
      template <unsigned n>
      struct convert_< graylevel<n> >
      {
	static graylevel<n> value_at_index(unsigned i);
	static unsigned index_of_value(graylevel<n> v);
      };

    } // end of namespace mln::value::internal


    /// Op<<.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const graylevel<n>& g);

    // graylevel<n> + graylevel<m>.
    template <unsigned n, unsigned m>
    mln_trait_op_plus(graylevel<n>, graylevel<m>)
      operator+(const graylevel<n>& lhs, const graylevel<m>& rhs);

    // graylevel<n> + Another type (doesn't compile).
    template <unsigned n, typename I>
    void
    operator+(const graylevel<n>& lhs, const I& i);

    // graylevel<n> + Another type (doesn't compile).
    template <unsigned n, typename I>
    void
    operator+(const I& i, const graylevel<n>& rhs);

    // graylevel<n> - graylevel<m>.
    template <unsigned n, unsigned m>
    mln_trait_op_minus(graylevel<n>, graylevel<m>)
      operator-(const graylevel<n>& lhs, const graylevel<m>& rhs);

    // graylevel<n> - Another type (doesn't compile).
    template <unsigned n, typename I>
    void
    operator-(const graylevel<n>& lhs, const I& i);

    // graylevel<n> - Another type (doesn't compile).
    template <unsigned n, typename I>
    void
    operator-(const I& i, const graylevel<n>& rhs);

    // graylevel<n> * graylevel<m>.
    template <unsigned n, unsigned m>
    mln_trait_op_times(graylevel<n>, graylevel<m>)
      operator*(const graylevel<n>& lhs, const graylevel<m>& rhs);

    // With Builtins.

    // graylevel<n> * T.
    template <unsigned n, typename T>
    mln_trait_op_times(graylevel<n>, T)
      operator*(const graylevel<n>& lhs, const T& rhs);

    // T * graylevel<n>.
    template <unsigned n, typename T>
    mln_trait_op_times(graylevel<n>, T)
      operator*(const T& lhs, const graylevel<n>& rhs);

    // graylevel<n> / T.
    template <unsigned n, typename T>
    internal::gray_f
    //mln_trait_op_div(graylevel<n>, T).
      operator/(const graylevel<n>& lhs, const T& rhs);

    // With Integer.

    // graylevel<n> * Integer<I>.
    template <unsigned n, typename I>
    mln_trait_op_times(graylevel<n>, I)
      operator*(const graylevel<n>& lhs, const Integer<I>& rhs);

    // Integer<I> * graylevel<n>.
    template <typename I, unsigned n>
    mln_trait_op_times(I, graylevel<n>)
      operator*(const Integer<I>& lhs, const graylevel<n>& rhs);

    // graylevel<n> / Integer<I>.
    template <unsigned n, typename I>
    mln_trait_op_div(graylevel<n>, I)
      operator/(const graylevel<n>& lhs, const Integer<I>& rhs);

    // Integer<I> / graylevel<n>.
    template <typename I, unsigned n>
    mln_trait_op_div(I, graylevel<n>)
      operator/(const Integer<I>& lhs, const graylevel<n>& rhs);

    // With Floating.

    // graylevel<n> * Floating<F>.
    template <unsigned n, typename F>
    mln_trait_op_times(graylevel<n>, F)
      operator*(const graylevel<n>& lhs, const Floating<F>& rhs);

    // Floating<F>, graylevel<n>.
    template <typename F, unsigned n>
    mln_trait_op_times(F, graylevel<n>)
      operator*(const Floating<F>& lhs, const graylevel<n>& rhs);


    // graylevel<n> / Floating<F>.
    template <unsigned n, typename F>
    mln_trait_op_div(graylevel<n>, F)
      operator/(const graylevel<n>& lhs, const Floating<F>& rhs);

    // Floating<F> / graylevel<n>.
    template <typename F, unsigned n>
    mln_trait_op_div(F, graylevel<n>)
      operator/(const Floating<F>& lhs, const graylevel<n>& rhs);

# ifndef MLN_INCLUDE_ONLY

    // Graylevel<n>.


    template <unsigned n>
    inline
    graylevel<n>::graylevel()
    {
    }


    template <unsigned n>
    inline
    graylevel<n>::graylevel(int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(mln_enc(int_u<n>)));
      this->v_ = val;
    }

    template <unsigned n>
    inline
    graylevel<n>&
    graylevel<n>::operator=(int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(mln_enc(int_u<n>)));
      this->v_ = val;
      return *this;
    }

    template <unsigned n>
    inline
    graylevel<n>::graylevel(const graylevel<n>& rhs) :
      Integer< graylevel<n> >()
    {
      this->v_ = rhs.v_;
    }

    template <unsigned n>
    inline
    graylevel<n>&
    graylevel<n>::operator=(const graylevel<n>& rhs)
    {
      this->v_ = rhs.v_;
      return *this;
    }

    template <unsigned n>
    template <unsigned m>
    inline
    graylevel<n>::graylevel(const graylevel<m>& rhs)
    {
      this->v_ = internal::convert<m, n>(rhs.value());
    }

    template <unsigned n>
    template <unsigned m>
    inline
    graylevel<n>&
    graylevel<n>::operator=(const graylevel<m>& rhs)
    {
      this->v_ = internal::convert<m, n>(rhs.value());
      return *this;
    }


    template <unsigned n>
    inline
    graylevel<n>::graylevel(const mln::literal::black_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    inline
    graylevel<n>&
    graylevel<n>::operator=(const mln::literal::black_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    inline
    graylevel<n>::graylevel(const mln::literal::medium_gray_t&)
    {
      this->v_ = metal::math::pow_int<2, n - 1>::value;
    }

    template <unsigned n>
    inline
    graylevel<n>&
    graylevel<n>::operator=(const mln::literal::medium_gray_t&)
    {
      this->v_ = metal::math::pow_int<2, n - 1>::value;
      return *this;
    }


    template <unsigned n>
    inline
    graylevel<n>::graylevel(const mln::literal::white_t&)
    {
      this->v_ = mln_max(mln_enc(int_u<n>));
    }

    template <unsigned n>
    inline
    graylevel<n>&
    graylevel<n>::operator=(const mln::literal::white_t&)
    {
      this->v_ = mln_max(mln_enc(int_u<n>));
      return *this;
    }

    template <unsigned n>
    inline
    unsigned
    graylevel<n>::value() const
    {
      return this->v_;
    }

    template <unsigned n>
    inline
    float
    graylevel<n>::to_float() const
    {
      static const float denom = float(metal::math::pow_int<2, n>::value) - 1.f;
      return float(this->v_) / denom;
    }



    namespace internal
    {

      template <unsigned n>
      inline
      graylevel<n>
      convert_< graylevel<n> >::value_at_index(unsigned i)
      {
	mln_assertion(i <= mln_max(mln_equiv(graylevel<n>)));
	return graylevel<n>(i);
      }

      template <unsigned n>
      inline
      unsigned
      convert_< graylevel<n> >::index_of_value(graylevel<n> v)
      {
	return v.value();
      }

    } // end of namespace mln::value::internal

    // Operators.

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const graylevel<n>& g)
    {
      return ostr << g.value() << "/gl" << n; // FIXME: Be more explicit!
    }

    // The remaining operators are in mln/value/internal/gray_.hh.

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#include <mln/value/internal/gray_f.hh>
#include <mln/value/internal/gray_.hh>

#endif // ! MLN_VALUE_GRAYLEVEL_HH
