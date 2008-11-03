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

#ifndef MLN_MORPHO_DILATION_HH
# define MLN_MORPHO_DILATION_HH

/// \file mln/morpho/dilation.hh
///
/// Morphological dilation.
///
/// \todo Revamp.

# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {


    /// Morphological dilation using windows.
    ///
    /// \{
    /// Perform a morphological dilation of \a input using \a win and
    /// return the result.
    template <typename I, typename W>
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win);
    /// \}




# ifndef MLN_INCLUDE_ONLY

    // Tests.

    namespace internal
    {

      template <typename I, typename W>
      void
      dilation_tests(const Image<I>& input_, const Window<W>& win_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);
	mln_precondition(input.has_data());
	// mln_precondition(win.is_valid());
	(void) input;
	(void) win;
      }

    } // end of namespace mln::morpho::internal


    // Implementations.

    namespace impl
    {

      template <typename I, typename W>
      mln_concrete(I)
      dilation_as_erosion_dual(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl::dilation_as_erosion_dual");
	
	mln_concrete(I) output = morpho::erosion(morpho::complementation(input),
						 win);
	morpho::complementation_inplace(output);

	trace::exiting("morpho::impl::dilation_as_erosion_dual");
	return output;
      }

    } // end of namespace mln::morpho::impl



    // Dispatch.

    namespace internal
    {

      template <typename I, typename W>
      mln_concrete(I)
      dilation_dispatch(const Image<I>& input, const Window<W>& win)
      {
	return impl::dilation_as_erosion_dual(input, win);
      }

    } // end of namespace mln::morpho::internal

    


    // ----------------------------------------- //
    // Facade for window-based implementations.  //
    // ----------------------------------------- //

    template <typename I, typename W>
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::dilation");

      internal::dilation_tests(input, win);
      mln_concrete(I) output = internal::dilation_dispatch(input, win);

      trace::exiting("morpho::dilation");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_DILATION_HH
