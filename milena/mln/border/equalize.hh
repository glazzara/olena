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

#ifndef MLN_BORDER_EQUALIZE_HH
# define MLN_BORDER_EQUALIZE_HH

/*! \file
 *
 * \brief Define a function that equalizes the thickness of an image
 * virtual border.
 */

# include <mln/border/resize.hh>


namespace mln
{

  namespace border
  {

    /*! \brief Equalize the virtual (outer) border of images \p ima1
     *  and \p ima2 so that their size is equal and is at least \p
     *  min_thickness.
     *
     * \param[in,out] ima1 The first image whose border is to be equalizeed.
     * \param[in,out] ima2 The second image whose border is to be equalizeed.
     * \param[in] min_thickness The expected border minimum thickness of both images.
     *
     * \pre \p ima1 has to be initialized.
     * \pre \p ima2 has to be initialized.
     *
     * \warning If both image borders already have the same thickness
     * and if this thickness is larger than \p min_thickness, this
     * routine is a no-op.
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename J>
    void equalize(const Image<I>& ima1, const Image<J>& ima2,
		  unsigned min_thickness);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      inline
      void equalize_(const I& ima1, const J& ima2, unsigned min_thickness)
      {
	trace::entering("border::impl::equalize_");

	unsigned b1 = border::get(ima1), b2 = border::get(ima2);

	if (! (b1 == b2 && b2 >= min_thickness))
	  // Something has to be done.
	  {
	    if (b1 < min_thickness && b2 < min_thickness)
	      // Both images have to be border-resized.
	      {
		border::resize(ima1, min_thickness);
		border::resize(ima2, min_thickness);
	      }
	    else
	      // A single image has to be border-resized with
	      // the other image thickness.
	      if (b1 < min_thickness)
		{
		  mln_assertion(b2 >= min_thickness);
		  border::resize(ima1, b2);
		}
	      else
		{
		  //mln_assertion(b2 < min_thickness); // why ?
		  mln_assertion(b1 >= min_thickness);
		  border::resize(ima2, b1);
		}
	  }

	trace::exiting("border::impl::equalize_");
      }

    } // end of namespace mln::border::impl


    // Facade

    template <typename I, typename J>
    inline
    void equalize(const Image<I>& ima1_, const Image<J>& ima2_,
		  unsigned min_thickness)
    {
      trace::entering("border::equalize");

      //FIXME: check border
      //mlc_is(mln_trait_image_border(I), trait::image::border::some)::check();
      //mlc_is(mln_trait_image_border(J), trait::image::border::some)::check();
      const I& ima1 = exact(ima1_);
      const J& ima2 = exact(ima2_);
      mln_precondition(ima1.is_valid() && ima2.is_valid());

      impl::equalize_(ima1, ima2, min_thickness);

      mln_postcondition(border::get(ima1) == border::get(ima2) &&
			border::get(ima1) >= min_thickness &&
			border::get(ima2) >= min_thickness);

      trace::exiting("border::equalize");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_EQUALIZE_HH
