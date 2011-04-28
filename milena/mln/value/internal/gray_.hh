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

#ifndef MLN_VALUE_INTERNAL_GRAY__HH
# define MLN_VALUE_INTERNAL_GRAY__HH

/// \file
///
/// FIXME.

# include <iostream>
# include <cmath>

# include <mln/metal/math/max.hh>
# include <mln/metal/math/pow.hh>

# include <mln/value/concept/integer.hh>
# include <mln/value/internal/value_like.hh>

# include <mln/value/graylevel_f.hh>

namespace mln
{

  namespace literal
  {
    /// \{ Forward declarations.
    struct black_t;
    struct white_t;
    /// \}
  }

  namespace value
  {
    /// \{ Forward declarations.
    template <unsigned n>
    struct graylevel;
    template <unsigned N> struct graylevel;
    struct graylevel_f;
    namespace internal
    {
      template <unsigned n> class gray_;
      struct gray_f;
    }
    /// \}
  }


  namespace trait
  {

    template < template <class, class> class Name, unsigned n, unsigned m>
    struct set_precise_binary_< Name, mln::value::internal::gray_<n>, mln::value::internal::gray_<m> >
    {
      typedef mln::value::internal::gray_< mlc_max_int(n, m) > ret;
    };

    template <unsigned n, unsigned m>
    struct set_precise_binary_< op::greater, mln::value::internal::gray_<n>, mln::value::internal::gray_<m> >
    {
      typedef bool ret;
    };

    template <unsigned n, unsigned m>
    struct set_precise_binary_< op::eq, mln::value::internal::gray_<n>, mln::value::internal::gray_<m> >
    {
      typedef bool ret;
    };

    // 'gray_' as a value.

    template <unsigned n>
    struct value_< mln::value::internal::gray_<n> >
    {
    private:
      typedef mln::value::internal::gray_<n> self_;
    public:

      enum {
	nbits = 8 * (sizeof(unsigned) + sizeof(long)),
	card  = 0
      };

      typedef trait::value::nature::integer nature; // FIXME: Or scalar?.
      typedef trait::value::kind::gray      kind;
      typedef trait::value::quant::high     quant;

      static self_ min();
      static self_ max();
      static self_ epsilon();

      typedef self_ sum; // FIXME: OK?.
    };

  } // end of namespace mln::trait



  namespace value
  {

    namespace internal
    {

      /// General gray-level class where n bits is not know at compile-time.
      /// This class is used for exchange between gray-level types purpose.
      template <unsigned n>
      class gray_
	:
	public Integer< gray_<n> >,

	public internal::value_like_< int,          // Equivalent.
				      int,          // Encoding.
				      gray_<n>,     // Interoperation.
				      gray_<n> >    // Exact.

      {
      public:

	/// Encoding associated type.
	typedef int enc;

	/// Equivalent associated type.
	typedef int equiv;

	/// Constructor without argument.
	gray_();
	/// Constructor with int.
	gray_(int val);

	/// \{ Constructors/assignments with literals.
	explicit gray_(const literal::white_t&);
	explicit gray_(const literal::black_t&);
	/// \}


	/// \{ Constructors/assigments with graylevel.
	template <unsigned m>
	gray_(const graylevel<m>& val);
	template <unsigned m>
	gray_<n>& operator=(const graylevel<m>& val);
	/// \}

	/// Access to std type.
	int value() const;

	/// Return an equivalent gray_ encoded on \p nbits bits.
	template <unsigned m>
	operator gray_<m>() const;

	/// Conversion to graylevel.
	template <unsigned m>
	operator graylevel<m>() const;

	/// Conversion to graylevel_f.
	operator graylevel_f() const;

      };


    // Operators.

      template <unsigned n>
      std::ostream& operator<<(std::ostream& ostr, const gray_<n>& g);

      template <unsigned n, unsigned m>
      bool operator==(const gray_<n>& lhs, const gray_<m>& rhs);
      template <unsigned n, unsigned m>
      bool operator<(const gray_<n>& lhs, const gray_<m>& rhs);

      template <unsigned n, unsigned m>
      mln_trait_op_plus(gray_<n>, gray_<m>)
      operator+(const gray_<n>& lhs, const gray_<m>& rhs);

      template <unsigned n, unsigned m>
      mln_trait_op_minus(gray_<n>, gray_<m>)
      operator-(const gray_<n>& lhs, const gray_<m>& rhs);

      template <unsigned n>
      gray_<n> operator*(int s, const gray_<n>& rhs);
      template <unsigned n>
      gray_<n> operator*(const gray_<n>& lhs, int s);
      template <unsigned n>
      gray_<n> operator/(const gray_<n>& lhs, int s);

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


# include <mln/value/internal/gray_f.hh>


namespace mln
{

# ifndef MLN_INCLUDE_ONLY

