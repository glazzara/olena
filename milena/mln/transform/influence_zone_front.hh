// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_INFLUENCE_ZONE_FRONT_HH
# define MLN_TRANSFORM_INFLUENCE_ZONE_FRONT_HH

/// \file mln/transform/influence_zone_front.hh
///
/// Influence zone transform.

# include <mln/canvas/distance_front.hh>
# include <mln/transform/internal/influence_zone_functor.hh>


namespace mln
{

  namespace transform
  {

    /// Influence zone transform.
    template <typename I, typename N, typename W, typename D>
    mln_concrete(I)
    influence_zone_front(const Image<I>& input,
			 const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max);

    /// Influence zone transform.
    template <typename I, typename N, typename W>
    mln_concrete(I)
    influence_zone_front(const Image<I>& input,
			 const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename W, typename D>
    mln_concrete(I)
    influence_zone_front(const Image<I>& input,
			 const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max)
    {
      trace::entering("transform::influence_zone_front");

      mln_precondition(exact(input).has_data());
      // mln_precondition(exact(nbh).is_valid());
      // mln_precondition(exact(w_win).is_valid());

      internal::influence_zone_functor<I> f;
      (void) mln::canvas::distance_front(input, nbh, w_win, max, f);

      trace::exiting("transform::influence_zone_front");
      return f.output;
    }

    template <typename I, typename N, typename W>
    mln_concrete(I)
    influence_zone_front(const Image<I>& input,
			 const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win)
    {
      return influence_zone_front(input, nbh, w_win, mln_max(unsigned));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INFLUENCE_ZONE_FRONT_HH
