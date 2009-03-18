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

#ifndef MLN_IO_MAGICK_LOAD_HH
# define MLN_IO_MAGICK_LOAD_HH

///
/// \file   mln/io/magick/load.hh
///
/// Define a function which loads an image of kind magick with
/// given path.

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <Magick++.h>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /*! Load a magick image in a Milena image.
       *
       * \param[out] ima A reference to the image which will receive
       * data.
       * \param[in] filename The source.
       */
      template <typename I>
      void load(Image<I>& ima,
		const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      inline
      bool do_it(const value::rgb8& in, bool& out, const std::string& filename)
      {
	if (in.red() == 255u && in.green() == 255u && in.blue() == 255u)
	{
	  out = true;
	  return true;
	}
	if (in.red() == 0u && in.green() == 0u && in.blue() == 0u)
	{
	  out = false;
	  return true;
	}
	if (in.red() == in.green() && in.green() == in.blue())
	  std::cerr << "error: trying to load '" << filename << "' which is a grayscale image into a bool image" << std::endl;
	else
	  std::cerr << "error: trying to load '" << filename << "' which is a truecolor image into a bool image" << std::endl;
	return false;
      }

      inline
      bool do_it(const value::rgb8& in, value::int_u8& out, const std::string& filename)
      {
	if (in.red() == in.green() && in.green() == in.blue())
	{
	  out = in.red();
	  return true;
	}
	std::cerr << "error: trying to load '" << filename << "' which is a truecolor image into a grayscale image" << std::endl;
	return false;
      }

      inline
      bool do_it(const value::rgb8& in, value::rgb8& out, const std::string& filename)
      {
	(void) filename;
	out = in;
	return true;
      }


      template <typename I>
      inline
      void load(Image<I>& ima_,
	  const std::string& filename)
      {
	trace::entering("mln::io::magick::load");

	I& ima = exact(ima_);

	//std::ifstream file(filename.c_str());
	//if (! file)
	//{
	//  std::cerr << "error: cannot open file '" << filename << "'!";
	//  abort();
	//}

	Magick::Image im_file(filename);
	im_file.modifyImage();
	im_file.type(Magick::TrueColorType);
	int columns = im_file.columns();
	int rows = im_file.rows();
	/*std::cout << "width: " <<columns << std::endl;
	  std::cout << "height: " <<rows << std::endl;
	  std::cout << "depth: " <<im_file.depth() << std::endl;
	  std::cout << "format: " <<im_file.format() << std::endl;
	  std::cout << "magick: " <<im_file.magick() << std::endl;*/

	const Magick::PixelPacket *pixel_cache = im_file.getConstPixels(0, 0, columns, rows);

	algebra::vec<mln_site_(I)::dim, unsigned int> vmin;
	algebra::vec<mln_site_(I)::dim, unsigned int> vmax;
	vmin[0] = 0;
	vmin[1] = 0;
	vmax[0] = rows - 1;
	vmax[1] = columns - 1;
	mln_site(I) pmin(vmin);
	mln_site(I) pmax(vmax);
	mln_concrete(I) result(box<mln_site(I)>(pmin, pmax));
	initialize(ima, result);
	mln_piter(I) p(ima.domain());
	for_all(p)
	{
	  const Magick::PixelPacket *pixel = pixel_cache + (int) p.to_site().to_vec()[0] * columns
					     + (int) p.to_site().to_vec()[1];
	  value::rgb8 pix(pixel->red % 256, pixel->green % 256, pixel->blue % 256); // WARNING: Quantum = 16bits but rgb is 8bits
	  mln_value(I) res;
	  if (!do_it(pix, res, filename))
	    abort();
	  ima(p) = res;
	}

	trace::exiting("mln::io::magick::load");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_LOAD_HH
