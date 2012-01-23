// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_BORDER_RESIZE_HH
# define MLN_BORDER_RESIZE_HH

/// \file
///
/// Define a function that resizes the virtual border of an
/// image.
///
/// \todo This code is not complete: an image can be composed of
/// several images whose multiple borders have to be resized!  For
/// instance an image extended by an image, or a stack of images.

# include <mln/core/concept/image.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/core/routine/primary.hh>
# include <mln/border/get.hh>
# include <mln/data/fill.hh>


namespace mln
{

  namespace border
  {

    /*! \brief Resize the virtual (outer) border of image \p ima to
     *  exactly \p thickness.
     *
     * \param[in,out] ima The image whose border is to be resized.
     * \param[in] thickness The expected border thickness.
     *
     * \pre \p ima has to be initialized.
     *
     * \warning If the image border already has the expected
     * thickness, this routine is a no-op.
     *
     * \ingroup mlnborderext
     */
    template <typename I>
    void resize(const Image<I>& ima, unsigned thickness);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void resize(I& ima, unsigned thickness)
      {
	if (border::get(ima) == thickness)
	  return; // No-op.

	mln_concrete(I) memo = mln::duplicate(ima);
	ima.resize_(thickness);
	data::fill(ima, memo);

	mln_postcondition(border::get(ima) == thickness);
      }

    } // end of namespace mln::border::impl


    namespace internal
    {

      template <typename I>
      inline
      void resize_dispatch(trait::image::ext_domain::any,
			   const Image<I>& ima, unsigned thickness)
      {
	// No-op.
	(void) ima;
	(void) thickness;
      }

      template <typename I>
      inline
      void resize_dispatch(trait::image::ext_domain::extendable,
			   const Image<I>& ima, unsigned thickness)
      {
	// Effective resizing.
	impl::resize(const_cast<I&>(exact(ima)), thickness);
      }

      template <typename I>
      inline
      void resize_dispatch(const Image<I>& ima, unsigned thickness)
      {
	resize_dispatch(mln_trait_image_ext_domain(I)(),
			ima, thickness);
      }

    } // end of namespace mln::border::internal


    /// Facade.

    template <typename I>
    inline
    void resize(const Image<I>& ima, unsigned thickness)
    {
      trace::entering("border::resize");

      mln_precondition(exact(ima).is_valid());

      // Try to resize the primary image behind ima.
      internal::resize_dispatch(primary(ima), thickness);

      trace::exiting("border::resize");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_RESIZE_HH
