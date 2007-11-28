// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_LEVEL_APPROX_MEDIAN_HH
# define MLN_LEVEL_APPROX_MEDIAN_HH

/*! \file mln/level/approx/median.hh
 *
 * \brief Approximates of some median filters of an image.
 */

# include <mln/level/median.hh>
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

  namespace level
  {

    namespace approx
    {

      /*! Compute in \p output an approximate of the median filter of
       *  image \p input by the 2D rectangle \p win.
       *
       * \param[in] input The image to be filtered.
       * \param[in] win The rectangle.
       * \param[in,out] output The output image.
       *
       * The approximation is based on a vertical median ran after
       * an horizontal median.
       *
       * \pre \p input and \p output have to be initialized.
       */
      template <typename I, typename O>
      void median(const Image<I>& input, const win::rectangle2d& win,
		  Image<O>& output);

      /*! Compute in \p output an approximate of the median filter of
       *  image \p input by the 2D disk \p win.
       *
       * \param[in] input The image to be filtered.
       * \param[in] win The disk.
       * \param[in,out] output The output image.
       *
       * The approximation is based on a vertical median and
       * an horizontal median an two diagonal median.
       *
       * \pre \p input and \p output have to be initialized.
       */
      template <typename I, typename O>
      void median(const Image<I>& input, const win::disk2d& win,
		  Image<O>& output);

      /*! Compute in \p output an approximate of the median filter of
       *  image \p input by the 2D octagon \p win.
       *
       * \param[in] input The image to be filtered.
       * \param[in] win The octagon.
       * \param[in,out] output The output image.
       *
       * The approximation is based on a vertical median and
       * an horizontal median an two diagonal median.
       *
       * \pre \p input and \p output have to be initialized.
       */
      template <typename I, typename O>
      void median(const Image<I>& input, const win::octagon2d& win,
		  Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	template <typename I, typename O>
	inline
	void median_(const I& input, const unsigned length,
		     O& output)
	{
	  const unsigned len = length / 3 + 1;

	  O
	    tmp1(output.domain()),
	    tmp2(output.domain());
	  {
	    clock_t c = clock();
	    level::median(input, win::diag2d(len),  tmp1);
	    std::cout << float(clock() - c) / CLOCKS_PER_SEC << std::endl;
	  }
	  {
	    clock_t c = clock();
	    level::median(tmp1, win::backdiag2d(len),  tmp2);
	    std::cout << float(clock() - c) / CLOCKS_PER_SEC << std::endl;
	  }
	  {
	    clock_t c = clock();
	    level::median(tmp2, win::hline2d(len),  tmp1);
	    std::cout << float(clock() - c) / CLOCKS_PER_SEC << std::endl;
	  }
	  {
	    clock_t c = clock();
	    level::median(tmp1, win::vline2d(len),  output);
	    std::cout << float(clock() - c) / CLOCKS_PER_SEC << std::endl;
	  }

	}

      } // end of namespace mln::level::approx::impl


      // Facades.

      template <typename I, typename O>
      inline
      void median(const Image<I>& input_, const win::rectangle2d& win,
		  Image<O>& output_)
      {
	const I& input = exact(input_);
	O& output = exact(output_);
	mln_assertion(output.domain() == input.domain());

	O tmp(output.domain());
	level::median(input, win::hline2d(win.width()),  tmp);
	level::median(tmp,   win::vline2d(win.height()), output);
      }

      template <typename I, typename O>
      inline
      void median(const Image<I>& input, const win::disk2d& win,
		  Image<O>& output)
      {
	mln_assertion(exact(output).domain() == exact(input).domain());

	impl::median_(exact(input), win.length(), exact(output));
      }

      template <typename I, typename O>
      inline
      void median(const Image<I>& input, const win::octagon2d& win,
		  Image<O>& output)
      {
	mln_assertion(exact(output).domain() == exact(input).domain());

	impl::median_(exact(input), win.length(), exact(output));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::level::approx

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_APPROX_MEDIAN_HH
