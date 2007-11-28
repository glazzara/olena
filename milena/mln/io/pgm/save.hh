// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef MLN_IO_PGM_SAVE_HH
# define MLN_IO_PGM_SAVE_HH

/*!
 * \file   mln/io/pgm/save.hh
 *
 * \brief Define a function which saves an image of kind pgm into
 * given path.
 *
 */

# include <iostream>
# include <fstream>

# include <mln/io/pnm/save.hh>

# include <mln/geom/size2d.hh>
# include <mln/metal/templated_by.hh>
# include <mln/metal/bexpr.hh>


namespace mln
{

  // Fwd decl.
  namespace value {
    template <unsigned> class int_u;
    template <unsigned> class int_u_sat;
  }


  namespace io
  {

    namespace pgm
    {

      /*! Save a milena image as a pgm image.
       *
       * \param[in] ima The image to save.
       * \param[in,out] filename the destination.
       */
      template <typename I>
      void save(const Image<I>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void save(const Image<I>& ima, const std::string& filename)
      {
	// FIXME : this is to restrictive. Check if I is compatible with PGM.
	// 	mln::metal::or_<
	// 	  mln::metal::templated_by<mln_value(I), value::int_u >,
	// 	  mln::metal::templated_by<mln_value(I), value::int_u_sat >
	// 	  >::check();

	io::pnm::save(PGM, exact(ima), filename);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::pgm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PGM_SAVE_HH
