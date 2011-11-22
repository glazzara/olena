// Copyright (C) 2006, 2007, 2008, 2009, 2011 EPITA Research and
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

#ifndef MLN_VALUE_INTERNAL_GRAY_F_HH
# define MLN_VALUE_INTERNAL_GRAY_F_HH

/// \file
///
/// Definition of the mln::value::gray_f class.


# include <iostream>

# include <mln/value/ops.hh>

# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/bexpr.hh>
# include <mln/literal/ops.hh>

# include <mln/value/float01_f.hh>
# include <mln/trait/value_.hh>

# include <mln/value/graylevel_f.hh>

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
    }

    struct float01_f;
    /// \}
  }



  namespace trait
  {


    template < template <class, class> class Name>
    struct set_precise_binary_< Name, mln::value::internal::gray_f, mln::value::internal::gray_f >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template <>
    struct set_precise_binary_< op::greater, mln::value::internal::gray_f, mln::value::internal::gray_f >
    {
      typedef bool ret;
    };

    template <>
    struct set_precise_binary_< op::eq, mln::value::internal::gray_f, mln::value::internal::gray_f >
    {
      typedef bool ret;
    };

    // Necessary??
//     template <typename F>
//     struct set_binary_< op::eq,
// 			mln::value::Floating,  mln::value::internal::gray_f,
// 			mln::value::Floating, F >
//     {
//       typedef bool ret;
//     };

    /// Forward declaration.
    template <typename T> struct value_;

    // 'gray_f' as a value.
    template <>
    struct value_<mln::value::internal::gray_f>
    {
    private:
      typedef mln::value::internal::gray_f self_;
      typedef float equiv_;

    public:

      enum {
	nbits = mln_nbits(equiv_),
	card  = 0
      };

      typedef trait::value::nature::floating nature;
      typedef trait::value::kind::gray       kind;
      typedef mln_trait_value_quant_(equiv_)   quant;

      static equiv_ min() { return 0; }
      static equiv_ max() { return 1; }
      static equiv_ epsilon() { return mln_epsilon(equiv_); }

      typedef float sum;
    };

  } // end of namespace mln::trait



  namespace value
  {

    namespace internal
    {

      /// General gray-level class on n bits.
      struct gray_f
	:
	public Floating< gray_f >,

	public internal::value_like_< float,     // Equivalent.
				      float,     // Encoding.
				      gray_f,    // Interoperation.
				      gray_f >   // Exact.
      {
	/// Constructor without argument.
	gray_f();

	/// \{ Constructors/assigments with gray_f.
	gray_f(const gray_f& rhs);
	gray_f& operator=(const gray_f& rhs);
	/// \}

	/// \{ Constructors/assigments with gray_<n>.
	template <unsigned n>
	gray_f(const gray_<n>& rhs);
	template <unsigned n>
	gray_f& operator=(const gray_<n>& rhs);
	/// \}

	/// \{ Constructors/assigments with float.
	gray_f(float val);
	gray_f& operator=(float val);
	/// \}

	/// \{ Constructors/assigments with graylevel_f.
	gray_f(const graylevel_f& val);
	gray_f& operator=(const graylevel_f& val);
	/// \}

	/// Conversion to graylevel<n>
	template <unsigned m>
	operator graylevel<m>() const;

	/// Conversion to gray_<n>
	template <unsigned m>
	operator gray_<m>() const;

	/// Conversion to graylevel_f
	operator graylevel_f() const;

	/// Access to std type.
	float value() const;
      };

      // Operators.

      /// Op<<.
      std::ostream& operator<<(std::ostream& ostr, const gray_f& g);

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


# include <mln/value/internal/gray_.hh>


namespace mln
{

  namespace value
  {

    namespace internal
    {

# ifndef MLN_INCLUDE_ONLY

      // gray_f.

      inline
      gray_f::gray_f()
      {
      }

      inline
      gray_f::gray_f(const gray_f& g)
	: Floating<gray_f>()
      {
	this->v_ = g.v_;
      }

      inline
      gray_f&
      gray_f::operator=(const gray_f& g)
      {
	this->v_ = g.v_;
	return *this;
      }

      template <unsigned n>
      gray_f::gray_f(const gray_<n>& rhs)
      {
	static const float denom = float(metal::math::pow_int<2, n>::value) - 1.f;
	this->v_ = float(rhs.value()) / denom;
      }

      template <unsigned n>
      gray_f&
      gray_f::operator=(const gray_<n>& rhs)
      {
	static const float denom = float(metal::math::pow_int<2, n>::value) - 1.f;
	this->v_ = float(rhs.value()) / denom;
	return *this;
      }

      inline
      gray_f::gray_f(float val)
      {
	this->v_ = val;
      }

      inline
      gray_f&
      gray_f::operator=(float val)
      {
	this->v_ = val;
	return *this;
      }

      inline
      gray_f::gray_f(const graylevel_f& rhs)
      {
	this->v_ = rhs.value();
      }

      inline
      gray_f&
      gray_f::operator=(const graylevel_f& rhs)
      {
	this->v_ = rhs.value();
	return *this;
      }

      template <unsigned m>
      inline
      gray_f::operator graylevel<m>() const
      {
	return graylevel<m>(int(round(this->v_ * (mlc_pow_int(2, m) - 1))));
      }


      template <unsigned m>
      inline
      gray_f::operator gray_<m>() const
      {
	return gray_<m>(int(round(this->v_ * (mlc_pow_int(2, m) - 1))));
      }

      inline
      gray_f::operator graylevel_f() const
      {
	return graylevel_f(this->v_);
      }

      inline
      float
      gray_f::value() const
      {
	return this->v_;
      }

      // Operators.

      inline
      std::ostream& operator<<(std::ostream& ostr, const gray_f& g)
      {
	return ostr << g.value() << "/gl_f"; // FIXME: Be more explicit!
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::value::internal


    // Graylevel_F operators.

    // Op glf == Int

    template <typename I>
    inline
    bool
    operator==(const Integer<I>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() == exact(lhs);
    }

    // Op glf == glf
    inline
    bool
    operator==(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return rhs.value() == lhs.value();
    }

    // Op glf + glf
    inline
    mln_trait_op_plus_(graylevel_f, graylevel_f)
      operator+(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return lhs.value() + rhs.value();
    }

    // Op glf + gl<n>
    template <unsigned n>
    inline
     mln_trait_op_plus(graylevel_f, graylevel<n>)
      operator+(const graylevel_f& lhs, const graylevel<n>& rhs)
    {
      return lhs.value() + graylevel_f(rhs).value();
    }

    // Op gl<n> + glf
    template <unsigned n>
    inline
     mln_trait_op_plus(graylevel_f, graylevel<n>)
      operator+(const graylevel<n>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() + graylevel_f(lhs).value();
    }


    // Op glf + Another type
    template <typename I>
    inline
    void
    operator+(const graylevel_f& lhs, const I& i)
    {
      (void) lhs;
      (void) i;
      typename Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }


    // Op  Another type + glf
    template <typename I>
    inline
    void
    operator+(const I& i, const graylevel_f& rhs)
    {
      (void) i;
      (void) rhs;
      typename Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }


    // Op glf - Another type
    template <typename I>
    inline
    void
    operator-(const graylevel_f& lhs, const I& i)
    {
      (void) lhs;
      (void) i;
      typename Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }


    // Op  Another type - glf
    template <typename I>
    inline
    void
    operator-(const I& i, const graylevel_f& rhs)
    {
      (void) i;
      (void) rhs;
      typename Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }

    // Op glf - glf

    inline
    mln_trait_op_minus_(graylevel_f, graylevel_f)
      operator-(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return lhs.value() - rhs.value();
    }

    // Op glf * glf
    inline
    mln_trait_op_times_(graylevel_f, graylevel_f)
      operator*(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return lhs.value() * rhs.value();
    }

    // Op symm glf * Int

    template <typename I>
    inline
    mln_trait_op_times(graylevel_f, I)
      operator*(const graylevel_f& lhs, const Integer<I>& rhs)
    {
      return lhs.value() * int(exact(rhs));
    }

    template <typename I>
    inline
    mln_trait_op_times(I, graylevel_f)
      operator*(const Integer<I>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() * int(exact(lhs));
    }

    // Op symm glf * Float

    template <typename F>
    inline
    mln_trait_op_times(graylevel_f, F)
      operator*(const graylevel_f& lhs, const Floating<F>& rhs)
    {
      return lhs.value() * exact(rhs);
    }

    template <typename F>
    inline
    mln_trait_op_times(F, graylevel_f)
      operator*(const Floating<F>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() * exact(lhs);
    }



    // Op * scalar
    template <typename S>
    inline
    mln_trait_op_times(graylevel_f, S)
      operator*(const graylevel_f& lhs, const scalar_<S>& rhs)
    {
      return lhs.value() * rhs.to_equiv();
    }

    // Op / scalar
    template <typename S>
    inline
    mln_trait_op_div(graylevel_f, S)
      operator/(const graylevel_f& lhs, const scalar_<S>& rhs)
    {
      mln_precondition(rhs.to_equiv() != S(0));
      return lhs.value() / rhs.to_equiv();
    }

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_INTERNAL_GRAY_F_HH
