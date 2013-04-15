// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
// 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PPM_SAVE_HH
# define MLN_IO_PPM_SAVE_HH

/*!
 * \file
 *
 * \brief Define a function which saves an image of kind ppm into
 * given path.
 *
 */

# include <mln/core/concept/image.hh>

# include <mln/metal/templated_by.hh>

# include <mln/io/pnm/save.hh>

namespace mln
{

  namespace io
  {

    namespace ppm
    {

      /*! \brief Save a Milena image as a ppm image.
       *
       * \param[in] ima The image to save.
       * \param[in,out] filename the destination.
       *
       * \ingroup ioppm
       */
      template <typename I>
      void save(const Image<I>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void save(const Image<I>& ima, const std::string& filename)
      {
	mln::metal::templated_by<mln_value(I), value::rgb >::check();

	//call the generic function for pnm files
	io::pnm::save(PPM, exact(ima), filename);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::ppm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PPM_SAVE_HH
