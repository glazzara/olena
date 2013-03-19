// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ARITH_TIMES_SPE_HH
# define MLN_ARITH_TIMES_SPE_HH

/// \file
///
/// Specializations for mln::arith::times.

# ifndef MLN_ARITH_TIMES_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_ARITH_TIMES_HH

# include <mln/arith/includes.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace arith
  {

    namespace impl
    {

      namespace generic
      {
	template <typename L, typename R, typename O>
	void times_(const L& lhs, const R& rhs, O& output);

	template <typename L, typename R>
	void times_inplace_(L& lhs, const R& rhs);

      }

      template <typename L, typename R, typename O>
      inline
      void times_(trait::image::speed::any, const L& lhs,
		  trait::image::speed::any, const R& rhs,
		  trait::image::speed::any, O& output)
      {
	generic::times_(lhs, rhs, output);
      }

      template <typename L, typename R, typename O>
      inline
      void times_(trait::image::speed::fastest, const L& lhs,
		  trait::image::speed::fastest, const R& rhs,
		  trait::image::speed::fastest, O& output)
      {
	mln_trace("arith::impl::times_");

	mln_pixter(const L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	mln_pixter(O)       op(output);
	for_all_3(lp, rp, op)
	  op.val() = lp.val() * rp.val();

      }

      template <typename L, typename R>
      inline
      void times_inplace_(trait::image::speed::any, L& lhs,
			  trait::image::speed::any, const R& rhs)
      {
	generic::times_inplace_(lhs, rhs);
      }

      template <typename L, typename R>
      inline
      void times_inplace_(trait::image::speed::fastest, L& lhs,
			  trait::image::speed::fastest, const R& rhs)
      {
	mln_trace("arith::impl::times_inplace_");

	mln_pixter(L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	for_all_2(rp, lp)
	  lp.val() *= rp.val();

      }

    } // end of namespace mln::arith::impl

  } // end of namespace mln::arith

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_ARITH_TIMES_SPE_HH
