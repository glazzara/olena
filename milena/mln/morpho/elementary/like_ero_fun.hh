// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_ELEMENTARY_LIKE_ERO_FUN_HH
# define MLN_MORPHO_ELEMENTARY_LIKE_ERO_FUN_HH

/// \file
///
/// \todo Add a choice between adjust_fill and adjust_duplicate.


# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {

    namespace elementary
    {

      template <typename A, typename F,
		typename I, typename N>
      mln_concrete(I)
      like_ero_fun(const Meta_Accumulator<A>& a, const F& f,
		   const Image<I>& input, const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	struct f_accu
	{
	  template <typename V, typename A>
	  V operator()(const V& /*input_p*/, const A& a) const
	  {
	    return a.to_result();
	  }
	};

      } // end of namespace mln::morpho::elementary::internal


      namespace impl
      {

	namespace generic
	{

	  template <typename A, typename F,
		    typename I, typename N>
	  mln_concrete(I)
	  like_ero_fun(const Meta_Accumulator<A>& a_, const F& f,
		       const Image<I>& input_, const Neighborhood<N>& nbh_)
	  {
	    mln_trace("morpho::elementary::impl::generic::like_ero_fun");

	    const I& input = exact(input_);
	    const N& nbh   = exact(nbh_);

	    mln_accu_with(A, mln_value(I)) a = accu::unmeta(exact(a_), mln_value(I)());
	    extension::adjust_fill(input, nbh, a);

	    mln_concrete(I) output;
	    initialize(output, input);

	    mln_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      {
		a.take_as_init(input(p));
		for_all(n) if (input.has(n))
		  a.take(input(n));
		output(p) = f(input(p), a);
	      }

	    return output;
	  }

	} // end of namespace mln::morpho::elementary::impl::generic


	template <typename A, typename F,
		  typename I, typename N>
	mln_concrete(I)
	like_ero_fun_fastest(const Meta_Accumulator<A>& a_, const F& f,
			     const Image<I>& input_, const Neighborhood<N>& nbh_)
	{
	  mln_trace("morpho::elementary::impl::like_ero_fun_fastest");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  mln_accu_with(A, mln_value(I)) a = accu::unmeta(exact(a_), mln_value(I)());
	  extension::adjust_fill(input, nbh, a);

	  mln_concrete(I) output;
	  initialize(output, input);

	  mln_pixter(const I) p_in(input);
	  mln_pixter(I) p_out(output);
	  mln_nixter(const I, N) n(p_in, nbh);
	  for_all_2(p_in, p_out)
	    {
	      a.take_as_init(p_in.val());
	      for_all(n)
		a.take(n.val());
	      p_out.val() = f(p_in.val(), a);
	    }

	  return output;
	}

      } // end of namespace mln::morpho::elementary::impl


      namespace internal
      {

	template <typename A, typename F,
		  typename I, typename N>
	mln_concrete(I)
	like_ero_fun_dispatch(metal::false_,
			      const A& a, const F& f,
			      const I& input, const N& nbh)
	{
	  return impl::generic::like_ero_fun(a, f, input, nbh);
	}

	template <typename A, typename F,
		  typename I, typename N>
	mln_concrete(I)
	like_ero_fun_dispatch(metal::true_,
			      const A& a, const F& f,
			      const I& input, const N& nbh)
	{
	  return impl::like_ero_fun_fastest(a, f, input, nbh);
	}

	template <typename A, typename F,
		  typename I, typename N>
	mln_concrete(I)
	like_ero_fun_dispatch(const A& a, const F& f,
			      const I& input, const N& nbh)
	{
	  typedef mlc_equal(mln_trait_image_speed(I),
			    trait::image::speed::fastest) I_fastest;
	  typedef mln_window(N) W;
	  typedef mln_is_simple_window(W) N_simple;

	  return like_ero_fun_dispatch(mlc_and(I_fastest, N_simple)(),
				       a, f, input, nbh);
	}

      } // end of namespace mln::morpho::elementary::internal


      // Facade.

      template <typename A, typename F,
		typename I, typename N>
      mln_concrete(I)
      like_ero_fun(const Meta_Accumulator<A>& a, const F& f,
		   const Image<I>& input, const Neighborhood<N>& nbh)
      {
	return internal::like_ero_fun_dispatch(a, f,
					       exact(input), exact(nbh));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::elementary

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ELEMENTARY_LIKE_ERO_FUN_HH
