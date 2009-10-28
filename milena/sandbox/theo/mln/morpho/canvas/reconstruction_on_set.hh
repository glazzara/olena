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

#ifndef MLN_MORPHO_CANVAS_RECONSTRUCTION_ON_SET_UNION_FIND_HH
# define MLN_MORPHO_CANVAS_RECONSTRUCTION_ON_SET_UNION_FIND_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/morpho/canvas/internal/find_root.hh>

# include <mln/data/fill.hh>
# include <mln/border/equalize.hh>
# include <mln/border/fill.hh>



namespace mln
{

  namespace morpho
  {

    namespace canvas
    {


      template <typename I, typename J, typename N,
		typename F>
      mln_concrete(I)
      reconstruction_on_set_union_find(const Image<I>& f, const Image<J>& g, const Neighborhood<N>& nbh,
				       F& functor);




# ifndef MLN_INCLUDE_ONLY


      // Implementations.

      namespace impl
      {

	namespace generic
	{

	  template <typename I, typename J, typename N,
		    typename F>
	  inline
	  mln_concrete(I)
	  reconstruction_on_set_union_find(const Image<I>& f_, const Image<J>& g_, const Neighborhood<N>& nbh_,
					   F& functor)
	  {
	    trace::entering("morpho::canvas::impl::generic::reconstruction_on_set_union_find");

	    const I& f = exact(f_);
	    const J& g = exact(g_);
	    const N& nbh = exact(nbh_);
	    
	    typedef mln_psite(I) P;
	    typedef mln_value(I) V;

	    // Auxiliary data.
	    mln_ch_value(I, bool) deja_vu;
	    mln_ch_value(I, P)    parent;
	    mln_concrete(I)       output;

	    // Initialization.
	    {
	      initialize(output, f);
	      functor.output = output;

	      initialize(parent, f);
	      initialize(deja_vu, f);
	      data::fill(deja_vu, false);

	      functor.set_default();                               //  <--  set_default
	    }

	    // First pass.
	    {
	      mln_fwd_piter(I) p(f.domain());
	      mln_niter(N) n(nbh, p);
	      for_all(p)
		if (functor.domain(p))                             //  <--  domain
		  {
		    // Make-Set.
		    parent(p) = p;
		    functor.init(p);                               //  <--  init

		    for_all(n) if (f.domain().has(n))
		      {
			// See below (*)
			if (functor.domain(n))                     //  <--  domain
			  {
			    if (deja_vu(n))
			      {
				// Do-Union.
				P r = internal::find_root(parent, n);
				if (r != p)
				  {
				    parent(r) = p;
				    functor.merge(p, r);           //  <--  merge
				  }
			      }
			  }
			else
			  {
			    mln_invariant(deja_vu(n) == false);
			    functor.border(p, n);                  //  <--  border
			  }
		      }
		    deja_vu(p) = true;
		  }
	    }

	    // Second pass.
	    {
	      mln_bkd_piter(I) p(f.domain());
	      for_all(p)
		if (functor.domain(p))                             //  <--  domain
		  if (parent(p) != p)
		    output(p) = output(parent(p));
	    }

	    // (*) Technical note.
	    //                      n in D'
	    //              |   true   |    false     |
	    // deja_vu(n)   +----------+--------------+
	    //      true    | do-union | impossible   |
	    //      false   |  no-op   | border(p, n) |
	    //
	    // The no-op is sound because /n/ will be processed as a "p in D'" later on.

	    trace::exiting("morpho::canvas::impl::generic::reconstruction_on_set_union_find");
	    return output;
	  }

	} // end of namespace morpho::canvas::impl::generic



