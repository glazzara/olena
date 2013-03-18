// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_LABELING_FILL_HOLES_HH
# define MLN_LABELING_FILL_HOLES_HH

/// \file
///
/// Filling holes of a single object in a binary image.

# include <mln/labeling/background.hh>
# include <mln/labeling/compute.hh>

# include <mln/data/transform.hh>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/accu/math/count.hh>


namespace mln
{

  namespace labeling
  {

    /// Filling holes of a single object in a binary image.
    ///
    /// \param[in]  input   The input image.
    /// \param[in]  nbh     The connexity of the background.
    /// \param[out] nlabels The number of labels.
    /// \return The binary image with a simple object without holes.
    ///
    /// \pre The input image has to be binary (checked at compile-time).
    ///
    /// This routine actually calls mln::labeling::background
    ///
    /// \see mln::labeling::background
    ///
    template <typename I, typename N, typename L>
    mln_concrete(I)
    fill_holes(const Image<I>& input, const Neighborhood<N>& nbh,
	       L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename L>
    inline
    mln_concrete(I)
    fill_holes(const Image<I>& input, const Neighborhood<N>& nbh,
	       L& nlabels)
    {
      mln_trace("labeling::fill_holes");

      mlc_equal(mln_trait_image_kind(I),
		mln::trait::image::kind::binary)::check();
      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());

      mln_ch_value(I, L) lbls = labeling::background(input, nbh, nlabels);

      accu::math::count<mln_value(I)> a_;
      util::array<unsigned> arr = labeling::compute(a_, input, lbls, nlabels);

      unsigned bg_count = 0;
      unsigned bg_lbl = 0;

      // We start at 1 to ignore the object.
      for (unsigned i = 1; i < arr.nelements(); ++i)
      {
	if (arr[i] > bg_count)
	{
	  bg_count = arr[i];
	  bg_lbl = i;
	}
      }

      util::array<bool> bg_relbl(arr.nelements(), true);
      bg_relbl(bg_lbl) = false;
      mln_ch_value(I, bool) output = data::transform(lbls, bg_relbl);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_FILL_HOLES_HH
