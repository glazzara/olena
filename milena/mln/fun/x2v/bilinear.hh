// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_FUN_X2V_BILINEAR_HH
# define MLN_FUN_X2V_BILINEAR_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>
# include <mln/convert/to.hh>
# include <mln/algebra/vec.hh>

/// \file
///
/// Define a bilinear interpolation of values from an underlying image

namespace mln
{

  namespace fun
  {

    namespace x2v
    {

      /// Represent a bilinear interolation of values from an underlying image
      ///
      template < typename I >
      struct bilinear
        : public fun::internal::selector_<const algebra::vec<3,float>,
                                          // 3,float is a dummy parameter (real is n,T)
                                          mln_value(I), bilinear<I> >::ret
      {
        typedef mln_value(I) result;

        bilinear(const I& ima);

	/// Bilinear filtering on 2d images.
        template <typename T>
        mln_value(I)
        operator()(const algebra::vec<2,T>& v) const;

	/// Bilinear filtering on 3d images. Work on slices.
	template <typename T>
        mln_value(I)
        operator()(const algebra::vec<3,T>& v) const;

        const I& ima;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      bilinear<I>::bilinear(const I& ima) : ima(ima)
      {
        mlc_or(mlc_bool(I::psite::dim == 2), mlc_bool(I::psite::dim == 3))::check();
      }

      template <typename I>
      template <typename T>
      mln_value(I)
      bilinear<I>::operator()(const algebra::vec<2,T>& v) const
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

        //Following access are supposed valid.
        vsum q11 = ima(point2d(static_cast<unsigned>(x1), static_cast<unsigned>(y1)));
        vsum q12 = ima(point2d(static_cast<unsigned>(x1), static_cast<unsigned>(y2)));
        vsum q21 = ima(point2d(static_cast<unsigned>(x2), static_cast<unsigned>(y1)));
        vsum q22 = ima(point2d(static_cast<unsigned>(x2), static_cast<unsigned>(y2)));

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


      template <typename I>
      template <typename T>
      mln_value(I)
      bilinear<I>::operator()(const algebra::vec<3,T>& v) const
      {
        typedef mln_sum(mln_value(I)) vsum;

        //  q12----r2----q22
        //   |      |     |
        //   |      x     |
        //   |      |     |
        //  q11----r1----q21

        double x = v[0];
        double y = v[1];

        double x1 = std::floor(x);
        double x2 = std::floor(x) + 1;
        double y1 = std::floor(y);
        double y2 = std::floor(y) + 1;
	def::coord z  = math::round<float>(v[3]);

        //Following access are supposed valid.
        vsum q11 = ima(point3d(z, static_cast<unsigned>(x1), static_cast<unsigned>(y1)));
        vsum q12 = ima(point3d(z, static_cast<unsigned>(x1), static_cast<unsigned>(y2)));
        vsum q21 = ima(point3d(z, static_cast<unsigned>(x2), static_cast<unsigned>(y1)));
        vsum q22 = ima(point3d(z, static_cast<unsigned>(x2), static_cast<unsigned>(y2)));

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

    } // end of namespace mln::fun::x2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2V_BILINEAR_HH
