// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_HSI_HH
# define MLN_VALUE_HSI_HH


namespace mln
{

  namespace value
  {

    template <typename E>
    struct HSI
    {
    };

    template <typename H, typename S, typename I>
    class hsi_ : public HSI< hsi_<H,S,I> >
    {
    public:

      typedef H h_type;
      typedef S s_type;
      typedef I i_type;

      /// Constructor without argument.
      hsi_()
      {
      }

      /// Constructor from component values.
      hsi_(const H& hue, const S& sat, const I& inty)
	: hue_(hue),
	  sat_(sat),
	  int_(inty)
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
      const I& inty() const
      {
	return this->int_;
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
      I& inty()
      {
	return this->int_;
      }

    private:
      H hue_;
      S sat_;
      I int_;
    };

    typedef hsi_<float, float, float> hsi_f;

    typedef hsi_<double, double, double> hsi_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSI_HH
