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

#ifndef SCRIBO_BINARIZATION_OTSU_THRESHOLD_HH
# define SCRIBO_BINARIZATION_OTSU_THRESHOLD_HH

# include <mln/core/concept/image.hh>
# include <mln/util/array.hh>
# include <mln/geom/nsites.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>
# include <mln/histo/compute.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! Compute a global binarization threshold using Otsu's algorithm.

      It is based on Ocropus's implementation.
      http://code.google.com/p/ocropus/

      \param[in] input A gray-scale image.

      \return A global threshold value.
     */
    template <typename I>
    mln_value(I)
    otsu_threshold(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_value(I)
    otsu_threshold(const Image<I>& input_)
    {
      trace::entering("scribo::binarization::otsu_threshold");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mlc_is_a(mln_value(I), value::Scalar)::check();
      // FIXME: Check that input value is gray level.

      mln_value(I) maxval = mln_max(mln_value(I));
      unsigned nsites = geom::nsites(input);

      /* Histogram generation */
      histo::array<mln_value(I)> hist = mln::histo::compute(input);


      /* calculation of probability density */
      util::array<double> pdf(hist.nvalues());  //probability distribution
      for(int i = 0; i< maxval; ++i)
	pdf[i] = (double)hist[i] / nsites;


      util::array<double> cdf(hist.nvalues()); //cumulative probability distribution
      util::array<double> myu(hist.nvalues()); // mean value for separation

      /* cdf & myu generation */
      cdf[0] = pdf[0];
      myu[0] = 0.0;       /* 0.0 times prob[0] equals zero */

      for(int i = 1; i < maxval; ++i)
      {
	cdf[i] = cdf[i-1] + pdf[i];
	myu[i] = myu[i-1] + i*pdf[i];
      }

      /* sigma maximization
	 sigma stands for inter-class variance
	 and determines optimal threshold value */
      mln_value(I) threshold = literal::zero;
      double max_sigma = 0.0;
      util::array<double> sigma(hist.nvalues()); // inter-class variance

      for(int i = 0; i < maxval - 1; ++i)
      {
	if(cdf[i] != 0.0 && cdf[i] != 1.0)
	{
	  double p1p2 = cdf[i] * (1.0 - cdf[i]);
	  double mu1mu2diff = myu[maxval - 1] * cdf[i] - myu[i];
	  sigma[i] = mu1mu2diff * mu1mu2diff / p1p2;
	}
	else
	  sigma[i] = 0.0;
	if(sigma[i] > max_sigma)
	{
	  max_sigma = sigma[i];
	  threshold = i;
	}
      }

      trace::exiting("scribo::binarization::otsu_threshold");
      return threshold;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_OTSU_THRESHOLD_HH
