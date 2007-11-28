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

#ifndef MLN_LEVEL_ASSIGN_SPE_HH
# define MLN_LEVEL_ASSIGN_SPE_HH

/*! \file mln/level/assign.spe.hh
 *
 * \brief Specializations for mln::level::assign.
 *
 */

# include <mln/core/concept/image.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace impl
    {

      namespace generic
      {
	template <typename L, typename R>
	void assign_(L& target, const R& data);
      }


      // Disjunction.


      template <typename L, typename R>
      inline
      void assign_(trait::image::speed::any, L& target,
		   trait::image::speed::any, const R& data)
      {
	generic::assign_(target, data);
      }

      template <typename L, typename R>
      inline
      void assign_(trait::image::speed::fastest, L& target,
		   trait::image::speed::fastest, const R& data)
      {
	trace::entering("level::impl::assign_");

	mln_pixter(L) lhs(target);
	mln_pixter(const R) rhs(data);
	for_all_2(lhs, rhs)
	  lhs.val() = rhs.val();

	trace::exiting("level::impl::assign_");
      }


    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LEVEL_ASSIGN_SPE_HH
