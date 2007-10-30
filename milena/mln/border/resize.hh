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

#ifndef MLN_BORDER_RESIZE_HH
# define MLN_BORDER_RESIZE_HH

/*! \file mln/border/resize.hh
 *
 * \brief Define a function that resizes the virtual border of an
 * image.
 */

# include <cstring>

# include <mln/core/concept/image.hh>
# include <mln/core/clone.hh>
# include <mln/level/fill.hh>
# include <mln/metal/is_not.hh>
# include <mln/border/get.hh>

namespace mln
{

  namespace border
  {

    /*! Resize the virtual (outer) border of image \p ima to at least
     *  \p thickness.
     *
     * \param[in,out] ima The image whose border is to be resized.
     * \param[in] thickness The expected minimum border size.
     *
     * \pre \p ima has to be initialized.
     *
     * \warning If the image border already has a size greater than \p
     * thickness, this routine is a no-op.
     *
     * \todo Implement it.
     */
    template <typename I>
    void resize(const Image<I>& ima, unsigned thickness);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename I>
      void resize_(const I& ima_, trait::image::category::morpher,
		   unsigned new_border)
      {
	return resize ( *ima_.delegatee_(), new_border);
      }


      template <typename I>
      void resize_(const I& ima_, trait::image::category::primary,
		   unsigned new_border)
      {
	I& ima = const_cast<I&> (ima_);
	I src = clone(ima);

	ima.resize_(new_border);
	level::fill(ima, src);
      }


    } // end of namespace mln::border::resize

    /// Facade.

    template <typename I>
    void resize(const Image<I>& ima_, unsigned thickness)
    {
      mlc_is(mln_trait_image_border(I), trait::image::border::some)::check();
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());

      if (border::get(ima) == thickness)
	return; // No-op.

      impl::resize_(ima, mln_trait_image_category(I)(), thickness); 
      mln_postcondition(border::get(ima) == thickness);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_RESIZE_HH