  namespace value
  {
    namespace internal
    {

      template <unsigned n_src, unsigned n_dest>
      inline
      long convert(int val)
      {
	using mln::metal::int_;
 	typedef mlc_max(int_<n_dest - n_src> , int_<n_src - n_dest>) m;

	if (n_dest == n_src)
	  return val;
	else
	  if (n_dest > n_src)
	    return val * mlc_pow_int(2, m::value);
	  else
	    return val / mlc_pow_int(2, m::value);
      }

      // Gray_<N>.

      template <unsigned n>
      inline
      gray_<n>::gray_()
      {
      }

      template <unsigned n>
      template <unsigned m>
      gray_<n>::gray_(const graylevel<m>& g)
      {
	this->v_ = convert<m, n>(g.value());
      }

      template <unsigned n>
      template <unsigned m>
      inline
      gray_<n>&
      gray_<n>::operator=(const graylevel<m>& g)
      {
	this->v_ = convert<m, n>(g.value());
	return *this;
      }

      template <unsigned n>
      inline
      gray_<n>::gray_(const literal::white_t&)
      {
	this->v_ = mlc_pow_int(2, n) - 1;
      }

      template <unsigned n>
      inline
      gray_<n>::gray_(const literal::black_t&)
      {
	this->v_ = 0;
      }

      template <unsigned n>
      inline
      gray_<n>::gray_(int val)
      {
	this->v_ = val;
      }

      template <unsigned n>
      inline
      int
      gray_<n>::value() const
      {
	return this->v_;
      }

      template <unsigned n>
      template <unsigned m>
      inline
      gray_<n>::operator gray_<m>() const
      {
	return gray_<m>(convert<n, m>(this->v_));
      }


      template <unsigned n>
      template <unsigned m>
      inline
      gray_<n>::operator graylevel<m>() const
      {
	graylevel<m> tmp(convert<n, m>(this->v_));
	mln_assertion(tmp.value() < std::pow(2.f, int(m)));
	return tmp;
      }


      template <unsigned n>
      inline
      gray_<n>::operator graylevel_f() const
      {
	static const float denom = float(metal::math::pow_int<2, n>::value) - 1.f;
	return graylevel_f(float(this->v_) / denom);
      }

      // Operators.

      template <unsigned n>
      inline
      std::ostream& operator<<(std::ostream& ostr, const gray_<n>& g)
      {
	return ostr << g.value() << "g/" << n << "bits";
      }

      template <unsigned n, unsigned m>
      inline
      bool operator==(const gray_<n>& lhs, const gray_<m>& rhs)
      {
	if (n == m)
	  return internal::gray_<mlc_max_int(n, m)>(lhs).value() ==
	    internal::gray_<mlc_max_int(n, m)>(rhs).value();
	if (n > m)
	{
	  gray_<n> down = rhs;
	  gray_<n> up(down.value() +
		      mlc_pow_int(2, m));
	  return (lhs.value() >= down.value() && lhs.value() < up.value());
	}
	else
	{
	  gray_<m> down = lhs;
	  gray_<m> up(down.value() +
		      mlc_pow_int(2, n));
	  return (rhs.value() >= down.value() && rhs.value() < up.value());
	}
      }

      template <unsigned n, unsigned m>
      inline
      bool operator<(const gray_<n>& lhs, const gray_<m>& rhs)
      {
	if (m == n)
	  return lhs.value() < rhs.value();
	if (n > m)
	  return lhs.value() < gray_<n>(rhs).value();
	else
	  return gray_<m>(lhs).value() < rhs.value();
      }

      template <unsigned n, unsigned m>
      inline
      mln_trait_op_plus(gray_<n>, gray_<m>)
	operator+(const gray_<n>& lhs, const gray_<m>& rhs)
      {
	typedef mln_trait_op_plus(gray_<n>, gray_<m>) ret;
	if (n > m)
	{
	  ret tmp(ret(lhs).value() + ret(rhs).value());
	  return tmp;
	}
	else
	{
	  ret tmp(ret(lhs).value() + ret(rhs).value());
	  return tmp;
	}
      }

      template <unsigned n, unsigned m>
      inline
      mln_trait_op_minus(gray_<n>, gray_<m>)
	operator-(const gray_<n>& lhs, const gray_<m>& rhs)
      {
	typedef mln_trait_op_minus(gray_<n>, gray_<m>) ret;


	if (lhs == rhs)
	  return ret(0);
	if (n == m)
	  return ret(lhs.value() - rhs.value());
	if (n > m)
	{
	  ret l = rhs;
	  mln_assertion(lhs.value() >= l.value());
	  ret tmp(lhs.value() - l.value());
	  return tmp;
	}
	else
	{
	  ret l = lhs;
	  mln_assertion(l.value() >= rhs.value());
	  ret tmp(l.value() - rhs.value());
	  return tmp;
	}
      }

