// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_BORDER_MIRROR_HH
# define MLN_BORDER_MIRROR_HH

/// \file
///
/// Define function that fills border using nearer pixels with a
/// mirroring effect.
///
/// \todo 2D version is not correct if the border is larger than the
/// image domain.
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

# include <mln/opt/element.hh>


namespace mln
{

  namespace border
  {

    /*! \brief Mirror the virtual (outer) border of image \p ima with
     *  the (inner) level contents of this image.
     *
     * \param[in,out] ima The image whose border is to be mirrored.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Implement 3d version + optimize with memset if possible.
     *
     * \ingroup mlnborderext
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
	mln_trace("border::impl::mirror_");
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
            opt::element(ima, border - 1 - i) = ima(point1d(i));

	  for (; i < border; ++i)
            opt::element(ima, border - 1 - i) = ima(point1d(static_cast<def::coord>(min - 1)));
	}

	/// right border
	{
	  def::coord
	    i = 0,
	    j = static_cast<def::coord>(nbinds - 1);
	  for (;
	       i < min;
	       ++i, --j)
            opt::element(ima, border + nbinds + i) = ima(point1d(j));
	  ++j;
	  for (;
	       i < border;
	       ++i)
            opt::element(ima, border + nbinds + i) = ima(point1d(j));
	}
      }

      template <typename I>
      inline
      void mirror_(const box2d&, const I& ima_)
      {
	mln_trace("border::impl::mirror_");
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
            opt::element(ima, i * ((nbcols + 1) + 2 * border) + j) =
              opt::element(ima, s);

	// mirror top left corner
	s = start + nbcols;
	for (unsigned i = 0; i < border; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, i * ((nbcols + 1) + 2 * border) + (nbcols + border + j)) = opt::element(ima, s);

	// mirror bottom left corner
	s = start + (nbrows * real_nbcols);
	for (unsigned i = 1; i <= border; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, s - i + (j * (real_nbcols))) =
              opt::element(ima, s);

	// mirror bottom right corner
	s = start + (nbrows * real_nbcols) + nbcols;
	for (unsigned i = 1; i <= border; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, s + i + (j * real_nbcols)) =
              opt::element(ima, s);

	// mirror top border
	s = start;
	for (unsigned i = 0; i <= nbcols; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, s + i - (j * real_nbcols)) =
              opt::element(ima, s + i + ((j - 1)* real_nbcols));

	// mirror left border
	s = start;
	for (unsigned i = 0; i <= nbrows; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, s + (i * real_nbcols) - j) =
              opt::element(ima, s + (i * real_nbcols) + (j - 1));

	// mirror right border
	s = start;
	for (unsigned i = 0; i <= nbrows; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, s + (i * real_nbcols + nbcols) + j) =
              opt::element(ima, s + (i * real_nbcols + nbcols) - (j - 1));

	// mirror bottom border
	s = start + (nbrows * real_nbcols);
	for (unsigned i = 0; i <= nbcols; ++i)
	  for (unsigned j = 1; j <= border; ++j)
            opt::element(ima, s + i + (j * real_nbcols)) =
              opt::element(ima, s + i - ((j - 1)* real_nbcols));

      }

      template <typename I>
      inline
      void mirror_(const box3d&, const I& ima)
      {
	mln_trace_warning("border::mirror for 3D image is not implemented,"
			      " so image borders have not been mirrored!");
	(void) ima;
	// FIXME write it!
      }


    } // end of namespace mln::border::impl


    template <typename I>
    inline
    void mirror(const Image<I>& ima_)
    {
      mln_trace("border::mirror");

      const I& ima = exact(ima_);

      mln_precondition(ima.is_valid());
      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      typedef mln_psite(I) P;

      if (!ima.border ())
	return;

      impl::mirror_(ima.bbox(), ima);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_MIRROR_HH
