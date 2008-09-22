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

#ifndef MLN_BORDER_ADJUST_HH
# define MLN_BORDER_ADJUST_HH

/*! \file mln/border/adjust.hh
 *
 * \brief Define a function that adjusts the thickness of an image
 * virtual border.
 */

# include <mln/border/resize.hh>
# include <mln/metal/is.hh>

namespace mln
{

  namespace border
  {

    /*! Adjust the virtual (outer) border of image \p ima so that its
     *  size is at least \p min_thickness.
     *
     * \param[in,out] ima The image whose border is to be adjusted.
     * \param[in] min_thickness The expected border minimum thickness.
     *
     * \pre \p ima has to be initialized.
     *
     * \warning If the image border is already larger than \p
     * min_thickness, this routine is a no-op.
     */
    template <typename I>
    void adjust(const Image<I>& ima, unsigned min_thickness);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void adjust(const Image<I>& ima_, unsigned min_thickness)
    {
      trace::entering("border::adjust");

      //FIXME: fix this check
      //mlc_is(mln_trait_image_border(I),
      //      trait::image::border::some)::check();
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());

      if (border::get(ima) < min_thickness)
	border::resize(ima, min_thickness);

      mln_postcondition(border::get(ima) >= min_thickness);

      trace::exiting("border::adjust");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_ADJUST_HH
