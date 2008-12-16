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

#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_HSL_HH
# define MLN_VALUE_HSL_HH


namespace mln
{

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
      const H& hue() const
      {
	return this->hue_;
      }
      const S& sat() const
      {
	return this->sat_;
      }
      const L& lum() const
      {
	return this->lum_;
      }

      /// Read-write access to the hue component.
      H& hue()
      {
	return this->hue_;
      }
      S& sat()
      {
	return this->sat_;
      }
      L& lum()
      {
	return this->lum_;
      }

    private:
      H hue_;
      S sat_;
      L lum_;
    };

    typedef hsl_<float, float, float> hsl_f;

    typedef hsl_<double, double, double> hsl_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSL_HH
