// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_BORDER_MIRROR_HH
# define MLN_BORDER_MIRROR_HH

/// \file mln/border/mirror.hh
///
/// Define function that fills border using nearer pixels with a
/// mirroring effect.
///
/// \todo Awful code: rewrite it!

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>

# include <mln/core/concept/image.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/max_col.hh>
# include <mln/geom/ninds.hh>


namespace mln
{

  namespace border
  {

    /*! Mirror the virtual (outer) border of image \p ima with the
     *  (inner) level contents of this image.
     *
     * \param[in,out] ima The image whose border is to be mirrored.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Implement 3d version + optimize with memset if possible.
     */
    template <typename I>
    void mirror(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void mirror_(const box1d&, const I& ima_)
      {
	trace::entering("border::impl::mirror_");
	I& ima = const_cast<I&>(ima_);

	def::coord
	  border = static_cast<def::coord>(ima.border()),
	  nbinds = static_cast<def::coord>(geom::ninds(ima)),
	  min;

	if (border > nbinds)
	  min = nbinds;
	else
	  min = border;

	/// left border
	{
	  def::coord i = 0;
	  for (; i < min; ++i)
	    ima.element(border - 1 - i) = ima(point1d(i));

	  for (; i < border; ++i)
	    ima.element(border - 1 - i) = ima(point1d(static_cast<def::coord>(min - 1)));
	}

	/// right border
	{
	  def::coord
	    i = 0,
	    j = static_cast<def::coord>(nbinds - 1);
	  for (;
	       i < min;
	       ++i, --j)
	    ima.element(border + nbinds + i) = ima(point1d(j));
	  ++j;
	  for (;
	       i < border;
	       ++i)
	    ima.element(border + nbinds + i) = ima(point1d(j));
	}
	trace::exiting("border::impl::mirror_");
      }

      template <typename I>
      inline
      void mirror_(const box2d&, const I& ima_)
      {
	trace::entering("border::impl::mirror_");
	I& ima = const_cast<I&>(ima_);

	unsigned border = ima.border ();
	unsigned nbrows = geom::max_row(ima) - geom::min_row(ima);
	unsigned nbcols = geom::max_col(ima) - geom::min_col(ima);
	unsigned real_nbcols = (nbcols + 1) + 2 * border;
	unsigned start = real_nbcols * border + border;
	unsigned s = start;

	// mirror top left corner
	for (unsigned i = 0; i < border; ++i)
	  for (unsigned j = 0; j < border; ++j)
	  ima.element(i * ((nbcols + 1) + 2 * border) + j) = ima.element(s);

	// mirror top left corner
	s = start + nbcols;
	for (unsigned i = 0; i < border; ++i)
	  for (unsigned j = 1; j <= border; ++j)
	    ima.element(i * ((nbcols + 1) + 2 * border) + (nbcols + border + j)) = ima.element(s);

	// mirror bottom left corner
	s = start + (nbrows * real_nbcols);
	for (unsigned i = 1; i <= border; ++i)
	  for (unsigned j = 1; j <= border; ++j)
	    ima.element(s - i + (j * (real_nbcols))) = ima.element(s);

	// mirror bottom right corner
	s = start + (nbrows * real_nbcols) + nbcols;
	for (unsigned i = 1; i <= border; ++i)
	  for (unsigned j = 1; j <= border; ++j)
	    ima.element(s + i + (j * real_nbcols)) = ima.element(s);

	// mirror top border
	s = start;
	for (unsigned i = 0; i <= nbcols; ++i)
	  for (unsigned j = 1; j <= border; ++j)
	    ima.element(s + i - (j * real_nbcols)) = ima.element(s + i + ((j - 1)* real_nbcols));

	// mirror left border
	s = start;
	for (unsigned i = 0; i <= nbrows; ++i)
	  for (unsigned j = 1; j <= border; ++j)
 	  ima.element(s + (i * real_nbcols) - j) = ima.element(s + (i * real_nbcols) + (j - 1));

	// mirror right border
	s = start;
	for (unsigned i = 0; i <= nbrows; ++i)
	  for (unsigned j = 1; j <= border; ++j)
	    ima.element(s + (i * real_nbcols + nbcols) + j) = ima.element(s + (i * real_nbcols + nbcols) - (j - 1));

	// mirror bottom border
	s = start + (nbrows * real_nbcols);
	for (unsigned i = 0; i <= nbcols; ++i)
	  for (unsigned j = 1; j <= border; ++j)
	    ima.element(s + i + (j * real_nbcols)) = ima.element(s + i - ((j - 1)* real_nbcols));

	trace::exiting("border::impl::mirror_");
      }

      template <typename I>
      inline
      void mirror_(const box3d&, const I& ima)
      {
	mln::internal::fixme();
      }


    } // end of namespace mln::border::impl


    template <typename I>
    inline
    void mirror(const Image<I>& ima_)
    {
      trace::entering("border::mirror");

      const I& ima = exact(ima_);

      mln_precondition(ima.is_valid());
      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      typedef mln_psite(I) P;

      if (!ima.border ())
	return;

      impl::mirror_(ima.bbox(), ima);

      trace::exiting("border::mirror");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_MIRROR_HH
