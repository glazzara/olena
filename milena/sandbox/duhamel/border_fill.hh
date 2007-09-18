// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_BORDER_FILL_HH
# define MLN_BORDER_FILL_HH

/*! \file mln/border/fill.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/level/memset_.hh>
//# include <mln/core/line_piter.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>
# include <mln/core/pixel.hh>
# include <mln/core/line_piter.hh>
# include <mln/core/inplace.hh>
# include <mln/level/memcpy_.hh>
# include <mln/core/point2d.hh>

namespace mln
{

  namespace border
  {

    /*! Fill the virtual (outer) border of image \p ima with the
     *  single value \p v.
     *
     * \param[in,out] ima The image whose border is to be filled.
     * \param[in] v The value to assign to all border pixels.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Implement it + optimize with memset if possible.
     */
    template <typename I>
    void fill(const Fast_Image<I>& ima, const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void fill(const Fast_Image<I>& ima_, const mln_value(I)& v)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      // FIX
      typedef mln_point(I) P;
      typedef mln_dpoint(I) delta_point;

      Fast_Image<I> im (ima);
      //      internal::fixme();
      std::size_t border = ima.border ();
      std::size_t nbrows = geom::max_row(ima) - geom::min_row(ima);
      std::size_t nbcols = geom::max_col(ima) - geom::min_col(ima);
      //      std::size_t n = ima.bbox().len(P::dim - 1);

      point2d p = ima.bbox ().pmin ();

      // FIXME : REMOVE THIS LOOP BY MEMSET
      std::size_t s = border * (2 * (border + 1) + nbcols);
      for (std::size_t i = 0; i < s; ++i)
	const_cast<I&>(ima)[i] = v;

      //      typename I::line_piter p(ima.domain());

//       for (std::size_t i = 0; i < border; ++i, p = p.next ())
// 	{
// // 	  memset_(inplace(make::pixel(ima, p)),
// // 		  v,
// // 		  n);
// 	}

      // ACCESS TO RIGHT UP CORNER
      s = nbcols + 1;
      for (std::size_t i = 0; i < s; ++i)
	p = p + right;

      // FILL BORDER
      for (std::size_t i = 0; i < nbrows; ++i)
	{
	  level::memset_(const_cast<I&>(ima), p, v, 2 * border);
	  p = p + down;
 	}

      // FILL THE BOTTOM OF BORDER
      level::memset_(const_cast<I&>(ima), p, v, border * (2 * (border + 1) + nbcols));
      // END FIX
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_FILL_HH
