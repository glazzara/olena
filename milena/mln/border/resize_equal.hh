// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_BORDER_RESIZE_EQUAL_HH
# define MLN_BORDER_RESIZE_EQUAL_HH

/// \file
///
/// Define a function that resizes the virtual border of a couple of
/// images so that they eventually have the same border thickness.

# include <mln/border/resize.hh>


namespace mln
{

  namespace border
  {

    // FIXEM: Doc!
    /*! \brief Define a function that resizes the virtual border of a
     *  couple of images so that they eventually have the same border
     *  thickness.
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename J>
    void
    resize_equal(const Image<I>& ima1, const Image<J>& ima2);



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename J>
    inline
    void
    resize_equal(const Image<I>& ima1_, const Image<J>& ima2_)
    {
      trace::entering("border::resize_equal");

      // Static checks.
      mlc_equal(mln_trait_image_ext_domain(I),
		trait::image::ext_domain::extendable)::check();
      mlc_equal(mln_trait_image_ext_domain(J),
		trait::image::ext_domain::extendable)::check();

      const I& ima1 = exact(ima1_);
      const J& ima2 = exact(ima2_);

      // Dynamic tests.
      mln_precondition(ima1.is_valid());
      mln_precondition(ima2.is_valid());
      mln_precondition(ima2.domain() == ima1.domain());

      unsigned
	b1 = border::get(ima1),
	b2 = border::get(ima2);

      if (b2 > b1)
	border::resize(ima1, b2);
      else
	if (b1 > b2)
	  border::resize(ima2, b1);
      // else no-op when b2 == b1.

      trace::exiting("border::resize_equal");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_RESIZE_EQUAL_HH
