// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ARITH_MIN_HH
# define MLN_ARITH_MIN_HH

/// \file mln/arith/min.hh
///
/// Point-wise min between images.

# include <mln/core/concept/image.hh>


// Specializations are in:
# include <mln/arith/min.spe.hh>

namespace mln
{

  namespace arith
  {

    /// Point-wise min of images \p lhs and \p rhs.
    /*!
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \result The result image.
     *
     * \pre \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R>
    mln_concrete(L)
    min(const Image<L>& lhs, const Image<R>& rhs);


    /// Point-wise min of image \p lhs in image \p rhs.
    /*!
     * \param[in,out] lhs First operand image.
     * \param[in] rhs Second operand image.
     *
     * \pre \p rhs.domain == \p lhs.domain
     */
    template <typename L, typename R>
    void min_inplace(Image<L>& lhs, const Image<R>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {
	template <typename L, typename R, typename O>
	inline
	void min_(const L& lhs, const R& rhs, O& output)
	{
	  trace::entering("level::arith::generic::min_");

	  mln_piter(L) p(lhs.domain());
	  for_all(p)
	    output(p) = lhs(p) < rhs(p) ? lhs(p) : rhs(p);

	  trace::entering("level::arith::generic::min_");
	}

	template <typename L, typename R>
	inline
	void min_inplace_(L& lhs, const R& rhs)
	{
	  trace::entering("level::arith::generic::min_inplace_");

	  mln_piter(L) p(lhs.domain());
	  for_all(p)
	  if (rhs(p) < lhs(p))
	    lhs(p) = rhs(p);

	  trace::exiting("level::arith::generic::min_inplace_");
	}

      } // end of namespace mln::arith::impl::generic

    } // end of namespace mln::arith::impl


    // Facades.

    template <typename L, typename R>
    inline
    mln_concrete(L) min(const Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("arith::min");
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_concrete(L) output;
      initialize(output, lhs);
      impl::min_(mln_trait_image_speed(L)(), exact(lhs),
		 mln_trait_image_speed(R)(), exact(rhs), output);

      trace::exiting("arith::min");
      return output;
    }

    template <typename L, typename R>
    inline
    void min_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("arith::min_inplace");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      impl::min_inplace_(mln_trait_image_speed(L)(), exact(lhs),
			 mln_trait_image_speed(R)(), exact(rhs));

      trace::exiting("arith::min_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_MIN_HH
