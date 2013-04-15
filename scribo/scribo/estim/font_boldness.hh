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

#ifndef SCRIBO_ESTIM_FONT_BOLDNESS_HH
# define SCRIBO_ESTIM_FONT_BOLDNESS_HH

/// \file
///
/// \brief Estimate the font color.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/labeling/blobs.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/value/next.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/def/lbl_type.hh>
# include <scribo/estim/internal/compute_skeleton.hh>


namespace scribo
{

  namespace estim
  {

    using namespace mln;

    /// \brief Estimate the font color.
    //
    template <typename I>
    float
    font_boldness(const Image<I>& text_ima);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      template <typename I, typename J>
      float
      boldness_from_lbl(const Image<I>& ima_lbl_,
			 const Image<J>& skel_)
      {
	mln_trace("scribo::estim::internal::boldness_from_lbl");

	const I& ima_lbl = exact(ima_lbl_);
	const J& skel = exact(skel_);
	mln_precondition(ima_lbl.is_valid());
	mln_precondition(skel.is_valid());

	mln_piter(I) p(ima_lbl.domain());
	int skel_p = 0, ima_p = 0;

	for_all(p)
	{
	  if (skel(p))
	    ++skel_p;

	  if (ima_lbl(p))
	    ++ima_p;
	}

	float res = ima_p / (float) skel_p;

	return res;
      }


      // FIXME: this version is invalid!!!  It supposes that image and
      // skeleton labeling associate the same id to a component and
      // its corresponding skeleton. This is wrong!
      template <typename I, typename J>
      float
      boldness_from_lbl(const Image<I>& ima_lbl_,
			 const mln_value(I)& nlabels,
			 const Image<J>& skel_lbl_)
      {
	mln_trace("scribo::estim::internal::boldness_from_lbl");

	const I& ima_lbl = exact(ima_lbl_);
	const J& skel_lbl = exact(skel_lbl_);
	mln_precondition(ima_lbl.is_valid());
	mln_precondition(skel_lbl.is_valid());

	mln_piter(I) p(ima_lbl.domain());
	std::vector<int> skel_p(value::next(nlabels), 0);
	std::vector<int> ima_p(value::next(nlabels), 0);

	for_all(p)
	{
	  int val = skel_lbl(p);

	  if (val)
	    ++skel_p[val];

	  val = ima_lbl(p);

	  if (val)
	    ++ima_p[val];
	}

	accu::stat::mean<float> mean;
	for_all_ncomponents(i, nlabels)
	  mean.take(ima_p[i] / skel_p[i]);

	// double variance = 0.;
	// for (unsigned int i = 0; i < nlabels; ++i)
	//   variance += ((ima_p[i] / skel_p[i]) - mean.to_result()) * ((ima_p[i] / skel_p[i]) - mean.to_result());

	// variance /= nlabels;

	return mean.to_result();
      }

    } // end of namespace scribo::estim::internal


    // IMPLEMENTATIONS

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	float
	font_boldness(const Image<I>& ima_)
	{
	  mln_trace("scribo::estim::impl::generic::font_boldness");

	  const I& ima = exact(ima_);
	  mln_precondition(ima.is_valid());
	  mlc_is(mln_value(I), bool)::check();

	  typedef scribo::def::lbl_type V;
	  typedef mln_ch_value(I,V) L;

	  mln_concrete(I) skel = internal::compute_skeleton(ima);

	  V nslabels;
	  L slbl = labeling::blobs(skel, c8(), nslabels);

	  V nlabels;
	  L lbl = labeling::blobs(ima, c8(), nlabels);

	  float
	    output = internal::boldness_from_lbl(lbl, nlabels, slbl);

	  return output;
	}

      } // end of namespace scribo::estim::generic::impl

    } // end of namespace scribo::estim::impl



    // FACADE

    template <typename I>
    float
    font_boldness(const Image<I>& ima)
    {
      mln_trace("scribo::estim::font_boldness");

      mln_precondition(exact(ima).is_valid());
      mlc_is(mln_value(I), bool)::check();

      float output = impl::generic::font_boldness(ima);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::estim

} // end of namespace scribo

# endif // ! SCRIBO_ESTIM_FONT_BOLDNESS_HH



