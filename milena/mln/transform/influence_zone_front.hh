// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_INFLUENCE_ZONE_FRONT_HH
# define MLN_TRANSFORM_INFLUENCE_ZONE_FRONT_HH

/// \file
///
/// Influence zone transform.

# include <mln/canvas/distance_front.hh>
# include <mln/transform/internal/influence_zone_functor.hh>


namespace mln
{

  namespace transform
  {

    /*! \brief Influence zone transform.
      \ingroup mlntransform
     */
    template <typename I, typename N, typename W, typename D>
    mln_concrete(I)
    influence_zone_front(const Image<I>& input,
			 const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max);

    /*! \brief Influence zone transform.
      \ingroup mlntransform
    */
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

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());
      mln_precondition(exact(w_win).is_valid());

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
