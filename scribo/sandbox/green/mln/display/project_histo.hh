// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DISPLAY_PROJECT_HISTO_HH
# define MLN_DISPLAY_PROJECT_HISTO_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/image/dmorph/unproject_image.hh>
# include <mln/fun/v2v/projection.hh>

# include <mln/accu/image/init.hh>
# include <mln/accu/image/take.hh>
# include <mln/accu/image/to_result.hh>

# include <mln/algebra/vec.hh>

# include <mln/opt/at.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/label_8.hh>

# include <mln/util/array.hh>

/// \file
///
/// \brief Allow the visualization of 3d histogram.
/// The 3d histogram is projected in 2d such as the data in that direction
/// are accumulated to the two others.

namespace mln
{

  namespace display
  {

    // Forward declaration.
    template <typename A, unsigned direction, typename V>
    image2d<mln_result(A)>
    project_histo(const image3d<V>& histo);

    template <typename A, unsigned n, unsigned direction, typename V>
    image2d<mln_result(A)>
    project2_histo(const image3d<V>&      histo,
		   const value::int_u<n>& ambiguous_color);

    template <unsigned n, unsigned direction, typename V>
    image2d<V>
    project2_histo(const image3d<unsigned>& histo,
		   const image3d<V>&        label);

    template <unsigned n, unsigned direction>
    image2d< value::rgb<n> >
    project3_histo(const image3d<unsigned>& histo,
		   const value::rgb<n>      ambiguous_color);

    template <unsigned n, unsigned direction>
    image2d< value::rgb8 >
    project3_histo(const image3d<unsigned>&                    histo,
		   const image3d<value::label_8>&              label,
		   const util::array<algebra::vec<3, float> >& pal,
		   const value::rgb8                           ambiguous_color);
    // FIXME ==> palette must be 1d-image not an array !!



# ifndef MLN_INCLUDE_ONLY

    /// \brief Allow the visualization of 3d histogram.
    ///
    /// The 3d histogram is projected in 2d such as the data in that direction
    /// are accumulated to the two others.
    ///
    /// Parameter A is the type of accumulator, for instance, accu::math::sum.
    /// Parameter direction is the way of the projection, for instance blue one.
    /// Parameter V is the value we use to accumulate information.
    ///
    /// \prameter[in] the histogram 3d.
    /// \result       the 2d projection of the 3d histogram.

    template <typename A, unsigned direction, typename V>
    image2d<mln_result(A)>
    project_histo(const image3d<V>& histo)
    {
      typedef fun::v2v::projection<point3d,direction> t_projection;

      image2d<A> histo_accu(histo.nrows(), histo.ncols());

      accu::image::init(histo_accu);

      accu::image::take(unproject(histo_accu,
				  histo.domain(),
				  t_projection()).rw(),
			histo);

      return accu::image::to_result(histo_accu);
    }

    // 0 ==> blue
    // 1 ==> red
    // 2 ==> green

    // mln::opt::at(histo, blue, red, green)

    template <unsigned n, unsigned direction>
    image2d< value::int_u<n> >
    project2_histo(const image3d<unsigned>& histo,
		   const value::int_u<n>&   ambiguous_color)
    {
      image2d< value::int_u<n> > result;

      if (0 == direction) // blue
      {
	image2d< value::int_u<n> > arg_max(histo.nrows(), histo.ncols());

	for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned   max = 0; // minimum as possible
	    def::coord pos = -1;

	    for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = blue;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,green) = ambiguous_color;
	    else
	      opt::at(arg_max,red,green) = pos;
	  }

	result = arg_max;
      }
      else if (1 == direction) // red
      {
	image2d< value::int_u<n> > arg_max(histo.ncols(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = red;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,green,blue) = ambiguous_color;
	    else
	      opt::at(arg_max,green,blue) = pos;
	  }

	result = arg_max;
      }
      else // 2 == direction // green
      {
	image2d< value::int_u<n> > arg_max(histo.nrows(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = green;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,blue) = ambiguous_color;
	    else
	      opt::at(arg_max,red,blue) = pos;
	  }

	result = arg_max;
      }

