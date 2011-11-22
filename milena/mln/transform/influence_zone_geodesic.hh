// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_TRANSFORM_INFLUENCE_ZONE_GEODESIC_HH
# define MLN_TRANSFORM_INFLUENCE_ZONE_GEODESIC_HH

/// \file
///
/// Geodesic influence zone transform.

# include <mln/extension/adjust.hh>
# include <mln/canvas/distance_geodesic.hh>
# include <mln/transform/internal/influence_zone_functor.hh>


namespace mln
{

  namespace transform
  {

    /// Geodesic influence zone transform.
    ///
    /// \param[in] input	    An image.
    /// \param[in] nbh		    A neighborhood.
    ///
    /// \return An image of influence zone.
    //
    template <typename I, typename N>
    mln_concrete(I)
    influence_zone_geodesic(const Image<I>& input, const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I, typename N>
      void
      influence_zone_geodesic_tests(const Image<I>& input,
				    const Neighborhood<N>& nbh)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	(void) input;
	(void) nbh;
      }

    } // end of namespace mln::transform::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N>
	mln_concrete(I)
	influence_zone_geodesic(const Image<I>& input,
				const Neighborhood<N>& nbh)
	{
	  (void) input;
	  (void) nbh;

	  // FIXME: To be written...
	  mlc_abort(I)::check();
	}

      } // end of namespace mln::transform::impl::generic


      template <typename I, typename N>
      mln_concrete(I)
      influence_zone_geodesic_fastest(const Image<I>& input_,
				      const Neighborhood<N>& nbh_)
      {
	trace::entering("transform::impl::influence_zone_geodesic_fastest");

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);

	internal::influence_zone_geodesic_tests(input, nbh);

	std::queue<mln_value(I)*> q;
	mln_concrete(I) output;

	util::array<int> dp = offsets_wrt(input, nbh);
	const unsigned n_nbhs = dp.nelements();

	// Initialization.
	{
	  extension::adjust(input, nbh);
	  output = duplicate(input);
	  // For the extension to be ignored:
	  extension::fill(input, 0);  // in initialization
	  extension::fill(output, 1); // in propagation

	  const unsigned nelts = input.nelements();
	  const mln_value(I)* p_i = input.buffer();
	  mln_value(I)* p_o = output.buffer();
	  for (unsigned i = 0; i < nelts; ++i, ++p_i, ++p_o)
	  {
	    if (*p_i == 0)
	      continue;
	    for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      const mln_value(I)* n_i = p_i + dp[j];
	      if (*n_i == 0)
	      {
		q.push(p_o);
		break;
	      }
	    }
	  }

	}

 	// Propagation.
	{
	  mln_value(I)* ptr;

	  while (! q.empty())
	  {
	    ptr = q.front();
	    q.pop();
	    mln_invariant(*ptr != 0);
	    for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      mln_value(I)* ntr = ptr + dp[j];
	      if (*ntr == 0)
	      {
		*ntr = *ptr;
		q.push(ntr);
	      }
	    }
	  }
	}

	trace::exiting("transform::impl::influence_zone_geodesic_fastest");
	return output;
      }


    } // end of namespace mln::transform::impl


    namespace internal
    {

      template <typename I, typename N>
      mln_concrete(I)
      influence_zone_geodesic_dispatch(trait::image::value_alignment::any,
				       trait::image::value_storage::any,
				       trait::image::value_access::any,
				       const I& input,
				       const N& nbh)
      {
	return impl::generic::influence_zone_geodesic(input, nbh);
      }


      template <typename I, typename N>
      mln_concrete(I)
      influence_zone_geodesic_dispatch(trait::image::value_alignment::with_grid,
				       trait::image::value_storage::one_block,
				       trait::image::value_access::direct,
				       const I& input,
				       const N& nbh)
      {
	return impl::influence_zone_geodesic_fastest(input, nbh);
      }


      template <typename I, typename N>
      mln_concrete(I)
      influence_zone_geodesic_dispatch(const Image<I>& input,
				       const Neighborhood<N>& nbh)
      {
	return
	  influence_zone_geodesic_dispatch(mln_trait_image_value_alignment(I)(),
					   mln_trait_image_value_storage(I)(),
					   mln_trait_image_value_access(I)(),
					   exact(input), exact(nbh));
      }

    } // end of namespace mln::transform::internal


    template <typename I, typename N>
    mln_concrete(I)
    influence_zone_geodesic(const Image<I>& input, const Neighborhood<N>& nbh)
    {
      trace::entering("transform::influence_zone_geodesic");

      internal::influence_zone_geodesic_tests(input, nbh);

      mln_concrete(I)
	output = internal::influence_zone_geodesic_dispatch(input, nbh);

      trace::exiting("transform::influence_zone_geodesic");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INFLUENCE_ZONE_GEODESIC_HH
