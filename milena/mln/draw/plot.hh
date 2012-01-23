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

#ifndef MLN_DRAW_PLOT_HH
# define MLN_DRAW_PLOT_HH

/*! \file
 *
 * \brief Plot a point (small cross) in an image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_line2d.hh>
# include <mln/data/paste.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>


namespace mln
{

  namespace draw
  {

    /*! \brief Plot a point at level \p v in image \p ima

       \param[in,out] ima The image to be drawn.
       \param[in] p The point to be plotted.
       \param[in] v The value to assign to all drawn pixels.

       \pre \p ima has to be initialized.
       \pre \p ima has \p p.

       \ingroup mlndraw
     */
    template <typename I>
    void plot(Image<I>& ima,
	      const mln_point(I)& p,
	      const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void plot(Image<I>& ima,
	      const mln_psite(I)& p,
	      const mln_value(I)& v)
    {
      mln_precondition(exact(ima).is_valid());
      mln_precondition(exact(ima).has(p));

      for (unsigned i = 0; i < I::point::dim; i++)
        {
          mln_point(I) beg = p, end = p;
          beg[i] = beg[i] - 1;
          end[i] = end[i] + 1;
          data::paste(pw::cst(v) | line2d(beg, end),
                       ima);
        }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_PLOT_HH
