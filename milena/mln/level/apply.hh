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
# include <mln/core/concept/function.hh>

// Specializations are in:
# include <mln/level/apply.spe.hh>


namespace mln
{

  namespace level
  {

    /*! Apply a function-object to the image \p input.
     *
     * \param[in,out] input The input image.
     * \param[in] f The function-object.
     *
     * This routine runs: \n
     *   for all p of \p input, \p input(p) = \p f( \p input(p) ) \n
     *
     * This routine is equivalent to level::tranform(input, f, input)
     * but it is faster since a single iterator is required.
     *
     * \todo Add versions for lowq images.
     */
    template <typename I, typename F>
    void apply(Image<I>& input, const Function_v2v<F>& f);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename F>
	inline
	void apply_(I& input, const F& f)
	{
	  trace::entering("level::impl::generic::apply_");

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    input(p) = f(input(p));

	  trace::exiting("level::impl::generic::apply_");
	}

      } // end of namespace mln::level::impl::generic

    } // end of namespace mln::level::impl


    // Facade.

    template <typename I, typename F>
    inline
    void apply(Image<I>& input, const Function_v2v<F>& f)
    {
      trace::entering("level::apply");

      mln_precondition(exact(input).is_valid());
      impl::apply_(mln_trait_image_speed(I)(), exact(input),
		   exact(f));

      trace::exiting("level::apply");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_APPLY_HH
