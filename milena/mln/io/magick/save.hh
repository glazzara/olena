// Copyright (C) 2009, 2010, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_IO_MAGICK_SAVE_HH
# define MLN_IO_MAGICK_SAVE_HH

/// \file
///
/// \brief Image output routines based on Magick++.
///
/// Do not forget to call Magick::InitializeMagick(*argv)
/// <em>before</em> using any of these functions, as advised by the
/// GraphicsMagick documentation
/// (http://www.graphicsmagick.org/Magick++/Image.html).

# include <cstdlib>

# include <Magick++.h>

# include <mln/metal/equal.hh>

# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/pixter2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/qt/rgb32.hh>

# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /** Save a Milena image into a file using Magick++.
	  \overload

	  \param[in] ima       The image to save.
	  \param[in] filename  The name of the output file.  */
      template <typename I>
      void
      save(const Image<I>& ima, const std::string& filename);

      /** Save a Milena image into a file using Magick++.

	  \param[in] ima       The image to save.

	  \param[in] opacity_mask Mask used to set pixel opacity_mask in output
	  image. Output format must support this feature to be taken
	  into account.

	  \param[in] filename  The name of the output file.
      */
      template <typename I, typename J>
      void
      save(const Image<I>& ima, const Image<J>& opacity_mask,
	   const std::string& filename);


      // FIXME: Unfinished?
#if 0
      /** Save a Milena tiled image into a file using Magick++.

	  \param[out] ima       The image to save.
	  \param[in]  filename  The name of the output file.  */
      template <typename T>
      void
      save(const Image< tiled2d<T> >& ima, const std::string& filename);
