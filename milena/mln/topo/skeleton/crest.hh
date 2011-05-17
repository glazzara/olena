// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_TOPO_SKELETON_CREST_HH
# define MLN_TOPO_SKELETON_CREST_HH

/// \file
///
/// \brief Compute skeletization constraints.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/extension/adjust.hh>
# include <mln/border/equalize.hh>


namespace mln
{

  namespace topo
  {

    namespace skeleton
    {


      /// Compute skeletization constraints.
      ///
      /// \param[in] input	   A binary image.
      /// \param[in] dist_map      A distance map of \p input. Contains the
      ///                          inner object distance map.
      /// \param[in] nbh	   A neighborhood.
      /// \param[in] psi_threshold Keep sites having a Point
      ///                          Superiority Index greated or equal
      ///                          to \p psi_threshold.
      ///
      /// \result A binary image.
      //
      /*!
       *
       *  This implementation is based on the following article:
       *    K. W. Kang, J. W. Suh, and J. H. Kim. Skeletonization of grayscale
       *    character images using pixel superiority index. In Proc. 3rd IAPR
       *    Workshop on Document Analysis Systems, pages 326-335, Nagano,
       *    Japan, 1998.
       *
       *  Abstract:
       *    "In this paper, we present pixel superiority index as a tool for
       *    designing a skeletonization algorithm which utilizes topographic
       *    features efficiently. We clarify a relationship between pixel
       *    superiority index and topographic features. Then, using the
       *    relationship, we transform a problem of skeletonization into a
       *    problem of skeleton growing. [...]"
       *
       *
       *  In Milena, the Pixel Superiority index is defined as follow:
       *  Let v = p.val(), the Pixel superiority index of p is the number of
       *  neighbor pixels having their value/level inferior or equal to
       *  p.val().
       *
       *  This algorithm keeps sites having their pixel superiority index
       *  greater than \p psi_threshold (6 by default).
       *
       *  For good results with 2D images, we advice you to use c8() as
       *  neighborhood.
       *
       */
      template <typename I, typename D, typename N>
      mln_concrete(I)
      crest(const Image<I>& input, const Image<D>& dist_map,
	    const Neighborhood<N>& nbh, unsigned psi_threshold);

      /// \overload
      /// psi_threshold is set to 6.
      template <typename I, typename D, typename N>
      mln_concrete(I)
      crest(const Image<I>& input, const Image<D>& dist_map,
	    const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY

      // IMPLEMENTATIONS

      namespace impl
      {

	namespace generic
	{

	  template <typename I, typename D, typename N>
	  mln_concrete(I)
	  crest(const Image<I>& input_, const Image<D>& dist_map_,
		const Neighborhood<N>& nbh_, unsigned psi_threshold)
	  {
	    trace::entering("topo::skeleton::impl::generic::crest");
	    const I& input = exact(input_);
	    const D& dist_map = exact(dist_map_);
	    const N& nbh = exact(nbh_);

	    mlc_equal(mln_value(I), bool)::check();
	    mln_precondition(input.is_valid());
	    mln_precondition(dist_map.is_valid());
	    mln_precondition(nbh.is_valid());

	    mln_concrete(I) is_crest;
	    initialize(is_crest, input);
	    data::fill(is_crest, false);

	    mln_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      if (!input(p) || dist_map(p) < static_cast<mln_value(D)>(0))
		continue;

	      unsigned nb_eq = 0;
	      unsigned nb_lt = 0;
	      for_all(n)
		if (input.domain().has(n)
		    // We want to only consider sites which are part of
		    // the skeleton. If this test is removed, sometimes
		    // edge sites are considered as sites with a high PSI
		    // which is wrong.
		    && dist_map(n) > static_cast<mln_value(D)>(0))
		{
		  if (dist_map(n) == dist_map(p))
		    ++nb_eq;
		  else if (dist_map(n) < dist_map(p))
		    ++nb_lt;
		}

	      if ((nb_lt + nb_eq) >= psi_threshold) // Pixel Superiority index
		is_crest(p) = true;
	    }

	    trace::exiting("topo::skeleton::impl::generic::crest");
	    return is_crest;
	  }

	} // end of namespace mln::topo::skeleton::impl::generic




	template <typename I, typename D, typename N>
	mln_concrete(I)
	crest_fastest_2d(const Image<I>& input_, const Image<D>& dist_map_,
			 const Neighborhood<N>& nbh_, unsigned psi_threshold)
	{
	  trace::entering("topo::skeleton::impl::crest_fastest_2d");

	  const I& input = exact(input_);
	  const D& dist_map = exact(dist_map_);
	  const N& nbh = exact(nbh_);

	  mlc_equal(mln_value(I), bool)::check();
	  mln_precondition(input.is_valid());
	  mln_precondition(dist_map.is_valid());
	  mln_precondition(nbh.is_valid());
	  mln_precondition(input.domain() == dist_map.domain());

	  extension::adjust(input, nbh);
	  border::equalize(input, dist_map, input.border());

	  mln_concrete(I) is_crest;
	  initialize(is_crest, input);
	  data::fill(is_crest, false);


	  mln_pixter(const I) p(input);
	  util::array<int> dp = mln::offsets_wrt(input, nbh);
	  unsigned n_nbhs = dp.nelements();
	  for_all(p)
	  {
	    if (!p.val()
		|| dist_map.element(p) < static_cast<mln_value(D)>(0))
	      continue;

	    unsigned nb_eq = 0;
	    unsigned nb_lt = 0;
	    for (unsigned i = 0; i < n_nbhs; ++i)
	    {
	      unsigned n = p.offset() + dp[i];
	      if (// We want to only consider sites which are part of
		  // the skeleton. If this test is removed, sometimes
		  // edge sites are considered as sites with a high PSI
		  // which is wrong.
		dist_map.element(n) > static_cast<mln_value(D)>(0))
	      {
		if (dist_map.element(n) == dist_map.element(p))
		  ++nb_eq;
		else if (dist_map.element(n) < dist_map.element(p))
		  ++nb_lt;
	      }

	      if ((nb_lt + nb_eq) >= psi_threshold) // Pixel Superiority index
		is_crest.element(p) = true;
	    }

	  }

	  trace::exiting("topo::skeleton::impl::crest_fastest_2d");
	  return is_crest;
	}

      } // end of namespace mln::topo::skeleton::impl


