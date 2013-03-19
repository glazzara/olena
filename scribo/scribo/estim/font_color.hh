// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_ESTIM_FONT_COLOR_HH
# define SCRIBO_ESTIM_FONT_COLOR_HH

/// \file
///
/// \brief Estimate the font color.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/accu/stat/median_h.hh>
# include <mln/value/int_u12.hh>
# include <mln/value/rgb8.hh>

# include <scribo/estim/internal/compute_skeleton.hh>


namespace scribo
{

  namespace estim
  {

    using namespace mln;

    /// \brief Estimate the font color.
    //
    template <typename I, typename J>
    mln_value(I)
    font_color(const Image<I>& text_ima, const Image<J>& bin_text_ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      mln_value(I)
      font_color_rgb(const Image<I>& text_ima_,
		     const Image<J>& bin_text_ima_)
      {
	mln_trace("scribo::estim::impl::font_color_rgb");

	typedef mln_value(I) V1;
	typedef mln_value(J) V2;
	//mlc_is(V1, value::rgb)::check();
	mlc_is(V2, bool)::check();

	const I& text_ima = exact(text_ima_);
	const J& bin_text_ima = exact(bin_text_ima_);

	mln_concrete(J) skel = internal::compute_skeleton(bin_text_ima);

	value::int_u8 nlabels = 0;
	mln_ch_value(I,value::int_u8)
	  lbl = labeling::blobs(skel, c8(), nlabels);

	mln::util::array<algebra::vec<3u, float> > res =
	  labeling::compute(accu::meta::stat::mean(), text_ima, lbl, nlabels);

	accu::stat::median_h<value::int_u12>	m_red;
	accu::stat::median_h<value::int_u12>	m_green;
	accu::stat::median_h<value::int_u12>	m_blue;
	m_red.init();
	m_green.init();
	m_blue.init();

	mln_value(I) med_rgb;

	for (unsigned int i = 1; i <= nlabels; ++i)
	{
	  m_red.take(res[i][0]);
	  m_green.take(res[i][1]);
	  m_blue.take(res[i][2]);
	}

	med_rgb.red() = m_red.to_result();
	med_rgb.green() = m_green.to_result();
	med_rgb.blue() = m_blue.to_result();

	return med_rgb;
      }


      template <typename I, typename J>
      mln_value(I)
      font_color_grayscale(const Image<I>& text_ima_,
			   const Image<J>& bin_text_ima_)
      {
	mln_trace("scribo::estim::impl::font_color_grayscale");

	typedef mln_value(I) V1;
	typedef mln_value(J) V2;
	//mlc_is(V1, value::rgb)::check();
	mlc_is(V2, bool)::check();

	const I& text_ima = exact(text_ima_);
	const J& bin_text_ima = exact(bin_text_ima_);

	mln_concrete(J) skel = internal::compute_skeleton(bin_text_ima);

	value::int_u8 nlabels = 0;
	mln_ch_value(J,value::int_u8) lbl = labeling::blobs(skel, c8(), nlabels);

	mln::util::array<algebra::vec<3u, float> > res =
	  labeling::compute(accu::meta::stat::mean(), text_ima, lbl, nlabels);

	accu::stat::median_h<value::int_u12>	m_val;
	m_val.init();

	mln_value(I) med_val;
	for (unsigned int i = 1; i <= nlabels; ++i)
	  m_val.take(res[i][0]);

	med_val = m_val.to_result();

	return med_val;
      }

    } // end of namespace scribo::estim::impl


    // DISPATCH

    namespace internal
    {

      template <typename I, typename J>
      mln_value(I)
      font_color_dispatch(const value::rgb8&,
			  const Image<I>& text_ima, const Image<J>& bin_text_ima)
      {
	return impl::font_color_rgb(exact(text_ima), exact(bin_text_ima));
      }


      template <typename V, typename I, typename J>
      mln_value(I)
      font_color_dispatch(const V&,
			  const Image<I>& text_ima, const Image<J>& bin_text_ima)
      {
	return impl::font_color_grayscale(exact(text_ima), exact(bin_text_ima));
      }


      template <typename I, typename J>
      mln_value(I)
      font_color_dispatch(const Image<I>& text_ima, const Image<J>& bin_text_ima)
      {
	return font_color_dispatch(mln_value(I)(), text_ima, bin_text_ima);
      }

    } // end of namespace scribo::estim::internal


    // FACADE

    template <typename I, typename J>
    mln_value(I)
    font_color(const Image<I>& text_ima, const Image<J>& bin_text_ima)
    {
      mln_trace("scribo::estim::font_color");

      mln_value(I) output = internal::font_color_dispatch(text_ima, bin_text_ima);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::estim

} // end of namespace scribo

# endif // ! SCRIBO_ESTIM_FONT_COLOR_HH
