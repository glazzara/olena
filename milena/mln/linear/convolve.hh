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

#ifndef MLN_LINEAR_CONVOLVE_HH
# define MLN_LINEAR_CONVOLVE_HH

/*! \file mln/linear/convolve.hh
 *
 * \brief Convolution.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/border/resize.hh>
# include <mln/border/duplicate.hh>



namespace mln
{

  namespace linear
  {

    /*! Convolution of image \p input by the weighted window \p win.
     *
     * \pre output.domain = input.domain
     */
    template <typename I, typename W, typename O>
    void convolve(const Image<I>& input, const Weighted_Window<W>& win,
		  Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename W, typename O>
      void convolve(const Image<I>& input_, const Weighted_Window<W>& win_,
		    Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win = exact(win_);
	O& output = exact(output_);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);

	for_all(p)
	  {
	    mln_value(O) v = 0;
	    for_all(q) if (input.has(q))
	      v += input(q) * q.weight();
	    output(p) = v;
	  }
      }

      template <typename I, typename W, typename O>
      void convolve(const Fast_Image<I>& input_, const Weighted_Window<W>& win_,
		    Fast_Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win = exact(win_);
	O& output = exact(output_);

	border::resize(input, win.delta());
	border::duplicate(input);

 	mln_pixter(O)          p_out(output);

	mln_pixter(const I)    p(input);
	mln_qixter(const I, W) q(input, win, p);

 	for_all_2(p, p_out)
	  {
	    mln_value(O) v = 0;
	    unsigned i = 0;
	    for_all(q)
	      v += win.weight(i++) * *q;
 	    *p_out = v;
	  }
      }

    } // end of namespace mln::linear::impl

    template <typename I, typename W, typename O>
    void convolve(const Image<I>& input, const Weighted_Window<W>& win,
		  Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::convolve(exact(input), exact(win), exact(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_CONVOLVE_HH
