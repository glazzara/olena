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

#ifndef MLN_BORDER_DUPLICATE_HH
# define MLN_BORDER_DUPLICATE_HH

/*! \file mln/border/duplicate.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/level/memset_.hh>
//# include <mln/core/line_piter.hh>
#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>
#include <mln/core/image2d_b.hh>
#include <mln/core/image2d_b.hh>
#include <mln/core/pixel.hh>


namespace mln
{

  namespace border
  {

    /*! Assign the virtual (outer) border of image \p ima with the
     *  dupplicate of the inner border of this image.
     *
     * \param[in,out] ima The image whose border is to be duplicated.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Implement it + optimize with memcpy if possible.
     */
    template <typename I>
    void duplicate(const Fast_Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void duplicate(const Fast_Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());

      // internal::fixme();
      // FIX
      std::size_t border = ima.border ();
      std::size_t nbrows = geom::max_row(ima) - geom::min_row(ima);
      std::size_t nbcols = geom::max_col(ima) - geom::min_col(ima);
      std::size_t real_nbcols = (nbcols + 1) + 2 * border;
      std::size_t start = real_nbcols * border + border;
      std::size_t s = start;

      // duplicate top left corner
      for (std::size_t i = 0; i < border + 1; ++i)
	for (std::size_t j = 0; j < border + 1; ++j)
	  const_cast<I&>(ima)[i * ((nbcols + 1) + 2 * border) + j] = ima[s];

      // duplicate top border
      s = start;
      for (std::size_t i = 1; i <= nbcols - 1; ++i)
	for (std::size_t j = 0; j <= border; ++j)
	  const_cast<I&>(ima)[s + i - (j * real_nbcols)] = ima[s + i];

      // duplicate top left corner
      s = start + nbcols;
      for (std::size_t i = 0; i <= border; ++i)
	for (std::size_t j = 0; j <= border; ++j)
	  const_cast<I&>(ima)[i * ((nbcols + 1) + 2 * border) + (nbcols + border + j)] = ima[s];

      // duplicate left border
      s = start;
      for (std::size_t i = 1; i <= nbrows - 1; ++i)
	for (std::size_t j = 1; j <= border; ++j)
	  const_cast<I&>(ima)[s - j + (i * real_nbcols)] = ima[s + (i * real_nbcols)];

      // duplicate right border
      s = start;
      for (std::size_t i = 1; i <= nbrows - 1; ++i)
	for (std::size_t j = 1; j <= border; ++j)
	  const_cast<I&>(ima)[s + (i * real_nbcols + nbcols) + j] = ima[s + (i * real_nbcols + nbcols)];


      // duplicate bottom left corner
      s = start + (nbrows * real_nbcols);
      for (std::size_t i = 0; i <= border; ++i)
	for (std::size_t j = 0; j <= border; ++j)
	  const_cast<I&>(ima)[s - i + (j * (real_nbcols))] = ima[s];

      // duplicate bottom border
      s = start + (nbrows * real_nbcols);
      for (std::size_t i = 1; i <= nbcols - 1; ++i)
	for (std::size_t j = 0; j <= border; ++j)
	  const_cast<I&>(ima)[s + i + (j * real_nbcols)] = ima[s + i];

      // duplicate bottom right corner
      s = start + (nbrows * real_nbcols) + nbcols;
      for (std::size_t i = 0; i <= border; ++i)
	for (std::size_t j = 0; j <= border; ++j)
	  const_cast<I&>(ima)[s + i + (j * real_nbcols)] = ima[s];



      //END FIX
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_DUPLICATE_HH
