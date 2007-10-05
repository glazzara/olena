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

#ifndef MLN_DEBUG_IOTA_HH
# define MLN_DEBUG_IOTA_HH

/*! \file mln/debug/iota.hh
 *
 * \brief FIXME
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace debug
  {

    /// FIXME
    template <typename I>
    void iota(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      void iota(trait::image::speed::any, I& input)
      {
	unsigned i = 0;
	mln_piter(I) p(input.domain());
	for_all(p)
	  input(p) = ++i;
      }

      template <typename I>
      void iota(trait::image::speed::fastest, I& input)
      {
	unsigned i = 0;
	mln_pixter(I) p(input);
	for_all(p)
	  p.val() = ++i;
      }

    } // end of namespace mln::debug::impl


    template <typename I>
    void iota(Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      impl::iota(mln_trait_image_speed(I)(), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_IOTA_HH
