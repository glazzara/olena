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

#ifndef MLN_MORPHO_GEODESIC_DILATION_PERMISSIVE_HH
# define MLN_MORPHO_GEODESIC_DILATION_PERMISSIVE_HH

/// \file
///
/// Morphological geodesic dilation; permissive version.

# include <mln/morpho/includes.hh>
# include <mln/morpho/elementary/dilation.hh>
# include <mln/logical/and.hh>
# include <mln/data/transform_inplace.hh>
# include <mln/fun/vv2v/max.hh>
# include <mln/border/equalize.hh>


namespace mln
{

  namespace morpho
  {

    namespace geodesic
    {

      /// Morphological geodesic dilation of size n; permissive version.
      template <typename I, typename J, typename N>
      mln_concrete(I)
      dilation_permissive(const Image<I>& f, const Image<J>& g,
			  const Neighborhood<N>& nbh,
			  unsigned n = 1);


# ifndef MLN_INCLUDE_ONLY

      // Tests.

      namespace internal
      {

	template <typename I, typename J, typename N>
	inline
	void
	dilation_permissive_tests(const Image<I>& f_, const Image<J>& g_,
				  const Neighborhood<N>& nbh_)
	{
	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  mln_precondition(f.is_valid());
	  mln_precondition(g.is_valid());
	  mln_precondition(nbh.is_valid());

	  mln_precondition(f.domain() == g.domain());

	  (void) f;
	  (void) g;
	  (void) nbh;
	}

      } // end of namespace morpho::geodesic::internal
      

      // Implementations.