#endif


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	inline
	Magick::Color get_color(bool value)
	{
	  return Magick::ColorMono(value);
	}

	inline
	Magick::Color get_color(const value::int_u8& value)
	{
	  /* Each channel of a Magick++ image is coded on a
	     Magick::Quantum value, which can be an 8-, 16- or 32-bit
	     integer.  Store the data from each mln::value::int_u8
	     values into the most significant bits of Magick::Color's
	     channels.  */
	  return Magick::Color
	    (value << 8 * (sizeof(Magick::Quantum) - sizeof(value::int_u8)),
	     value << 8 * (sizeof(Magick::Quantum) - sizeof(value::int_u8)),
	     value << 8 * (sizeof(Magick::Quantum) - sizeof(value::int_u8)));
	}

	inline
	Magick::Color get_color(const value::rgb8& value)
	{
	  /* Each channel of a Magick++ image is coded on a
	     Magick::Quantum value, which can be an 8-, 16- or 32-bit
	     integer.  Store the data from each component of
	     mln::value::rgb8 values (of type mln::value::int_u8) into
	     the most significant bits of Magick::Color's
	     channels.  */
	  return Magick::Color
	    (value.red()   << 8 * (sizeof(Magick::Quantum)
				   - sizeof(value::rgb8::red_t)),
	     value.green() << 8 * (sizeof(Magick::Quantum)
				   - sizeof(value::rgb8::green_t)),
	     value.blue()  << 8 * (sizeof(Magick::Quantum)
				   - sizeof(value::rgb8::blue_t)));
	}


	namespace generic
	{

	  template <typename I>
	  void
	  paste_data(const Image<I>& ima_, Magick::Image& magick_ima)
	  {
	    trace::entering("io::magick::impl::generic::paste_data");

	    const I& ima = exact(ima_);

	    def::coord
	      ncols  = geom::ncols(ima),
	      nrows  = geom::nrows(ima);

	    // Ensure that there is only one reference to underlying image
	    // If this is not done, then image pixels will not be modified.
	    magick_ima.modifyImage();

	    Magick::Pixels view(magick_ima);
	    // As above, `ncols' is passed before `nrows'.
	    Magick::PixelPacket* pixels = view.get(0, 0, ncols, nrows);

	    mln_piter(I) p(ima.domain());
	    for_all(p)
	      *pixels++ = impl::get_color(ima(p));

	    view.sync();

	    trace::exiting("io::magick::impl::generic::paste_data");
	  }

	  template <typename I, typename J>
	  void
	  paste_data_opacity(const Image<I>& ima_,
			     const Image<J>& opacity_mask_,
			     Magick::Image& magick_ima)
	  {
	    trace::entering("io::magick::impl::generic::paste_data_opacity");

	    const I& ima = exact(ima_);
	    const J& opacity_mask = exact(opacity_mask_);

	    def::coord
	      ncols  = geom::ncols(ima),
	      nrows  = geom::nrows(ima);

	    // Ensure that there is only one reference to underlying image
	    // If this is not done, then image pixels will not be modified.
	    magick_ima.modifyImage();

	    Magick::Pixels view(magick_ima);
	    // As above, `ncols' is passed before `nrows'.
	    Magick::PixelPacket* pixels = view.get(0, 0, ncols, nrows);

	    mln_piter(I) p(ima.domain());
	    mln_piter(J) pm(opacity_mask.domain());

	    for_all_2(p, pm)
	    {
	      *pixels = impl::get_color(ima(p));
	      (*pixels).opacity = (opacity_mask(pm) ? 255 : 0);
	      ++pixels;
	    }

	    view.sync();

	    trace::exiting("io::magick::impl::generic::paste_data_opacity");
	  }

	} // end of namespace mln::io::magick::impl::generic


	template <typename I>
	void
	paste_data_fastest(const Image<I>& ima_, Magick::Image& magick_ima)
	{
	  const I& ima = exact(ima_);

	  def::coord
	    minrow = geom::min_row(ima),
	    mincol = geom::min_col(ima),
	    maxrow = geom::max_row(ima),
	    maxcol = geom::max_col(ima),
	    ncols  = geom::ncols(ima),
	    nrows  = geom::nrows(ima);

	  // Ensure that there is only one reference to underlying image
	  // If this is not done, then image pixels will not be modified.
	  magick_ima.modifyImage();

	  Magick::Pixels view(magick_ima);
	  // As above, `ncols' is passed before `nrows'.
	  Magick::PixelPacket* pixels = view.get(0, 0, ncols, nrows);
	  const mln_value(I) *ptr_ima = &ima(ima.domain().pmin());

	  unsigned row_offset = ima.delta_index(dpoint2d(+1, - ncols));

	  for (def::coord row = minrow; row <= maxrow;
	       ++row, ptr_ima += row_offset)
	    for (def::coord col = mincol; col <= maxcol; ++col)
	      *pixels++ = impl::get_color(*ptr_ima++);

	  view.sync();
	}

	template <typename I>
	void
	paste_data_fast(const Image<I>& ima_, Magick::Image& magick_ima)
	{
	  const I& ima = exact(ima_);

	  def::coord
	    ncols  = geom::ncols(ima),
	    nrows  = geom::nrows(ima);

	  // Ensure that there is only one reference to underlying image
	  // If this is not done, then image pixels will not be modified.
	  magick_ima.modifyImage();

	  Magick::Pixels view(magick_ima);
	  // As above, `ncols' is passed before `nrows'.
	  Magick::PixelPacket* pixels = view.get(0, 0, ncols, nrows);


	  fwd_pixter2d<const I> pi(ima);
	  for_all(pi)
	    *pixels++ = impl::get_color(pi.val());

	  view.sync();
	}

	template <typename I, typename J>
	void
	paste_data_opacity_fastest(const Image<I>& ima_,
				   const Image<J>& opacity_mask_,
				   Magick::Image& magick_ima)
	{
	  const I& ima = exact(ima_);
	  const J& opacity_mask = exact(opacity_mask_);

	  def::coord
	    minrow = geom::min_row(ima),
	    mincol = geom::min_col(ima),
	    maxrow = geom::max_row(ima),
	    maxcol = geom::max_col(ima),
	    ncols  = geom::ncols(ima),
	    nrows  = geom::nrows(ima);

	  // Ensure that there is only one reference to underlying image
	  // If this is not done, then image pixels will not be modified.
	  magick_ima.modifyImage();

	  Magick::Pixels view(magick_ima);
	  // As above, `ncols' is passed before `nrows'.
	  Magick::PixelPacket* pixels = view.get(0, 0, ncols, nrows);
	  const mln_value(I) *ptr_ima = &ima(ima.domain().pmin());
	  const mln_value(J) *ptr_opacity_mask = &opacity_mask(opacity_mask.domain().pmin());

	  unsigned row_offset = ima.delta_index(dpoint2d(+1, - ncols));
	  unsigned opacity_row_offset = opacity_mask.delta_index(dpoint2d(+1, - ncols));

	  for (def::coord row = minrow; row <= maxrow;
	       ++row, ptr_ima += row_offset,
		 ptr_opacity_mask += opacity_row_offset)
	    for (def::coord col = mincol; col <= maxcol; ++col)
	    {
	      *pixels = impl::get_color(*ptr_ima++);
	      (*pixels).opacity = ((*ptr_opacity_mask++) ? 255 : 0);
	      ++pixels;
	    }

	  view.sync();
	}




	template <typename I, typename J>
	void
	paste_data_opacity_fast(const Image<I>& ima_,
				const Image<J>& opacity_mask_,
				Magick::Image& magick_ima)
	{
	  const I& ima = exact(ima_);
	  const J& opacity_mask = exact(opacity_mask_);

	  def::coord
	    ncols  = geom::ncols(ima),
	    nrows  = geom::nrows(ima);

	  // Ensure that there is only one reference to underlying image
	  // If this is not done, then image pixels will not be modified.
	  magick_ima.modifyImage();

	  Magick::Pixels view(magick_ima);
	  // As above, `ncols' is passed before `nrows'.
	  Magick::PixelPacket* pixels = view.get(0, 0, ncols, nrows);

	  mln_pixter(const I) pi(ima);
	  mln_pixter(const J) pom(opacity_mask);

	  for_all_2(pi, pom)
	  {
	    *pixels = impl::get_color(pi.val());
	    (*pixels).opacity = (pom.val() ? 255 : 0);
	    ++pixels;
	  }

	  view.sync();
	}

      } // end of namespace mln::io::magick::impl


      namespace internal
      {

	// paste_data_opacity

	template <typename I, typename J>
	void
	paste_data_opacity_fast_dispatch(metal::false_, // No images are fast or fastest.
					 const Image<I>& ima,
					 const Image<J>& opacity_mask,
					 Magick::Image& magick_ima)
	{
	  impl::generic::paste_data_opacity(ima, opacity_mask, magick_ima);
	}

	template <typename I, typename J>
	void
	paste_data_opacity_fast_dispatch(metal::true_, // Bost images are "fast" or one is fast and the other one is fastest.
					 const Image<I>& ima,
					 const Image<J>& opacity_mask,
					 Magick::Image& magick_ima)
	{
	  impl::paste_data_opacity_fast(ima, opacity_mask, magick_ima);
	}

	template <typename I, typename J>
	void
	paste_data_opacity_fastest_dispatch(metal::false_, // At least one of the images is not "fastest".
					    const Image<I>& ima,
					    const Image<J>& opacity_mask,
					    Magick::Image& magick_ima)
	{

	  enum { fast = mlc_and(mlc_is(mln_trait_image_value_storage(I), trait::image::value_storage::one_block),
				mlc_is(mln_trait_image_value_storage(J), trait::image::value_storage::one_block))::value };
	  paste_data_opacity_fast_dispatch(metal::bool_<fast>(),
					   ima, opacity_mask, magick_ima);
	}


	template <typename I, typename J>
	void
	paste_data_opacity_fastest_dispatch(metal::true_, // Both images are "fastest".
					    const Image<I>& ima,
					    const Image<J>& opacity_mask,
					    Magick::Image& magick_ima)
	{
	  impl::paste_data_opacity_fastest(ima, opacity_mask, magick_ima);
	}

	template <typename I, typename J>
	void
	paste_data_opacity_dispatch(const Image<I>& ima,
				    const Image<J>& opacity_mask,
				    Magick::Image& magick_ima)
	{
	  enum { fastest = mlc_and(mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest),
				   mlc_is(mln_trait_image_speed(J), trait::image::speed::fastest))::value };
	  paste_data_opacity_fastest_dispatch(metal::bool_<fastest>(),
					      ima, opacity_mask, magick_ima);
	}


	// paste_data


	template <typename I>
	void
	paste_data_dispatch_fast(const mln::trait::image::value_storage::any&,
			    const Image<I>& ima, Magick::Image& magick_ima)
	{
	  impl::generic::paste_data(ima, magick_ima);
	}


	template <typename I>
	void
	paste_data_dispatch_fast(const mln::trait::image::value_storage::one_block&,
			    const Image<I>& ima, Magick::Image& magick_ima)
	{
	  impl::paste_data_fast(ima, magick_ima);
	}


	template <typename I>
	void
	paste_data_dispatch_fastest(const mln::trait::image::speed::any&,
			    const Image<I>& ima, Magick::Image& magick_ima)
	{
	  paste_data_dispatch_fast(mln_trait_image_value_storage(I)(), ima, magick_ima);
	}


	template <typename I>
	void
	paste_data_dispatch_fastest(const mln::trait::image::speed::fastest&,
				    const Image<I>& ima, Magick::Image& magick_ima)
	{
	  impl::paste_data_fastest(ima, magick_ima);
	}

	template <typename I>
	void
	paste_data_dispatch(const Image<I>& ima, Magick::Image& magick_ima)
	{
	  paste_data_dispatch_fastest(mln_trait_image_speed(I)(), ima, magick_ima);
	}

      } // end of namespace mln::io::magick::internal


      template <typename I, typename J>
      void
      save(const Image<I>& ima_, const Image<J>& opacity_mask_,
	   const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	mln_precondition(mln_site_(I)::dim == 2);
	// Turn this into a static check?
	if (!(mln::metal::equal<mln_value(I), bool>::value ||
	    mln::metal::equal<mln_value(I), value::int_u8>::value ||
	    mln::metal::equal<mln_value(I), value::rgb8>::value ||
	      mln::metal::equal<mln_value(I), value::qt::rgb32>::value))
	{
	  std::cerr <<
	    "error: trying to save an unsupported format\n"
	    "supported formats are:\n"
	    "  binary (bool)\n"
	    "  8-bit grayscale (mln::value::int_u8)\n"
	    "  3x8-bit truecolor (rgb8)" << std::endl;
	  abort();
	}

	const I& ima = exact(ima_);
	const J& opacity_mask = exact(opacity_mask_);

	def::coord
	  ncols  = geom::ncols(ima),
	  nrows  = geom::nrows(ima);


	// In the construction of a Geometry object, the width (i.e.
	// `ncols') comes first, then the height (i.e. `nrows')
	// follows.
	//
	// FIXME: Default pixel value is set to "white". If the image is
	// declared with the default constructor, without specifying a
	// default value, no data seems to be allocated and the Pixel view
	// declared further fails and segfault...
	Magick::Image magick_ima(Magick::Geometry(ncols, nrows), "white");

	if (opacity_mask.is_valid())
	{
	  magick_ima.type(Magick::TrueColorMatteType);
	  internal::paste_data_opacity_dispatch(ima, opacity_mask, magick_ima);
	}
	else
	{
	  magick_ima.type(Magick::TrueColorType);
	  internal::paste_data_dispatch(ima, magick_ima);
	}

	magick_ima.write(filename);

	trace::exiting("mln::io::magick::save");
      }



      template <typename I>
      inline
      void
      save(const Image<I>& ima, const std::string& filename)
      {
	mln_ch_value(I,bool) opacity_mask;
	save(ima, opacity_mask, filename);
      }


      // FIXME: Unfinished?
#if 0
      template <typename T>
      void
      save(const Image< tiled2d<T> >& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	tiled2d<T>& ima = exact(ima_);

	ima.buffer().write(filename);

	trace::exiting("mln::io::magick::save");
      }
#endif


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_SAVE_HH
