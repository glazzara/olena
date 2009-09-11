// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_X2V_SAMPLER_HH
# define MLN_FUN_X2V_SAMPLER_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>
# include <mln/convert/to.hh>
# include <mln/algebra/vec.hh>

/// \file
///
/// Define a sampling process to attach real coordonnates to discrete ones.

namespace mln
{

  namespace fun
  {

    namespace x2v
    {

      /// Define a sampling process to attach real coordonnates to a discrete
      /// grid.

      // min | | | | | | | | max
      //
      // h = (max-min)/q (division reelle)
      // i = (val-min)/h (division entiere)
      //
      //
      // Hue [0,360°], q = 8 bits ==> h = 45°  , 0/45/90/135/180/225/270/315/360
      // Sat [0,1]   , q = 8 bits ==> h = 0.125
      // Lum [0,1]   , q = 8 bits ==> h = 0.125

      template < typename I >
      struct sampler : public Function_v2v< sampler<I> >
      {
        typedef mln_value(I) result;

        sampler(const I& ima);

	/// Bilinear filtering on 2d images.
        template <typename T>
        mln_value(I)
        operator()(const algebra::vec<2,T>& v) const;

	template <typename T>
        mln_value(I)
        operator()(const algebra::vec<3,T>& v) const;

        const I& ima;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      bilinear<I>::bilinear(const I& ima) : ima(ima)
      {
        mlc_bool(I::psite::dim == 3)::check();
      }

      template <typename I>
      template <typename T>
      mln_value(I)
      sampler<I>::operator()(const algebra::vec<3,T>& v) const
      {

        return convert::to<mln_value(I)>(res);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2V_SAMPLER_HH
