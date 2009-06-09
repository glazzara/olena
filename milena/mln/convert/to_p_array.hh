// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CONVERT_TO_P_ARRAY_HH
# define MLN_CONVERT_TO_P_ARRAY_HH

/// \file
///
/// \brief Conversions to mln::p_array.
///
/// \todo Move these conversion in from_to related mechanisms.

# include <mln/core/site_set/p_array.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a point set \p pset into a p_array (point set vector).
    template <typename S>
    p_array<mln_psite(S)> to_p_array(const Site_Set<S>& pset);


    /// Convert a window \p win centered at point \p p into a p_array
    /// (point set vector).
    template <typename W>
    p_array<mln_psite(W)> to_p_array(const Window<W>& win,
				     const mln_psite(W)& p);

    /// Convert an image \p img into a p_array.
    template <typename I>
    p_array<mln_psite(I)>
    to_p_array(const Image<I>& img);



# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    inline
    p_array<mln_psite(S)> to_p_array(const Site_Set<S>& pset_)
    {
      const S& pset = exact(pset_);
      p_array<mln_psite(S)> v;
//       v.reserve(pset.nsites());
      // FIXME: Why mln_fwd_piter and not mln_piter?
      mln_fwd_piter(S) p(pset);
      for_all(p)
	v.append(p);
      return v;
    }

    template <typename W>
    inline
    p_array<mln_psite(W)> to_p_array(const Window<W>& win_,
				     const mln_psite(W)& p)
    {
      const W& win = exact(win_);
      p_array<mln_psite(W)> v;
      v.reserve(win.size());
      mln_qiter(W) q(win, p);
      for_all(q)
	v.append(q);
      return v;
    }

    //FIXME: to remove, already added to from_image_to_site_set.hh.
    template <typename I>
    inline
    p_array<mln_psite(I)>
    to_p_array(const Image<I>& img_)
    {
      const I& img = exact(img_);

      p_array<mln_psite(I)> a;

      mln_piter(I) p(img.domain());
      for_all(p)
        if (img(p))
          a.append(p);

      return a;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_P_ARRAY_HH
