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

#ifndef MLN_LEVEL_APPLY_HH
# define MLN_LEVEL_APPLY_HH

/*! \file mln/level/apply.hh
 *
 * \brief Apply a function-object onto image pixel values.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/accumulator.hh>


namespace mln
{

  namespace level
  {

    /*! Apply a function-object to the image \p input.
     *
     * \param[in] input The input image.
     * \param[in] f The function-object.
     * \result A copy of the function-object.
     *
     * This routine runs: \n
     *   for all p of \p input, \p f( \p input(p) ) \n
     *   return \p f
     *
     * \todo Find a meaning for this routine! (Clue: f is mutable
     * and/or same for input?)
     */
    template <typename I, typename F>
    F apply(const Image<I>& input, const Function<F>& f);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename F>
      F apply(const Image<I>& input_, const Function<F>& f_)
      {
	const I& input  = exact(input_);
	F f = exact(f_);
	
	mln_piter(I) p(input.domain());
	for_all(p)
	  f(input(p));
	return f;
      }

      template <typename I, typename F>
      F apply(const Fast_Image<I>& input_, const Function<F>& f_)
      {
	const I& input  = exact(input_);
	F f = exact(f_);
	
	mln_pixter(const I) pxl(input);
	for_all(pxl)
	  f(pxl.val());
	return f;
      }

    } // end of namespace mln::level::impl


    // Facades.

    template <typename I, typename F>
    F apply(const Image<I>& input, const Function<F>& f)
    {
      mln_precondition(exact(input).has_data());
      return impl::apply(exact(input), f);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_APPLY_HH
