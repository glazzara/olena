// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_CANVAS_MORPHO_CONNECTED_FILTER_HH
# define MLN_CANVAS_MORPHO_CONNECTED_FILTER_HH

/// \file mln/canvas/morpho/connected_filter.hh
///
/// Connected filters dispatch (algebraic & leveling filters).
///

# include "leveling_filter.hh"
# include <mln/canvas/morpho/algebraic_filter.hh>
# include <mln/trait/accumulators.hh>

namespace mln {
  namespace canvas {
    namespace morpho {

      // Facade Fwd Declaration
      template <typename I, typename N, typename A>
      mln_concrete(I)
      connected_filter(const Image<I>& input, const Neighborhood<N>& nbh,
		      const Accumulator<A>& a, const typename A::result& lambda, bool increasing);

# ifndef MLN_INCLUDE_ONLY

      // Dispatch.
      namespace internal
      {

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	connected_filter_dispatch(mln::trait::accumulator::when_pix::use_none,
				   const Image<I>& input, const Neighborhood<N>& nbh,
				   const Accumulator<A>& a, const typename A::result& lambda,
				   bool increasing)
	{
	  return algebraic_filter_dispatch(input, nbh, a, lambda, increasing);
	}

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	connected_filter_dispatch(mln::trait::accumulator::when_pix::use_p,
				   const Image<I>& input, const Neighborhood<N>& nbh,
				   const Accumulator<A>& a, const typename A::result& lambda,
				   bool increasing)
	{
	  return algebraic_filter_dispatch(input, nbh, a, lambda, increasing);
	}

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	connected_filter_dispatch(mln::trait::accumulator::when_pix::use_v,
				   const Image<I>& input, const Neighborhood<N>& nbh,
				   const Accumulator<A>& a, const typename A::result& lambda,
				   bool increasing)
	{
	  return leveling_filter_dispatch(input, nbh, a, lambda, increasing);
	}

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	connected_filter_dispatch(mln::trait::accumulator::when_pix::use_pix,
				   const Image<I>& input, const Neighborhood<N>& nbh,
				   const Accumulator<A>& a, const typename A::result& lambda,
				   bool increasing)
	{
	  return leveling_filter_dispatch(input, nbh, a, lambda, increasing);
	}
	
      } // end of namespace mln::canvas::morpho::internal


# endif // ! MLN_INCLUDE_ONLY

      // Facade.

      template <typename I, typename N, typename A>
      mln_concrete(I)
      connected_filter(const Image<I>& input, const Neighborhood<N>& nbh,
			const Accumulator<A>& a, const typename A::result& lambda, bool increasing)
      {
	return internal::connected_filter_dispatch(mln_trait_accu_when_pix(A)(),
						    input, nbh, a, lambda, increasing);
      }


    } // end of namespace mln::canvas::morpho
  } // end of namespace mln::canvas
} // end of namespace mln


#endif // ! MLN_CANVAS_MORPHO_CONNECTED_FILTER_HH
