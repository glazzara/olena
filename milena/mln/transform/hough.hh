// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_TRANSFORM_HOUGH_HH
# define MLN_TRANSFORM_HOUGH_HH

/// \file
///
/// Compute the hough transform.


# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>

# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/bbox.hh>

# include <mln/opt/at.hh>

# include <mln/math/sin.hh>
# include <mln/math/cos.hh>
# include <mln/math/pi.hh>

# include <mln/make/box2d.hh>

# include <mln/value/int_u8.hh>


namespace mln
{

  namespace transform
  {

    /*! \brief Compute the hough transform from a binary image.

        \param[in] input_ A binary image.

        \return A 2D image of float. Rows are used for the distance
        and columns are used for the angles. Angles go from 0 to 359.
        Distance goes from 0 to the maximum distance between the
        center and a corner.
        The site having the maximum value indicates through its column
        index the document inclination.

        \pre Objects used for computation must be set to 'true'.

	\ingroup mlntransform
    */
    template <typename I>
    image2d<float>
    hough(const Image<I>& input_);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /// FIXME: we may prefer to have an angle value type.
      double to_radians(double angle)
      {
       return angle * math::pi / 180.0f;
      }


    } // end of namespace mln::transform::internal



    template <typename I>
    image2d<float>
    hough(const Image<I>& input_)
    {
      trace::entering("mln::transform::hough");

      const I& input = exact(input_);
      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      def::coord
	minrow = geom::min_row(input),
	mincol = geom::min_col(input);
      unsigned
       ncols = geom::ncols(input),
       nrows = geom::nrows(input);
      int maxRho = (int)(sqrt((ncols * nrows)
			      + (ncols * nrows))
                         + 0.5);

      image2d<float> accu(360, 2*maxRho);
      data::fill(accu, 0.f);

      mln_piter(image2d<int>) p(input.domain());
      for_all(p)
	if (input(p))
	  for (int angle = 0 ; angle < 360 ; ++angle)
	  {
           double
             theta = internal::to_radians(angle),
             rho = (p.row() - minrow) * math::cos(theta)
	     + (p.col() - mincol) * math::sin(theta);
           int
             indexAngle = (int) (angle),
             indexRho = (int)(rho + maxRho + 0.5);

           ++opt::at(accu, indexAngle, indexRho);
          }

      trace::exiting("mln::transform::hough");
      return accu;
    }


# endif // ! MLN_INCLUDE_ONLY

  }  // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_HOUGH_HH
