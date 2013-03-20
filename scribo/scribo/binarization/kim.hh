// Copyright (C) 2012, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_BINARIZATION_KIM_HH
# define SCRIBO_BINARIZATION_KIM_HH

/// \file
///
///

#include <cmath>
#include <map>
#include <scribo/binarization/sauvola.hh>
#include <scribo/util/integral_sum_sum2_functor.hh>
#include <scribo/text/extract_lines.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! \brief Kim's binarization

      \copydetails grpalgobinkim

      \param[in] input A gray-level image.
      \param[in] window_size the window size to be used for the first
      binarization.
      \param[in] k Sauvola's formula parameter.

      \ingroup grpalgobinkim
     */
    template <typename I>
    mln_ch_value(I, bool)
    kim(const Image<I>& input, unsigned window_size, double k);

    /*! \overload
      k is set to SCRIBO_DEFAULT_SAUVOLA_K.

      \ingroup grpalgobinkim
     */
    template <typename I>
    mln_ch_value(I, bool)
    kim(const Image<I>& input, unsigned window_size);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      double
      compute_thres(const mln::image2d<mln::util::couple<double,double> >& integral_sum_sum_2,
		    int row, int col, unsigned win_size,
		    const scribo::binarization::internal::sauvola_formula& formula)
      {
	point2d
	  tl(row - win_size - 1,
	     col - win_size - 1),
	  br(row + win_size,
	     col + win_size);

	box2d b(tl, br);
	b.crop_wrt(integral_sum_sum_2.domain());

	point2d tr = b.pmax();
	tr.row() = b.pmin().row();
	point2d bl = b.pmin();
	bl.row() = b.pmax().row();

	unsigned card_min = b.nsites() - b.height() - b.width() + 1;

	const mln::util::couple<double,double>&
	  D = integral_sum_sum_2(b.pmax()),
	  B = integral_sum_sum_2(tr),
	  C = integral_sum_sum_2(bl),
	  A = integral_sum_sum_2(b.pmin());

	double sum = D.first() - B.first() - C.first() + A.first();
	double sum_2 = D.second() - B.second() - C.second() + A.second();
	double mean = sum / card_min;

	double num = (sum_2 - sum * sum / card_min);
	double stddev;
	if (num > 0)
	  stddev = std::sqrt(num / (card_min - 1));
	else
	  stddev = 0;

	return formula(mean, stddev);
      }

    } // end of namespace scribo::binarization::internal


    template <typename I>
    mln_ch_value(I, bool)
    kim(const Image<I>& input_, unsigned window_size, double k)
    {
      mln_trace("scribo::binarization::kim");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());

      // 1st simple binarization
      image2d<bool>
	output = scribo::binarization::sauvola(input, window_size, k);

      // Compute integral image
      scribo::util::integral_sum_sum2_functor<mln_value(I),double> f_sum_sum2;
      image2d<mln::util::couple<double,double> >
	integral_sum_sum_2 = scribo::util::init_integral_image(input, 1, f_sum_sum2);

      // Find text lines
      line_set<image2d<scribo::def::lbl_type> >
	lines = scribo::text::extract_lines(output, c8());

      typedef scribo::def::lbl_type V;
      typedef image2d<V> L;
      mln::util::array<unsigned> thickness(lines.nelements() + 1, 0);
      const component_set<L>& comp_set = lines.components();
      const L& lbl = comp_set.labeled_image();

      // Compute run-lengths histogram in order to compute character
      // thickness for each line.
      for_all_lines(i, lines)
      {
	if (!lines(i).is_textline())
	  continue;

	std::map<unsigned, unsigned> histo;
	int count = 0;
	for (int l = lines(i).bbox().pmin().row(); l <= lines(i).bbox().pmax().row(); ++l)
	{
	  const V* end_ptr = &lbl.at_(l, lines(i).bbox().pmax().col() + 1);
	  for (const V* run_ptr = &lbl.at_(l, lines(i).bbox().pmin().col()); run_ptr != end_ptr; ++run_ptr)
	    if (*run_ptr)
	      ++count;
	    else
	      if (count)
	      {
		if (histo.find(count) != histo.end())
		  histo[count]++;
		else
		  histo.insert(std::make_pair(count, 1));
		count = 0;
	      }
	}

	unsigned max = 0;
	unsigned thick = 0;
	for (std::map<unsigned, unsigned>::const_iterator it = histo.begin(); it != histo.end(); ++it)
	  if (it->second > max)
	  {
	    max = it->second;
	    thick = it->first;
	  }

	thickness(lines(i).id()) = thick;
      }

      // Compute thresholds for each pixel of each line and binarize again!
      for_all_lines(i, lines)
      {
	if (!lines(i).is_textline())
	  continue;

	double
	  win_min = thickness(lines(i).id()),
	  win_max = lines(i).bbox().height();;

	mln_assertion(win_min != 0);
	mln_assertion(win_max != 0);

	scribo::binarization::internal::sauvola_formula formula;

	double teta = 0.3; // Good results from 0.1 to 0.3 according
			   // to the paper.
	for (int row = lines(i).bbox().pmin().row();
	     row <= lines(i).bbox().pmax().row();
	     ++row)
	{
	  bool* out_ptr = &output.at_(row, lines(i).bbox().pmin().col());
	  const mln_value(I)* in_ptr = &input.at_(row, lines(i).bbox().pmin().col());
	  for (int col = lines(i).bbox().pmin().col();
	       col <= lines(i).bbox().pmax().col();
	       ++col)
	  {
	    // Min case
	    double T_min = compute_thres(integral_sum_sum_2, row, col, win_min, formula);

	    // Max case
	    double T_max = compute_thres(integral_sum_sum_2, row, col, win_max, formula);

	    // Final threshold
	    double T = teta * T_max + (1 - teta) * T_min;

	    mln_assertion(T_min <= 255);
	    mln_assertion(T_max <= 255);
	    mln_assertion(T <= 255);

	    *out_ptr++ = *in_ptr++ <= T;
	  }
	}
      }

      return output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    kim(const Image<I>& input, unsigned window_size)
    {
      return kim(input, window_size, SCRIBO_DEFAULT_SAUVOLA_K);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_KIM_HH
