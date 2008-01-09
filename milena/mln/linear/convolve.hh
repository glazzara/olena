// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

    /*! Convolution of an image \p input by the weighted window \p w_win.
     *
     * \warning Computation of \p output(p) is performed with the
     * value type of \p output.
     *
     * \warning The weighted window is used as-is, considering that
     * its symmetrization is handled by the client.
     *
     * \pre output.domain = input.domain
     */
    template <typename I, typename W, typename O>
    void convolve(const Image<I>& input, const Weighted_Window<W>& w_win,
		  Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      /* FIXME: We must clean up the interface of
	 mln::linear::impl::convolve_:

	 - either allow certain patterns of speed traits (e.g.,
	   any/any, fastest/fastest, fastest/any, etc.).  In this
	   case, the generic version should abort at compile time;

	 - or accept all combinations (which is the current case), and
           default to the slowest one (presumably any/any).
       */

      // Fwd decl.
      template <typename I, typename W, typename O>
      inline
      void convolve_(const I& input,
		     const Weighted_Window<W>& w_win_,
		     O& output);

      /// Default version, delegating to the generic version.
      template <typename Speed_I, typename I, typename W,
		typename Speed_O, typename O>
      inline
      void convolve_(Speed_I, const I& input,
		     const Weighted_Window<W>& w_win_,
		     Speed_O, O& output)
      {
	/* Don't delegate using such a call:

	   \code
	   impl::convolve_(trait::image::speed::any(), input,
			   w_win_,
			   trait::image::speed::any(), output);
	   \endcode

	   since it would end up with infinite recursion.  The reason
	   is that the compiler would select this function (in which
	   you read this very comment), instead of the next one (with
	   input and output speed traits set to `any'), to resolve the
	   call.  This is because C++ overloading rules favor the
	   generic function over the more specialized one.  And we
	   cannot use explicit partial specialization, since it just
	   doesn't exist for functions.

	   Hence the chosen solution: create and call another
	   overloading for mln::linear::impl::convolve_, with no
	   unnatural selection behavior.  */
	impl::convolve_(input, w_win_, output);
      }

      template <typename I, typename W, typename O>
      inline
      void convolve_(trait::image::speed::any, const I& input,
		     const Weighted_Window<W>& w_win_,
		     trait::image::speed::any, O& output)
      {
	// Delegate the call to the generic version.
	impl::convolve_(input, w_win_, output);
      }

      /// A factored implementation of the most generic version of
      ///  mln::linear::impl::convolve_.
      template <typename I, typename W, typename O>
      inline
      void convolve_(const I& input,
		     const Weighted_Window<W>& w_win_,
		     O& output)
      {
	const W& w_win = exact(w_win_);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(w_win, p);

	for_all(p)
	  {
	    mln_value(O) v = 0;
	    for_all(q) if (input.has(q))
	      v += input(q) * q.w();
	    output(p) = v;
	  }
      }

      template <typename I, typename W, typename O>
      inline
      void convolve_(trait::image::speed::fastest, const I& input,
		     const Weighted_Window<W>& w_win_,
		     trait::image::speed::fastest, O& output)
      {
	const W& w_win = exact(w_win_);

	border::resize(input, w_win.delta());
	border::duplicate(input);

 	mln_pixter(O)          p_out(output);

	mln_pixter(const I)    p(input);
	mln_qixter(const I, W) q(p, w_win);

 	for_all_2(p, p_out)
	  {
	    mln_value(O) v = 0;
	    unsigned i = 0;
	    for_all(q)
	      v += w_win.w(i++) * q.val();
 	    p_out.val() = v;
	  }
      }

    } // end of namespace mln::linear::impl


    // Facade.

    template <typename I, typename W, typename O>
    inline
    void convolve(const Image<I>& input, const Weighted_Window<W>& w_win,
		  Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::convolve_(mln_trait_image_speed(I)(), exact(input),
		      exact(w_win),
		      mln_trait_image_speed(O)(), exact(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_CONVOLVE_HH
