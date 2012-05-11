// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
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

#ifndef MLN_IO_MAGICK_LOAD_HH
# define MLN_IO_MAGICK_LOAD_HH

/// \file
///
/// \brief Image intput routines based on Magick++.
///
/// Do not forget to call Magick::InitializeMagick(*argv)
/// <em>before</em> using any of these functions, as advised by the
/// GraphicsMagick documentation
/// (http://www.graphicsmagick.org/Magick++/Image.html).

# include <cstdlib>

# include <Magick++.h>

# include <mln/core/image/image2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /** Load data from a file into a Milena image using Magick++.

	  \param[out] ima       The image data are loaded into.
	  \param[in]  filename  The name of the input file.  */
      template <typename I>
      void load(Image<I>& ima, const std::string& filename);


      // FIXME: Unfinished?
#if 0
      /** Load data from a file into a Milena tiled image using
	  Magick++.

	  \param[out] ima       The image data are loaded into.
	  \param[in]  filename  The name of the input file.  */
      template <typename T>
      void load(Image<tiled2d<T> >& ima, const std::string& filename);
#endif


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	inline
	bool
	do_it(const value::rgb8& in, bool& out)
	{
	  if (in.red() != in.green() || in.green() != in.blue())
	    {
	      std::cerr <<
		"error: attempt to load what looks like a color\n"
		"(mln::value::rgb8) image into a Boolean (bool) image" <<
		std::endl;
	      return false;
	    }
	  if (in.red() != 0 &&
	      in.red() != mln_max(value::rgb8::red_t))
	    {
	      std::cerr <<
		"error: attempt to load what looks like a grayscale\n"
		"(mln::value::int_u8) image into a Boolean (bool) image" <<
		std::endl;
	      return false;
	    }

	  out = (in.red() != 0);
	  return true;
	}

	inline
	bool
	do_it(const value::rgb8& in, value::int_u8& out)
	{
	  if (in.red() != in.green() || in.green() != in.blue())
	    {
	      std::cerr <<
		"error: attempt to load what looks like a color\n"
		"(mln::value::rgb8) image into a grayscale\n"
		"(mln::int_u8 values) image" << std::endl;
	      return false;
	    }

	  out = in.red();
	  return true;
	}

	inline
	bool
	do_it(const value::rgb8& in, value::rgb8& out)
	{
	  out = in;
	  return true;
	}

      } // end of namespace mln::io::magick::impl


      template <typename I>
      inline
      void load(Image<I>& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::load");

	I& ima = exact(ima_);

	// FIXME: Handle Magick++'s exceptions (see either
	// ImageMagick++'s or GraphicsMagick++'s documentation).
	Magick::Image magick_ima(filename);
	magick_ima.read(filename);
	magick_ima.type(Magick::TrueColorType);
	int nrows = magick_ima.rows();
	int ncols = magick_ima.columns();
	mln_site(I) pmin(0, 0);
	mln_site(I) pmax(nrows - 1, ncols - 1);

	mln_concrete(I) result(box<mln_site(I)>(pmin, pmax));
	initialize(ima, result);

	def::coord
	  minrow = geom::min_row(ima),
	  mincol = geom::min_col(ima),
	  maxrow = geom::max_row(ima),
	  maxcol = geom::max_col(ima);

	Magick::Pixels view(magick_ima);
	// Note that `ncols' is passed before `nrows'.
	Magick::PixelPacket* pixels = view.get(0, 0, ima.ncols(), ima.nrows());
	mln_value(I) *ptr_ima = &ima(ima.domain().pmin());

	unsigned row_offset = ima.delta_offset(dpoint2d(+1, - ncols));

	for (def::coord row = minrow; row <= maxrow;
	     ++row, ptr_ima += row_offset)
	  for (def::coord col = mincol; col <= maxcol; ++col)
	  {

	    /* Each channel of a Magick++ image is coded on a
	       Magick::Quantum value, which can be an 8-, 16- or 32-bit
	       integer.  Load the most significant bits of each channel
	       into a component of an mln::value::rgb8 value (i.e., into
	       an mln::value::int_u8 value).  */
	    value::rgb8 c(pixels->red   >> 8 * (sizeof(Magick::Quantum)
						- sizeof(value::rgb8::red_t)),
			  pixels->green >> 8 * (sizeof(Magick::Quantum)
						- sizeof(value::rgb8::green_t)),
			  pixels->blue  >> 8 * (sizeof(Magick::Quantum)
						- sizeof(value::rgb8::blue_t)));
	    mln_value(I) res;
	    if (!impl::do_it(c, res))
	    {
	      std::cerr << "while trying to load `" << filename << "'"
			<< std::endl;
	      abort();
	    }
	    *ptr_ima++ = res;
	    ++pixels;
	  }

	trace::exiting("mln::io::magick::load");
      }


      // FIXME: Unfinished?
#if 0
      template<typename T>
      inline
      void
      load(Image<tiled2d<T> >& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::load");

	tiled2d<T>& ima = exact(ima_);

	tiled2d<T> result(filename);

	ima = result;
	trace::exiting("mln::io::magick::load");
      }
#endif


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_LOAD_HH
