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

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/accu/min.hh>
# include <mln/value/props.hh>
# include <mln/level/compare.hh>
# include <mln/level/fill.hh>
// FIXME: # include <mln/border/assign.hh>


namespace mln
{

  namespace morpho
  {

    template <typename I, typename W, typename O>
    void erosion(const Image<I>& input, const Window<W>& win, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // on function

      template <typename I, typename W, typename O>
      void erosion_on_function(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	O& output      = exact(output_);

	accu::min<mln_value(I)> min;

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  {
	    min.init();
	    for_all(q) if (input.has(q))
	      min.take(input(q));
	    output(p) = min;
	  }
      }


      // on set

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



      // FIXME: stage 3: dispatch w.r.t. fast property


      // stage 2: dispatch w.r.t. the value kind

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



      // stage 1: dispatch w.r.t. the window type
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_wrt_win(const Image<I>& input, const Window<W>& win, Image<O>& output)
      {
	erosion_wrt_value(mln_value_kind(I)(), exact(input), exact(win), output);
	//                   | 
	//                    -->  call stage 2: dispatch w.r.t. the value kind
      }

#  ifdef MLN_CORE_RECTANGLE2D_HH

      template <typename I, typename O>
      void erosion_wrt_win(const Image<I>& input, const rectangle2d& win, Image<O>& output)
      {
	O tmp(exact(output).domain());
	morpho::erosion(input, hline2d(win.width()),  tmp);
	morpho::erosion(tmp,   vline2d(win.height()), output);
      }

#  endif // MLN_CORE_RECTANGLE2D_HH

      //   ^
      //   |
      // end of stage1 (dispatch w.r.t. the window type)


    } // end of namespace mln::morpho::impl


    // facade

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