      template <unsigned n, unsigned m>
      inline
      mln_trait_op_times(gray_<n>, gray_<m>)
	operator*(const gray_<n>& lhs, const gray_<m>& rhs)
      {
	typedef mln_trait_op_times(gray_<n>, gray_<m>) ret;

	return ret(ret(lhs).value() * ret(rhs).value());
      }

      template <unsigned n>
      inline
      gray_<n> operator*(int s, const gray_<n>& rhs)
      {
	gray_<n> tmp(rhs.value() * s);
	return tmp;
      }

      template <unsigned n>
      inline
      gray_<n> operator*(const gray_<n>& lhs, int s)
      {
	gray_<n> tmp(lhs.value() * s);
	return tmp;
      }

      template <unsigned n>
      inline
      gray_<n> operator/(const gray_<n>& lhs, int s)
      {
	mln_precondition(s != 0);
	gray_<n> tmp(lhs.value() / s);
	return tmp;
      }

    } // end of namespace mln::value::internal

    // Graylevel operators.

    // Op gl == Int.

    template <typename I, unsigned n>
    inline
    bool
    operator==(const Integer<I>& lhs, const graylevel<n>& rhs)
    {
      return rhs.value() == exact(lhs);
    }

    // Op gl == gl.

    template <unsigned n, unsigned m>
    inline
    bool
    operator==(const graylevel<n>& lhs, const graylevel<m>& rhs)
    {
      return internal::gray_<n>(rhs) == internal::gray_<m>(lhs);
    }

    // Op gl + gl.

    template <unsigned n, unsigned m>
    inline
    mln_trait_op_plus(graylevel<n>, graylevel<m>)
      operator+(const graylevel<n>& lhs, const graylevel<m>& rhs)
    {
      return internal::gray_<n>(lhs) + internal::gray_<m>(rhs);
    }

    // Op gl + Another type.
    template <unsigned n, typename I>
    inline
    void
    operator+(const graylevel<n>& lhs, const I& i)
    {
      (void) lhs;
      (void) i;
      typename graylevel<n>::wrong_use_of_graylevel___Please_use_the__to_enc__method a;
    }


    // Op  Another type + gl.
    template <unsigned n, typename I>
    inline
    void
    operator+(const I& i, const graylevel<n>& rhs)
    {
      (void) i;
      (void) rhs;
      typename graylevel<n>::wrong_use_of_graylevel___Please_use_the__to_enc__method a;
    }

    // Op gl - gl.
    template <unsigned n, unsigned m>
    inline
    mln_trait_op_minus(graylevel<n>, graylevel<m>)
      operator-(const graylevel<n>& lhs, const graylevel<m>& rhs)
    {
      return internal::gray_<n>(lhs) - internal::gray_<m>(rhs);
    }

    // Op gl - Another type.
    template <unsigned n, typename I>
    inline
    void
    operator-(const graylevel<n>& lhs, const I& i)
    {
      (void) lhs;
      (void) i;
      typename graylevel<n>::wrong_use_of_graylevel___Please_use_the__to_enc__method a;
    }


    // Op  Another type - gl.
    template <unsigned n, typename I>
    inline
    void
    operator-(const I& i, const graylevel<n>& rhs)
    {
      (void) i;
      (void) rhs;
      typename graylevel<n>::wrong_use_of_graylevel___Please_use_the__to_enc__method a;
    }

    // Op gl * gl.
    template <unsigned n, unsigned m>
    inline
    mln_trait_op_times(graylevel<n>, graylevel<m>)
      operator*(const graylevel<n>& lhs, const graylevel<m>& rhs)
    {
      return internal::gray_<n>(lhs) * internal::gray_<m>(rhs);
    }

    // Op symm gl * Integer.
    template <unsigned n, typename I>
    inline
    internal::gray_<n>
      operator*(const graylevel<n>& lhs, const Integer<I>& rhs)
    {
      return internal::gray_<n>(lhs) * int(exact(rhs));
    }

    template <typename I, unsigned n>
    inline
    mln_trait_op_times(I, graylevel<n>)
      operator*(const Integer<I>& lhs, const graylevel<n>& rhs)
    {
      return internal::gray_<n>(rhs) * int(exact(lhs));
    }

    // Op symm gl * Floating.
    template <unsigned n, typename F>
    inline
    mln_trait_op_times(graylevel<n>, F)
      operator*(const graylevel<n>& lhs, const Floating<F>& rhs)
    {
      return lhs.to_float() * float(exact(rhs));
    }

