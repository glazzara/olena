// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DRAW_SITE_SET_HH
# define MLN_DRAW_SITE_SET_HH

/*! \file
 *
 * \brief Draw a site set in an image.
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>

namespace mln
{

  namespace draw
  {

    /*! Draw a sites with value \p v in image \p ima
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] b the site set to draw.
     * \param[in] v The value to assign to all drawn pixels.
     * \param[in] output_ratio size ratio between output image and the
     * image from which the bboxes were calculated.
     *
     * \pre \p s is included in \p ima domain.
     *
     */
    template <typename I, typename S>
    void site_set(Image<I>& ima,
		  const Site_Set<S>& s,
		  const mln_value(I)& v,
		  unsigned output_ratio = 1);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename S>
    void site_set(Image<I>& ima_,
		  const Site_Set<S>& s_,
		  const mln_value(I)& v,
		  unsigned output_ratio)
    {
      I& ima = exact(ima_);
      const S& s = exact(s_);

      mln_precondition(ima.is_valid());
      mln_precondition(ima.domain() >= s);

      mln_piter(S) p(s);
      for_all(p)
	ima(p / output_ratio) = v;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_SITE_SET_HH
