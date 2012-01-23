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

#ifndef MLN_IO_PGM_LOAD_HH
# define MLN_IO_PGM_LOAD_HH

/// \file
///
/// \brief Define a function which loads an image of kind pgm with

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pnm/load.hh>


namespace mln
{

  namespace io
  {

    namespace pgm
    {

      /*! \brief Load a pgm image in a Milena image.
       *
       * \param[out] ima A reference to the image which will receive
       * data.
       * \param[in] filename The source.
       *
       * \ingroup iopgm
       */
      template <typename I>
      void load(Image<I>& ima,
		const std::string& filename);


      /*! \brief Load a pgm image in a Milena image.
       *
       * To use this routine, you should specialize the template whith
       * the value type of the image loaded. (ex :
       * load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       *
       * \ingroup iopgm
       */
      template <typename V>
      image2d<V> load(const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      image2d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::pgm::load");
	image2d<V> ima = io::pnm::load<V>(PGM, filename);
	trace::exiting("mln::io::pgm::load");
	return ima;
      }

      template <typename I>
      inline
      void load(Image<I>& ima,
		const std::string& filename)
      {
	trace::entering("mln::io::pgm::load");
	io::pnm::load<I>(PGM, ima, filename);
	trace::exiting("mln::io::pgm::load");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pgm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PGM_LOAD_HH
