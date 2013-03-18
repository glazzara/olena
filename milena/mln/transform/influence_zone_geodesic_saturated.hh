// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_TRANSFORM_INFLUENCE_ZONE_GEODESIC_SATURATED_HH
# define MLN_TRANSFORM_INFLUENCE_ZONE_GEODESIC_SATURATED_HH

/// \file
///
/// Geodesic influence zone transform.

# include <mln/canvas/distance_geodesic.hh>
# include <mln/transform/internal/influence_zone_functor.hh>


namespace mln
{

  namespace transform
  {

    /*! \brief Geodesic influence zone transform.

        \param[in] input	    An image.
        \param[in] nbh		    A neighborhood.
        \param[in] max		    The maximum influence zone distance.
        \param[in] background_value The value used as background (i.e.
       				    not propagated).

        \return An image of influence zone.

	\ingroup mlntransform
    */
    template <typename I, typename N, typename D>
    mln_concrete(I)
    influence_zone_geodesic_saturated(const Image<I>& input,
				      const Neighborhood<N>& nbh,
				      const D& max,
				      const mln_value(I)& background_value);

    /*! \overload
      \ingroup mlntransform
    */
    template <typename I, typename N, typename D>
    mln_concrete(I)
    influence_zone_geodesic_saturated(const Image<I>& input,
				      const Neighborhood<N>& nbh,
				      const D& max);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N, typename D>
    mln_concrete(I)
    influence_zone_geodesic_saturated(const Image<I>& input,
				      const Neighborhood<N>& nbh,
				      const D& max,
				      const mln_value(I)& background_value)
    {
      mln_trace("transform::influence_zone_geodesic_saturated");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());

      internal::influence_zone_functor<I> f(background_value);
      (void) mln::canvas::distance_geodesic(input, nbh, max, f);

      return f.output;
    }


    template <typename I, typename N, typename D>
    mln_concrete(I)
    influence_zone_geodesic_saturated(const Image<I>& input,
				      const Neighborhood<N>& nbh,
				      const D& max)
    {
      return influence_zone_geodesic_saturated(input, nbh, max, literal::zero);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INFLUENCE_ZONE_GEODESIC_SATURATED_HH