    template <typename F, unsigned n>
    inline
    mln_trait_op_times(F, graylevel<n>)
      operator*(const Floating<F>& lhs, const graylevel<n>& rhs)
    {
      return rhs.to_float() * float(exact(lhs));
    }

    // Op * Builtin.
    template <unsigned n, typename T>
    mln_trait_op_times(graylevel<n>, T)
      operator*(const graylevel<n>& lhs, const T& rhs)
    {
      return lhs * scalar_<T>(rhs);
    }

    template <unsigned n, typename T>
    mln_trait_op_times(graylevel<n>, T)
      operator*(const T& lhs, const graylevel<n>& rhs)
    {
      return rhs * scalar_<T>(lhs);
    }


    // Op / Builtin.
    /// \{ Forward declarations.
    namespace internal
    {
      struct gray_f;
    }
    /// \}

    template <unsigned n, typename T>
    mln::value::internal::gray_f
    operator/(const graylevel<n>& lhs, const T& rhs)
    {
      return lhs / scalar_<T>(rhs);
    }


    // Op * scalar.
    namespace internal
    {

      template <typename ret>
      struct helper_gray__op_;

      template <unsigned n>
      struct helper_gray__op_< gray_<n> >
      {
	template <unsigned m, typename S>
	inline static
	mln_trait_op_times(graylevel<m>, scalar_<S>)
	times(const graylevel<m>& lhs, const scalar_<S>& rhs)
	{
	  typedef mln_trait_op_times(graylevel<m>, scalar_<S>) ret;
	  ret tmp(typename ret::equiv
		  (lhs.value() * typename ret::equiv(rhs.to_equiv())));
	  return tmp;
	}

	template <unsigned m, typename S>
	inline static
	mln_trait_op_times(graylevel<m>, scalar_<S>)
	div(const graylevel<m>& lhs, const scalar_<S>& rhs)
	{
	  typedef mln_trait_op_times(graylevel<m>, scalar_<S>) ret;
	  ret tmp(typename ret::equiv
		  (lhs.value() / typename ret::equiv(rhs.to_equiv())));
	  return tmp;
	}
      };

      template <>
      struct helper_gray__op_< gray_f >
      {
	template <unsigned n, typename S>
	inline static
	mln_trait_op_times(graylevel<n>, scalar_<S>)
	  times(const graylevel<n>& lhs, const scalar_<S>& rhs)
	{
	  typedef mln_trait_op_times(graylevel<n>, scalar_<S>) ret;
	  ret tmp(lhs.to_float() * typename ret::equiv(rhs.to_equiv()));
	  return tmp;
	}
	template <unsigned n, typename S>
	inline static
	mln_trait_op_div(graylevel<n>, scalar_<S>)
	  div(const graylevel<n>& lhs, const scalar_<S>& rhs)
	{
	  typedef mln_trait_op_div(graylevel<n>, scalar_<S>) ret;
	  ret tmp(typename ret::equiv
		  (lhs.to_float() / typename ret::equiv(rhs.to_equiv())));
	  return tmp;
	}
      };

    } // end of namespace mln::value::internal

    // Op graylevel<n> * scalar_<S>.
    template <unsigned n, typename S>
    inline
    mln_trait_op_times(graylevel<n>, scalar_<S>)
      operator*(const graylevel<n>& lhs, const scalar_<S>& rhs)
    {
      typedef mln_trait_op_times(graylevel<n>, scalar_<S>) ret;
      return internal::helper_gray__op_<ret>::times(lhs, rhs);
    }

    // Op graylevel<n> / scalar_<S>.
    template <unsigned n, typename S>
    inline
    mln_trait_op_div(graylevel<n>, scalar_<S>)
      operator/(const graylevel<n>& lhs, const scalar_<S>& rhs)
    {
      mln_precondition(rhs.to_equiv() != S(0));
      typedef mln_trait_op_div(graylevel<n>, scalar_<S>) ret;
      return internal::helper_gray__op_<ret>::div(lhs, rhs);
    }

  } //end of namespace mln::value



namespace trait
{

  // 'gray_<n>' as a value.

  template <unsigned n>
  inline
  mln::value::internal::gray_<n>
  value_< mln::value::internal::gray_<n> >::min()
  {
    return mln::value::internal::gray_<n>(1, 0);
  }

  template <unsigned n>
  inline
  mln::value::internal::gray_<n>
  value_< mln::value::internal::gray_<n> >::max()
  {
    return mln::value::internal::gray_<n>(1, 1);
  }

  template <unsigned n>
  inline
  mln::value::internal::gray_<n>
  value_< mln::value::internal::gray_<n> >::epsilon()
  {
    return mln::value::internal::gray_<n>(1, 0); // Means '0'.
  }

} // end of namespace mln::trait


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_VALUE_INTERNAL_GRAY__HH
