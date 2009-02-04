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

#ifndef MLN_CANVAS_LABELING_HH
# define MLN_CANVAS_LABELING_HH

/// \file mln/canvas/labeling.hh
///
/// Connected component labeling of the object part in a binary image.
///
/// \todo Make the fastest version work.

# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>


namespace mln
{

  namespace canvas
  {

    // General version.
    template <typename I, typename N, typename F, typename L>
    mln_ch_value(I, L)
    labeling(const Image<I>& input, const Neighborhood<N>& nbh,
	     F& functor, L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    // Tests.

    namespace internal
    {

      template <typename I, typename N, typename F, typename L>
      void
      labeling_tests(const Image<I>& input_, const Neighborhood<N>& nbh_,
		     const F& f, const L& nlabels)
      {
	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	mln_precondition(input.is_valid());
	// mln_precondition(nbh.is_valid());

	(void) input;
	(void) nbh;
	(void) f;
	(void) nlabels;
      }

    } // end of namespace mln::canvas::internal



    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	static inline
	mln_psite(I)
	find_root(I& parent, const mln_psite(I)& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent, parent(x));
	}

	template <typename I, typename N, typename F, typename L>
	mln_ch_value(I, L)
	labeling(const Image<I>& input_, const Neighborhood<N>& nbh_,
		 F& f, L& nlabels)
	{
	  trace::entering("canvas::impl::generic::labeling");

	  // FIXME: Test?!

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  typedef typename F::S S;

	  // Local type.
	  typedef mln_psite(I) P;

	  // Auxiliary data.
	  mln_ch_value(I, bool) deja_vu;
	  mln_ch_value(I, P)    parent;

	  // Output.
	  mln_ch_value(I, L) output;
	  bool status;

	  // Initialization.
	  {
	    initialize(deja_vu, input);
	    mln::data::fill(deja_vu, false);

	    initialize(parent, input);

	    initialize(output, input);
	    mln::data::fill(output, L(literal::zero));
	    nlabels = 0;

	    f.init(); // Client initialization.
	  }

	  // First Pass.
	  {
	    mln_fwd_piter(S) p(f.s);
	    mln_niter(N) n(nbh, p);
	    for_all(p) if (f.handles(p))
	      {
		// Make-Set.
		parent(p) = p;
		f.init_attr(p);

		for_all(n)
		  if (input.domain().has(n) && deja_vu(n))
		    {
		      if (f.equiv(n, p))
			{
			  // Do-Union.
			  P r = find_root(parent, n);
			  if (r != p)
			    {
			      parent(r) = p;
			      f.merge_attr(r, p);
			    }
			}
		      else
			f.do_no_union(n, p);
		    }
		deja_vu(p) = true;
	      }
	  }

	  // Second Pass.
	  {
	    mln_bkd_piter(S) p(f.s);
	    for_all(p) if (f.handles(p))
	      {
		if (parent(p) == p) // if p is root
		  {
		    if (f.labels(p))
		      {
			if (nlabels == mln_max(L))
			  {
			    status = false;
			    return output;
			  }
			output(p) = ++nlabels;
		      }
		  }
		else
		  output(p) = output(parent(p));
	      }
	    status = true;
	  }

	  trace::exiting("canvas::impl::generic::labeling");
	  return output;
	}

      } // end of namespace mln::canvas::impl::generic



      // Fastest video version

      template <typename I>
      static inline
      unsigned
      find_root_fastest(I& parent, unsigned x)
      {
	if (parent.element(x) == x)
	  return x;
	else
	  return parent.element(x) = find_root(parent, parent.element(x));
      }

      // FIXME: Use the same functer for the generic and the fastest versions 

