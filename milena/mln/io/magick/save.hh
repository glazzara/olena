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

#ifndef MLN_IO_MAGICK_SAVE_HH
# define MLN_IO_MAGICK_SAVE_HH

/// \file
///
/// Define a function which saves an image of kind magick with
/// given path.
///
/// FIXME: there seems to be roundness errors while computing rgb values.

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
      void save(const Image<I>& ima,
		const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      inline
      Magick::Color get_color(bool value)
      {
	std::cout << "bool color!" << std::endl;
	return Magick::ColorMono(value);
      }

      inline
      Magick::Color get_color(const value::int_u8& value)
      {
	return Magick::ColorGray(value / (double)256.0f);
      }

      inline
      Magick::Color get_color(const value::rgb8& value)
      {
	return Magick::ColorRGB(value.red()   / (double)256.0f,
				value.green() / (double)256.0f,
				value.blue()  / (double)256.0f);
      }

      template <typename I>
      inline
      void save(const Image<I>& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	mln_precondition(mln_site_(I)::dim == 2);
	const I& ima = exact(ima_);
	if (!(mln::metal::equal<mln_value(I), bool>::value ||
	    mln::metal::equal<mln_value(I), value::int_u8>::value ||
	    mln::metal::equal<mln_value(I), value::rgb8>::value))
	{
	  std::cerr << "error: trying to save an unsupported format" << std::endl;
	  std::cerr << "supported formats: binary, 8bits grayscale (int_u8), 8bits truecolor (rgb8)" << std::endl;
	  abort();
	}

	Magick::Image im_file;
	im_file.size(Magick::Geometry(ima.nrows(), ima.ncols()));

	Magick::PixelPacket* pixel_cache = im_file.getPixels(0, 0, ima.nrows(), ima.ncols());
	Magick::PixelPacket* pixel;
	mln_piter(I) p(ima.domain());
	for_all(p)
	{
	  pixel = pixel_cache + (int) p.to_site().to_vec()[0] * ima.ncols()
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
