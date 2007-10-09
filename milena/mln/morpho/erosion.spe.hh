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

#ifndef MLN_MORPHO_EROSION_SPE_HH
# define MLN_MORPHO_EROSION_SPE_HH

/*! \file mln/morpho/erosion.spe.hh
 *
 * \brief Specialization for mln::morpho::erosion.
 *
 * Dispatch of erosion(input, win, output):
 *
 *  If win = rectangle
 *    -> particular case
 *  Else If win = octagon
 *    -> particular case
 *  Else
 *    (If win is quite agglomerated (disjonction)
 *       -> using incremental algorithm 
 *     Else
 *       -> using conventional algorithm)
 *    If input value kind = logical
 *      -> If image is Fast
 *           -> particular case on sets
 *         Else
 *           -> general case on sets
 *    Else
 *      If input value are high quantified
 *        -> using conventional accumulator
 *      Else
 *        -> using histogram based accumulator
 *      If image is Fast
 *        -> particular case on functions
 *      Else
 *        -> general case on functions
 *    
 */

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace morpho
  {

    // Fwd decl.
    template <typename I, typename W, typename O>
    void erosion(const Image<I>& input, const Window<W>& win, Image<O>& output);

    namespace impl
    {

      namespace generic
      {
	// Fwd decl.
	template <typename I, typename W, typename O, typename A>
	void erosion_on_function(const I& input, const W& win, O& output, A& min);

	// Fwd decl.
	template <typename I, typename W, typename O>
	void erosion_on_set(const I& input, const W& win, O& output);
      }


      template <typename I, typename W, typename O, typename A>
      void erosion_on_function_fast(const I& input, const W& win, O& output, A& min)
      {
	trace::entering("morpho::impl::erosion_on_function_fast");
	border::fill(input, mln_max(mln_value(I)));

	mln_pixter(const I) p(input);
	mln_pixter(O) p_out(output);
	mln_qixter(const I, W) q(p, win);
	for_all_2(p, p_out)
	  {
	    min.init();
	    for_all(q)
	      min.take(q.val());
	    p_out.val() = min.to_result();
	  }
	trace::exiting("morpho::impl::erosion_on_function_fast");
      }

      template <typename I, typename W, typename O>
      void erosion_on_set_fast(const I& input, const W& win, O& output)
      {
	trace::entering("morpho::impl::erosion_on_set_fast");
	level::fill(output, input);
	border::fill(input, true);

	mln_pixter(const I) p(input);
	mln_pixter(O) p_out(output);
	mln_qixter(const I, W) q(p, win);
	for_all_2(p, p_out)
	  if (p.val())
	    for_all(q)
	      if (! q.val())
	      {
		p_out.val() = false;
		break;
	      }
	trace::exiting("morpho::impl::erosion_on_set_fast");
      }


      // Stage 5: dispatch w.r.t. fast property
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_set_wrt_fast(trait::image::speed::any, const I& input,
				const W& win, O& output)
      {
	generic::erosion_on_set(input, win, output);
      }

      template <typename I, typename W, typename O>
      void erosion_set_wrt_fast(trait::image::speed::fastest, const I& input,
				const W& win, O& output)
      {
	impl::erosion_on_set_fast(input, win, output);
      }

      template <typename I, typename W, typename O, typename A>
      void erosion_fun_wrt_fast(trait::image::speed::any, const I& input,
				const W& win, O& output, A& min)
      {
	generic::erosion_on_function(input, win, output, min);
      }

      template <typename I, typename W, typename O, typename A>
      void erosion_fun_wrt_fast(trait::image::speed::fastest, const I& input,
				const W& win, O& output, A& min)
      {
	impl::erosion_on_function_fast(input, win, output, min);
      }

      //   ^
      //   |
      // end of stage 5 (dispatch w.r.t. fast property)

      // Stage 4: dispatch w.r.t. data quantification
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_wrt_data(trait::image::quant::high, const I& input,
			    const W& win, O& output)
      {
	accu::min_<mln_value(I)> min;
	impl::erosion_fun_wrt_fast(mln_trait_image_speed(I)(), input,
				   win, output, min);
      }

      template <typename I, typename W, typename O>
      void erosion_wrt_data(trait::image::quant::low, const I& input,
			    const W& win, O& output)
      {
	accu::min_h<mln_vset(I)> min;
	impl::erosion_fun_wrt_fast(mln_trait_image_speed(I)(), input,
				   win, output, min);
      }

      //   ^
      //   |
      // end of stage 4 (dispatch w.r.t. the data quantification)

      // Stage 3: dispatch w.r.t. the value type
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_wrt_value(const I& input, const W& win, O& output)
      {
	if (mlc_is(mln_trait_image_kind(I), trait::image::kind::logic)::value)
	  impl::erosion_set_wrt_fast(mln_trait_image_speed(I)(), input,
				     win, output);
	//           |
	//           `--> call stage 5: dispatch w.r.t. fast property
	else
	  impl::erosion_wrt_data(mln_trait_image_quant(I)(), input,
				 win, output);
	//           |
	//           `--> call stage 4: dispatch w.r.t. the data quantification
      }

      //   ^
      //   |
      // end of stage 3 (dispatch w.r.t. the value type)



      // Stage 2: dispatch w.r.t. the window morphology
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_wrt_mor(const I& input, const W& win, O& output)
      {
	// FIXME : Choose the right algorithm between :
	impl::erosion_wrt_value(input, win, output);
	// and :
	// impl::erosion_incr_wrt_value(input, win, output);
      }

      //   ^
      //   |
      // end of stage 2 (dispatch w.r.t. the window morphology)



      // Stage 1: dispatch w.r.t. the window type.
      //   |
      //   V

      template <typename I, typename W, typename O>
      void erosion_wrt_win(const Image<I>& input_, const W& win_, Image<O>& output_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	O& output      = exact(output_);

	impl::erosion_wrt_mor(input, win, output);
	//                   |
	//                   `-->  call stage 2: dispatch w.r.t. the data quantification
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

#  endif // MLN_CORE_WIN_OCTAGON2D_HH

      //   ^
      //   |
      // end of stage 1 (dispatch w.r.t. the window type)



    } // end of namespace mln::morpho::impl

  } // end of namespace mln::morpho

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_MORPHO_EROSION_SPE_HH
