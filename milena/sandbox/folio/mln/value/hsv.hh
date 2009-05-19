// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file mln/value/hsv.hh
///
/// Color class.
///
/// \todo Write a better doc.
/// \todo Cleanup/Revamp!
/// \todo Need for more preconditions?

#ifndef MLN_VALUE_HSV_HH
# define MLN_VALUE_HSV_HH

#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include "circular.hh"

// Used in from_to
#include "../fun/v2v/rgb_to_hsv.hh"


namespace mln
{

  // Forward declarations.
  namespace value
  {

    template <typename H, typename S, typename V>
    class hsv_;

    typedef hsv_< circular<8, 0, 360>, float01_<8>, float01_<8> > hsv_8f;
    typedef hsv_< circular<16, 0, 360>, float01_<16>, float01_<16> > hsv_16f;

  }



  namespace convert
  {

    namespace over_load
    {

      template <int n>
      void
      from_to_(const value::rgb<n>& from,
	       value::hsv_<value::circular<n, 0, 360>, float01_<n>, float01_<n> >& to)

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  namespace trait
  {

    template <typename H, typename S, typename V>
    struct set_precise_binary_< op::plus,
				mln::value::hsv_<H, S, V>,
				mln::value::hsv_<H, S, V> >
    {
      typedef mln::value::hsv_<H, S, V> ret;
    };

    template <typename H, typename S, typename V>
    struct set_precise_binary_< op::minus,
				mln::value::hsv_<H, S, V>,
				mln::value::hsv_<H, S, V> >
    {
      typedef mln::value::hsv_<H, S, V> ret;
    };

    template <typename H, typename S, typename V, typename S2>
    struct set_precise_binary_< op::times,
				mln::value::hsv_<H, S, V>,
				mln::value::scalar_<S2> >
    {
      typedef mln::value::hsv_<H, S, V> ret;
    };

    template <typename H, typename S, typename V, typename S2>
    struct set_precise_binary_< op::div,
				mln::value::hsv_<H, S, V>,
				mln::value::scalar_<S2> >
    {
      typedef mln::value::hsv_<H, S, V> ret;
    };


    // FIXME : Is there any way more generic? a way to factor
    //  set_precise_binary_< op::div,
    //                       mln::value::hsv_<H, S, V>,
    //                       mln::value::scalar_<S> >
    //  and
    //  set_precise_binary_< op::div,
    //                       mln::value::hsv_<H, S, V>,
    //                       mln::value::int_u<m> >
    //  as for op::times.

    template <typename H, typename S, typename V, unsigned m>
    struct set_precise_binary_< op::times,
				mln::value::hsv_<H, S, V>,
				mln::value::int_u<m> >
    {
      typedef mln::value::hsv_<H, S, V> ret;
    };

    template <typename H, typename S, typename V, unsigned m>
    struct set_precise_binary_< op::div,
				mln::value::hsv_<H, S, V>,
				mln::value::int_u<m> >
    {
      typedef mln::value::hsv_<H, S, V> ret;
    };


    template <typename H, typename S, typename V>
    struct value_< mln::value::hsv_<H, S, V> >
    {
      enum {
	dim = 3,
	nbits = (sizeof (H) + sizeof (S) + sizeof (V)) * 8,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef mln_value_quant_from_(card)     quant;

      typedef H comp_0;
      typedef S comp_1;
      typedef V comp_2;

      typedef mln::value::hsv_<H, S, V> sum;
    };

  } // end of namespace trait


  namespace value
  {

    // ?
    template <typename E>
    struct HSV : Object<E>
    {
    };


    template <typename H, typename S, typename V>
    class hsv_ : public HSV< hsv_<H, S, V> >
    {
    public:

      /// Constructor without argument.
      hsv_()
      {
      }

      hsv_(const literal::zero_t&)
	: hue_(0),
	  sat_(0),
	  val_(0)
      {
      }

      /// Constructor from component values.
      hsv_(const H& hue, const S& sat, const V& lum)
      {
	mln_precondition(sat >= 0);
	mln_precondition(val >= 0);
	mln_precondition(sat <= 1.);
	mln_precondition(val <= 1.);
	hue_ = hue;
	sat_ = sat;
	val_ = val;
      }

      /// Read-only access to the hue component.
      const H& hue() const;
      const S& sat() const;
      const V& val() const;

      /// Read-write access to the hue component.
      H& hue();
      S& sat();
      V& val();

    private:
      H hue_;
      S sat_;
      V val_;
    };

    /// Print an hsv \p c into the output stream \p ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] c An rgb.
    ///
    /// \return The modified output stream \p ostr.
    template <typename H, typename S, typename V>
    std::ostream& operator<<(std::ostream& ostr, const hsv_<H, S, V>& c);


    /// Addition.
    /// {
    template <typename H, typename S, typename V>
    hsv_<H, S, V>
    operator+(const hsv_<H, S, V>& lhs, const hsv_<H, S, V>& rhs);
    /// \}

    /// Subtraction.
    /// \{
    template <typename H, typename S, typename V>
    hsv_<H, S, V>
    operator-(const hsv_<H, S, V>& lhs, const hsv_<H, S, V>& rhs);
    /// \}

    /// Product.
    /// \{
    template <typename H, typename S, typename V, typename S2>
    hsv_<H, S, V>
    operator*(const hsv_<H, S, V>& lhs, const mln::value::scalar_<S2>& s);
    /// \}

    /// Division.
    /// \{
    template <typename H, typename S, typename V, typename S2>
    hsv_<H, S, V>
    operator/(const hsv_<H, S, V>& lhs, const mln::value::scalar_<S2>& s);
    /// \}

  } // end of namespace mln::value



  // More forward declarations
  namespace fun
  {
    namespace v2v
    {

      template <typename T_hsv>
      struct f_rgb_to_hsv_;

//      typedef f_rgb_to_hsv_<value::hsv_f> f_rgb_to_hsv_f_t;
//      extern f_rgb_to_hsv_f_t f_rgb_to_hsv_f;

    }

  }

# ifndef MLN_INCLUDE_ONLY


  namespace value
  {

    template <typename H, typename S, typename V>
    const H&
    hsv_<H, S, V>::hue() const
    {
      return this->hue_;
    }

    template <typename H, typename S, typename V>
    const S&
    hsv_<H, S, V>::sat() const
    {
      return this->sat_;
    }

    template <typename H, typename S, typename V>
    const V&
    hsv_<H, S, V>::val() const
    {
      return this->val_;
    }

    template <typename H, typename S, typename V>
    H&
    hsv_<H, S, V>::hue()
    {
      return this->hue_;
    }

    template <typename H, typename S, typename V>
    S&
    hsv_<H, S, V>::sat()
    {
      return this->sat_;
    }

    template <typename H, typename S, typename V>
    V&
    hsv_<H, S, V>::val()
    {
      return this->val_;
    }


    template <typename H, typename S, typename V>
    inline
    std::ostream& operator<<(std::ostream& ostr, const hsv_<H, S, V>& v)
    {
      return ostr << '(' << debug::format(v.hue())
		  << ',' << debug::format(v.sat())
		  << ',' << debug::format(v.val())
		  << ')';
    }


    template <typename H, typename S, typename V>
    hsv_<H, S, V>
    operator+(const hsv_<H, S, V>& lhs, const hsv_<H, S, V>& rhs)
    {
      return hsv_<H, S, V>(lhs.hue() + rhs.hue(),
			   lhs.sat() + rhs.sat(),
			   lhs.val() + rhs.val());
    }


    template <typename H, typename S, typename V>
    hsv_<H, S, V>
    operator-(const hsv_<H, S, V>& lhs, const hsv_<H, S, V>& rhs)
    {
      return hsv_<H, S, V>(lhs.hue() - rhs.hue(),
			   lhs.sat() - rhs.sat(),
			   lhs.val() - rhs.val());
    }


    template <typename H, typename S, typename V, typename S2>
    hsv_<H, S, V>
    operator*(const hsv_<H, S, V>& lhs, const mln::value::scalar_<S2>& s)
    {
      return hsv_<H, S, V>(lhs.hue() * s,
			   lhs.sat() * s,
			   lhs.val() * s);
    }


    template <typename H, typename S, typename V, typename S2>
    hsv_<H, S, V>
    operator/(const hsv_<H, S, V>& lhs, const mln::value::scalar_<S2>& s)
    {
      return hsv_<H, S, V>(lhs.hue() / s,
			   lhs.sat() / s,
			   lhs.val() / s);
    }

  } // end of namespace mln::value


  namespace convert
  {

    namespace over_load
    {

      template <int n>
      inline
      void
      from_to_(const value::rgb<n>& from,
	       value::hsv_<value::circular<n, 0, 360>, float01_<n>, float01_<n> >& to)
      {
	to = fun::v2v::f_rgb_to_hsv_<
	value::hsv_<value::circular<n, 0, 360>, float01_<n>, float01_<n> > >(from);
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_VALUE_HSV_HH
