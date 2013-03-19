// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_BORDER_GET_HH
# define MLN_BORDER_GET_HH

/// \file
///
/// Define function that returns the real border thickness of image.

# include <mln/trait/images.hh>
# include <mln/debug/trace.hh>

namespace mln
{

  namespace border
  {

    /*! Get the virtual (outer) border thickness of image \a ima.
     *
     * \param[in] ima The image.
     * \result    The border thickness (0 if there is no border).
     *
     * \pre \a ima has to be initialized.
     *
     * \ingroup mlnborderext
     */
    template <typename I>
    unsigned get(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      unsigned get_(trait::image::ext_domain::some, trait::image::category::primary,
		    const I& ima)
      {
	return ima.border();
      }

      template <typename I>
      inline
      unsigned get_(trait::image::ext_domain::some, trait::image::category::morpher,
		    const I& ima)
      {
	return border::get(*ima.delegatee_());
      }


      template <typename I>
      inline
      unsigned get_(trait::image::ext_domain::none, trait::image::category::any,
		    const I&)
      {
	return 0;
      }

    } // end of namespace mln::border::impl


    // Facade.

    template <typename I>
    inline
    unsigned get(const Image<I>& ima)
    {
      mln_trace("border::get");

      mln_precondition(exact(ima).is_valid());
      unsigned res = border::impl::get_(mln_trait_image_ext_domain(I)(),
					mln_trait_image_category(I)(),
					exact(ima));

      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_GET_HH
