// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
// 2011, 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PGM_SAVE_HH
# define MLN_IO_PGM_SAVE_HH

/*!
 * \file
 *
 * \brief Define a function which saves an image of kind pgm into
 * given path.
 *
 */

# include <iostream>
# include <fstream>

# include <mln/io/pnm/save.hh>

# include <mln/geom/size2d.hh>
# include <mln/metal/bexpr.hh>
# include <mln/metal/is_not_a.hh>
# include <mln/value/concept/vectorial.hh>


namespace mln
{

  // Fwd decl.
  namespace value {
    template <unsigned> struct int_u;
    template <unsigned> struct int_u_sat;
  }


  namespace io
  {

    namespace pgm
    {

      /*! \brief Save a Milena image as a pgm image.
       *
       * \param[in] ima The image to save.
       * \param[in,out] filename the destination.
       *
       * \ingroup iopgm
       */
      template <typename I>
      void save(const Image<I>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void save(const Image<I>& ima, const std::string& filename)
      {
	// FIXME: Is it exhaustive?
	mlc_is_not_a(mln_value(I), value::Vectorial)::check();
	mlc_converts_to(mln_value(I),unsigned)::check();

	trace::entering("mln::io::pgm::save");
	io::pnm::save(PGM, exact(ima), filename);
	trace::exiting("mln::io::pgm::save");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::pgm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PGM_SAVE_HH
