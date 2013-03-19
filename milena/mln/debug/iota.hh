// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_DEBUG_IOTA_HH
# define MLN_DEBUG_IOTA_HH

/*! \file
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

    /*! \brief Fill the image \p input with successive values.

       \param[in,out] input The image in which values are
                             assigned.
       \param[in] base_index Start numbering from this value.

      \ingroup mlndebug
     */
    template <typename I>
    void iota(Image<I>& input, unsigned base_index);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void
      iota(trait::image::speed::any, I& input, unsigned base_index)
      {
	unsigned i = base_index;
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
      iota(input, 0);
    }


    template <typename I>
    inline
    void
    iota(Image<I>& input, unsigned base_index)
    {
      mln_trace("debug::iota");
      mln_precondition(exact(input).is_valid());
      impl::iota(mln_trait_image_speed(I)(), exact(input), base_index);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_IOTA_HH
