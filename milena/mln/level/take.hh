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

#ifndef MLN_LEVEL_TAKE_HH
# define MLN_LEVEL_TAKE_HH

/*! \file mln/level/take.hh
 *
 * \brief Make an accumulator take image pixel values.
 */

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace level
  {

    /*! Make an accumulator take the values of the image \p input.
     *
     * \param[in,out] a The accumulator.
     * \param[in] input The input image.
     *
     * This routine runs: \n
     *   for all p of \p input, \p a.take( \p input(p) ) \n
     *
     * \warning This routine does not perform a.init().
     */
    template <typename A, typename I>
    void take(Accumulator<A>& a, const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename A, typename I>
      void take(A& a, const Image<I>& input_)
      {
	const I& input = exact(input_);
	mln_piter(I) p(input.domain());
	for_all(p)
	  a.take(input(p));
      }

      template <typename A, typename I>
      void take(A& a, const Fast_Image<I>& input_)
      {
	const I& input = exact(input_);
	mln_pixter(const I) pxl(input);
	for_all(pxl)
	  a.take(*pxl);
      }

    } // end of namespace mln::level::impl


    // Facade.

    template <typename A, typename I>
    void take(Accumulator<A>& a, const Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      impl::take(exact(a), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_TAKE_HH