      // DISPATCH

      namespace internal
      {

	template <typename I, typename D, typename N>
	mln_concrete(I)
	crest_dispatch_2d(mln::trait::image::value_storage::any,
			  mln::trait::image::value_access::any,
			  mln::trait::image::ext_domain::any,
			  const Image<I>& input, const Image<D>& dist_map,
			  const Neighborhood<N>& nbh, unsigned psi_threshold)
	{
	  return skeleton::impl::generic::crest(input, dist_map,
						nbh, psi_threshold);
	}


	template <typename I, typename D, typename N>
	mln_concrete(I)
	crest_dispatch_2d(mln::trait::image::value_storage::one_block,
			  mln::trait::image::value_access::direct,
			  mln::trait::image::ext_domain::some,
			  const Image<I>& input, const Image<D>& dist_map,
			  const Neighborhood<N>& nbh, unsigned psi_threshold)
	{
	  return skeleton::impl::crest_fastest_2d(input, dist_map,
						  nbh, psi_threshold);
	}


	template <typename I, typename D, typename N>
	mln_concrete(I)
	crest_dispatch(const Image<I>& input, const Image<D>& dist_map,
	      const Neighborhood<N>& nbh, unsigned psi_threshold)
	{
	  unsigned dim = mln_site_(I)::dim;

	  if (dim == 2)
	    return
	      crest_dispatch_2d(mln_trait_image_value_storage(I)(),
				mln_trait_image_value_access(I)(),
				mln_trait_image_ext_domain(I)(),
				input, dist_map, nbh, psi_threshold);

	  return impl::generic::crest(input, dist_map, nbh, psi_threshold);
	}


      } // end of namespace mln::topo::skeleton::internal


      // FACADES

      template <typename I, typename D, typename N>
      mln_concrete(I)
      crest(const Image<I>& input, const Image<D>& dist_map,
	    const Neighborhood<N>& nbh, unsigned psi_threshold)
      {
	trace::entering("topo::skeleton::crest");

	mlc_equal(mln_value(I), bool)::check();
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(dist_map).is_valid());
	mln_precondition(exact(nbh).is_valid());

	mln_concrete(I)
	  output = internal::crest_dispatch(input, dist_map,
					    nbh, psi_threshold);

	trace::exiting("topo::skeleton::crest");
	return output;
      }


      template <typename I, typename D, typename N>
      mln_concrete(I)
      crest(const Image<I>& input, const Image<D>& dist_map,
	    const Neighborhood<N>& nbh)
      {
	return crest(input, dist_map, nbh, 6);
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::topo::skeleton

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_SKELETON_CREST_HH
