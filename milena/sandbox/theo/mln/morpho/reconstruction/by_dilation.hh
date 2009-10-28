// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_RECONSTRUCTION_BY_DILATION_HH
# define MLN_MORPHO_RECONSTRUCTION_BY_DILATION_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/data/compare.hh>
# include <mln/border/equalize.hh>
# include <mln/border/fill.hh>

# include <mln/core/site_set/p_queue.hh>
# include <mln/debug/println.hh>



namespace mln
{

  namespace morpho
  {

    namespace reconstruction
    {

      template <typename I, typename J, typename N>
      mln_concrete(I)
      by_dilation(const Image<I>& f, const Image<J>& g,
		  const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


      // Tests.

      namespace internal
      {

	template <typename I, typename J, typename N>
	inline
	void
	by_dilation_tests(const Image<I>& f_, const Image<J>& g_,
			  const Neighborhood<N>& nbh_)
	{
	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  mln_precondition(f.is_valid());
	  mln_precondition(g.is_valid());
	  mln_precondition(nbh.is_valid());

	  mln_precondition(f.domain() == g.domain()); // FIXME: Relax?
	  mln_precondition(f <= g);

	  // mlc_equal(mln_value(I), mln_value(J))::check(); // FIXME: Too strong!
	  // FIXME: Also check that we have a total ordering for values.

	  (void) f;
	  (void) g;
	  (void) nbh;
	}


	  
	template <typename Par>
	inline
	mln_site(Par) find_root(Par& parent, mln_site(Par) x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent, parent(x));
	}

	template <typename Par>
	inline
	unsigned find_root_fastest(Par& parent, unsigned x)
	{
	  if (parent.element(x) == x)
	    return x;
	  else
	    return parent.element(x) = find_root_fastest(parent, parent.element(x));
	}

      } // end of namespace mln::morpho::reconstruction::internal



      // Implementations.

