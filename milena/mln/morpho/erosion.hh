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

#ifndef MLN_MORPHO_EROSION_HH
# define MLN_MORPHO_EROSION_HH

/*! \file mln/morpho/erosion.hh
 *
 * \brief Morphological erosion.
 *
 */

# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological erosion.
     *
     * \todo Overload erosion_wrt_win for hline and vline and for fast
     * images.
     */
    template <typename I, typename W, typename O>
    void erosion(const Image<I>& input, const Window<W>& win, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // On function.

      template <typename I, typename W, typename O>
      void erosion_on_function(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	O& output      = exact(output_);

	accu::min_<mln_value(I)> min;

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  {
	    min.init();
	    for_all(q) if (input.has(q))
	      min.take(input(q));
	    output(p) = min.to_result();
	  }
      }


      // On set.

      template <typename I, typename W, typename O>
      void erosion_on_set(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	O& output      = exact(output_);

	level::fill(output, input);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  if (input(p))
	    for_all(q) if (input.has(q))
	      if (! input(q))
		{
		  output(p) = false;
		  break;
		}
      }



      // ...



      // FIXME: Stage 3: dispatch w.r.t. fast property


      // Stage 2: dispatch w.r.t. the value kind.

      template <typename I, typename W, typename O>
      void erosion_wrt_value(value::binary_kind, // binary => morphology on sets
			     const Image<I>& input, const Window<W>& win, Image<O>& output)
      {
	return impl::erosion_on_set(exact(input), exact(win), output);
      }

      template <typename K, typename I, typename W, typename O>
      void erosion_wrt_value(K, // otherwise => morphology on functions
			     const Image<I>& input, const Window<W>& win, Image<O>& output)
      {
	return impl::erosion_on_function(exact(input), exact(win), output);
      }



      // Stage 1: dispatch w.r.t. the window type.
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_wrt_win(const Image<I>& input, const Window<W>& win, Image<O>& output)
      {
	erosion_wrt_value(mln_value_kind(I)(), exact(input), exact(win), output);
	//                   |
	//                    -->  call stage 2: dispatch w.r.t. the value kind
      }

#  ifdef MLN_CORE_WIN_RECTANGLE2D_HH

      template <typename I, typename O>
      void erosion_wrt_win(const Image<I>& input, const win::rectangle2d& win, Image<O>& output)
      {
	O temp(exact(output).domain());
	morpho::erosion(input, win::hline2d(win.width()),  temp);
	morpho::erosion(temp,  win::vline2d(win.height()), output);
      }

#  endif // MLN_CORE_WIN_RECTANGLE2D_HH


#  ifdef MLN_CORE_WIN_OCTAGON2D_HH
#   ifdef MLN_CORE_WIN_DIAG2D_HH
#    ifdef MLN_CORE_WIN_BACKDIAG2D_HH

      template <typename I, typename O>
      void erosion_wrt_win(const Image<I>& input, const win::octagon2d& win, Image<O>& output)
      {
	const unsigned len = win.length() / 3 + 1;

	O temp1(exact(output).domain());
	O temp2(exact(output).domain());
	morpho::erosion(input, win::hline2d(len),  temp1);
	morpho::erosion(temp1, win::vline2d(len),  temp2);
	morpho::erosion(temp2, win::diag2d(len),  temp1);
	morpho::erosion(temp1, win::backdiag2d(len),  output);
      }

#    endif // MLN_CORE_WIN_BACKDIAG2D_HH
#   endif // MLN_CORE_WIN_DIAG2D_HH
#  endif // MLN_CORE_WIN_OCTAGON2D_HH

      //   ^
      //   |
      // end of stage1 (dispatch w.r.t. the window type)


    } // end of namespace mln::morpho::impl


    // Facade.

    template <typename I, typename W, typename O>
    void erosion(const Image<I>& input, const Window<W>& win, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      mln_precondition(! exact(win).is_empty());

      impl::erosion_wrt_win(input, exact(win), output);

      if (exact(win).is_centered())
	mln_postcondition(output <= input);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_EROSION_HH
