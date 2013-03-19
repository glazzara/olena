// Copyright (C) 2008, 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_CANVAS_DISTANCE_GEODESIC_HH
# define MLN_CANVAS_DISTANCE_GEODESIC_HH

/// \file
///
/// Discrete geodesic distance canvas.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/core/site_set/p_queue_fast.hh>
# include <queue>

# include <mln/data/fill.hh>
# include <mln/extension/adjust_fill.hh>


namespace mln
{

  namespace canvas
  {

    /*! \brief Discrete geodesic distance canvas.
     *
     * \ingroup modcanvas
     */
    template <typename I, typename N, typename D,
	      typename F>
    mln_ch_value(I, D)
    distance_geodesic(const Image<I>& input, const Neighborhood<N>& nbh,
		      D max, F& functor);



# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I, typename N, typename D,
		typename F>
      void
      distance_geodesic_tests(const Image<I>& input_,
			      const Neighborhood<N>& nbh_, D max,
			      F& functor)
      {
	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	mln_precondition(input.is_valid());
	mln_precondition(nbh.is_valid());

	(void) input;
	(void) nbh;
	(void) max;
	(void) functor;
      }


    } // of namespace mln::canvas::internal



    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N, typename D,
		  typename F>
	mln_ch_value(I, D)
	distance_geodesic(const Image<I>& input_, const Neighborhood<N>& nbh_,
			  D max, F& functor)
	{
	  mln_trace("canvas::impl::generic::distance_geodesic");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  internal::distance_geodesic_tests(input, nbh, max, functor);

	  mln_precondition(input.is_valid());
	  mln_precondition(nbh.is_valid());

	  mln_ch_value(I, D) dmap; // Distance map is aux data.
	  initialize(dmap, input);

	  typedef mln_site(I) P;
	  p_queue_fast<P> q;

	  // Initialization.
	  {
	    functor.init(input); // <-- init
	    data::fill(dmap, max);

	    mln_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (functor.inqueue_p_wrt_input_p(input(p))) // <-- inqueue_p_wrt_input_p
		{
		  functor.init_p(p); // <-- init_p
		  dmap(p) = 0;
		  for_all(n)
		    if (input.domain().has(n) &&
			functor.inqueue_p_wrt_input_n(input(n))) // <-- inqueue_p_wrt_input_n
		      {
			q.push(p);
			break;
		      }
		}
	  }

	  // Propagation.
	  {
	    P p;
	    mln_niter(N) n(nbh, p);
	    while (! q.is_empty())
	      {
		p = q.pop_front();
		if (dmap(p) == max)
		  {
		    // Saturation so stop.
		    q.clear();
		    break;
		  }
		for_all(n)
		  if (input.domain().has(n) && dmap(n) == max)
		    {
		      dmap(n) = dmap(p) + 1;
		      functor.process(p, n); // <- process
		      q.push(n);
		    }
	      }
	  }

	  return dmap;
	}

      } // of namespace mln::canvas::impl::generic



      // Fastest version.

      template <typename I, typename N, typename D,
		typename F>
      mln_ch_value(I, D)
      distance_geodesic_fastest(const Image<I>& input_,
				const Neighborhood<N>& nbh_,
				D max,
				F& functor)
      {
	mln_trace("canvas::impl::distance_geodesic_fastest");

	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	internal::distance_geodesic_tests(input, nbh, max, functor);

	extension::adjust(input, nbh);

	mln_ch_value(I, D) dmap; // Distance map is aux data.
	initialize(dmap, input);

	std::queue<unsigned> q;

	// Initialization.
	{
	  functor.init_(input); // <-- init
	  data::fill(dmap, max);
	  // For the extension to be ignored:
	  extension::fill(input, true);
	  extension::fill(dmap, D(0));

	  mln_pixter(const I)    p(input);
	  mln_nixter(const I, N) n(p, nbh);
	  for_all(p)
	    if (functor.inqueue_p_wrt_input_p_(p.val())) // <-- inqueue_p_wrt_input_p
	      {
		functor.init_p_(p); // <-- init_p
		dmap.element(p.offset()) = 0;
		for_all(n)
		  if (functor.inqueue_p_wrt_input_n_(n.val())) // <-- inqueue_p_wrt_input_n
		    {
		      q.push(p.offset());
		      break;
		    }
	      }
	}

	// Propagation.
	{
	  unsigned p;

	  util::array<int> dp = offsets_wrt(input, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  while (! q.empty())
	    {
	      p = q.front();
	      q.pop();
	      if (dmap.element(p) == max)
		// Saturation so stop.
		break;
	      for (unsigned i = 0; i < n_nbhs; ++i)
		{
		  unsigned n = p + dp[i];
		  if (dmap.element(n) == max)
		    {
		      dmap.element(n) = dmap.element(p) + 1;
		      functor.process_(p, n); // <- process
		      q.push(n);
		    }
		}
	    }
	}

	return dmap;
      }


    } // of namespace mln::canvas::impl



    // Dispatch.

    namespace internal
    {

      template <typename I, typename N, typename D,
		typename F>
      inline
      mln_ch_value(I, D)
      distance_geodesic_dispatch(metal::false_,
				 const Image<I>& input,
				 const Neighborhood<N>& nbh, D max,
				 F& functor)
      {
	return impl::generic::distance_geodesic(input, nbh, max,
						functor);
      }

      template <typename I, typename N, typename D,
		typename F>
      inline
      mln_ch_value(I, D)
      distance_geodesic_dispatch(metal::true_,
				 const Image<I>& input,
				 const Neighborhood<N>& nbh, D max,
				 F& functor)
      {
  	return impl::distance_geodesic_fastest(input, nbh, max, functor);
// 	return impl::generic::distance_geodesic(input, nbh, max,
// 						functor);
      }

      template <typename I, typename N, typename D,
		typename F>
      inline
      mln_ch_value(I, D)
      distance_geodesic_dispatch(const Image<I>& input,
				 const Neighborhood<N>& nbh, D max,
				 F& functor)
      {
	enum {
	  test = mlc_equal(mln_trait_image_speed(I),
			   trait::image::speed::fastest)::value
	  &&
	  mln_is_simple_neighborhood(N)::value
	};
	return distance_geodesic_dispatch(metal::bool_<test>(),
					  input, nbh, max,
					  functor);
      }


    } // of namespace mln::canvas::internal



    // Facade.

    template <typename I, typename N, typename D,
	      typename F>
    inline
    mln_ch_value(I, D)
    distance_geodesic(const Image<I>& input, const Neighborhood<N>& nbh,
		      D max, F& functor)
    {
      mln_trace("canvas::distance_geodesic");

      internal::distance_geodesic_tests(input, nbh, max, functor);

      mln_ch_value(I,D) output;
      output = internal::distance_geodesic_dispatch(input, nbh, max, functor);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_DISTANCE_GEODESIC_HH
