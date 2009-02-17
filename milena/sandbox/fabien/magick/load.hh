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

/*!
 * \file   mln/io/magick/load.hh
 *
 * \brief Define a function which loads an image of kind magick with
 * given path.
 *
 */

# include <mln/core/image/image2d.hh>
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

      /*! Load a magick image in a Milena image. To use this routine, you
       *  should specialize the template whith the value type of the
       *  image loaded. (ex : load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       */
      template <typename V>
      image2d<V> load(const std::string& filename);

      /*! Load a magick image in a Milena image. To use this routine, you
       *  should specialize the template whith the value type of the
       *  image loaded. (ex : load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       */
      template <typename V>
      image3d<V> load(const std::string& filename);

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      image2d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::magick::load");
	image2d<V> ima;// = io::pnm::load<V>(MAGICK, filename);
	trace::exiting("mln::io::magick::load");
	return ima;
      }

      template <typename V>
      inline
      image3d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::magick::load");
	image2d<V> ima;// = io::pnm::load<V>(MAGICK, filename);
	trace::exiting("mln::io::magick::load");
	return ima;
      }


      template <typename I>
      inline
      void load(Image<I>& ima,
		const std::string& filename)
      {
	trace::entering("mln::io::magick::load");

	//std::ifstream file(filename.c_str());
	//if (! file)
	//{
	//  std::cerr << "error: cannot open file '" << filename << "'!";
	//  abort();
	//}

	Magick::Image file(filename);
	//std::cout << "file attribute: " << file.attribute() << std::endl;
	std::cout << "width: " << file.columns() << std::endl;
	std::cout << "height: " << file.rows() << std::endl;
	std::cout << "x resolution: " << file.xResolution() << std::endl;
	std::cout << "y resolution: " << file.yResolution() << std::endl;
	std::cout << "comment: " << file.comment() << std::endl;
	std::cout << "format: " << file.format() << std::endl;

	trace::exiting("mln::io::magick::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_LOAD_HH
