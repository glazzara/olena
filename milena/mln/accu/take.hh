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

#ifndef MLN_ACCU_TAKE_HH
# define MLN_ACCU_TAKE_HH

/// \file
///
/// Define an accumulator that takes image pixels.

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/util/pix.hh>

// FIXME: to be removed? Seems to duplicate data::compute and
// set::compute. Moreover it does not work...

namespace mln
{

  namespace accu
  {

    /// Make an accumulator take the pixels of the image \p input.
    /*!
     * \param[in] input The input image.
     * \param[in,out] a The accumulator.
     *
     * This routine runs: \n
     *   for all p of \p input, \p a.take( pix(\p input, p) ) \n
     *
     * \warning This routine does not perform a.init().
     */
    template <typename A, typename I>
    void take(const Image<I>& input, Accumulator<A>& a);


    // FIXME: Overload for point sets.


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename A, typename I>
      inline
      void take(const Image<I>& input_, A& a)
      {
	const I& input = exact(input_);
	mln_piter(I) p(input.domain());
	for_all(p)
	  a.take(make::pix(input, p));
      }

      // FIXME: We need an impl for pixels of fastest images.

    } // end of namespace mln::accu::impl


    // Facade.

    template <typename A, typename I>
    inline
    void take(const Image<I>& input, Accumulator<A>& a)
    {
      mln_precondition(exact(input).is_valid());
      impl::take(exact(input), exact(a));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TAKE_HH
