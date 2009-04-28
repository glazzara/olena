// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_MAGICK_SAVE_HH
# define MLN_IO_MAGICK_SAVE_HH

///
/// \file   mln/io/magick/save.hh
///
/// Define a function which saves an image of kind magick with
/// given path.

# include <mln/core/image/image2d.hh>
# include <mln/metal/equal.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <Magick++.h>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /*! Save a Milena image in a magick image.
       *
       * \param[out] ima A reference to the image to save.
       * \param[in] filename The output.
       */
      template <typename I>
      void save(Image<I>& ima,
		const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      inline
      Magick::Color get_color(bool value)
      {
	if (value)
	  return Magick::Color("white");
	else
	  return Magick::Color("black");
      }

      inline
      Magick::Color get_color(value::int_u8 value)
      {
	return Magick::Color((unsigned) value * MaxRGB / 256,
			     (unsigned) value * MaxRGB / 256,
			     (unsigned) value * MaxRGB / 256,
			     0);
      }

      inline
      Magick::Color get_color(value::rgb8 value)
      {
	return Magick::Color((unsigned) value.red() * MaxRGB / 256,
			     (unsigned) value.green() * MaxRGB / 256,
			     (unsigned) value.blue() * MaxRGB / 256,
			     0);
      }

      template <typename I>
      inline
      void save(Image<I>& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	mln_precondition(mln_site_(I)::dim == 2);
	I& ima = exact(ima_);
	if (!(mln::metal::equal<mln_value(I), bool>::value ||
	    mln::metal::equal<mln_value(I), value::int_u8>::value ||
	    mln::metal::equal<mln_value(I), value::rgb8>::value))
	{
	  std::cerr << "error: trying to save an unsupported format" << std::endl;
	  std::cerr << "supported formats: binary, 8bits grayscale (int_u8), 8bits truecolor (rgb8)" << std::endl;
	  abort();
	}

	Magick::Image im_file(Magick::Geometry(ima.nrows(), ima.ncols()),
			      Magick::Color(0, 0, 0, 0));
	Magick::PixelPacket* pixel_cache = im_file.getPixels(0, 0, ima.nrows(), ima.ncols());
	Magick::PixelPacket* pixel;
	mln_piter(I) p(ima.domain());
	unsigned columns = ima.ncols();
	for_all(p)
	{
	  pixel = pixel_cache + (int) p.to_site().to_vec()[0] * columns
			      + (int) p.to_site().to_vec()[1];
	  *pixel = get_color(ima(p));
	}
	im_file.syncPixels();
	im_file.write(filename);

	trace::exiting("mln::io::magick::save");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_SAVE_HH
