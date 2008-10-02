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

#ifndef MLN_MORPHO_DILATION_HH
# define MLN_MORPHO_DILATION_HH

/// \file mln/morpho/dilation.hh
/// \brief Morphological dilation.
///
/// \todo Activate the FIXMEs (border::adjust).

# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {

    /// Morphological dilation using the neighborhood bound to an image.
    ///
    /// \{
    /* FIXME: Re-enable this routine for Olena 1.1, when associated
       neighborhoods (i.e., the neighb::image morpher) are supported
       and shipped.  */
#if 0
    /// Perform a morphological dilation of \a input using its
    /// neighborhood and return the result.
    ///
    /// \pre \a input must be an image with a neighborhood.
    template <typename I>
    mln_concrete(I)
    dilation(const Image<I>& input);
    /// \}
#endif

    /// Morphological dilation using windows.
    ///
    /// \todo Overload dilation_wrt_win for hline and vline and for fast
    /// images.
    ///
    /// \{
    /// Perform a morphological dilation of \a input using \a win and
    /// return the result.
    template <typename I, typename W>
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win);
    /// \}



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      /*---------------.
      | Window-based.  |
      `---------------*/

      // On function.

      template <typename I, typename W, typename O>
      inline
      void dilation_on_function(const Image<I>& input_, const Window<W>& win_,
				Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	O& output      = exact(output_);

	// FIXME: border::adjust(input, win.delta());
	extension::fill(input, mln_min(mln_value(I)));

	accu::max_<mln_value(I)> max;

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  {
	    max.init();
	    for_all(q) if (input.has(q))
	      max.take(input(q));
	    output(p) = max.to_result();
	  }
      }

      // On set.

      template <typename I, typename W, typename O>
      inline
      void dilation_on_set(const Image<I>& input_, const Window<W>& win_,
			   Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	O& output      = exact(output_);

	// FIXME: border::adjust(input, win.delta());
	extension::fill(input, false);

	level::fill(output, input);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  if (!input(p))
	    for_all(q) if (input.has(q))
	      if (input(q))
		{
		  output(p) = true;
		  break;
		}
      }


      // ------------- //
      // Dispatchers.  //
      // ------------- //

      // FIXME: Stage 3: dispatch w.r.t. speed property.
      // ...


      /// Stage 2: dispatch w.r.t. the value kind.
      /// \{
      
      /// Binary =>  morphology on sets.
      template <typename I, typename W, typename O>
      inline
      void dilation_wrt_value(trait::image::kind::logic,
			      const Image<I>& input, const Window<W>& win,
			      Image<O>& output)
      {
	return impl::dilation_on_set(exact(input), exact(win), output);
      }

      /// Otherwise => morphology on functions.
      template <typename I, typename W, typename O>
      inline
      void dilation_wrt_value(trait::image::kind::any,
			      const Image<I>& input, const Window<W>& win,
			      Image<O>& output)
      {
	return impl::dilation_on_function(exact(input), exact(win), output);
      }

      // End of stage 2.

      /// \}



      /// Stage 1: dispatch w.r.t. the window type.
      /// \{

      /// Default case.
      template <typename I, typename W, typename O>
      inline
      void dilation_wrt_win(const Image<I>& input, const Window<W>& win,
			    Image<O>& output)
      {
	// Perform stage 2: dispatch w.r.t. the value kind.
	dilation_wrt_value(mln_trait_image_kind(I)(), exact(input),
			   exact(win), output);
      }

#  ifdef MLN_CORE_WIN_RECTANGLE2D_HH

      /// Rectangle window.
      template <typename I, typename O>
      inline
      void dilation_wrt_win(const Image<I>& input, const win::rectangle2d& win,
			    Image<O>& output)
      {
	O temp(exact(output).domain());
	morpho::dilation(input, win::hline2d(win.width()),  temp);
	morpho::dilation(temp,  win::vline2d(win.height()), output);
      }

#  endif // MLN_CORE_WIN_RECTANGLE2D_HH


#  ifdef MLN_CORE_WIN_OCTAGON2D_HH
#   ifdef MLN_CORE_WIN_DIAG2D_HH
#    ifdef MLN_CORE_WIN_BACKDIAG2D_HH

      /// Octagon window.
      template <typename I, typename O>
      void dilation_wrt_win(const Image<I>& input, const win::octagon2d& win,
			    Image<O>& output)
      {
	const unsigned len = win.length() / 3 + 1;

	O temp1(exact(output).domain());
	O temp2(exact(output).domain());
	morpho::dilation(input, win::hline2d(len),  temp1);
	morpho::dilation(temp1, win::vline2d(len),  temp2);
	morpho::dilation(temp2, win::diag2d(len),  temp1);
	morpho::dilation(temp1, win::backdiag2d(len),  output);
      }

#    endif // MLN_CORE_WIN_BACKDIAG2D_HH
#   endif // MLN_CORE_WIN_DIAG2D_HH
#  endif // MLN_CORE_WIN_OCTAGON2D_HH

      // End of stage 1.

      /// \}


      /*---------------------.
      | Neighborhood-based.  |
      `---------------------*/

    // FIXME: Disabled (see above).
#if 0
      /* FIXME: We might want to move this function into the body of
	 the facade (see at the bottom of the file.  */
      // Sole case.  Convert the neighborhood into a window, and
      // delegate to the window-based implementation.
      template <typename I, typename N, typename O>
      inline
      void dilation_wrt_nbh(const Image<I>& input, const Neighborhood<N>& nbh,
			    Image<O>& output)
      {
	/* FIXME: The following comment applies to every algorithm
	   having a neighborhood and a window flavor: move it
	   elsewhere.

	   We solely depend on the neighborhood-to-window conversion
	   here.  This means the conversion should be smart enough to
	   produce a working window, even in the case of a non
	   dpoint-set-based neighborhood.  */
	dilation_wrt_win(input, nbh.win(), output);
      }
#endif

    } // end of namespace mln::morpho::impl


    /*----------.
    | Facades.  |
    `----------*/

    // ----------------------------------------------- //
    // Facade for neighborhood-based implementations.  //
    // ----------------------------------------------- //

    // FIXME: Disabled (see above).
#if 0
    template <typename I>
    mln_concrete(I)
    dilation(const Image<I>& input)
    {
      trace::entering("morpho::dilation");

      metal::has_neighborhood<I>::check();
      typedef mln_neighb(I) neighb;

      mln_concrete(I) output;
      initialize(output, input);
      impl::dilation_wrt_nbh(input, exact(input).neighborhood(), output);

      trace::exiting("morpho::dilation");
      return output;
    }
#endif

    // ----------------------------------------- //
    // Facade for window-based implementations.  //
    // ----------------------------------------- //

    template <typename I, typename W>
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::dilation");
      mln_precondition(exact(input).has_data());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output;
      initialize(output, input);
      impl::dilation_wrt_win(input, exact(win), output);

      if (exact(win).is_centered())
	mln_postcondition(output >= input);
      trace::exiting("morpho::dilation");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_DILATION_HH
