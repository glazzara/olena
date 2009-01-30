// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_CANVAS_MORPHO_ALGEBRAIC_UNION_FIND_HH
# define MLN_CANVAS_MORPHO_ALGEBRAIC_UNION_FIND_HH

/// \file mln/canvas/morpho/algebraic_union_find.hh
///
/// FIXME: Doc!

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace canvas
  {

    namespace morpho
    {

      namespace impl
      {

	// Generic version.

	namespace generic
	{

	  template <typename I>
	  inline
	  mln_psite(I)
	  find_root(I& parent, const mln_psite(I)& x)
	  {
	    if (parent(x) == x)
	      return x;
	    else
	      return parent(x) = find_root(parent, parent(x));
	  }

	  template <typename I, typename N, typename F>
	  inline
	  mln_concrete(I)
	  algebraic_union_find(const Image<I>& input_,
			       const Neighborhood<N>& nbh_,
			       F& f)
	  {
	    trace::entering("canvas::morpho::impl::generic::algebraic_union_find");

	    // FIXME: Tests?

	    typedef typename F::S S;
	    typedef typename F::A A;

	    const I& input = exact(input_);
	    const N& nbh = exact(nbh_);

	    mln_concrete(I) output;
	    initialize(output, input);

	    // Local type.
	    typedef mln_psite(I) P;

	    // Auxiliary data.
	    mln_ch_value(I, bool)  deja_vu;
	    mln_ch_value(I, bool)  activity;
	    mln_ch_value(I, P)     parent;
	    mln_ch_value(I, A)     data;

	    // Initialization.
	    {
	      initialize(deja_vu, input);
	      mln::data::fill(deja_vu, false);
	      initialize(activity, input);
	      mln::data::fill(activity, true);
	      initialize(parent, input);
	      initialize(data, input);
	      f.init(); // init required.
	    }

	    // First pass.
	    {
	      mln_fwd_piter(S) p(f.s); // s required.
	      mln_niter(N) n(nbh, p);
	      for_all(p)
	      {
		// Make set.
		{
		  parent(p) = p;
		  /* FIXME: What if the value_type of DATA (i.e., A) were not
		     based on a accu::count<mln::pix>?  Currently, nothing
		     enforces this, but the code below expects this line to be
		     valid:

		     data(p).take_as_init(make::pix(f.input, p))

		     which probably restricts the kind of input images.

		     If we want to be more generic, the initialization should
		     read something like:

		     init_data(p);

		     i.e., the functor for the initialization of data should
		     be passed as an argument to the canvas' ctor.

		     Of course, we might want to restrict attributes to the
		     accumulator accu::count<mln::pix> (which is perfectly
		     acceptable), but then this class should statically check
		     the conformance of the template parameter A to this
		     constraint.  */
		  data(p).take_as_init(make::pix(input, p)); // FIXME: algebraic so p!
		}

		for_all(n)
		  if (input.domain().has(n) && deja_vu(n))
		    {
		      //do_union(n, p);
		      P r = find_root(parent, n);
		      if (r != p)
			{
			  if (input(r) == input(p) || (activity(r) && f.is_active(data(r)))) // Equiv(r, p)
			    // Either a flat zone or the component of r is still growing.
			    {
			      /* FIXME: Same remark as above concerning the
				 initialization of data(p); instead of

				 data(p).take(data(r));

				 we should (or could) have

				 unite_data(p, r);

				 so as to keep the generic aspect of this canvas
				 (as long as the set of acceptable types for the
				 template parameter A is not bound).  */
			      data(p).take(data(r));
			      parent(r) = p;
			      if (activity(r) == false)
				activity(p) = false;
			    }
			  else
			    {
			      activity(p) = false;
			      f.inactivate(data(p));
			    }
			}
		    }
		deja_vu(p) = true;
	      }
	    }

	    // Second pass.
	    {
	      mln_bkd_piter(S) p(f.s);
	      for_all(p)
		if (parent(p) == p) // p is root.
		  output(p) = input(p);
		else
		  output(p) = output(parent(p));
	    }

	    /*
	      Change 2nd pass into:
	      for_all(p) if (not is_root(p)) output(p) = output(parent(p));
	      and add in init:
	      mln::data::fill(output, input);
	    */
	    trace::exiting("canvas::morpho::impl::generic::algebraic_union_find");

	    return output;
	  }

	} // end of namespace mln::canvas::morpho::impl::generic



	// Fastest version.

	template <typename I>
	inline
	unsigned
	find_root_fastest(I& parent, unsigned x)
	{
	  if (parent.element(x) == x)
	    return x;
	  else
	    return parent.element(x) = find_root(parent, parent.element(x));
	}


	template <typename I, typename N, typename F>
	inline
	mln_concrete(I)
	algebraic_union_find_fastest(const Image<I>& input_,
				     const Neighborhood<N>& nbh_,
				     F& f)
	{
	  trace::entering("canvas::morpho::impl::algebraic_union_find_fastest");

	  // FIXME: Tests?

	  typedef typename F::S S;
	  typedef typename F::A A;

	  const I& input = exact(input_);
	  const N& nbh = exact(nbh_);

	  mln_concrete(I) output;
	  initialize(output, input);

	  // Local type.
	  typedef mln_psite(I) P;

	  // Auxiliary data.
	  mln_ch_value(I, bool)  deja_vu;
	  mln_ch_value(I, bool)  activity;
	  mln_ch_value(I, unsigned) parent;
	  mln_ch_value(I, A)     data;

	  // Initialization.
	  {
	    initialize(deja_vu, input);
	    mln::data::fill(deja_vu, false);
	    initialize(activity, input);
	    mln::data::fill(activity, true);
	    initialize(parent, input);
	    mln::data::fill(parent, 0);
	    initialize(data, input);
	    f.init(); // init required.
	  }

	  util::array<int> dp = offsets_wrt(input, nbh);
	  for (unsigned i = 0; i < dp.nelements(); ++i)
	    std::cout << dp[i] << ' ';
	  std::cout << std::endl;

	  /*

	  // First pass.
	  {
	  for (unsigned p = 0; p < f.s.size(); ++p)
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	  {
	  // Make set.
	  {
	  parent(p) = p;
	  data(p).take_as_init(make::pix(input, p)); // FIXME: algebraic so p!
	  }

	  for_all(n)
	  if (input.domain().has(n) && deja_vu(n))
	  {
	  P r = find_root(parent, n);
	  if (r != p)
	  {
	  if (input(r) == input(p) || (activity(r) && f.is_active(data(r))))
	  {
	  data(p).take(data(r));
	  parent(r) = p;
	  if (activity(r) == false)
	  activity(p) = false;
	  }
	  else
	  {
	  activity(p) = false;
	  f.inactivate(data(p));
	  }
	  }
	  }
	  deja_vu(p) = true;
	  }
	  }

	  */

	  /*

	  // Second pass.
	  {
	  mln_bkd_piter(S) p(f.s);
	  for_all(p)
	  if (parent(p) == p) // p is root.
	  output(p) = input(p);
	  else
	  output(p) = output(parent(p));
	  }

	  */

	  trace::exiting("canvas::morpho::impl::algebraic_union_find_fastest");
	  return output;
	}


      } // end of namespace mln::canvas::morpho::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename N, typename F>
	inline
	mln_concrete(I)
	algebraic_union_find_dispatch(trait::image::speed::any,
				      const Image<I>& input,
				      const Neighborhood<N>& nbh,
				      F& f)
	{
	  return impl::generic::algebraic_union_find(input, nbh, f);
	}

// 	template <typename I, typename N, typename F>
// 	inline
// 	mln_concrete(I)
// 	algebraic_union_find_dispatch(trait::image::speed::fastest,
// 				      const Image<I>& input,
// 				      const Neighborhood<N>& nbh,
// 				      F& f)
// 	{
// 	  return impl::algebraic_union_find_fastest(input, nbh, f);
// 	}

	template <typename I, typename N, typename F>
	inline
	mln_concrete(I)
	algebraic_union_find_dispatch(const Image<I>& input,
				      const Neighborhood<N>& nbh,
				      F& f)
	{
	  return algebraic_union_find_dispatch(mln_trait_image_speed(I)(),
					       input, nbh, f);
	}

      } // end of namespace mln::canvas::morpho::internal



      // Facade.

      template <typename I, typename N, typename F>
      inline
      mln_concrete(I)
      algebraic_union_find(const Image<I>& input,
			   const Neighborhood<N>& nbh,
			   F& f)
      {
	return internal::algebraic_union_find_dispatch(input, nbh, f);
      }


    } // end of namespace mln::canvas::morpho

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_MORPHO_ALGEBRAIC_UNION_FIND_HH
