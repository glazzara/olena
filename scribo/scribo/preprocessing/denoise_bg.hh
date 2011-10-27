// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PREPROCESSING_DENOISE_BG_HH
# define SCRIBO_PREPROCESSING_DENOISE_BG_HH

/// \file
///
/// \brief Denoise image background.

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>
# include <mln/data/transform.hh>

# include <mln/accu/math/count.hh>

# include <mln/core/alias/neighb2d.hh>

# include <mln/util/array.hh>
# include <mln/fun/i2v/array.hh>

# include <mln/labeling/compute.hh>
# include <mln/labeling/background.hh>

# include <scribo/fun/v2b/label_to_bool.hh>


namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;

    /// \brief Denoise image background.
    ///
    /// \param[in] input    A binary image. True for objects, False for
    ///                     background.
    /// \param[in] nbh      Neighborhood to use for denoising.
    /// \param[in] min_card Minimum component cardinality to not be
    ///                     considered as noise.
    ///
    /// \return A binary image with the same domain as \p input. All
    /// small components have been removed and merged with the
    /// background.
    //
    template <typename I, typename N>
    mln_concrete(I)
    denoise_bg(const Image<I>& input, const Neighborhood<N>& nbh,
	       unsigned min_card);


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY

namespace mln
{


  template <typename L>
  struct label_to_bool : Function_v2b<label_to_bool<L> >
  {
    typedef bool result;

    label_to_bool(const fun::i2v::array<bool>& f)
      : f_(f)
    {}

    bool operator()(const L& v) const
    {
      return f_(v);
    }

    fun::i2v::array<bool> f_;
  };


} // end of namespace mln



namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;


    template <typename I, typename N>
    mln_concrete(I)
    denoise_bg(const Image<I>& input_, const Neighborhood<N>& nbh_,
	       unsigned min_card)
    {
      trace::entering("scribo::preprocessing::denoise_bg");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      unsigned nlabels;
      image2d<unsigned> lbl = labeling::background(input, nbh, nlabels);

      mln::util::array<unsigned>
	result = labeling::compute(accu::meta::math::count(), lbl, nlabels);

      mln::fun::i2v::array<bool> f(nlabels + 1, false);
      f(0) = true;
      for (unsigned i = 1; i <= nlabels; ++i)
	if (result(i) < min_card)
	  f(i) = true;

      scribo::fun::v2b::label_to_bool<unsigned> relabel_f(f);
      mln_concrete(I)
	output = data::transform(lbl, relabel_f);

      trace::exiting("scribo::preprocessing::denoise_bg");
      return output;
    }


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

# endif // SCRIBO_PREPROCESSING_DENOISE_BG_HH
