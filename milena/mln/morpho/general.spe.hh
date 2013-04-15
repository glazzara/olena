// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_GENERAL_SPE_HH
# define MLN_MORPHO_GENERAL_SPE_HH

/// \file
///
/// Specialization for mln::morpho::general.
///
/// \todo Warning: we should also have the "arbitrary" versions working
/// on sets (not only on functions!)
///
/// \todo HERE Factor dispatch for lines...
///
/// \todo Re-activate impl::line_on_function.
///
/// \todo Re-activate special code for centered window on sets.
///
/// \todo Dispatch transform_line when there is no extension to
/// perform a side-effect.

# ifndef MLN_MORPHO_GENERAL_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_MORPHO_GENERAL_HH

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/dpoint2d.hh>

# include <mln/win/octagon2d.hh>
# include <mln/win/rectangle2d.hh>

# include <mln/accu/transform_directional.hh>
# include <mln/accu/transform_line.hh>
# include <mln/accu/transform_snake.hh>
# include <mln/accu/transform_stop.hh>
# include <mln/accu/transform_diagonal.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace morpho
  {

    // Fwd decl of the facade.
    template <typename Op, typename I, typename W>
    mln_concrete(I)
    general(const Op& op, const Image<I>& input, const Window<W>& win);


    namespace impl
    {


      namespace generic
      {
	// Forward declaration.
	// Implementation is in mln/morpho/general.hh.
	template <typename Op, typename I, typename W>
	mln_concrete(I)
	general_on_function(const Op& op, const Image<I>& input_, const Window<W>& win_);

	// Forward declaration.
	// Implementation is in mln/morpho/general.hh.
	template <typename Op, typename I, typename W>
	mln_concrete(I)
	general_on_set(const Op& op, const Image<I>& input_, const Window<W>& win_);
      }



      // On set with centered window.

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_on_set_centered(const Op& op, const Image<I>& input, const Window<W>& win)
      {
	return generic::general_on_set(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_on_set_centered_fastest(const Op& op, const Image<I>& input, const Window<W>& win)
      {
	return generic::general_on_set(op, input, win);
      }


/*

      // Line case.


      template <typename Op, typename I, typename G, unsigned Dir, typename C>
      inline
      mln_concrete(I)
      general_line_on_function(const Op& op, const Image<I>& input_, const win::line<G,Dir,C>& win)
      {
	mln_trace("morpho::impl::general_line");

	typedef mln_site(I) P;
	enum { dim = P::dim };

	const I& input = exact(input_);

	mln_concrete(I) output;
	initialize(output, input);

	mln_psite(I)
	  pmin = input.domain().pmin(),
	  pmax = input.domain().pmax(),
	  p = pmin;

	const unsigned len = input.domain().len(Dir);
	const unsigned win_half_length = win.length() / 2;

	do
	  {
	    accu::line< accu::meta::stat::min_h, Dir >(input,
						 p, len,
						 win_half_length,
						 output);

	    for (int c = dim - 1; c >= 0; --c)
	      {
		if (c == int(Dir))
		  continue;
		if (p[c] != pmax[c])
		  {
		    ++p[c];
		    break;
		  }
		p[c] = pmin[c];
	      }
	  }
	while (p != pmin);

	return output;
      }

*/


      // Particular windows.


      template <typename Op, typename I>
      inline
      mln_concrete(I)
      general_rectangle2d(const Op& op, const Image<I>& input_, const win::rectangle2d& win)
      {
	mln_trace("morpho::impl::general_rectangle2d");

	const I& input = exact(input_);

	mln_concrete(I) temp, output;
	temp   = morpho::general(op, input, win::hline2d(win.width()));
	output = morpho::general(op, temp,  win::vline2d(win.height()));

	return output;
      }


      template <typename Op, typename I>
      inline
      mln_concrete(I)
      general_octagon2d(const Op& op, const Image<I>& input_, const win::octagon2d& win)
      {
	mln_trace("morpho::impl::general_octagon2d");

	const I& input = exact(input_);

	const unsigned len = (win.length() + 2) / 3;

	mln_concrete(I) output;
	output = morpho::general(op, input,  win::hline2d(len));
	output = morpho::general(op, output, win::vline2d(len));
	output = morpho::general(op, output, win::diag2d(len));
	output = morpho::general(op, output, win::backdiag2d(len));

	return output;
      }


      template <typename Op, typename I, typename W>
      inline
      mln_concrete(I)
      general_arbitrary_2d(const Op& op, const Image<I>& input, const Window<W>& win)
      {
	mln_trace("morpho::impl:general_arbitrary_2d");

	extension::adjust_fill(input, geom::delta(win) + 1, op.neutral(input));
	mln_concrete(I) output;
	output = accu::transform_snake(op.accu_incr(input), input, win);

	return output;
      }


      template <typename Op, typename I, typename W>
      inline
      mln_concrete(I)
      general_directional(const Op& op, const Image<I>& input, const Window<W>& win, unsigned dir)
      {
	mln_trace("morpho::impl:general_directional");

	extension::adjust_fill(input, geom::delta(win) + 1, op.neutral(input));
	mln_concrete(I) output;
	output = accu::transform_directional(op.accu_incr(input), input, win, dir);

	return output;
      }


      template <typename Op, typename I, typename W>
      inline
      mln_concrete(I)
      general_line(const Op& op, const Image<I>& input, const Window<W>& win_)
      {
	mln_trace("morpho::impl:general_line");

	const W& win = exact(win_);

	extension::adjust_fill(input, geom::delta(win), op.neutral(input));

	mln_concrete(I) output;
	output = accu::transform_line(op.accu_incr(input), input,
				      win.length(), win.dir);

	return output;
      }


      template <typename Op, typename I, typename W>
      inline
      mln_concrete(I)
      general_diagonal_2d(const Op& op, const Image<I>& input, const Window<W>& win)
      {
	mln_trace("morpho::impl:general_diagonal_2d");

	extension::adjust_fill(input, geom::delta(win) + 1, op.neutral(input));
	mln_concrete(I) output;
	output = accu::transform_diagonal(op.accu_incr(input), input, win);

	return output;
      }



    } // end of namespace mln::morpho::impl




    // Dispatch.


    namespace internal
    {

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_for_generic(trait::image::kind::logic, // On sets.
				   trait::image::speed::any,  // Any speed.
				   const Op& op, const I& input, const W& win)
      {
	if (win.is_centered())
	  return impl::general_on_set_centered(op, input, win);
	else
	  return impl::generic::general_on_set(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_for_generic(trait::image::kind::logic,    // On sets.
				   trait::image::speed::fastest, // Fastest.
				   const Op& op, const I& input, const W& win)
      {
	if (win.is_centered())
	  return impl::general_on_set_centered_fastest(op, input, win);
	else
	  return impl::generic::general_on_set(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_for_generic(trait::image::kind::any, // On functions.
				   trait::image::speed::any,
				   const Op& op, const I& input, const W& win)
      {
	return impl::generic::general_on_function(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_for_generic(const Op& op, const I& input, const W& win) // Entry point.
      {
	mln_concrete(I) ima =
	  general_dispatch_for_generic(mln_trait_image_kind(I)(),
				       mln_trait_image_speed(I)(),
				       op, input, win);
	return ima;
      }




      // dispatch w.r.t. win


      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_wrt_arbitrary_win(metal::true_,
					 const Op& op, const I& input, const W& win)
      {
	return morpho::impl::general_arbitrary_2d(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_wrt_arbitrary_win(metal::false_,
					 const Op& op, const I& input, const W& win)
      {
	return general_dispatch_for_generic(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_wrt_win(const Op& op, const I& input, const W& win)
      {
	// FIXME:
	// The test "win.size() >= 10" (size is not too small) has been
	// de-activated...
	  enum { test = mlc_equal(mln_trait_image_dimension(I),
				  trait::image::dimension::two_d)::value
		 &&
		        mlc_is_a(mln_site(I), Gpoint)::value
		 &&
		        mln_is_simple_window(W)::value
		 &&
		        mlc_equal(mln_trait_image_quant(I),
				  trait::image::quant::low)::value
		 &&
		        mlc_not_equal(mln_trait_image_value_storage(I),
				      trait::image::value_storage::disrupted)::value };
	return general_dispatch_wrt_arbitrary_win(metal::bool_<test>(),
						  op, input, win);
      }



      template <typename Op, typename I>
      mln_concrete(I)
      general_dispatch_wrt_win(const Op& op, const I& input, const win::rectangle2d& win)
      {
	if (win.size() == 1)
	  return duplicate(input);
	if (win.size() <= 9) // FIXME: Hard-coded!
	  return general_dispatch_for_generic(op, input, win);
	return impl::general_rectangle2d(op, input, win);
      }


      template <typename Op, typename I>
      mln_concrete(I)
      general_dispatch_wrt_win(const Op& op, const I& input, const win::octagon2d& win)
      {
	if (win.length() < 5)
	  return impl::general_arbitrary_2d(op, input, win);
	else
	  return impl::general_octagon2d(op, input, win);
      }



      /// Handling win::line(s).
      /// \{

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_line(metal::true_,
			    const Op& op, const I& input, const W& win)
      {
	return impl::general_line(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_line(metal::false_,
			    const Op& op, const I& input, const W& win)
      {
	return general_dispatch_for_generic(op, input, win);
      }

      template <typename Op, typename I, typename M, unsigned i, typename C>
      mln_concrete(I)
      general_dispatch_wrt_win(const Op& op, const I& input, const win::line<M,i,C>& win)
      {
	if (win.size() == 1)
	  return duplicate(input);
	else if (win.size() == 3)
	  return general_dispatch_for_generic(op, input, win);
	else
	  {
	    enum { test = mlc_is_a(mln_domain(I), Box)::value
		   && mlc_equal(mln_trait_image_quant(I),
				mln::trait::image::quant::low)::value };
	    return general_dispatch_line(metal::bool_<test>(),
					 op, input, win);
	  }
      }

      /// \}




      /// Handling diagonals.
      /// \{

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_diagonal(metal::true_,
				const Op& op, const I& input, const W& win)
      {
	return impl::general_diagonal_2d(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_diagonal(metal::false_,
				const Op& op, const I& input, const W& win)
      {
	return general_dispatch_for_generic(op, input, win);
      }

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch_diagonal(const Op& op, const I& input, const W& win)
      {
	if (win.size() == 1)
	  return duplicate(input);
	else if (win.size() == 3)
	  return general_dispatch_for_generic(op, input, win);
	else
	  {
	    enum { test = mlc_is_a(mln_domain(I), Box)::value
		   && mlc_equal(mln_trait_image_quant(I),
				mln::trait::image::quant::low)::value };
	    return general_dispatch_diagonal(metal::bool_<test>(),
					     op, input, win);
	  }
      }


      template <typename Op, typename I>
      mln_concrete(I)
      general_dispatch_wrt_win(const Op& op, const I& input, const win::diag2d& win)
      {
	return general_dispatch_diagonal(op, input, win);
      }

      template <typename Op, typename I>
      mln_concrete(I)
      general_dispatch_wrt_win(const Op& op, const I& input, const win::backdiag2d& win)
      {
	return general_dispatch_diagonal(op, input, win);
      }

      /// \}



      // The dispatch entry point.

      template <typename Op, typename I, typename W>
      mln_concrete(I)
      general_dispatch(const Op& op, const Image<I>& input, const Window<W>& win)
      {
	return general_dispatch_wrt_win(op, exact(input), exact(win));
      }

    } // end of namespace mln::morpho::internal

  } // end of namespace mln::morpho

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_MORPHO_GENERAL_SPE_HH