      namespace impl
      {


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_strict_1_set_formula(const Image<I>& f, const Image<J>& g,
				      const Neighborhood<N>& nbh)
	{
	  trace::entering("morpho::geodesic::impl::dilation_strict_1_set_formula");

	  internal::dilation_permissive_tests(f, g, nbh);

	  mln_concrete(I) output = morpho::elementary::dilation(f, nbh);
	  logical::and_inplace(output, g);

	  trace::exiting("morpho::geodesic::impl::dilation_strict_1_set_formula");
	  return output;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_strict_1_function_formula(const Image<I>& f, const Image<J>& g,
					   const Neighborhood<N>& nbh)
	{
	  trace::entering("morpho::geodesic::impl::dilation_strict_1_function_formula");

	  internal::dilation_permissive_tests(f, g, nbh);

	  mln_concrete(I) output = morpho::elementary::dilation(f, nbh);
	  fun::vv2v::max<mln_value(I)> max_;
	  data::transform_inplace(output, g, max_);

	  trace::exiting("morpho::geodesic::impl::dilation_strict_1_function_formula");
	  return output;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_n(const Image<I>& f, const Image<J>& g,
			      const Neighborhood<N>& nbh,
			      unsigned n)
	{
	  trace::entering("morpho::geodesic::impl::dilation_permissive_n");

	  mln_precondition(n != 0);
	  internal::dilation_permissive_tests(f, g, nbh);

	  mln_concrete(I) work = duplicate(f);
	  for (unsigned i = 0; i < n; ++i)
	    work = dilation_permissive(work, g, nbh, 1);

	  trace::exiting("morpho::geodesic::impl::dilation_permissive_n");
	  return work;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_set_generic(const Image<I>& f_, const Image<J>& g_,
					const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::geodesic::impl::dilation_permissive_1_set_generic");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::dilation_permissive_tests(f, g, nbh);

	  mln_piter(I) p(f.domain());
	  mln_niter(N) n(nbh, p);

	  // We dilate from the inner boundary of f.

	  mln_concrete(I) output = duplicate(f);
	  for_all(p)
	  {
	    if (! f(p))
	      continue;
	    for_all(n) if (f.domain().has(n))
	      {
		if (output(n)) // already at true
		  continue;
		if (g(n)) // conditioning
		  output(n) = true;
	      }
	  }

	  trace::exiting("morpho::geodesic::impl::dilation_permissive_1_set_generic");
	  return output;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_set_fastest(const Image<I>& f_, const Image<J>& g_,
					  const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::geodesic::impl::dilation_permissive_1_set_fastest");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::dilation_permissive_tests(f, g, nbh);

	  border::equalize(f, g, nbh.delta());
	  typedef mln_concrete(I) O;
	  O output = duplicate(f);
	  border::fill(output, true);

	  util::array<int> dp = offsets_wrt(f, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  mln_box_runstart_piter(I) s(f.domain());
	  s.start();
	  const unsigned len = s.run_length();

	  // We dilate from the inner boundary of f.

	  for (; s.is_valid(); s.next())
	    {
	      unsigned p = f.index_of_point(s);
	      for (unsigned i = 0; i < len; ++i, ++p)
		{
		  if (! f.element(p))
		    continue;
		  for (unsigned j = 0; j < n_nbhs; ++j)
		    {
		      unsigned n = p + dp[j];
		      if (output.element(n)) // already at true
			continue;
		      if (g.element(n)) // conditioning
			output.element(n) = true;
		    }
		}
	    }

	  trace::exiting("morpho::geodesic::impl::dilation_permissive_1_set_fastest");
	  return output;
	}


	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_set_generic_alt(const Image<I>& f_, const Image<J>& g_,
					      const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::geodesic::impl::dilation_permissive_1_set_generic_alt");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::dilation_permissive_tests(f, g, nbh);

	  mln_piter(I) p(f.domain());
	  mln_niter(N) n(nbh, p);

	  // We browse the "(not f) and g" domain.

	  mln_concrete(I) output = duplicate(f);
	  for_all(p)
	  {
	    if (f(p) // output(p) is already at true... 
		|| ! g(p) // ...or conditioning by g not satisfied
		)
	      continue;
	    for_all(n) if (f.domain().has(n))
	      if (f(n)) // p is in the outer boundary of f
		{
		  output(p) = true;
		  break; // we are done for p
		}
	  }

	  trace::exiting("morpho::geodesic::impl::dilation_permissive_1_set_generic_alt");
	  return output;
	}



	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_set_fastest_alt(const Image<I>& f_, const Image<J>& g_,
					      const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::geodesic::impl::dilation_permissive_1_set_fastest_alt");

	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh = exact(nbh_);

	  internal::dilation_permissive_tests(f, g, nbh);

	  border::equalize(f, g, nbh.delta());
	  typedef mln_concrete(I) O;
	  O output = duplicate(f);
	  border::fill(f, false);

	  util::array<int> dp = offsets_wrt(f, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  mln_box_runstart_piter(I) s(f.domain());
	  s.start();
	  const unsigned len = s.run_length();

	  // We browse the "(not f) and g" domain.

	  for (; s.is_valid(); s.next())
	    {
	      unsigned p = f.index_of_point(s);
	      for (unsigned i = 0; i < len; ++i, ++p)
		{
		  if (f.element(p) // output.element(p) is already at true... 
		      || ! g.element(p) // ...or conditioning by g not satisfied
		      )
		    continue;
		  for (unsigned j = 0; j < n_nbhs; ++j)
		    {
		      unsigned n = p + dp[j];
		      if (f.element(n)) // p is in the outer boundary of f
			{
			  output.element(p) = true;
			  break; // we are done for p
			}
		    }
		}
	    }

	  trace::exiting("morpho::geodesic::impl::dilation_permissive_1_set_fastest_alt");
	  return output;
	}


      } // end of namespace morpho::geodesic::impl



      // Dispatch.

      namespace internal
      {


	// For size 1.

	template <typename M,
		  typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_dispatch(trait::image::kind::any,
				       M,
				       const Image<I>& f, const Image<J>& g,
				       const Neighborhood<N>& nbh)
	{
	  // FIXME
	  std::cerr << "NYI: replace by strict version!" << std::endl;
	  return impl::dilation_strict_1_function_formula(f, g, nbh);
	}

	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_dispatch(trait::image::kind::binary,
				       metal::false_,
				       const Image<I>& f, const Image<J>& g,
				       const Neighborhood<N>& nbh)
	{
	  return impl::dilation_permissive_1_set_generic(f, g, nbh);
	}

	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_1_dispatch(trait::image::kind::binary,
				       metal::true_,
				       const Image<I>& f, const Image<J>& g,
				       const Neighborhood<N>& nbh)
	{
	  return impl::dilation_permissive_1_set_fastest(f, g, nbh);
	}



	// Entry point.
	
	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	dilation_permissive_dispatch(const Image<I>& f, const Image<J>& g,
				     const Neighborhood<N>& nbh,
				     unsigned n)
	{
	  enum {
	    is_fastest = (mlc_equal(mln_trait_image_speed(I),
				    trait::image::speed::fastest)::value &&
			  mlc_equal(mln_trait_image_speed(J),
				    trait::image::speed::fastest)::value &&
			  mln_is_simple_neighborhood(N)::value)
	  };
	  if (n == 1)
	    return dilation_permissive_1_dispatch(mln_trait_image_kind(I)(),
						  metal::bool_<is_fastest>(),
						  f, g, nbh);
	  else
	    return impl::dilation_permissive_n(f, g, nbh, n);
	}


      } // end of namespace morpho::geodesic::internal



      // Facade.

      template <typename I, typename J, typename N>
      inline
      mln_concrete(I)
      dilation_permissive(const Image<I>& f, const Image<J>& g,
			  const Neighborhood<N>& nbh,
			  unsigned n)
      {
	trace::entering("morpho::geodesic::dilation_permissive");

	mln_precondition(n != 0);
	internal::dilation_permissive_tests(f, g, nbh);

	mln_concrete(I) output;
	output = internal::dilation_permissive_dispatch(f, g, nbh, n);

	trace::exiting("morpho::geodesic::dilation_permissive");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::geodesic

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_GEODESIC_DILATION_PERMISSIVE_HH
