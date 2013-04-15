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

#ifndef MLN_CANVAS_DISTANCE_FRONT_HH
# define MLN_CANVAS_DISTANCE_FRONT_HH

/// \file
///
/// Canvas of discrete distance computation by thick front propagation.

# include <vector>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/data/fill.hh>
# include <mln/accu/stat/max.hh>
# include <mln/extension/adjust_fill.hh>


namespace mln
{

  namespace canvas
  {

    /*! \brief Canvas of discrete distance computation by thick front
     *  propagation.
     *
     * \ingroup modcanvas
     */
    template <typename I,
	      typename N, typename W, typename D,
	      typename F>
    mln_ch_value(I, D)
      distance_front(const Image<I>& input,
		     const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max,
		     F& functor);



# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I,
		typename N, typename W, typename D,
		typename F>
      void
      distance_front_tests(const Image<I>& input_,
			   const Neighborhood<N>& nbh_,
			   const Weighted_Window<W>& w_win_,
			   D max,
			   F& functor)
      {
	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);
	const W& w_win = exact(w_win_);

	mln_precondition(input.is_valid());
	mln_precondition(nbh.is_valid());

	(void) input;
	(void) nbh;
	(void) max;
	(void) functor;
	(void) w_win;
      }


    } // of namespace mln::canvas::internal



    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I,
		  typename N, typename W, typename D,
		  typename F>
	mln_ch_value(I, D)
	  distance_front(const Image<I>& input_,
			 const Neighborhood<N>& nbh_,
			 const Weighted_Window<W>& w_win_,
			 D max,
			 F& functor)
	{
	  mln_trace("canvas::impl::generic::distance_front");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);
	  const W& w_win = exact(w_win_);

	  mln_precondition(input.is_valid());
	  mln_precondition(w_win.is_valid());

	  typedef mln_site(I) P;
	  typedef std::vector<P> bucket_t;

	  // Distance map.
	  mln_ch_value(I, D) dmap;
	  initialize(dmap, input);
	  data::fill(dmap, max);

	  // Mod determination.
	  unsigned mod;
	  {
	    accu::stat::max<unsigned> m;
	    for (unsigned i = 0; i < w_win.size(); ++i)
	      m.take(w_win.w(i));
	    mod = unsigned(m) + 1;
	  }

	  // Aux data.
	  std::vector<bucket_t> bucket(mod);
	  unsigned bucket_size = 0;

	  // Initialization.
	  {
	    functor.init(input); // <-- init
	    mln_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (functor.inqueue_p_wrt_input_p(input(p))) // <-- inqueue_p_wrt_input_p
		{
		  dmap(p) = 0;
		  for_all(n)
		    if (input.domain().has(n) &&
			functor.inqueue_p_wrt_input_n(input(n))) // <-- inqueue_p_wrt_input_n
		      {
			bucket[0].push_back(p);
			++bucket_size;
			break;
		      }
		}
	  } // end of Initialization.

	  // Propagation.
	  {
	    P p;
	    mln_qiter(W) q(w_win, p);
	    for (unsigned d = 0; bucket_size != 0; ++d)
	      {
		bucket_t& bucket_d = bucket[d % mod];
		for (unsigned i = 0; i < bucket_d.size(); ++i)
		  {
		    p = bucket_d[i];

		    if (dmap(p) == max)
		      {
			// Saturation so stop.
			bucket_size = bucket_d.size(); // So at end bucket_size == 0.
			break;
		      }

		    if (dmap(p) < d)
		      // p has already been processed, having a distance less than d.
		      continue;

		    for_all(q)
		      if (dmap.domain().has(q) && dmap(q) > d)
			{
			  unsigned d_ = d + q.w();
			  if (d_ < dmap(q))
			    {
			      dmap(q) = d_;
			      functor.process(p, q); // <- process
			      bucket[d_ % mod].push_back(q);
			      ++bucket_size;
			    }
			}
		  }
		bucket_size -= bucket_d.size();
		bucket_d.clear();
	      }
	  } // end of Propagation.

	  return dmap;
	}

      } // of namespace mln::canvas::impl::generic



      // Fastest version.

      template <typename I,
		typename N, typename W, typename D,
		typename F>
      mln_ch_value(I, D)
	distance_front_fastest(const Image<I>& input_,
			       const Neighborhood<N>& nbh_,
			       const Weighted_Window<W>& w_win_,
			       D max, F& functor)
      {
	mln_trace("canvas::impl::distance_front_fastest");

	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);
	const W& w_win = exact(w_win_);

	mln_precondition(input.is_valid());
	mln_precondition(w_win.is_valid());

	// Handling w_win.
	extension::adjust(input, w_win);
	const unsigned n_ws = w_win.size();
	util::array<int> dp = offsets_wrt(input, w_win.win());
	mln_invariant(dp.nelements() == n_ws);

	// Distance map.
	mln_ch_value(I, D) dmap;
	initialize(dmap, input);
	data::fill(dmap, max);

	// Mod determination.
	unsigned mod;
	{
	  accu::stat::max<unsigned> m;
	  for (unsigned i = 0; i < w_win.size(); ++i)
	    m.take(w_win.w(i));
	  mod = unsigned(m) + 1;
	}

	// Aux data.
	typedef std::vector<unsigned> bucket_t;
	std::vector<bucket_t> bucket(mod);
	unsigned bucket_size = 0;

	// Initialization.
	{
	  functor.init_(input); // <-- init

	  // For the extension to be ignored:
	  extension::fill(input, true);
	  extension::fill(dmap, D(0));

	  mln_pixter(const I)    p(input);
	  mln_nixter(const I, N) n(p, nbh);
	  for_all(p)
	    if (functor.inqueue_p_wrt_input_p_(p.val())) // <-- inqueue_p_wrt_input_p
	      {
		dmap.element(p.offset()) = 0;
		for_all(n)
		  if (functor.inqueue_p_wrt_input_n_(n.val())) // <-- inqueue_p_wrt_input_n
		    {
		      bucket[0].push_back(p.offset());
		      ++bucket_size;
		      break;
		    }
	      }
	} // end of Initialization.

	// Propagation.
	{
	  unsigned p;

	  for (unsigned d = 0; bucket_size != 0; ++d)
	    {
	      bucket_t& bucket_d = bucket[d % mod];
	      for (unsigned i = 0; i < bucket_d.size(); ++i)
		{
		  p = bucket_d[i];

		  if (dmap.element(p) == max)
		    {
		      // Saturation so stop.
		      bucket_size = bucket_d.size(); // So at end bucket_size == 0.
		      break;
		    }

		  if (dmap.element(p) < d)
		    // p has already been processed, having a distance less than d.
		    continue;

		  for (unsigned i = 0; i < n_ws; ++i)
		  {
		    unsigned q = p + dp[i];
		    if (dmap.element(q) > d)
		      {
			unsigned d_ = d + w_win.w(i);
			if (d_ < dmap.element(q))
			  {
			    dmap.element(q) = d_;
			    functor.process_(p, q); // <- process
			    bucket[d_ % mod].push_back(q);
			    ++bucket_size;
			  }
		      }
		  }
		}
	      bucket_size -= bucket_d.size();
	      bucket_d.clear();
	    }
	} // end of Propagation.

	return dmap;
      }


    } // of namespace mln::canvas::impl



    // Dispatch.

    namespace internal
    {

      template <typename I,
		typename N, typename W, typename D,
		typename F>
      inline
      mln_ch_value(I, D)
	distance_front_dispatch(metal::false_,
				const Image<I>& input,
				const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win,
				D max, F& functor)
      {
	return impl::generic::distance_front(input, nbh, w_win, max, functor);
      }

      template <typename I,
		typename N, typename W, typename D,
		typename F>
      inline
      mln_ch_value(I, D)
	distance_front_dispatch(metal::true_,
				const Image<I>& input,
				const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win,
				D max, F& functor)
      {
  	return impl::distance_front_fastest(input, nbh, w_win, max, functor);
      }

      template <typename I,
		typename N, typename W, typename D,
		typename F>
      inline
      mln_ch_value(I, D)
	distance_front_dispatch(const Image<I>& input,
				const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win,
				D max, F& functor)
      {
	enum {
	  test = mlc_equal(mln_trait_image_speed(I),
			   trait::image::speed::fastest)::value
	  &&
	  mln_is_simple_neighborhood(N)::value
	  &&
	  mln_is_simple_weighted_window(W)::value
	};
	return distance_front_dispatch(metal::bool_<test>(),
				       input, nbh, w_win, max, functor);
      }


    } // of namespace mln::canvas::internal



    // Facade.

    template <typename I,
	      typename N, typename W, typename D,
	      typename F>
    inline
    mln_ch_value(I, D)
      distance_front(const Image<I>& input,
		     const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win,
		     D max, F& functor)
    {
      mln_trace("canvas::distance_front");

      internal::distance_front_tests(input, nbh, w_win, max, functor);

      mln_ch_value(I,D) output;
      output = internal::distance_front_dispatch(input, nbh, w_win, max, functor);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_DISTANCE_FRONT_HH
