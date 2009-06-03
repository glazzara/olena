// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_FUN_X2X_INTERPOL_BILINEAR_HH
# define MLN_FUN_X2X_INTERPOL_BILINEAR_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>

/*! \file mln/fun/x2x/interpol/bilinear.hh
 *
 * \brief Define a bilinear interpolation of values from an underlying image
 */

namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      namespace interpol
      {


        /*! \brief Represent a bilinear interolation of values from an underlying image
         *
         */
        template < typename I >
        struct bilinear
          : public fun::internal::selector_<const algebra::vec<3,float>,
                                            // 3,float is a dummy parameter (real is n,T)
                                            mln_value(I), bilinear<I> >::ret
        {
          typedef mln_value(I) result;

          bilinear(const I& ima);

          template <unsigned n, typename T>
          mln_value(I)
          operator()(const algebra::vec<n,T>& v) const;

          const I& ima;
        };


# ifndef MLN_INCLUDE_ONLY

        template <typename I>
        bilinear<I>::bilinear(const I& ima) : ima(ima)
        {
          mlc_bool(I::psite::dim == 2)::check();
        }

        template <typename I>
        template <unsigned n, typename T>
        mln_value(I)
        bilinear<I>::operator()(const algebra::vec<n,T>& v) const
        {
          typedef mln_sum(mln_value(I)) vsum;

          //  q12----r2----q22
          //   |      |     |
          //   |      x     |
          //   |      |     |
          //  q11----r1----q21

          // looking for img(P(x,y))
          double x = v[0];
          double y = v[1];

          double x1 = std::floor(v[0]);
          double x2 = std::floor(v[0]) + 1;
          double y1 = std::floor(v[1]);
          double y2 = std::floor(v[1]) + 1;

          //if (not ima.has(point2d(x1, y1))) return mln_value(I)();
          vsum q11 = ima(point2d(x1, y1));
          vsum q12 = ima(point2d(x1, y2));
          vsum q21 = ima(point2d(x2, y1));
          vsum q22 = ima(point2d(x2, y2));

          double x2_x1 = x2 - x1;
          double y2_y1 = y2 - y1;

          // linear interpolation #1
          vsum img_r1 = q11 * (x2 - x) / (x2_x1) +
            q21 * (x - x1) / (x2_x1);

          // linear interpolation #2
          vsum img_r2 =  q12 * (x2 - x) / (x2_x1) + q22 * (x - x1) / (x2_x1);

          // interpolating in y direction
          vsum res = (img_r1 * (y2 - y) / (y2_y1)
                      + img_r2 * (y - y1) / (y2_y1));

          return convert::to<mln_value(I)>(res);
        }


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::fun::x2x::interpol

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_INTERPOL_BILINEAR_HH
