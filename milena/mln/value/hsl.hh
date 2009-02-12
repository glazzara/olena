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

/// \file mln/value/hsl.hh
///
/// Color class.
///
/// \todo write a better doc.
/// \todo Cleanup/Revamp!

#ifndef MLN_VALUE_HSL_HH
# define MLN_VALUE_HSL_HH

#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

// Used in from_to
#include <mln/fun/v2v/rgb_to_hsl.hh>


namespace mln
{

  // Forward declarations.
  namespace value
  {

    template <typename H, typename S, typename L>
    class hsl_;

  }



  namespace convert
  {

    namespace over_load
    {

      // rgb to hsl_
      void
      from_to_(const value::rgb<16>& from, value::hsl_<float,float,float>& to);

      // rgb to hsl_
      void
      from_to_(const value::rgb<8>& from, value::hsl_<float,float,float>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  namespace trait
  {

    template <typename H, typename S, typename L>
    struct set_precise_binary_< op::plus, mln::value::hsl_<H,S,L>, mln::value::hsl_<H,S,L> >
    {
      typedef mln::value::hsl_<H,S,L> ret;
    };

    template <typename H, typename S, typename L>
    struct set_precise_binary_< op::minus, mln::value::hsl_<H,S,L>, mln::value::hsl_<H,S,L> >
    {
      typedef mln::value::hsl_<H,S,L> ret;
    };

    template <typename H, typename S, typename L, typename S2>
    struct set_precise_binary_< op::times, mln::value::hsl_<H,S,L>, mln::value::scalar_<S2> >
    {
      typedef mln::value::hsl_<H,S,L> ret;
    };

    template <typename H, typename S, typename L, typename S2>
    struct set_precise_binary_< op::div, mln::value::hsl_<H,S,L>, mln::value::scalar_<S2> >
    {
      typedef mln::value::hsl_<H,S,L> ret;
    };


    // FIXME : Is there any way more generic? a way to factor
    //  set_precise_binary_< op::div, mln::value::hsl_<H,S,L>, mln::value::scalar_<S> >
    //  and
    //  set_precise_binary_< op::div, mln::value::hsl_<H,S,L>, mln::value::int_u<m> >
    //  as for op::times.

    template <typename H, typename S, typename L, unsigned m>
    struct set_precise_binary_< op::times, mln::value::hsl_<H,S,L>, mln::value::int_u<m> >
    {
      typedef mln::value::hsl_<H,S,L> ret;
    };

    template <typename H, typename S, typename L, unsigned m>
    struct set_precise_binary_< op::div, mln::value::hsl_<H,S,L>, mln::value::int_u<m> >
    {
      typedef mln::value::hsl_<H,S,L> ret;
    };

    template <typename H, typename S, typename L>
    struct value_< mln::value::hsl_<H,S,L> >
    {
      enum {
	nbits = (sizeof (H) + sizeof (S) + sizeof (L)) * 8,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef mln_value_quant_from_(card)     quant;

//      typedef algebra::vec<3, float> sum;
      typedef mln::value::hsl_<H,S,L> sum;
    };

  } // end of namespace trait


  namespace value
  {

    template <typename E>
    struct HSL : Object<E>
    {
    };

    template <typename H, typename S, typename L>
    class hsl_ : public HSL< hsl_<H,S,L> >
    {
    public:

      typedef H h_type;
      typedef S s_type;
      typedef L l_type;

      /// Constructor without argument.
      hsl_()
      {
      }

      hsl_(const literal::zero_t&)
	: hue_(0),
	  sat_(0),
	  lum_(0)
      {
      }

      /// Constructor from component values.
      hsl_(const H& hue, const S& sat, const L& lum)
	: hue_(hue),
	  sat_(sat),
	  lum_(lum)
      {
      }

      /// Read-only access to the hue component.
      const H& hue() const;
      const S& sat() const;
      const L& lum() const;

      /// Read-write access to the hue component.
      H& hue();
      S& sat();
      L& lum();

    private:
      //FIXME: Don't we want to store these values in a vector?
      H hue_;
      S sat_;
      L lum_;
    };

    typedef hsl_<float, float, float> hsl_f;

    typedef hsl_<double, double, double> hsl_d;


    /// Print an hsl \p c into the output stream \p ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] c An rgb.
    ///
    /// \return The modified output stream \p ostr.
    template <typename H, typename S, typename L>
    std::ostream& operator<<(std::ostream& ostr, const hsl_<H,S,L>& c);


    /// Addition.
    /// {
    template <typename H, typename S, typename L>
    hsl_<H,S,L>
    operator+(const hsl_<H,S,L>& lhs, const hsl_<H,S,L>& rhs);
    /// \}

    /// Subtraction.
    /// \{
    template <typename H, typename S, typename L>
    hsl_<H,S,L>
    operator-(const hsl_<H,S,L>& lhs, const hsl_<H,S,L>& rhs);
    /// \}

    /// Product.
    /// \{
    template <typename H, typename S, typename L, typename S2>
    hsl_<H,S,L>
    operator*(const hsl_<H,S,L>& lhs, const mln::value::scalar_<S2>& s);
    /// \}

    /// Division.
    /// \{
    template <typename H, typename S, typename L, typename S2>
    hsl_<H,S,L>
    operator/(const hsl_<H,S,L>& lhs, const mln::value::scalar_<S2>& s);
    /// \}

  } // end of namespace mln::value



  // More forward declarations
  namespace fun
  {
    namespace v2v
    {

      template <typename T_hsl>
      struct f_rgb_to_hsl_;

      typedef f_rgb_to_hsl_<value::hsl_f> f_rgb_to_hsl_f_t;

      extern f_rgb_to_hsl_f_t f_rgb_to_hsl_f;

    }

  }

# ifndef MLN_INCLUDE_ONLY


  namespace value
  {

    template <typename H, typename S, typename L>
    const H&
    hsl_<H,S,L>::hue() const
    {
      return this->hue_;
    }

    template <typename H, typename S, typename L>
    const S&
    hsl_<H,S,L>::sat() const
    {
      return this->sat_;
    }

    template <typename H, typename S, typename L>
    const L&
    hsl_<H,S,L>::lum() const
    {
      return this->lum_;
    }

    template <typename H, typename S, typename L>
    H&
    hsl_<H,S,L>::hue()
    {
      return this->hue_;
    }

    template <typename H, typename S, typename L>
    S&
    hsl_<H,S,L>::sat()
    {
      return this->sat_;
    }

    template <typename H, typename S, typename L>
    L&
    hsl_<H,S,L>::lum()
    {
      return this->lum_;
    }


    template <typename H, typename S, typename L>
    inline
    std::ostream& operator<<(std::ostream& ostr, const hsl_<H,S,L>& v)
    {
      return ostr << '(' << debug::format(v.hue())
		  << ',' << debug::format(v.sat())
		  << ',' << debug::format(v.lum())
		  << ')';
    }


    template <typename H, typename S, typename L>
    hsl_<H,S,L>
    operator+(const hsl_<H,S,L>& lhs, const hsl_<H,S,L>& rhs)
    {
      return hsl_<H,S,L>(lhs.hue() + rhs.hue(),
			 lhs.sat() + rhs.sat(),
			 lhs.lum() + rhs.lum());
    }


    template <typename H, typename S, typename L>
    hsl_<H,S,L>
    operator-(const hsl_<H,S,L>& lhs, const hsl_<H,S,L>& rhs)
    {
      return hsl_<H,S,L>(lhs.hue() - rhs.hue(),
			 lhs.sat() - rhs.sat(),
			 lhs.lum() - rhs.lum());
    }


    template <typename H, typename S, typename L, typename S2>
    hsl_<H,S,L>
    operator*(const hsl_<H,S,L>& lhs, const mln::value::scalar_<S2>& s)
    {
      return hsl_<H,S,L>(lhs.hue() * s,
			 lhs.sat() * s,
			 lhs.lum() * s);
    }


    template <typename H, typename S, typename L, typename S2>
    hsl_<H,S,L>
    operator/(const hsl_<H,S,L>& lhs, const mln::value::scalar_<S2>& s)
    {
      return hsl_<H,S,L>(lhs.hue() / s,
			 lhs.sat() / s,
			 lhs.lum() / s);
    }

  } // end of namespace mln::value


  namespace convert
  {

    namespace over_load
    {

      inline
      void
      from_to_(const value::rgb<16>& from, value::hsl_<float,float,float>& to)
      {
	to = fun::v2v::f_rgb_to_hsl_f(from);
      }

      inline
      void
      from_to_(const value::rgb<8>& from, value::hsl_<float,float,float>& to)
      {
	to = fun::v2v::f_rgb_to_hsl_f(from);
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_VALUE_HSL_HH
