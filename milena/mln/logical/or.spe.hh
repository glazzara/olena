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

#ifndef MLN_LOGICAL_OR_SPE_HH
# define MLN_LOGICAL_OR_SPE_HH

/*! \file mln/logical/or.spe.hh
 *
 * \brief Specializations for mln::logical::or.
 *
 */

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
	void or__(const L& lhs, const R& rhs, O& output);
      }

      template <typename L, typename R, typename O>
      inline
      void or__(trait::image::speed::any, const L& lhs,
		trait::image::speed::any, const R& rhs, O& output)
      {
	generic::or__(lhs, rhs, output);
      }

      template <typename L, typename R, typename O>
      inline
      void or__(trait::image::speed::fastest, const L& lhs,
		trait::image::speed::fastest, const R& rhs, O& output)
      {
	trace::entering("logical::impl::or__");

	mln_pixter(const L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	mln_pixter(O)       op(output);
	for_all_3(lp, rp, op)
	  op.val() = lp.val() || rp.val();

	trace::entering("logical::impl::or__");
      }

    } // end of namespace mln::logical::impl

  } // end of namespace mln::logical

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LOGICAL_OR_SPE_HH
