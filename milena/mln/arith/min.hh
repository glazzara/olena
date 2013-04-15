// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ARITH_MIN_HH
# define MLN_ARITH_MIN_HH

/// \file
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
	  mln_trace("data::arith::generic::min_");

	  mln_piter(L) p(lhs.domain());
	  for_all(p)
	    output(p) = lhs(p) < rhs(p) ? lhs(p) : rhs(p);
	}

	template <typename L, typename R>
	inline
	void min_inplace_(L& lhs, const R& rhs)
	{
	  mln_trace("data::arith::generic::min_inplace_");

	  mln_piter(L) p(lhs.domain());
	  for_all(p)
	  if (rhs(p) < lhs(p))
	    lhs(p) = rhs(p);

	}

      } // end of namespace mln::arith::impl::generic

    } // end of namespace mln::arith::impl


    // Facades.

    template <typename L, typename R>
    inline
    mln_concrete(L) min(const Image<L>& lhs, const Image<R>& rhs)
    {
      mln_trace("arith::min");
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_concrete(L) output;
      initialize(output, lhs);
      impl::min_(mln_trait_image_speed(L)(), exact(lhs),
		 mln_trait_image_speed(R)(), exact(rhs), output);

      return output;
    }

    template <typename L, typename R>
    inline
    void min_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      mln_trace("arith::min_inplace");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      impl::min_inplace_(mln_trait_image_speed(L)(), exact(lhs),
			 mln_trait_image_speed(R)(), exact(rhs));

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_MIN_HH
