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

#ifndef MLN_BORDER_FIND_HH
# define MLN_BORDER_FIND_HH

/*! \file mln/border/find.hh
 *
 * \brief FIXME.
 */

# include <mln/core/internal/image_morpher.hh>


namespace mln
{

  namespace border
  {

    /*! Find the virtual (outer) border thickness of image \p ima.
     *
     * \param[in] ima The image.
     * \result The border thickness (0 if there is no border).
     */
    template <typename I>
    unsigned find(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename S, typename E>
      unsigned find__(const mln::internal::image_morpher_<I,S,E>& ima)
      {
	return border::find(*ima.delegatee_());
      }

      template <typename S, typename E>
      unsigned find__(const mln::internal::image_base_<S,E>&)
      {
	return 0;
      }

      template <typename I>
      unsigned find_(trait::image::speed::any, const I& ima)
      {
	return border::impl::find__(ima);
      }

      template <typename I>
      unsigned find_(trait::image::speed::fastest, const I& ima)
      {
	return ima.border();
      }

    } // end of namespace mln::border::impl


    // Facade.

    template <typename I>
    unsigned find(const Image<I>& ima)
    {
      mln_precondition(exact(ima).has_data());
      return border::impl::find_(mln_trait_image_speed(I)(), exact(ima));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_FIND_HH
