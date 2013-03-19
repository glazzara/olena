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

#ifndef MLN_TRANSFORM_DISTANCE_FRONT_HH
# define MLN_TRANSFORM_DISTANCE_FRONT_HH

/// \file
///
/// Discrete front distance transform.

# include <mln/canvas/distance_front.hh>
# include <mln/transform/internal/distance_functor.hh>



namespace mln
{

  namespace transform
  {

    /*! \brief Discrete front distance transform.
      \ingroup mlntransform
    */
    template <typename I, typename N, typename W, typename D>
    mln_ch_value(I, D)
    distance_front(const Image<I>& input,
		   const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename W, typename D>
    inline
    mln_ch_value(I, D)
    distance_front(const Image<I>& input,
		   const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max)
    {
      mln_trace("transform::distance_front");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());
      mln_precondition(exact(w_win).is_valid());

      mln_ch_value(I, D) output;
      internal::distance_functor<I> f;
      output = mln::canvas::distance_front(input, nbh, w_win, max, f);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_DISTANCE_FRONT_HH
