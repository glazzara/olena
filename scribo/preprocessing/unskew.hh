// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_UNSKEW_HOUGH_HH
# define SCRIBO_UNSKEW_HOUGH_HH

/// \file scribo/preprocessing/unskew.hh
///
/// Unskew an image.

# include <mln/core/image/image2d.hh>

# include <mln/accu/compute.hh>
# include <mln/accu/max_site.hh>

# include <mln/transform/hough.hh>
# include <mln/transformation/rotate.hh>

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
    mln_concrete(I)
    unskew(const Image<I>& input_);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    unskew(const Image<I>& input_)
    {
      trace::entering("scribo::preprocessing::unskew");

      const I& input = exact(input_);
      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      image2d<float> hough_ima = transform::hough(input);

      point2d max_p = accu::compute(accu::max_site<image2d<float> >(), hough_ima);

      std::cout << max_p.col() << std::endl;
      double angle = 0;
      int max_angle = max_p.col();

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

      mln_concrete(I) output = transformation::rotate(input, angle);

      trace::exiting("scribo::preprocessing::unskew");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::preprocessing

} // end of namespace mln

# endif // SCRIBO_UNSKEW_HOUGH_HH
