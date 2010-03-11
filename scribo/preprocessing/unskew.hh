// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PREPROCESSING_UNSKEW_HH
# define SCRIBO_PREPROCESSING_UNSKEW_HH

/// \file
///
/// Unskew an image.

# include <mln/core/image/image2d.hh>

# include <mln/accu/compute.hh>
# include <mln/accu/max_site.hh>

# include <mln/transform/hough.hh>
# include <mln/geom/rotate.hh>

# include <mln/io/pgm/save.hh>
# include <mln/core/image/vmorph/cast_image.hh>
# include <mln/value/int_u8.hh>

# include <mln/util/couple.hh>

namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;

    /// Unskew a document.
    /// Based on the Hough transform.
    ///
    /// \param[in] input_ A binary image. Objects to be unskewed must be set
    ///			  to "true".
    ///
    /// \return A binary image.
    //
    template <typename I>
    mln::util::couple<mln_concrete(I), double>
    unskew(const Image<I>& input_);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln::util::couple<mln_concrete(I), double>
    unskew(const Image<I>& input_)
    {
      trace::entering("scribo::preprocessing::unskew");

      const I& input = exact(input_);
      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      image2d<float> hough_ima = transform::hough(input);

      point2d max_p = accu::compute(accu::max_site<image2d<float> >(), hough_ima);

      double angle = 0;
      int max_angle = max_p.row();

//      std::cout << "max_angle = " << max_angle << std::endl;
      if (max_angle > 180)
	max_angle = - max_angle % 180;

      if (max_angle < 90 && max_angle > 0)
	angle = - max_angle;
      else if (max_angle < 0 && max_angle > -90)
	angle = max_angle;
      else if (max_angle < 180 && max_angle > 90)
	angle = 180 - max_angle;
      else if (max_angle < -90 && max_angle > -180)
	angle = 180 + max_angle;

//      std::cout << "effective angle = " << angle << std::endl;
      mln_concrete(I) output = geom::rotate(input, angle);

      trace::exiting("scribo::preprocessing::unskew");
      return mln::make::couple(output, angle);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::preprocessing

} // end of namespace mln

# endif // SCRIBO_PREPROCESSING_UNSKEW_HH
