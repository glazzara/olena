// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_LEVEL_NAIVE_MEDIAN_HH
# define MLN_LEVEL_NAIVE_MEDIAN_HH

/// \file mln/level/naive/median.hh
///
/// Naive version of median filtering.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/level/median.hh>


namespace mln
{

  namespace level
  {

    namespace naive
    {

      /// Compute in \p output the median filter of image \p input by
      ///  the window \p win.
      /*!
       * \param[in] input The image to be filtered.
       * \param[in] win The window.
       * \param[in,out] output The output image.
       *
       * This is a NAIVE version for test / comparison purpose so
       * do NOT use it.
       *
       * \pre \p input and \p output have to be initialized.
       *
       * \see mln::level::median
       */
      template <typename I, typename W, typename O>
      void median(const Image<I>& input, const Window<W>& win,
		  Image<O>& output);


# ifndef MLN_INCLUDE_ONLY


      namespace impl
      {

	template <typename I, typename W, typename O>
	inline
	void median(const I& input, const W& win,
		    O& output)
	{
	  mln_piter(I) p(input.domain());
	  mln_qiter(W) q(win, p);
	  accu::median_h<mln_value(I)> med;

	  for_all(p)
	    {
	      med.init();
	      for_all(q) if (input.has(q))
		med.take(input(q));
	      output(p) = med.to_result();
	    }
	}


      } // end of namespace mln::level::naive::impl



      // facade

      template <typename I, typename W, typename O>
      inline
      void median(const Image<I>& input, const Window<W>& win,
		  Image<O>& output)
      {
	impl::median(exact(input), exact(win), exact(output));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::level::naive

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEDIAN_HH