      template <typename I, typename N, typename F, typename L>
      mln_ch_value(I, L)
      labeling_fastest_video(const Image<I>& input_, const Neighborhood<N>& nbh_,
	       F& f, L& nlabels)
      {
	trace::entering("canvas::impl::labeling");

	// FIXME: Test?!

	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	typedef typename F::S S;

	// Local type.
	typedef mln_psite(I) P;

	// Auxiliary data.
	mln_ch_value(I, bool) deja_vu;
	mln_ch_value(I, P)    parent;

	// Output.
	mln_ch_value(I, L) output;
	bool status;

	// Initialization.
	{
	  initialize(deja_vu, input);
	  mln::data::fill(deja_vu, false);

	  initialize(parent, input);

	  initialize(output, input);
	  mln::data::fill(output, L(literal::zero));
	  nlabels = 0;

	  f.init(); // Client initialization.
	}

	// First Pass.
	{
	  mln_pixter(const S) p(f.s);
	  mln_nixter(const S, N) n(p, nbh);
	  for_all(p) if (f.handles(p))
	  {
	    // Make-Set.
	    parent.element(p) = p;
	    f.init_attr(p);

	    for_all(n)
	      if (input.has(n) && deja_vu(n))
	      {
		if (f.equiv(n, p))
		{
		  // Do-Union.
		  unsigned r = find_root_fastest(parent, n);
		  if (r != p)
		  {
		    parent.element(r) = p;
		    f.merge_attr(r, p);
		  }
		}
		else
		  f.do_no_union(n, p);
	     (p) = true;
	  }
	}

	// Second Pass.
	{
	  mln_bkd_pixter(S) p(f.s);
	  for_all(p) if (f.handles(p))
	  {
	    if (parent.element(p) == p) // if p is root
	    {
	      if (f.labels(p))
	      {
		if (nlabels == mln_max(L))
		{
		  status = false;
		  return output;
		}
		output.element(p) = ++nlabels;
	      }
	    }
	    else
	      output.element(p) = output(parent.element(p));
	  }
	  status = true;
	  }

	trace::exiting("canvas::impl::labeling");
	return output;
      }



      // Fastest sorted version

      template <typename I, typename N, typename F, typename L>
      mln_ch_value(I, L)
      labeling_fastest_sorted(const Image<I>& input_,
      			      const Neighborhood<N>& nbh_,
	       		      S& s, F& f, L& nlabels)
      {
	trace::entering("canvas::impl::labeling");

	// FIXME: Test?!

	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	typedef typename F::S S;

	// Local type.
	typedef mln_psite(I) P;

	// Auxiliary data.
	mln_ch_value(I, bool) deja_vu;
	mln_ch_value(I, P)    parent;

	// Output.
	mln_ch_value(I, L) output;
	bool status;

	// Initialization.
	{
	  initialize(deja_vu, input);
	  mln::data::fill(deja_vu, false);

	  initialize(parent, input);

	  initialize(output, input);
	  mln::data::fill(output, L(literal::zero));
	  nlabels = 0;

	  f.init(); // Client initialization.
	}

	util::array<int> dp = offsers_wrt(input, nbh);
	const unsigned n_nbhs = dp.nelements();

	const unsigned n_points = s.elements();

	// First Pass.
	{
	  
          for (unsigned i = 0; i < n_points; ++i)
          {
            unsigned p = s[i];
            if (!f.handles(p))
              continue;

            // Make-Set.
            parent.element(p) = p;
            f.init_attr(p);

            for (unsigned j = 0; j < n_nbhs; ++j)
            {
              unsigned n = p + dp[j];
              if (!deja_vu[n])
                continue;

              if (f.equiv(n, p))
              {
                // Do-Union.
                unsigned r = find_root_fastest(parent, n);
                if (input.element(r) != input.element(p))
                {
                  parent.element(r) = p;
                  f.merge_attr(r, p);
                }
              }
              else
                f.do_no_union(n, p);
            }
            deja_vu.element(p) = true;

          }

          // Second Pass.
          {
            for (int i = n_points - 1; i >=0; --i)
            {
              unsigned p = s[i];
              if (!f.handles(p))
                continue;

              if (parent.element(p) == p) // if p is root
              {
                if (f.labels(p))
                {
                  if (nlabels == mln_max(L))
                  {
                    status = false;
                    return output;
                  }
                  output.element(p) = ++nlabels;
                }
              }
              else
                output.element(p) = output(parent.element(p));
            }
            status = true;
          }

          trace::exiting("canvas::impl::labeling");
          return output;
        }

      } // end of namespace mln::canvas::impl



      // Dispatch.

      namespace internal
      {

        // Video

        template <typename I, typename N, typename F, typename L>
          inline
          mln_ch_value(I, L)
          labeling_video(metal::false_, const Image<I>& input,
              const Neighborhood<N>& nbh, F& functor, L& nlabels)
          {
            // FIXME:s = input.domain()
            return impl::generic::labeling(input, nbh, functor, nlabels);
          }

        template <typename I, typename N, typename F, typename L>
          inline
          mln_ch_value(I, L)
          labeling_video(metal::true_, const Image<I>& input,
              const Neighborhood<N>& nbh, F& functor, L& nlabels)
          {
            return impl::labeling_fastest_video(input, nbh, functor, nlabels);
          }

        template <typename I, typename N, typename F, typename L>
          inline
          mln_ch_value(I, L)
          labeling_video_dispatch(const Image<I>& input, const Neighborhood<N>& nbh,
              F& functor, L& nlabels)
          {
            enum {
              test = mlc_equal(mln_trait_image_speed(I),
                  trait::image::speed::fastest)::value
                &&
                mln_is_simple_neighborhood(N)::value
            };
            return impl::generic::labeling_video(metal::bool_<test>(), input, nbh,
                functor, nlabels);
          }


        // Sorted

        template <typename I, typename N, typename F, typename L>
          inline
          mln_ch_value(I, L)
          labeling_sorted(metal::false_, const Image<I>& input,
              const Neighborhood<N>& nbh, F& functor, L& nlabels)
          {
            return impl::generic::labeling(input, nbh, functor, nlabels);
          }

        template <typename I, typename N, typename F, typename L>
          inline
          mln_ch_value(I, L)
          labeling_sorted(metal::true_, const Image<I>& input,
              const Neighborhood<N>& nbh, F& functor, L& nlabels)
          {
            return impl::labeling_fastest_sorted(input, nbh, functor, nlabels);
          }

        template <typename I, typename N, typename F, typename L>
          inline
          mln_ch_value(I, L)
          labeling_sorted_dispatch(const Image<I>& input, const Neighborhood<N>& nbh,
              F& functor, L& nlabels)
          {
            enum {
              test = mlc_equal(mln_trait_image_speed(I),
                  trait::image::speed::fastest)::value
                &&
                mln_is_simple_neighborhood(N)::value
            };
            return impl::generic::labeling_video(metal::bool_<test>(), input, nbh,
                functor, nlabels);
          }


      } // end of namespace mln::canvas::internal



      // Facade.

      template <typename I, typename N, typename F, typename L>
        inline
        mln_ch_value(I, L)
        labeling_video(const Image<I>& input, const Neighborhood<N>& nbh,
            F& functor, L& nlabels)
        {
          trace::entering("canvas::labeling_video");

          internal::labeling_tests(input, nbh, functor, nlabels);

          mln_ch_value(I, L) output;
          output = internal::labeling_dispatch(input, nbh, functor, nlabels);

          trace::exiting("canvas::labeling_video");
          return output;
        }

      template <typename I, typename N, typename F, typename L>
        inline
        mln_ch_value(I, L)
        labeling_sorted(const Image<I>& input, const Neighborhood<N>& nbh,
            F& functor, L& nlabels)
        {
          trace::entering("canvas::labeling_sorted");

          internal::labeling_tests(input, nbh, functor, nlabels);

          mln_ch_value(I, L) output;
          output = internal::labeling_dispatch(input, nbh, functor, nlabels);

          trace::exiting("canvas::labeling_sorted");
          return output;
        }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas

  } // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_HH
