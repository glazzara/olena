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

#ifndef MLN_LOGICAL_AND_NOT_SPE_HH
# define MLN_LOGICAL_AND_NOT_SPE_HH

/*! \file mln/logical/and_not.spe.hh
 *
 * \brief Specializations for mln::logical::and_not.
 *
 */

# ifndef MLN_LOGICAL_AND_NOT_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LOGICAL_AND_NOT_HH

# include <mln/core/concept/image.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace logical
  {

    namespace impl
    {

      namespace generic
      {
	template <typename L, typename R, typename O>
	void and_not_(const L& lhs, const R& rhs, O& output);
      }


      template <typename L, typename R, typename O>
      inline
      void and_not_(trait::image::speed::any, const L& lhs,
		    trait::image::speed::any, const R& rhs, O& output)
      {
	generic::and_not_(lhs, rhs, output);
      }

      template <typename L, typename R, typename O>
      inline
      void and_not_(trait::image::speed::fastest, const L& lhs,
		    trait::image::speed::fastest, const R& rhs, O& output)
      {
	trace::entering("logical::impl::and_not_");

	mln_pixter(const L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	mln_pixter(O)       op(output);
	for_all_3(lp, rp, op)
	  op.val() = lp.val() && ! rp.val();

	trace::exiting("logical::impl::and_not_");
      }

    } // end of namespace mln::logical::impl

  } // end of namespace mln::logical

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LOGICAL_AND_NOT_SPE_HH
