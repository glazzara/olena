// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_APPLY_HH
# define MLN_DATA_APPLY_HH

/*! \file
 *
 * \brief Apply a function-object onto image pixel values.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

// Specializations are in:
# include <mln/data/apply.spe.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Apply a function-object to the image \p input.
     *
     * \param[in,out] input The input image.
     * \param[in] f The function-object.
     *
     * This routine runs: \n
     *   for all p of \p input, \p input(p) = \p f( \p input(p) ) \n
     *
     * This routine is equivalent to data::tranform(input, f, input)
     * but it is faster since a single iterator is required.
     *
     * \todo Add versions for lowq images.
     *
     * \ingroup mlndata
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
	  trace::entering("data::impl::generic::apply_");

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    input(p) = f(input(p));

	  trace::exiting("data::impl::generic::apply_");
	}

      } // end of namespace mln::data::impl::generic

    } // end of namespace mln::data::impl


    // Facade.

    template <typename I, typename F>
    inline
    void apply(Image<I>& input, const Function_v2v<F>& f)
    {
      trace::entering("data::apply");

      mln_precondition(exact(input).is_valid());
      impl::apply_(mln_trait_image_speed(I)(), exact(input),
		   exact(f));

      trace::exiting("data::apply");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_APPLY_HH
