// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DATA_APPROX_MEDIAN_HH
# define MLN_DATA_APPROX_MEDIAN_HH

/// \file
///
/// Approximates of some median filters of an image.

# include <mln/data/median.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/win/disk2d.hh>
# include <mln/win/octagon2d.hh>
# include <mln/win/hline2d.hh>
# include <mln/win/vline2d.hh>
# include <mln/win/diag2d.hh>
# include <mln/win/backdiag2d.hh>

#include <time.h>


namespace mln
{

  namespace data
  {

    namespace approx
    {

      /*! Compute in \p output an approximate of the median filter of
       *  image \p input by the 2D rectangle \p win.
       *
       * \param[in] input The image to be filtered.
       * \param[in] win The rectangle.
       *
       * The approximation is based on a vertical median ran after
       * an horizontal median.
       *
       * \pre \p input and \p output have to be initialized.
       */
      template <typename I>
      mln_concrete(I)
      median(const Image<I>& input, const win::rectangle2d& win);


      /*! Compute in \p output an approximate of the median filter of
       *  image \p input by the 2D disk \p win.
       *
       * \param[in] input The image to be filtered.
       * \param[in] win The disk.
       *
       * The approximation is based on a vertical median and
       * an horizontal median an two diagonal median.
       *
       * \pre \p input and \p output have to be initialized.
       */
      template <typename I>
      mln_concrete(I)
      median(const Image<I>& input, const win::disk2d& win);


      /*! Compute in \p output an approximate of the median filter of
       *  image \p input by the 2D octagon \p win.
       *
       * \param[in] input The image to be filtered.
       * \param[in] win The octagon.
       *
       * The approximation is based on a vertical median and
       * an horizontal median an two diagonal median.
       *
       * \pre \p input and \p output have to be initialized.
       */
      template <typename I>
      mln_concrete(I)
      median(const Image<I>& input, const win::octagon2d& win);



# ifndef MLN_INCLUDE_ONLY


      // Facades.


      template <typename I>
      inline
      mln_concrete(I)
      median(const Image<I>& input, const win::rectangle2d& win)
      {
	mln_trace("data::approx::median");

	mln_concrete(I) output;

	win::hline2d win1(win.width());
	output = data::median(input, win1);

	win::vline2d win2(win.height());
	output = data::median(output, win2);

	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      median(const Image<I>& input, const win::disk2d& win)
      {
	mln_trace("data::approx::median");

	const unsigned len = win.diameter() / 3 + 1;
	mln_concrete(I) output;

	win::diag2d win1(len);
	output = data::median(input, win1);

	win::backdiag2d win2(len);
	output = data::median(output, win2);

	win::hline2d win3(len);
	output = data::median(input, win3);

	win::vline2d win4(len);
	output = data::median(output, win4);

	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      median(const Image<I>& input, const win::octagon2d& win)
      {
	return median(input, win::disk2d(win.length()));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::data::approx

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_APPROX_MEDIAN_HH
