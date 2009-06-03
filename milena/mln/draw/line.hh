// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_DRAW_LINE_HH
# define MLN_DRAW_LINE_HH

/// \file mln/draw/line.hh
///
/// Draw a line in an image.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_line2d.hh>
# include <mln/core/image/imorph/safe.hh>
# include <mln/data/paste.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>


namespace mln
{

  namespace draw
  {

    /*! Draw a line at level \p v in image \p ima between the points
     *  \p beg and \p end.
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] beg The start point to drawn line.
     * \param[in] end The end point to drawn line.
     * \param[in] v The value to assign to all drawn pixels.
     *
     * \pre \p ima has to be initialized.
     * \pre \p ima has \p beg.
     * \pre \p ima has \p end.
     *
     */
    template <typename I>
    void line(Image<I>& ima,
	      const mln_psite(I)& beg, const mln_psite(I)& end,
	      const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void line(Image<I>& ima_,
	      const mln_psite(I)& beg, const mln_psite(I)& end,
	      const mln_value(I)& v)
    {
      I& ima = exact(ima_);
      mln_precondition(ima.is_valid());
      if (! ima.has(beg) ||  ! ima.has(end))
	trace::warning("Begin or end site is not part of the given image.");
      data::paste(pw::cst(v) | p_line2d(beg, end),
		   safe(ima).rw());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_LINE_HH
