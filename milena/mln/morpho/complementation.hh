// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_MORPHO_COMPLEMENTATION_HH
# define MLN_MORPHO_COMPLEMENTATION_HH

/*!
 * \file
 *
 * \brief Definition of a function that make a complementation
 * (logical or arithmetical) of an image.
 */

# include <mln/data/compare.hh>
# include <mln/logical/not.hh>
# include <mln/arith/revert.hh>


namespace mln
{

  namespace morpho
  {

    /*! \brief Morphological complementation: either a logical "not"
     *  (if morpho on sets) or an arithmetical complementation (if
     *  morpho on functions).
     *
     * \ingroup mlnmorpho
     */
    template <typename I>
    mln_concrete(I) complementation(const Image<I>& input);


    /*! \brief Morphological complementation, inplace version: either
     *  a logical "not" (if morpho on sets) or an arithmetical
     *  complementation (if morpho on functions).
     *
     * \ingroup mlnmorpho
     */
    template <typename I>
    void complementation_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Binary => morphology on sets.

      template <typename I>
      inline
      mln_concrete(I)
	complementation_(trait::image::kind::logic,
			 const Image<I>& input)
      {
	return logical::not_(input);
      }

      template <typename I>
      inline
      void
      complementation_inplace_(trait::image::kind::logic,
			       Image<I>& input)
      {
	logical::not_inplace(input);
      }


      // Otherwise => morphology on functions.

      template <typename I>
      inline
      mln_concrete(I)
	complementation_(trait::image::kind::any,
			 const Image<I>& input)
      {
	return arith::revert(input);
      }

      template <typename I>
      inline
      void
	complementation_inplace_(trait::image::kind::any,
				 Image<I>& input)
      {
	arith::revert_inplace(input);
      }

    } // end of namespace mln::morpho::impl


    // Facades.

    template <typename I>
    inline
    mln_concrete(I) complementation(const Image<I>& input)
    {
      trace::entering("morpho::complementation");
      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output = impl::complementation_(mln_trait_image_kind(I)(),
						      input);

      trace::exiting("morpho::complementation");
      return output;
    }

    template <typename I>
    inline
    void complementation_inplace(Image<I>& input)
    {
      trace::entering("morpho::complementation_inplace");
      mln_precondition(exact(input).is_valid());

      impl::complementation_inplace_(mln_trait_image_kind(I)(),
				     input);

      trace::exiting("morpho::complementation_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_COMPLEMENTATION_HH
