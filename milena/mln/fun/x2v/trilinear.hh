// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_X2V_TRILINEAR_HH
# define MLN_FUN_X2V_TRILINEAR_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>
# include <mln/convert/to.hh>
# include <mln/algebra/vec.hh>

/// \file
///
/// Define a trilinear interpolation of values from an underlying image
///
/// Source: http://en.wikipedia.org/wiki/Rotation_matrix

namespace mln
{

  namespace fun
  {

    namespace x2v
    {

      /// Represent a trilinear interolation of values from an underlying image
      ///
      template < typename I >
      struct trilinear
        : public fun::internal::selector_<const algebra::vec<3,float>,
                                          // 3,float is a dummy parameter (real is n,T)
                                          mln_value(I), trilinear<I> >::ret
      {
        typedef mln_value(I) result;

        trilinear(const I& ima);

	template <typename T>
        mln_value(I)
        operator()(const algebra::vec<3,T>& v) const;

        const I& ima;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      trilinear<I>::trilinear(const I& ima) : ima(ima)
      {
        mlc_bool(I::psite::dim == 3)::check();
      }


      template <typename I>
      template <typename T>
      mln_value(I)
      trilinear<I>::operator()(const algebra::vec<3,T>& v) const
      {
        typedef mln_sum(mln_value(I)) vsum;

        double x = v[0]; // row
        double y = v[1]; // col
        double z = v[2]; // sli

        unsigned x1 = math::round<double>(std::floor(x));
        unsigned x2 = math::round<double>(std::floor(x) + 1);
        unsigned y1 = math::round<double>(std::floor(y));
        unsigned y2 = math::round<double>(std::floor(y) + 1);
        unsigned z1 = math::round<double>(std::floor(z));
        unsigned z2 = math::round<double>(std::floor(z) + 1);

	double xd = x - x1;
        double yd = y - y1;
	double zd = z - z1;

        // interpolating in z direction
        // Following access are supposed valid.
        vsum i1 = ima(point3d(z1,x1,y1)) * (1 - zd)
		+ ima(point3d(z2,x1,y1)) * zd;

	vsum i2 = ima(point3d(z1,x1,y2)) * (1 - zd)
		+ ima(point3d(z2,x1,y2)) * zd;

        vsum j1 = ima(point3d(z1,x2,y1)) * (1 - zd)
		+ ima(point3d(z2,x2,y1)) * zd;

        vsum j2 = ima(point3d(z1,x2,y2)) * (1 - zd)
		+ ima(point3d(z2,x2,y2)) * zd;

        // interpolating in y direction
	vsum w1 = i1 * (1 - yd) + i2 * yd;
	vsum w2 = j1 * (1 - yd) + j2 * yd;

        // interpolating in x direction
	vsum res = w1 * (1 - xd) + w2 * xd;

        return convert::to<mln_value(I)>(res);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2V_TRILINEAR_HH
