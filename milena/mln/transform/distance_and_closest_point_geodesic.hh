// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_DISTANCE_AND_CLOSEST_POINT_GEODESIC_HH
# define MLN_TRANSFORM_DISTANCE_AND_CLOSEST_POINT_GEODESIC_HH

/// \file
///
/// Distance and geodesic closest point transform.

# include <mln/canvas/distance_geodesic.hh>
# include <mln/transform/internal/closest_point_functor.hh>

# include <mln/data/fill.hh>
# include <mln/util/couple.hh>


namespace mln
{

  namespace transform
  {

    /// Discrete geodesic distance transform
    ///
    /// \param[in] input  Image from which the geodesic distance is computed.
    /// \param[in] nbh	  Neighborhood
    /// \param[in] max	  Max distance of propagation.
    ///
    /// \return a couple of images. The first one is the distance map and the
    ///		second one is the closest point image. The closest point image
    ///		contains sites.
    ///
    /// \post The returned images have the same domain as \p input.
    template <typename I, typename N, typename D>
    util::couple<mln_ch_value(I,D), mln_ch_value(I,mln_psite(I))>
    distance_and_closest_point_geodesic(const Image<I>& input,
					const Neighborhood<N>& nbh,
					D max);


    /// Discrete geodesic distance transform
    ///
    /// \param[in] pset			an array of sites.
    /// \param[in] closest_point_domain domain of the returned image.
    /// \param[in] nbh			neighborhood
    /// \param[in] max			max distance of propagation.
    ///
    /// \return A couple of images. The first one is the distance map and the
    ///		second one is the closest point image. The closest point image
    ///		contains site indexes.
    ///
    /// \post The returned image domains are defined on \p closest_point_domain.
    template <typename P, typename N, typename D>
    util::couple<mln_image_from_grid(mln_grid(P),D),
		 mln_image_from_grid(mln_grid(P),unsigned)>
    distance_and_closest_point_geodesic(const p_array<P>& pset,
					const box<P>& closest_point_domain,
					const Neighborhood<N>& nbh,
					D max);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename D>
    inline
    util::couple<mln_ch_value(I,D), mln_ch_value(I, mln_psite(I))>
    distance_and_closest_point_geodesic(const Image<I>& input,
					const Neighborhood<N>& nbh,
					D max)
    {
      trace::entering("transform::distance_closest_point_geodesic");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());

      internal::closest_point_functor_with_sites<I> f;
      mln_ch_value(I,D) dmap = mln::canvas::distance_geodesic(input, nbh,
							      max, f);

      trace::exiting("transform::distance_and_closest_point_geodesic");
      return make::couple(dmap, f.cp_ima);
    }

    template <typename P, typename N, typename D>
    inline
    util::couple<mln_image_from_grid(mln_grid(P),D), mln_image_from_grid(mln_grid(P),unsigned)>
    distance_and_closest_point_geodesic(const p_array<P>& pset,
					const box<P>& closest_point_domain,
					const Neighborhood<N>& nbh,
					D max)
    {
      trace::entering("transform::distance_and_closest_point_geodesic");

      mln_precondition(closest_point_domain.is_valid());
      mln_precondition(exact(nbh).is_valid());
      mln_precondition(geom::bbox(pset) <= closest_point_domain);

      typedef mln_image_from_grid(mln_grid(P), bool) I;
      internal::closest_point_functor_with_indexes<I> f(pset);

      I ima(closest_point_domain);
      data::fill(ima, false);
      data::fill((ima | pset).rw(), true);

      mln_ch_value(I,D) dmap = mln::canvas::distance_geodesic(ima, nbh, max, f);

      trace::exiting("transform::distance_and_closest_point_geodesic");
      return make::couple(dmap, f.cp_ima);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_DISTANCE_AND_CLOSEST_POINT_GEODESIC_HH
