// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MORPHO_GENERAL_HH
# define MLN_MORPHO_GENERAL_HH

/// \file mln/morpho/general.hh
///
/// Morphological general routine with dispatch.
///
/// \todo Have infimum and supremum for value types, apart from
/// morphology!

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/extension/adjust_fill.hh>
# include <mln/accu/transform.hh>

// Specializations are in:
# include <mln/morpho/general.spe.hh>


# define mln_morpho_select_accu(I, S, F)				\
typename mln::metal::if_< mln::metal::is< mln_trait_image_kind(I),	\
					  trait::image::kind::binary >,	\
			  mln::accu::meta::S,				\
			  mln::accu::meta::F >::ret


namespace mln
{

  namespace morpho
  {

    /// Morphological general routine.
    template <typename Op, typename I, typename W>
    mln_concrete(I)
    general(const Op& op, const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename Op, typename I, typename W>
      inline
      void
      general_tests(const Op& op, const Image<I>& input_, const Window<W>& win_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);

	mln_precondition(input.is_valid());
	mln_precondition(! win.is_empty());
	mln_precondition(win.is_valid());

	(void) op;
	(void) input;
	(void) win;
      }


      // Temporary code; see 'todo' above.
      template <typename I, bool is_binary> // true
      struct neutral_impl
      {
	static mln_value(I) infimum()  { return false; }
	static mln_value(I) supremum() { return true;  }
      };
      template <typename I>
      struct neutral_impl< I, false >
      {
	static mln_value(I) infimum()  { return mln_min(mln_value(I)); }
	static mln_value(I) supremum() { return mln_max(mln_value(I)); }
      };
      template <typename I>
      struct neutral : neutral_impl< I, mlc_is(mln_trait_image_kind(I),
					       trait::image::kind::binary)::value >
      {
      };

    } // end of mln::morpho::internal


    namespace impl
    {

      namespace generic
      {


	// On function.

	template <typename Op, typename I, typename W>
	inline
	mln_concrete(I)
	general_on_function(const Op& op, const Image<I>& input, const Window<W>& win)
	{
	  trace::entering("morpho::impl::generic::general_on_function");

 	  internal::general_tests(op, input, win);

	  extension::adjust_fill(input, win, op.neutral(input));
	  mln_concrete(I) output;
	  output = accu::transform(input, op.accu(input), win);

	  trace::exiting("morpho::impl::generic::general_on_function");
	  return output;
	}

	// On set.

	template <typename Op, typename I, typename W>
	inline
	mln_concrete(I)
	general_on_set(const Op& op, const Image<I>& input, const Window<W>& win)
	{
	  trace::entering("morpho::impl::generic::general_on_set");

	  internal::general_tests(op, input, win);

	  extension::adjust_fill(input, win, op.neutral(input));
	  mln_concrete(I) output;
	  output = accu::transform_stop(input, op.accu(input), win);

	  trace::exiting("morpho::impl::generic::general_on_set");
	  return output;
	}

      } // end of namespace mln::morpho::impl::generic

    } // end of namespace mln::morpho::impl



    // Facades.

    template <typename Op, typename I, typename W>
    inline
    mln_concrete(I)
    general(const Op& op, const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::general");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      internal::general_tests(op, input, win);
      mln_concrete(I) output = internal::general_dispatch(op, input, win);

      trace::exiting("morpho::general");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_GENERAL_HH
