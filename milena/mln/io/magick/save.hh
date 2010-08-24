// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_MAGICK_SAVE_HH
# define MLN_IO_MAGICK_SAVE_HH

/// \file
///
/// \brief Image output routines based on Magick++.
///
/// Do not forget to call Magick::InitializeMagick(*argv)
/// <em>before</em> using any of these functions, as advised by the
/// GraphicsMagick documentation
/// (http://www.graphicsmagick.org/Magick++/Image.html).

# include <cstdlib>

# include <Magick++.h>

# include <mln/metal/equal.hh>

# include <mln/core/image/image2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/qt/rgb32.hh>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /** Save a Milena image into a file using Magick++.

	  \param[out] ima       The image to save.
	  \param[in]  filename  The name of the output file.  */
      template <typename I>
      void
      save(const Image<I>& ima, const std::string& filename);


      // FIXME: Unfinished?
#if 0
      /** Save a Milena tiled image into a file using Magick++.

	  \param[out] ima       The image to save.
	  \param[in]  filename  The name of the output file.  */
      template <typename T>
      void
      save(const Image< tiled2d<T> >& ima, const std::string& filename);
#endif


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	inline
	Magick::Color get_color(bool value)
	{
	  return Magick::ColorMono(value);
	}

	inline
	Magick::Color get_color(const value::int_u8& value)
	{
	  // Ensure a Magick++'s Quantum is an 8-bit value.
	  mln::metal::equal<Magick::Quantum, unsigned char>::check();
	  return Magick::Color(value, value, value);
	}

	inline
	Magick::Color get_color(const value::rgb8& value)
	{
	  // Ensure a Magick++'s Quantum is an 8-bit value.
	  mln::metal::equal<Magick::Quantum, unsigned char>::check();
	  return Magick::Color(value.red(), value.green(), value.blue());
	}

      } // end of namespace mln::io::magick::impl


      template <typename I>
      inline
      void
      save(const Image<I>& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	mln_precondition(mln_site_(I)::dim == 2);
	// Turn this into a static check?
	if (!(mln::metal::equal<mln_value(I), bool>::value ||
	    mln::metal::equal<mln_value(I), value::int_u8>::value ||
	    mln::metal::equal<mln_value(I), value::rgb8>::value ||
	      mln::metal::equal<mln_value(I), value::qt::rgb32>::value))
	{
	  std::cerr <<
	    "error: trying to save an unsupported format\n"
	    "supported formats are:\n"
	    "  binary (bool)\n"
	    "  8-bit grayscale (mln::value::int_u8)\n"
	    "  3x8-bit truecolor (rgb8)" << std::endl;
	  abort();
	}

	const I& ima = exact(ima_);

	Magick::Image magick_ima;
	// In the construction of a Geometry object, the width (i.e.
	// `ncols') comes first, then the height (i.e. `nrows')
	// follows.
	magick_ima.size(Magick::Geometry(ima.ncols(), ima.nrows()));

	Magick::Pixels view(magick_ima);
	// As above, `ncols' is passed before `nrows'.
	Magick::PixelPacket* pixels = view.get(0, 0, ima.ncols(), ima.nrows());
	mln_piter(I) p(ima.domain());
	for_all(p)
	  *pixels++ = impl::get_color(ima(p));

	view.sync();
	magick_ima.write(filename);

	trace::exiting("mln::io::magick::save");
      }


      // FIXME: Unfinished?
#if 0
      template <typename T>
      void
      save(const Image< tiled2d<T> >& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	tiled2d<T>& ima = exact(ima_);

	ima.buffer().write(filename);

	trace::exiting("mln::io::magick::save");
      }
#endif


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_SAVE_HH
