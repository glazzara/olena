// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_GRAYLEVEL_HH
# define MLN_VALUE_GRAYLEVEL_HH

/*! \file mln/value/graylevel.hh
 *
 * \brief Definition of the mln::value::graylevel class.
 */

# include <iostream>

# include <mln/value/ops.hh>

# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/bexpr.hh>
# include <mln/literal/ops.hh>

# include <mln/value/int_u.hh>
# include <mln/value/gray.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace literal
  {
    /// \{ Fwd decls.
    struct black_t;
    struct medium_gray_t;
    struct white_t;
    /// \}
  }
  namespace value
  {
    /// \{ Fwd decls.
    class gray;
    template <unsigned n> struct graylevel;
    struct float01_f;
    /// \}
  }



  namespace trait
  {
    // FIXME: Change into
    // - for op::plus:  mln::value::graylevel<1 + (n > m ? n : m)>
    // - for op::times: mln::value::graylevel<n + m>
    // - ...

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::plus, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::gray ret;
    };

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::minus, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::gray ret;
    };

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::times, mln::value::graylevel<n>, mln::value::graylevel<m> >
    {
      typedef mln::value::gray ret;
    };


    template < unsigned n, typename I >
    struct set_binary_< op::times,
			mln::value::Integer, mln::value::graylevel<n>,
			mln::value::Integer, I >
    {
      typedef mln::value::gray ret;
    };

    template < typename I, unsigned n >
    struct set_binary_< op::times,
			mln::value::Integer, I,
			mln::value::Integer, mln::value::graylevel<n> >
    {
      typedef mln::value::gray ret;
    };


    template < unsigned n, typename F >
    struct set_binary_< op::times,
			mln::value::Integer,  mln::value::graylevel<n>,
			mln::value::Floating, F >
    {
      typedef float ret;
    };

    template < typename F, unsigned n >
    struct set_binary_< op::times,
			mln::value::Floating, F,
			mln::value::Integer,  mln::value::graylevel<n> >
    {
      typedef float ret;
    };


    template < unsigned n, typename S >
    struct set_precise_binary_< op::times, mln::value::graylevel<n>, mln::value::scalar_<S> >
    {
      typedef mln_value_equiv(S) E;
      typedef mlc_equal(E, float) is_float; // FIXME: Or double...
      typedef mlc_if(is_float, float, mln::value::gray) ret;
    };

    template < unsigned n, typename S >
    struct set_precise_binary_< op::div, mln::value::graylevel<n>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_times(mln::value::graylevel<n>,
				 mln::value::scalar_<S>) ret;
    };


    // 'graylevel<n>' as a value.

    template <unsigned n>
    struct value_< mln::value::graylevel<n> >
    {
    private:
      typedef mln::value::graylevel<n> self_;
    public:

      enum {
	nbits = n,
	card  = mln_value_card_from_(n)
      };

      typedef trait::value::nature::integer nature; // FIXME: Or scalar?
      typedef trait::value::kind::gray      kind;
      typedef mln_value_quant_from_(card)   quant;

      static const self_ min() { return 0; }
      static const self_ max() { return card - 1; }
      static const self_ epsilon() { return 0; }

      typedef float sum;
    };


  } // end of namespace mln::trait



  namespace value
  {

    /// General gray-level class on n bits.
    template <unsigned n>
    struct graylevel
      :
      public Integer< graylevel<n> >,

      public internal::value_like_< int_u<n>,          // Equivalent.
				    mln_enc(int_u<n>), // Encoding.
				    gray,              // Interoperation.
				    graylevel<n> >     // Exact.
    {
      /// Constructor without argument.
      graylevel();


      /// Constructor from int.
      graylevel(int val);

      /// Assigment with int.
      graylevel<n>& operator=(int val);


      /// Constructor from gray.
      graylevel(const gray& rhs);

      /// Assigment with gray.
      graylevel<n>& operator=(const gray& rhs);


      /// Copy constructor.
      graylevel(const graylevel<n>& rhs);

      /// Assigment.
      graylevel<n>& operator=(const graylevel<n>& rhs);


      /// Constructor from any graylevel.
      template <unsigned m>
      graylevel(const graylevel<m>& rhs);

      /// Assigment with any graylevel.
      template <unsigned m>
      graylevel<n>& operator=(const graylevel<m>& rhs);


      /// Ctors with literals.
      /// \{
      graylevel(const literal::black_t&);
      graylevel(const literal::medium_gray_t&);
      graylevel(const literal::white_t&);
      /// \}

      /// Assigment with literals.
      /// \{
      graylevel<n>& operator=(const literal::white_t&);
      graylevel<n>& operator=(const literal::medium_gray_t&);
      graylevel<n>& operator=(const literal::black_t&);
      /// \}


      /// Access to std type.
      unsigned value() const;

      /// Conversion to float between 0 and 1.
      float to_float() const;
    };



    /// Op<<.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const graylevel<n>& g);


    /// Aliases.
    typedef graylevel<8>  gl8;
    typedef graylevel<16> gl16;
    typedef graylevel<32> gl32;



    template <unsigned n, unsigned m>
    gray
    operator+(const graylevel<n>& lhs, const graylevel<m>& rhs);

    template <unsigned n, unsigned m>
    gray
    operator-(const graylevel<n>& lhs, const graylevel<m>& rhs);

    template <unsigned n, unsigned m>
    gray
    operator*(const graylevel<n>& lhs, const graylevel<m>& rhs);

    // With Integer.

    template <unsigned n, typename I>
    gray
    operator*(const graylevel<n>& lhs, const Integer<I>& rhs);

    template <typename I, unsigned n>
    gray
    operator*(const Integer<I>& lhs, const graylevel<n>& rhs);

    // With Floating.

    template <unsigned n, typename F>
    float
    operator*(const graylevel<n>& lhs, const Floating<F>& rhs);

    template <typename F, unsigned n>
    float
    operator*(const Floating<F>& lhs, const graylevel<n>& rhs);




# ifndef MLN_INCLUDE_ONLY

    // Graylevel<n>.


    template <unsigned n>
    graylevel<n>::graylevel()
    {
    }


    template <unsigned n>
    graylevel<n>::graylevel(int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(mln_enc(int_u<n>)));
      this->v_ = val;
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(mln_enc(int_u<n>)));
      this->v_ = val;
      return *this;
    }


    template <unsigned n>
    graylevel<n>::graylevel(const gray& g)
    {
      gray tmp = g.to_nbits(n);
      this->v_ = tmp.value();
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(const gray& g)
    {
      gray tmp = g.to_nbits(n);
      this->v_ = tmp.value();
      return *this;
    }

    template <unsigned n>
    graylevel<n>::graylevel(const graylevel<n>& rhs)
    {
      this->v_ = rhs.v_;
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(const graylevel<n>& rhs)
    {
      this->v_ = rhs.v_;
      return *this;
    }

    template <unsigned n>
    template <unsigned m>
    graylevel<n>::graylevel(const graylevel<m>& rhs)
    {
      *this = gray(rhs).to_nbits(n);
    }

    template <unsigned n>
    template <unsigned m>
    graylevel<n>&
    graylevel<n>::operator=(const graylevel<m>& rhs)
    {
      *this = gray(rhs).to_nbits(n);
      return *this;
    }


    template <unsigned n>
    graylevel<n>::graylevel(const literal::black_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(const literal::black_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    graylevel<n>::graylevel(const literal::medium_gray_t&)
    {
      this->v_ = metal::math::pow_int<2, n - 1>::value;
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(const literal::medium_gray_t&)
    {
      this->v_ = metal::math::pow_int<2, n - 1>::value;
      return *this;
    }

    template <unsigned n>
    graylevel<n>::graylevel(const literal::white_t&)
    {
      this->v_ = mln_max(mln_enc(int_u<n>));
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(const literal::white_t&)
    {
      this->v_ = mln_max(mln_enc(int_u<n>));
      return *this;
    }


    template <unsigned n>
    unsigned
    graylevel<n>::value() const
    {
      return this->v_;
    }

    template <unsigned n>
    float
    graylevel<n>::to_float() const
    {
      static const float denom = float(metal::math::pow_int<2, n>::value) - 1.f;
      return float(this->v_) / denom;
    }


    // Operators.

    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const graylevel<n>& g)
    {
      return ostr << g.value() << "/gl" << n; // FIXME: Be more explicit!
    }

    // The remaining operators are in mln/value/gray.hh.

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_GRAYLEVEL_HH
