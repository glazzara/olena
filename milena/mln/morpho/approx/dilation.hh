// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MORPHO_APPROX_DILATION_HH
# define MLN_MORPHO_APPROX_DILATION_HH

/// \file
///
/// Approximate mathematical morphology dilation.

# include <mln/core/concept/image.hh>

# include <mln/core/routine/duplicate.hh>
# include <mln/data/compare.hh>

# include <mln/transform/distance_front.hh>
# include <mln/pw/all.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/make/w_window2d_int.hh>
# include <mln/win/disk2d.hh>

# include <mln/core/alias/neighb3d.hh>
# include <mln/make/w_window3d_int.hh>
# include <mln/win/sphere3d.hh>



namespace mln
{

  namespace morpho
  {

    namespace approx
    {

      /*! \brief Approximate mathematical morphology dilation.
       *
       * \ingroup mlnmorpho
       */
      template <typename I, typename W>
      mln_concrete(I)
      dilation(const Image<I>& input, const Window<W>& win);



# ifndef MLN_INCLUDE_ONLY


      // Implementations.

      namespace impl
      {


	// By distance thresholding.

	template <typename I>
	mln_concrete(I)
	dilation_by_distance_thresholding_2d(const Image<I>& input_,
					     const Window< win::disk2d >& win_)
	{
	  trace::entering("morpho::approx::impl::dilation_by_distance_thresholding_2d");

	  const I& input         = exact(input_);
	  const win::disk2d& win = exact(win_);

	  mln_precondition(input.is_valid());
	  mln_precondition(win.is_valid());

	  int ws[] = { 00, 11,  0, 11,  0,
		       11,  7,  5,  7, 11,
		       00,  5,  0,  5,  0,
		       11,  7,  5,  7, 11,
		       00, 11,  0, 11,  0 };
	  const unsigned coef = 5;

	  unsigned
	    radius = coef * win.diameter() / 2,
	    dmax   = radius + 1;

	  mln_ch_value(I, unsigned) dmap = transform::distance_front(input,
								     c4(), make::w_window2d_int(ws),
								     dmax);
	  mln_concrete(I) output;
	  output = duplicate((pw::value(dmap) <= pw::cst(radius)) | input.domain());

	  trace::exiting("morpho::approx::impl::dilation_by_distance_thresholding_2d");
	  return output;
	}



	template <typename I>
	mln_concrete(I)
	dilation_by_distance_thresholding_3d(const Image<I>& input_,
					     const Window< win::sphere3d >& win_)
	{
	  trace::entering("morpho::approx::impl::dilation_by_distance_thresholding_3d");

	  const I& input           = exact(input_);
	  const win::sphere3d& win = exact(win_);

	  mln_precondition(input.is_valid());
	  mln_precondition(win.is_valid());

	  int ws[] = { 00, 21, 00,
		       21, 17, 21,
		       00, 21, 00,

		       17, 12, 17,
		       12, 00, 12,
		       17, 12, 17,

		       00, 21, 00,
		       21, 17, 21,
		       00, 21, 00 };
	  const unsigned coef = 12;

	  unsigned
	    radius = coef * win.diameter() / 2,
	    dmax   = radius + 1;

	  mln_ch_value(I, unsigned) dmap = transform::distance_front(input,
								     c6(), make::w_window3d_int(ws),
								     dmax);
	  mln_concrete(I) output;
	  output = duplicate((pw::value(dmap) <= pw::cst(radius)) | input.domain());

	  trace::exiting("morpho::approx::impl::dilation_by_distance_thresholding_3d");
	  return output;
	}


      } // end of namespace mln::morpho::approx::impl



      // Dispatch.

      namespace internal
      {

	template <typename I>
	mln_concrete(I)
	dilation_dispatch(trait::image::kind::logic,
			  const I& input,
			  const win::disk2d& win)
	{
	  return impl::dilation_by_distance_thresholding_2d(input, win);
	}

	template <typename I>
	mln_concrete(I)
	dilation_dispatch(trait::image::kind::logic,
			  const I& input,
			  const win::sphere3d& win)
	{
	  return impl::dilation_by_distance_thresholding_3d(input, win);
	}

	// Entry point.

	template <typename I, typename W>
	mln_concrete(I)
	dilation_dispatch(const I& input, const W& win)
	{
	  return dilation_dispatch(mln_trait_image_kind(I)(),
				   input, win);
	}

      } // end of namespace mln::morpho::approx::internal


      // Facade.

      template <typename I, typename W>
      inline
      mln_concrete(I)
      dilation(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::approx::dilation");

	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(win).is_valid());

	mln_concrete(I) output;
	output = internal::dilation_dispatch(exact(input), exact(win));

	if (exact(win).is_centered())
	  mln_postcondition(output >= input);

	trace::exiting("morpho::approx::dilation");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::approx

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_APPROX_DILATION_HH
