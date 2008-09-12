// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CONVERT_TO_IMAGE_HH
# define MLN_CONVERT_TO_IMAGE_HH

/*! \file mln/convert/to_image.hh
 *
 * \brief Conversions to mln::Image.
 *
 * \todo Remove. Use from_to instead...
 */

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/literal/zero.hh>

# include <mln/convert/to_window.hh>
# include <mln/geom/bbox.hh>
# include <mln/level/fill.hh>
# include <mln/histo/data.hh>


# define mln_image_from(Src, Value) typename mln::image_from_< Src, Value >::ret



namespace mln
{

  // FIXME: Move elsewhere.
  namespace internal
  {

    template <typename T>
    struct helper_dim_
    {
      typedef mln_site(T) P;
      enum { value = P::dim };
    };

    template <unsigned dim, typename V> struct helper_image_from_;

    template <typename V>
    struct helper_image_from_< 3, V >
    {
      typedef image3d<V> ret;
    };

    template <typename V>
    struct helper_image_from_< 2, V >
    {
      typedef image2d<V> ret;
    };

    template <typename V>
    struct helper_image_from_< 1, V >
    {
      typedef image1d<V> ret;
    };

  } // end of namespace mln::internal


  // FIXME: Doc + move elsewhere!
  template <typename T, typename V>
  struct image_from_
  {
    enum { dim = internal::helper_dim_<T>::value };
    typedef typename internal::helper_image_from_< dim, V >::ret ret;
  };



  namespace convert
  {


    /// Convert a point set \p pset into a binary image. Width of the
    /// converted image will be pset.bbox + 2 * \p border.
    template <typename S>
    mln_image_from(S, bool) to_image(const Site_Set<S>& pset, unsigned border = 1);

    /// Convert a window \p win into a binary image.
    template <typename W>
    mln_image_from(W, bool) to_image(const Window<W>& win);

    /// Convert a neighborhood \p nbh into a binary image.
    template <typename N>
    mln_image_from(N, bool) to_image(const Neighborhood<N>& nbh);

    /// Convert a weighted window \p w_win into an image.
    template <typename W>
    mln_image_from(W, mln_weight(W)) to_image(const Weighted_Window<W>& w_win);

    /// Convert an histo \p h into an image1d<std::size_t>.
    template <typename S>
    image1d<std::size_t> to_image(const mln::histo::data<S>& h);



# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    inline
    mln_image_from(S, bool) to_image(const Site_Set<S>& pset_, unsigned border)
    {
      const S& pset = exact(pset_);
      mln_image_from(S, bool) ima(pset.bbox().to_larger(border));
      level::fill(ima, false);
      mln_piter(S) p(pset);
      for_all(p)
	ima(p) = true;
      return ima;
    }

    template <typename W>
    inline
    mln_image_from(W, bool) to_image(const Window<W>& win_)
    {
      const W& win = exact(win_);
      mln_precondition(! win.is_empty());

      typedef mln_site(W) P;
      box<P> b = geom::bbox(win);
      mln_image_from(W, bool) ima(b);
      level::fill(ima, false);
      mln_qiter(W) q(win, P::origin);
      for_all(q)
	ima(q) = true;
      return ima;
    }

    template <typename N>
    inline
    mln_image_from(N, bool) to_image(const Neighborhood<N>& nbh)
    {
      return to_image(convert::to_window(nbh));
    }

    template <typename W>
    inline
    mln_image_from(W, mln_weight(W)) to_image(const Weighted_Window<W>& w_win_)
    {
      const W& w_win = exact(w_win_);
      mln_precondition(! w_win.is_empty());

      typedef mln_site(W) P;
      box<P> b = geom::bbox(w_win);
      mln_image_from(W, mln_weight(W)) ima(b);
      // Fill the image with zeros, as (weighted) windows are not
      // necessarily box-shaped (there might be holes corresponding to
      // null weights).
      level::fill(ima, literal::zero);
      mln_qiter(W) q(w_win, P::origin);
      for_all(q)
	ima(q) = q.w();
      return ima;
    }

    template <typename T>
    inline
    image1d<std::size_t> to_image(const histo::data<T>& h)
    {
      mln_value(histo::data<T>)
	v_min = h.vset()[0],
	v_max = h.vset()[h.vset().nvalues() - 1];
      image1d<std::size_t> ima(make::box1d(v_min, v_max));
      for(unsigned i = 0; i < h.vset().nvalues(); ++i)
	ima(i) = h[i];
      return ima;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_IMAGE_HH
