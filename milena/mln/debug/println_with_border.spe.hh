// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_DEBUG_PRINTLN_WITH_BORDER_SPE_HH
# define MLN_DEBUG_PRINTLN_WITH_BORDER_SPE_HH

/*! \file mln/debug/println_with_border_spe.hh
 *
 * \brief  Specializations for mln::debug::println_with_border.
 */

# ifndef MLN_DEBUG_PRINTLN_WITH_BORDER_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DEBUG_PRINTLN_WITH_BORDER_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/debug/format.hh>
# include <mln/opt/element.hh>

namespace mln
{

  namespace debug
  {

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

# ifdef MLN_CORE_SITE_SET_BOX2D_HH

      // 2D version.
      template <typename I>
      inline
      void
      println_with_border(const box2d& b, const I& input)
      {
	const unsigned ncols = b.ncols() + 2 * input.border();
 	for (size_t i = 0; i < opt::nelements(input); i++)
	{
	  std::cout << format(input.buffer()[i]) << ' ';
	  if (((i + 1) % ncols) == 0)
	    std::cout << std::endl;
	}
	std::cout << std::endl;
      }
# endif // MLN_CORE_SITE_SET_BOX2D_HH


# ifdef MLN_CORE_SITE_SET_BOX3D_HH

      // 3D version.
      template <typename I>
      void
      println_with_border(const box3d& b, const I& input)
      {
	typedef mln_psite(I) P;

	unsigned len_s = b.len(P::dim - 3);
	unsigned len_r = b.len(P::dim - 2);
	unsigned len_c = b.len(P::dim - 1);

	unsigned border = input.border();
	unsigned real_len_s = len_s + 2 * border;
	unsigned real_len_c = len_c + 2 * border;
	unsigned real_len_r = len_r + 2 * border;

	for (unsigned k = 0; k < real_len_s; ++k)
	  {
	    for (unsigned j = 0; j < real_len_r; ++j)
	      {
		for (unsigned i = 0; i < real_len_c; ++i)
		  std::cout << format(input.point_at_index(k * (real_len_r * real_len_c) + j * real_len_c + i))
			    << ' ';
		std::cout << std::endl;
	      }
	    std::cout << std::endl;
	  }
	std::cout << std::endl;
      }

# endif // MLN_CORE_SITE_SET_BOX3D_HH

    } // end of namespace mln::debug::impl

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_WITH_BORDER_SPE_HH
