// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CONVERT_TO_IMAGE_HH
# define MLN_CONVERT_TO_IMAGE_HH

/// mln/convert/to_image.hh
///
/// Conversions to mln::Image.

# include <mln/trait/image_from_grid.hh>

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/geom/bbox.hh>
# include <mln/data/fill.hh>
# include <mln/histo/array.hh>

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  namespace convert
  {


    /// Convert a point set \p pset into a binary image. Width of the
    /// converted image will be pset.bbox + 2 * \p border.
    template <typename S>
    mln_image_from_grid(mln_site(S)::grid, bool)
    to_image(const Site_Set<S>& pset, unsigned border = 1);

    /// Convert a window \p win into a binary image.
    template <typename W>
    mln_image_from_grid(mln_site(W)::grid, bool)
    to_image(const Window<W>& win);

    /// Convert a weighted window \p w_win into an image.
    template <typename W>
    mln_image_from_grid(mln_site(W)::grid, mln_weight(W))
    to_image(const Weighted_Window<W>& w_win);

    /// Convert a neighborhood \p nbh into a binary image.
    template <typename N>
    mln_image_from_grid(mln_site(N)::grid, bool)
    to_image(const Neighborhood<N>& nbh);

    /// Convert an histo \p h into an image1d<unsigned>.
    template <typename T>
    image1d<unsigned>
    to_image(const histo::array<T>& h);



# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    mln_image_from_grid(mln_site(S)::grid, bool)
    to_image(const Site_Set<S>& pset_, unsigned border)
    {
      const S& pset = exact(pset_);
      mln_image_from_grid(mln_site(S)::grid, bool) ima;
      ima.init_(geom::bbox(pset).to_larger(border));
      mln::data::fill(ima, false);
      mln_piter(S) p(pset);
      for_all(p)
	ima(p) = true;
      return ima;
    }

    template <typename W>
    mln_image_from_grid(mln_site(W)::grid, bool)
    to_image(const Window<W>& win)
    {
      mln_is_simple_window(W)::check();
      mln_precondition(exact(win).is_valid());
      mln_image_from_grid(mln_site(W)::grid, bool) tmp;
      convert::from_to(win, tmp);
      return tmp;
    }

    template <typename W>
    mln_image_from_grid(mln_site(W)::grid, mln_weight(W))
    to_image(const Weighted_Window<W>& w_win)
    {
      mln_is_simple_window(W)::check();
      mln_precondition(exact(w_win).is_valid());
      mln_image_from_grid(mln_site(W)::grid, mln_weight(W)) tmp;
      convert::from_to(exact(w_win), tmp);
      return tmp;
    }

    template <typename N>
    mln_image_from_grid(mln_site(N)::grid, bool)
    to_image(const Neighborhood<N>& nbh)
    {
      mln_precondition(exact(nbh).is_valid());
      return to_image(exact(nbh).win());
    }

    template <typename T>
    image1d<unsigned>
    to_image(const histo::array<T>& h)
    {
      def::coord
	n = static_cast<def::coord>(h.vset().nvalues()),
	v_min = static_cast<def::coord>(h.vset()[0]),
	v_max = static_cast<def::coord>(h.vset()[n - 1]);
      image1d<unsigned> ima(make::box1d(v_min, v_max));
      for (def::coord i = 0; i < n; ++i)
	ima(point1d(i)) = h[i];
      return ima;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_IMAGE_HH
