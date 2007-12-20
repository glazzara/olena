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

#ifndef MLN_LEVEL_FILL_SPE_HH
# define MLN_LEVEL_FILL_SPE_HH

/*! \file mln/level/fill.spe.hh
 *
 * \brief Specializations for mln::level::fill.
 *
 */

# ifndef MLN_LEVEL_FILL_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_FILL_HH

# include <cstring>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/inplace.hh>
# include <mln/level/memset_.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace impl
    {

      namespace generic
      {
	template <typename I>
	void fill_with_value(I& ima, const mln_value(I)& value);
      }


      // Disjunction.


      template <typename I>
      inline
      void fill_with_value(trait::image::speed::any, I& ima,
			   const mln_value(I)& value)
      {
	generic::fill_with_value(ima, value);
      }

      template <typename I>
      inline
      void fill_with_value(trait::image::speed::fastest, I& ima,
			   const mln_value(I)& value)
      {
	trace::entering("level::impl::fill_with_value");

	level::memset_(ima, ima.point_at_offset(0), value, ima.ncells());

	trace::exiting("level::impl::fill_with_value");
      }


    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LEVEL_FILL_SPE_HH