      namespace impl
      {

	
	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_tufa_on_set(const Image<I>& f_, const Image<J>& g_,
				const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::reconstruction::impl::by_dilation_tufa_on_set");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::by_dilation_tests(f, g, nbh);
	      
	  typedef mln_site(I)  P;
	  typedef mln_value(I) V;

	  // Auxiliary data.
	  mln_ch_value(I, bool) deja_vu;
	  mln_ch_value(I, P)    parent;
	  mln_concrete(I)       output;

	  // Initialization.
	  {
	    initialize(parent, f);
	    initialize(deja_vu, f);
	    initialize(output, f);

	    data::fill(deja_vu, false);
	    data::fill(output, false);
	  }

	  // First pass.
	  {
	    mln_fwd_piter(I) p(f.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (g(p))                                            //  <-  D'
		{
		  // Make-Set.
		  parent(p) = p;
		  output(p) = f(p);                                //  <--  INIT

		  for_all(n) if (f.domain().has(n))
		    {
		      if (g(n)                                     //  <-  D'
			  && deja_vu(n))
			{
			  // Do-Union.
			  P r = internal::find_root(parent, n);
			  if (r != p)
			    {
			      // set_parent, i.e., union
			      parent(r) = p;
			      output(p) = output(p) || output(r);  //  <--  MERGE
			    }
			}
		      else
			;                                          //  <--  BORDER
		    }
		  deja_vu(p) = true;
		}
	  }

	  // Second pass.
	  {
	    mln_bkd_piter(I) p(f.domain());
	    for_all(p)
	      if (g(p))                                            //  <-  D'
		if (parent(p) != p)
		  output(p) = output(parent(p));
	  }

	  mln_postcondition(output >= f);
	  mln_postcondition(output <= g);

	  trace::exiting("morpho::reconstruction::impl::by_dilation_tufa_on_set");
	  return output;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_tufa_on_set_fastest(const Image<I>& f_, const Image<J>& g_,
					const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::reconstruction::impl::by_dilation_tufa_on_set_fastest");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::by_dilation_tests(f, g, nbh);

	  typedef mln_site(I)  P;
	  typedef mln_value(I) V;

	  // Auxiliary data.
	  mln_ch_value(I, bool)     deja_vu;
	  mln_ch_value(I, unsigned) parent;
	  mln_concrete(I)           output;

	  // Initialization.
	  {
	    border::equalize(f, g, nbh.delta());
	    border::fill(g, false);
	    initialize(parent, f);
	    initialize(output, f);

	    data::fill(output, false);
	  }

	  util::array<int> dp = negative_offsets_wrt(f, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  // First pass.
	  {
	    mln_fwd_pixter(const I) pxl(f);
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (g.element(p))                                    //  <-  D'
		{
		  // Make-Set.
		  parent.element(p) = p;
		  output.element(p) = f.element(p);                //  <--  INIT
		  
		  for (unsigned j = 0; j < n_nbhs; ++j)
		    {
		      unsigned n = p + dp[j];
		      if (g.element(n))                            //  <-  D'
			{
			  // Do-Union.
			  unsigned r = internal::find_root_fastest(parent, n);
			  if (r != p)
			    {
			      // set_parent, i.e., union
			      parent.element(r) = p;
			      output.element(p) = output.element(p) || output.element(r);  //  <--  MERGE
			    }
			}
		      else
			;                                          //  <--  BORDER
		    }
		}
	    }
	  }

	  // Second pass.
	  {
	    mln_bkd_pixter(const I) pxl(f);
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (g.element(p))                                            //  <-  D'
		if (parent.element(p) != p)
		  output.element(p) = output.element(parent.element(p));
	    }
	  }

	  mln_postcondition(output >= f);
	  mln_postcondition(output <= g);

	  trace::exiting("morpho::reconstruction::impl::by_dilation_tufa_on_set_fastest");
	  return output;
	}



	
	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_tufa_on_set_alt(const Image<I>& f_, const Image<J>& g_,
				    const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::reconstruction::impl::by_dilation_tufa_on_set_alt");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::by_dilation_tests(f, g, nbh);
	      
	  typedef mln_site(I)  P;
	  typedef mln_value(I) V;

	  // Auxiliary data.
	  mln_ch_value(I, bool) deja_vu;
	  mln_ch_value(I, P)    parent;
	  mln_concrete(I)       output;

	  // Initialization.
	  {
	    initialize(parent, f);
	    initialize(deja_vu, f);
	    initialize(output, f);

	    data::fill(deja_vu, false);
	    data::fill(output, f);
	  }

	  // First pass.
	  {
	    mln_fwd_piter(I) p(f.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (g(p) && ! f(p))                                  //  <-  D'
		{
		  // Make-Set.
		  parent(p) = p;
		  output(p) = false;                               //  <--  INIT

		  for_all(n) if (f.domain().has(n))
		    {
		      if (g(n) && ! f(n)                           //  <-  D'
			  && deja_vu(n))
			{
			  // Do-Union.
			  P r = internal::find_root(parent, n);
			  if (r != p)
			    {
			      // set_parent, i.e., union
			      parent(r) = p;
			      output(p) = output(p) || output(r);  //  <--  MERGE
			    }
			}
		      else
			if (f(n))
			  output(p) = true;                        //  <--  BORDER
		    }
		  deja_vu(p) = true;
		}
	  }

	  // Second pass.
	  {
	    mln_bkd_piter(I) p(f.domain());
	    for_all(p)
	      if (g(p) && ! f(p))                                            //  <-  D'
		if (parent(p) != p)
		  output(p) = output(parent(p));
	  }

	  mln_postcondition(output >= f);
	  mln_postcondition(output <= g);

	  trace::exiting("morpho::reconstruction::impl::by_dilation_tufa_on_set_alt");
	  return output;
	}




	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_hybrid_on_set(const Image<I>& f_, const Image<J>& g_,
				  const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::reconstruction::impl::by_dilation_hybrid_on_set");

	  // 	Both loops (just below) are merged into a single one
	  //    in the implementation below.  This merge results in
	  //    -10% at run-time.
	  
	  // 	// Sequence---Forth.
	  // 	{
	  // 	  data::fill(deja_vu, false);
	  // 	  mln_fwd_piter(I) p(f.domain());
	  // 	  mln_niter(N) n(nbh, p);
	  // 	  for_all(p)
	  // 	  {
	  // 	    if (g(p) && ! output(p))
	  // 	      {
	  // 		for_all(n)
	  // 		  if (f.domain().has(n) && deja_vu(n) // N+
	  // 		      && g(n) && output(n))
	  // 		    {
	  // 		      output(p) = true;
	  // 		      break;
	  // 		    }
	  // 	      }
	  // 	    if (output(p))
	  // 	      {
	  // 		for_all(n)
	  // 		  if (f.domain().has(n) && deja_vu(n) // N+
	  // 		      && output(n) == false && g(n) == true)
	  // 		    {
	  // 		      q.push(p);
	  // 		      break;
	  // 		    }
	  // 	      }
	  // 	  }
	  // 	}

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::by_dilation_tests(f, g, nbh);

	  typedef mln_value(I) V;
	  typedef mln_psite(I) P;
	  p_queue<P> q;

	  // Initialisation.
	  mln_concrete(I) output = duplicate(f);
	  mln_ch_value(I, bool) deja_vu;
	  initialize(deja_vu, f);

	  // Sequence---Back.
	  {
	    data::fill(deja_vu, false);
	    mln_bkd_piter(I) p(f.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      if (g(p) && ! output(p))
		{
		  for_all(n)
		    if (f.domain().has(n) && deja_vu(n) // N-
			&& g(n) && output(n))
		      {
			output(p) = true;
			break;
		      }
		}
	      deja_vu(p) = true;
	    }
	  }

	  // Sequence---Forth.
	  {
	    data::fill(deja_vu, false);
	    mln_fwd_piter(I) p(f.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      if (g(p) && ! output(p))
		{
		  bool queueable = false;
		  for_all(n)
		    if (f.domain().has(n) && deja_vu(n) // N+
			&& g(n))
		      {
			if (output(n))
			  output(p) = true;
			else
			  queueable = true;
		      }
		  if (output(p) && queueable)
		    q.push(p); // Need for propagation from p.
		}

	      deja_vu(p) = true;
	    }
	  }

	  // Propagation.
	  {
	    P p;
	    mln_niter(N) n(nbh, p);
	    while (! q.is_empty())
	      {
		p = q.pop_front();
		mln_invariant(output(p) == true);
		for_all(n)
		  if (f.domain().has(n)
		      && output(n) == false && g(n) == true)
		    {
		      output(n) = true;
		      q.push(n);
		    }
	      }
	  }

	  trace::exiting("morpho::reconstruction::impl::by_dilation_hybrid_on_set");
	  return output;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_hybrid_on_set_fastest(const Image<I>& f_, const Image<J>& g_,
					  const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::reconstruction::impl::by_dilation_hybrid_on_set_fastest");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::by_dilation_tests(f, g, nbh);

	  typedef mln_value(I) V;
	  typedef mln_psite(I) P;
	  p_queue<unsigned> q;

	  // Initialisation.
	  border::equalize(f, g, nbh.delta());
	  mln_concrete(I) output = duplicate(f);
	  
	  util::array<int> dp = negative_offsets_wrt(f, nbh);
	  unsigned n_nbhs = dp.nelements();

	  // Sequence---Back.
	  {
	    mln_bkd_pixter(const I) pxl(f);
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (g.element(p) && ! output.element(p))
		{
		  for (unsigned j = 0; j < n_nbhs; ++j)
		    {
		      unsigned n = p - dp[j]; // n in N-
		      if (g.element(n) && output.element(n))
			{
			  output.element(p) = true;
			  break;
			}
		    }
		}
	    }
	  }

	  // 	  debug::println("output (back)", output);
	  
	  // Sequence---Forth.
	  {
	    mln_fwd_pixter(const I) pxl(f);
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (g.element(p) && ! output.element(p))
		{
		  bool queueable = false;
		  for (unsigned j = 0; j < n_nbhs; ++j)
		    {
		      unsigned n = p + dp[j]; // n in N+
		      if (g.element(n))
			{
			  if (output.element(n))
			    output.element(p) = true;
			  else
			    queueable = true;
			}
		    }
		  if (output.element(p) && queueable)
		    q.push(p); // Need for propagation from p.
		}
	    }
	  }

	  // 	  debug::println("output (forth)", output);
	  // 	  std::cout << q << std::endl;

	  // Propagation.
	  {
	    dp = offsets_wrt(f, nbh);
	    n_nbhs = dp.nelements();

	    unsigned p;
	    while (! q.is_empty())
	      {
		p = q.pop_front();
		mln_invariant(output.element(p) == true);
		for (unsigned j = 0; j < n_nbhs; ++j)
		  {
		    unsigned n = p + dp[j]; // n in N
		    if (output.element(n) == false && g.element(n) == true)
		      {
			output.element(n) = true;
			q.push(n);
		      }
		  }
	      }
	  }

	  // 	  debug::println("output (propag)", output);

	  trace::exiting("morpho::reconstruction::impl::by_dilation_hybrid_on_set_fastest");
	  return output;
	}







	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_hybrid_on_set__two_loops(const Image<I>& f_, const Image<J>& g_,
					     const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::reconstruction::impl::by_dilation_hybrid_on_set__two_loops");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::by_dilation_tests(f, g, nbh);

	  typedef mln_value(I) V;
	  typedef mln_psite(I) P;
	  p_queue<P> q;

	  // Initialisation.
	  mln_concrete(I) output = duplicate(f);
	  mln_ch_value(I, bool) deja_vu;
	  initialize(deja_vu, f);

	  // Sequence---Back.
	  {
	    data::fill(deja_vu, false);
	    mln_bkd_piter(I) p(f.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      if (g(p) && ! output(p))
		{
		  for_all(n)
		    if (f.domain().has(n) && deja_vu(n) // N-
			&& g(n) && output(n))
		      {
			output(p) = true;
			break;
		      }
		}
	      deja_vu(p) = true;
	    }
	  }
	  
	  // Sequence---Forth.
	  {
	    data::fill(deja_vu, false);
	    mln_fwd_piter(I) p(f.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      if (g(p) && ! output(p))
		{
		  for_all(n)
		    if (f.domain().has(n) && deja_vu(n) // N+
			&& g(n) && output(n))
		      {
			output(p) = true;
			break;
		      }
		}
	      if (output(p))
		{
		  for_all(n)
		    if (f.domain().has(n) && deja_vu(n) // N+
			&& output(n) == false && g(n) == true)
		      {
			q.push(p);
			break;
		      }
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
		mln_invariant(output(p) == true);
		for_all(n)
		  if (f.domain().has(n)
		      && output(n) == false && g(n) == true)
		    {
		      output(n) = true;
		      q.push(n);
		    }
	      }
	  }

	  trace::exiting("morpho::reconstruction::impl::by_dilation_hybrid_on_set__two_loops");
	  return output;
	}






      } // end of namespace mln::morpho::reconstruction::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_dispatch(trait::image::kind::logic,
			     const Image<I>& f, const Image<J>& g,
			     const Neighborhood<N>& nbh)
	{
	  return impl::by_dilation_tufa_on_set(f, g, nbh); // FIXME
	}

	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	by_dilation_dispatch(const Image<I>& f, const Image<J>& g,
			     const Neighborhood<N>& nbh)
	{
	  return by_dilation_dispatch(mln_trait_image_kind(I)(),
				      f, g, nbh);
	}

      } // end of namespace mln::morpho::reconstruction::internal


      // Facade.

      template <typename I, typename J, typename N>
      inline
      mln_concrete(I)
      by_dilation(const Image<I>& f, const Image<J>& g,
		  const Neighborhood<N>& nbh)
      {
	trace::entering("morpho::reconstruction::by_dilation");

	internal::by_dilation_tests(f, g, nbh);

	mln_concrete(I) output;
	output = internal::by_dilation_dispatch(f, g, nbh);

	trace::exiting("morpho::reconstruction::by_dilation");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::reconstruction

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RECONSTRUCTION_BY_DILATION_HH
