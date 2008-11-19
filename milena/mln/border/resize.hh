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

#ifndef MLN_BORDER_RESIZE_HH
# define MLN_BORDER_RESIZE_HH

/*! \file mln/border/resize.hh
 *
 * \brief Define a function that resizes the virtual border of an
 * image.
 *
 * \todo Test with a primary image with no notion of border; I guess
 * it does not work.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/routine/clone.hh>
# include <mln/border/get.hh>
# include <mln/level/fill.hh>


namespace mln
{

  namespace border
  {

    /*! Resize the virtual (outer) border of image \p ima to exactly
     *  \p thickness.
     *
     * \param[in,out] ima The image whose border is to be resized.
     * \param[in] thickness The expected border thickness.
     *
     * \pre \p ima has to be initialized.
     *
     * \warning If the image border already has the expected
     * thickness, this routine is a no-op.
     */
    template <typename I>
    void resize(const Image<I>& ima, unsigned thickness);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Effective resizing.

      template <typename I>
      inline
      void resize_(trait::image::category::morpher,
		   const I& ima, unsigned thickness)
      {
	return resize(*ima.delegatee_(), thickness);
      }

      template <typename I>
      inline
      void resize_(trait::image::category::primary,
		   const I& ima_, unsigned thickness)
      {
	I& ima = const_cast<I&>(ima_);

	mln_concrete(I) memo = clone(ima);
	ima.resize_(thickness);
	level::fill(ima, memo);
      }

// ext_domain:   /any/
//                 |
//                 + -- none
//                 |
//                 + -- /some/
//                        |
//                        + -- fixed
//                        |     |
//                        |     + -- infinite
//                        |
//                        + -- extendable

      template <typename I>
      inline
      void resize_(trait::image::ext_domain::none,
		   const I&, unsigned)
      {
	// No-op.
      }

      template <typename I>
      inline
      void resize_(trait::image::ext_domain::fixed,
		   const I&, unsigned)
      {
	// No-op.
      }

      template <typename I>
      inline
      void resize_(trait::image::ext_domain::extendable,
		   const I& ima, unsigned thickness)
      {
	if (border::get(ima) == thickness)
	  return; // No-op.
	resize_(mln_trait_image_category(I)(),
		ima, thickness);
	mln_postcondition(border::get(ima) == thickness);
      }


      // Selector.

      template <typename I>
      inline
      void resize_(const I& ima, unsigned thickness)
      {
	resize_(mln_trait_image_ext_domain(I)(),
		ima, thickness);
      }

    } // end of namespace mln::border::resize


    /// Facade.

    template <typename I>
    inline
    void resize(const Image<I>& ima_, unsigned thickness)
    {
      trace::entering("border::resize");

      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());

      impl::resize_(ima, thickness);

      trace::exiting("border::resize");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_RESIZE_HH
