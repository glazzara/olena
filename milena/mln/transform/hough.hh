// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_TRANSFORM_HOUGH_HH
# define MLN_TRANSFORM_HOUGH_HH

/// \file mln/transform/hough.hh
///
/// Compute the hough transform.


# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>

# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/bbox.hh>

# include <mln/opt/at.hh>

# include <mln/math/sin.hh>
# include <mln/math/cos.hh>
# include <mln/math/pi.hh>

# include <mln/make/box2d.hh>

# include <mln/value/int_u8.hh>


//FIXME: to be removed. For debug purpose.
//#include <mln/data/convert.hh>
//#include <mln/value/rgb8.hh>
//#include <mln/draw/line.hh>
//#include <mln/literal/colors.hh>
//#include <mln/io/ppm/save.hh>


namespace mln
{

  namespace transform
  {

    /// Compute the hough transform from a binary image.
    /// Objects used for computation must be set to 'true'.
    ///
    /// \param[in] input_ A binary image.
    /// \param[in] min_angle Minimum angle which can be found.
    /// \param[in] max_angle Maximum angle which can be found.
    ///
    /// \return A 2D image of float. Rows are used for the distance and
    /// columns are used for the angles. Angles go from 0 to 359.
    /// Distance goes from 0 to the maximum distance between the center and a
    /// corner.
    /// The site having the maximum value indicates through its column index
    /// the document inclination.
    //
    template <typename I>
    image2d<float>
    hough(const Image<I>& input_, int min_angle, int max_angle);


    /// \overload
    template <typename I>
    image2d<float>
    hough(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    image2d<float>
    hough(const Image<I>& input_, int min_angle, int max_angle)
    {
      trace::entering("mln::transform::hough");
      mln_precondition(min_angle < max_angle);

      const I& input = exact(input_);
      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      math::round<int> rd;
      double deg2rad = math::pi / 180.0f;
      int range = rd(sqrt((double)(geom::ncols(input) * geom::ncols(input)
			  + geom::nrows(input) * geom::nrows(input))));

      long temp = min_angle;
      min_angle = 450 - max_angle;
      max_angle = 450 - temp;

      // Pre-compute sin and cos values.
      util::array<double> sin_cache(360),
			  cos_cache(360);
      for (int omega = 0; omega < 360; ++omega)
      {
	sin_cache[omega] = math::sin((double)(omega * deg2rad));
	cos_cache[omega] = math::cos((double)(omega * deg2rad));
      }

      image2d<float> output(make::box2d(range,360));
      data::fill(output, 0);

      mln_piter(I) p(input.domain());
      for_all(p)
	if (input(p)) // Is this site part of an objet?
	{

	  long teta1 = min_angle;
	  long teta2 = max_angle;
	  for (int omega = teta1; omega < teta2; ++omega)
	  {
	    long tetad = omega%360;
	    long r = rd(p.col() * sin_cache[tetad]
		        + p.row() * cos_cache[tetad]);
	    if (r > 0 && r < range)
	      output.at_(r, tetad) += 1;
	  }

	  teta1 = min_angle + 180;
	  teta2 = max_angle + 180;
	  for (int omega = teta1; omega < teta2; ++omega)
	  {
	    long tetad = omega%360;
	    long r = rd(p.col() * sin_cache[tetad]
		        + p.row() * cos_cache[tetad]);
	    if (r > 0 && r < range)
	      output.at_(r, tetad) += 1;
	  }

	}

//      {
//      point2d max_p(0,0);
//      mln_piter_(image2d<float>) p(output.domain());
//      for_all(p)
//	if (output(max_p) < output(p))
//      max_p = p;
//
//      point2d b,e;
//      b.col() = 0;
//      b.row() = max_p.row()/cos(deg2rad*max_p.col());
//      if (b.row() < 0)
//      {
//	b.row() = 0;
//	b.col() = max_p.row()/sin(deg2rad*max_p.col());
//      } else if (b.row() >= input.nrows())
//      {
//	b.row() = input.nrows() - 1;
//	b.col() = max_p.row() - b.row() * cos(deg2rad*max_p.col())/sin(deg2rad*max_p.col());
//      }
//
//      e.col() = input.ncols() - 1;
//      e.row() = max_p.row() - e.col() * sin(deg2rad*max_p.col()) / cos(deg2rad*max_p.col());
//      if (e.row() < 0)
//      {
//	e.row() = 0;
//	e.col() = max_p.row()/sin(deg2rad*max_p.col());
//      } else if (e.row() >= input.nrows())
//      {
//	e.row() = input.nrows() - 1;
//	e.col() = max_p.row() - e.row() * cos(deg2rad*max_p.col())/sin(deg2rad*max_p.col());
//      }
//
//      std::cout << b << " - " << e << std::endl;
//
//      image2d<value::rgb8> toto = data::convert(value::rgb8(), input);
//      draw::line(toto, b, e, literal::red);
//      io::ppm::save(toto, "tmp_input.ppm");
//      }
      trace::exiting("mln::transform::hough");
      return output;
    }


    template <typename I>
    image2d<float>
    hough(const Image<I>& input)
    {
      return hough(input, -180, 180);
    }

# endif // ! MLN_INCLUDE_ONLY

  }  // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_HOUGH_HH

