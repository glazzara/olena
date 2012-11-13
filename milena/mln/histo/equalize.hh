// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_HISTO_EQUALIZE_HH
# define MLN_HISTO_EQUALIZE_HH

# include <cmath>
# include <mln/core/concept/image.hh>
# include <mln/histo/all.hh>

/// \file
///
/// Histogram equalization.

namespace mln
{

  namespace histo
  {

    /*! \brief Equalizes the histogram of image \p input.

     \author J. Fabrizio, R. Levillain

     Source:
     http://en.wikipedia.org/wiki/Histogram_equalization
     */
    template <typename I>
    mln_concrete(I) equalize(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    mln_concrete(I)
    equalize(const Image<I>& input_)
    {
      trace::entering("mln::histo::equalize");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      typedef mln_value(I) V;
      array<V> histogram = compute(input);
      array<V> histo_correction;

      unsigned nsites = input.nsites();
      V max_color = mln_max(V);

      unsigned h_min = nsites;
      mln_viter(mln::value::set<V>) v(histogram.vset());

      // Looking for minimum occurence in histogram
      for_all(v)
	if (histogram(v) > 0 && h_min > histogram(v))
	  h_min = histogram(v);

      // Computing new histogram.
      unsigned cdf_v = 0;
      for_all(v)
	if (histogram(v) != 0)
	{
	  cdf_v += histogram(v);
	  histo_correction(v)
	    = round((cdf_v - h_min) / (float)(nsites - h_min) * max_color);
	}

      mln_concrete(I) output;
      initialize(output, input);
      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = histo_correction[input(p)];

      trace::exiting("mln::histo::equalize");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::histo

} // end of namespace mln



#endif // ! MLN_HISTO_EQUALIZE_HH
