// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_HSL_HH
# define MLN_VALUE_HSL_HH

#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

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

      // rgb to hsl
      template <typename H, typename S, typename L>
      void
      from_to_(const value::rgb<16>& from, value::hsl_<H,S,L>& to);

      // rgb to hsl
      template <typename H, typename S, typename L>
      void
      from_to_(const value::rgb<8>& from, value::hsl_<H,S,L>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  namespace value
  {

    template <typename E>
    struct HSL
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

  } // end of namespace mln::value


  namespace convert
  {

    namespace over_load
    {

      template <typename H, typename S, typename L>
      void
      from_to_(const value::rgb<16>& from, value::hsl_<H,S,L>& to)
      {
	to = fun::v2v::f_rgb_to_hsl_f(from);
      }


      template <typename H, typename S, typename L>
      void
      from_to_(const value::rgb<8>& from, value::hsl_<H,S,L>& to)
      {
	to = fun::v2v::f_rgb_to_hsl_f(from);
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_VALUE_HSL_HH
