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
// instantiate templates or use macros or inpolygon functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_DRAW_POLYGON_HH
# define MLN_DRAW_POLYGON_HH

/// \file
///
/// Draw a polygon in an image.
/// \fixme Add specializations for horizontal polygons (use pointers/memset).

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/draw/line.hh>


namespace mln
{

  namespace draw
  {

    /*! Draw a polygon at level \p v in image \p ima.
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] par The polygon site set.
     * \param[in] v The value to assign to all drawn pixels.
     *
     * \pre \p ima has to be initialized.
     *
     */
    template <typename I>
    void polygon(Image<I>& ima,
		 const p_array<mln_site(I)>& par,
		 const mln_value(I)& v,
		 unsigned output_ratio);

    // \overload
    template <typename I>
    void polygon(Image<I>& ima,
		 const p_array<mln_site(I)>& par,
		 const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void polygon(Image<I>& ima_,
		 const p_array<mln_site(I)>& par,
		 const mln_value(I)& v,
		 unsigned output_ratio)
    {
      I& ima = exact(ima_);
      mln_precondition(ima.is_valid());
      mln_precondition(par.nelements() > 1);

      mln_site(I) p_last, tmp;
      mln_piter(p_array<mln_site(I)>) p(par);
      p_last = par[0] / output_ratio;
      for_all(p)
      {
	tmp = p / output_ratio;
	draw::line(ima, p_last, tmp, v);
	p_last = tmp;
      }
    }

    template <typename I>
    void polygon(Image<I>& ima,
		 const p_array<mln_site(I)>& par,
		 const mln_value(I)& v)
    {
      polygon(ima, par, v, 1);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_POLYGON_HH
