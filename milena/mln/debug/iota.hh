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
 * \brief Fill an image with successive values.
 */

# include <mln/core/concept/image.hh>

// Specializations are in:
# include <mln/debug/iota.spe.hh>


namespace mln
{

  namespace debug
  {

    /*! Fill the image \p input with successive values.
     *
     * \param[in,out] input The image in which values are
     * assigned.
     */
    template <typename I>
    void iota(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void
      iota(trait::image::speed::any, I& input)
      {
	unsigned i = 0;
	mln_piter(I) p(input.domain());
	for_all(p)
	  input(p) = ++i % mln_max(mln_value(I));
      }

    } // end of namespace mln::debug::impl


    template <typename I>
    inline
    void
    iota(Image<I>& input)
    {
      trace::entering("debug::iota");
      mln_precondition(exact(input).is_valid());
      impl::iota(mln_trait_image_speed(I)(), exact(input));
      trace::exiting("debug::iota");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_IOTA_HH