      return result;
    }

    template <unsigned n, unsigned direction>
    image2d<value::label_8>
    project2_histo(const image3d<unsigned>&       histo,
		   const image3d<value::label_8>& label,
		   const value::label_8           ambiguous_label)
    {
      image2d<value::label_8> result;

      if (0 == direction) // blue
      {
	image2d<value::label_8> arg_max(histo.nrows(), histo.ncols());

	for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned   max = 0; // minimum as possible
	    def::coord pos = -1;

	    for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = blue;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,green) = ambiguous_label;
	    else
	      opt::at(arg_max,red,green) = opt::at(label, pos, red, green);
	  }

	result = arg_max;
      }
      else if (1 == direction) // red
      {
	image2d<value::label_8> arg_max(histo.ncols(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = red;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,green,blue) = ambiguous_label;
	    else
	      opt::at(arg_max,green,blue) = opt::at(label, blue, pos, green);
	  }

	result = arg_max;
      }
      else // 2 == direction // green
      {
	image2d<value::label_8> arg_max(histo.nrows(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = green;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,blue) = ambiguous_label;
	    else
	      opt::at(arg_max,red,blue) = opt::at(label, blue, red, pos);
	  }

	result = arg_max;
      }

      return result;
    }




    // FIXME ... determine the color of each class.
    // FIXME la palette est supposée en 8 bits
    template <unsigned n, unsigned direction>
    image2d< value::rgb8 >
    project3_histo(const image3d<unsigned>&                   histo,
		   const image3d<value::label_8>&             label,
		   const util::array<algebra::vec<3,float> >& pal,
		   const value::rgb8                          ambiguous_color)
    {
      image2d< value::rgb8 > result;

      if (0 == direction) // blue
      {
	image2d< value::rgb8 > arg_max(histo.nrows(), histo.ncols());

	for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned   max = 0; // minimum as possible
	    def::coord pos = -1;

	    for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = blue;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,green) = ambiguous_color;
	    else
	    {
	      value::int_u8 r = pal[opt::at(label,pos,red,green)][0];
	      value::int_u8 g = pal[opt::at(label,pos,red,green)][1];
	      value::int_u8 b = pal[opt::at(label,pos,red,green)][2];
	      value::rgb8   color(r,g,b);

	      opt::at(arg_max,red,green) = color;
	    }
	  }

	result = arg_max;
      }
      else if (1 == direction) // red
      {
	image2d< value::rgb8 > arg_max(histo.ncols(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = red;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,green,blue) = ambiguous_color;
	    else
	    {
	      value::int_u8 r = pal[opt::at(label,blue,pos,green)][0];
	      value::int_u8 g = pal[opt::at(label,blue,pos,green)][1];
	      value::int_u8 b = pal[opt::at(label,blue,pos,green)][2];
	      value::rgb8   color(r,g,b);

	      opt::at(arg_max,green,blue) = color;
	    }
	  }

	result = arg_max;
      }
      else // 2 == direction // green
      {
	image2d< value::rgb8 > arg_max(histo.nrows(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = green;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,blue) = ambiguous_color;
	    else
	    {
	      value::int_u8 r = pal[opt::at(label,blue,red,pos)][0];
	      value::int_u8 g = pal[opt::at(label,blue,red,pos)][1];
	      value::int_u8 b = pal[opt::at(label,blue,red,pos)][2];
	      value::rgb8   color(r,g,b);

	      opt::at(arg_max,red,blue) = color;
	    }
	  }

	result = arg_max;
      }

      return result;
    }


    template <unsigned n, unsigned direction>
    image2d< value::rgb<n> >
    project3_histo(const image3d<unsigned>& histo,
		   const value::rgb<n>      ambiguous_color)
    {
      image2d< value::rgb<n> > result;

      if (0 == direction) // blue
      {
	image2d< value::rgb<n> > arg_max(histo.nrows(), histo.ncols());

	for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned   max = 0; // minimum as possible
	    def::coord pos = -1;

	    for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = blue;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,green) = ambiguous_color;
	    else
	      opt::at(arg_max,red,green) = value::rgb<n>(red,green,pos);
	  }

	result = arg_max;
      }
      else if (1 == direction) // red
      {
	image2d< value::rgb<n> > arg_max(histo.ncols(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = red;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,green,blue) = ambiguous_color;
	    else
	      opt::at(arg_max,green,blue) = value::rgb<n>(pos,green,blue);;
	  }

	result = arg_max;
      }
      else // 2 == direction // green
      {
	image2d< value::rgb<n> > arg_max(histo.nrows(), histo.nslices());

	for (def::coord blue = 0; blue < (signed)histo.nslices(); ++blue)
	  for (def::coord red = 0; red < (signed)histo.nrows(); ++red)
	  {
	    unsigned max = 0; // minimum as possible
	    signed   pos = -1;

	    for (def::coord green = 0; green < (signed)histo.ncols(); ++green)
	    {
	      if (max < opt::at(histo,blue,red,green))
	      {
		max = opt::at(histo,blue,red,green);
		pos = green;
	      }
	    }

	    if (-1 == pos)
	      opt::at(arg_max,red,blue) = ambiguous_color;
	    else
	      opt::at(arg_max,red,blue) = value::rgb<n>(red,pos,blue);
	  }

	result = arg_max;
      }

      return result;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_DISPLAY_PROJECT_HISTO_HH