	template <typename I, typename J, typename N,
		  typename F>
	inline
	mln_concrete(I)
	reconstruction_on_set_union_find_fastest(const Image<I>& f_, const Image<J>& g_, const Neighborhood<N>& nbh_,
						 F& functor)
	{
	  trace::entering("morpho::canvas::impl::reconstruction_on_set_union_find_fastest");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  typedef mln_psite(I) P;
	  typedef mln_value(I) V;

	  // Auxiliary data.
	  mln_ch_value(I, bool)     deja_vu;
	  mln_ch_value(I, unsigned) parent;
	  mln_concrete(I)           output;

	  // Initialization.
	  {
	    border::equalize(f, g, nbh.delta());
	    functor.set_extension_();                              //  <--  set_extension

	    initialize(output, f);
	    functor.output = output;

	    initialize(parent, f);

	    functor.set_default();                                 //  <--  set_default
	  }

	  util::array<int> dp = negative_offsets_wrt(f, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  // First pass.
	  {
	    mln_fwd_pixter(const I) pxl(f);
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (functor.domain_(p))                              //  <-- domain
		{
		  // Make-Set.
		  parent.element(p) = p;
		  functor.init_(p);                                //  <--  init
		  
		  // Deja-vu part.
		  for (unsigned j = 0; j < n_nbhs; ++j)
		    {
		      unsigned n = p + dp[j];
		      if (functor.domain_(n))                      //  <--  domain
			{
			  mln_invariant(f.domain().has(f.point_at_index(n)));
			  // Do-Union.
			  unsigned r = internal::find_root_fastest(parent, n);
			  if (r != p)
			    {
			      // set_parent, i.e., union
			      parent.element(r) = p;
			      functor.merge_(p, r);                //  <--  merge
			    }
			}
		      else
			functor.border_(p, n);                     //  <--  border
		    }

		  // Non-(deja-vu) part.
		  if (functor.inspect_border_())
		    for (unsigned j = 0; j < n_nbhs; ++j)
		      {
			unsigned n = p - dp[j];
			if (! functor.domain_(n))                  //  <--  domain
			  functor.border_(p, n);                   //  <--  border
		      }

		  // (*) Technical note.
		  //                      n in D'
		  //              |   true   |    false     |
		  // deja_vu(n)   +----------+--------------+
		  //      true    | do-union | border(p, n) |
		  //      false   |  no-op   | border(p, n) |
		  //
		  // In this 'fastest' version, deja-vu(n) does not
		  // mean that n is in D' so we have to handle the
		  // case "deja_vu(n) and n not in D'".
		}
	    }
	  }

	  // Second pass.
	  {
	    mln_bkd_pixter(const I) pxl(f);
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (functor.domain_(p))                              //  <-- domain
		if (parent.element(p) != p)
		  output.element(p) = output.element(parent.element(p));
	    }
	  }

	  trace::exiting("morpho::canvas::impl::reconstruction_on_set_union_find_fastest");
	  return output;
	}


      } // end of namespace mln::morpho::canvas::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename J, typename N,
		  typename F>
	inline
	mln_concrete(I)
	reconstruction_on_set_union_find_dispatch(metal::false_,
						  const Image<I>& f, const Image<J>& g, const Neighborhood<N>& nbh,
						  F& functor)
	{
	  return impl::generic::reconstruction_on_set_union_find(f, g, nbh,
								 functor);
	}

	template <typename I, typename J, typename N,
		  typename F>
	inline
	mln_concrete(I)
	reconstruction_on_set_union_find_dispatch(metal::true_,
						  const Image<I>& f, const Image<J>& g, const Neighborhood<N>& nbh,
						  F& functor)
	{
	  return impl::reconstruction_on_set_union_find_fastest(f, g, nbh,
								functor);
	}

	template <typename I, typename J, typename N,
		  typename F>
	inline
	mln_concrete(I)
	reconstruction_on_set_union_find_dispatch(const Image<I>& f, const Image<J>& g, const Neighborhood<N>& nbh,
						  F& functor)
	{
	  enum {
	    is_fastest = (mlc_equal(mln_trait_image_speed(I),
				    trait::image::speed::fastest)::value &&
			  mlc_equal(mln_trait_image_speed(J),
				    trait::image::speed::fastest)::value &&
			  mln_is_simple_neighborhood(N)::value)
	  };
	  return reconstruction_on_set_union_find_dispatch(metal::bool_<is_fastest>(),
							   f, g, nbh,
							   functor);
	}

      } // end of namespace mln::morpho::canvas::internal


      // Facade.

      template <typename I, typename J, typename N,
		typename F>
      inline
      mln_concrete(I)
      reconstruction_on_set_union_find(const Image<I>& f, const Image<J>& g, const Neighborhood<N>& nbh,
				       F& functor)
      {
	trace::entering("morpho::canvas::reconstruction_on_set_union_find");

	mln_concrete(I) output;
	output = internal::reconstruction_on_set_union_find_dispatch(f, g, nbh, functor);

	trace::exiting("morpho::canvas::reconstruction_on_set_union_find");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::canvas

  } // end of namespace mln::morpho
  
} // end of namespace mln


#endif // ! MLN_MORPHO_CANVAS_RECONSTRUCTION_ON_SET_UNION_FIND_HH
