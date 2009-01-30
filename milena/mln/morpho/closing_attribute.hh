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

#ifndef MLN_MORPHO_CLOSING_ATTRIBUTE_HH
# define MLN_MORPHO_CLOSING_ATTRIBUTE_HH

/// \file mln/morpho/closing_attribute.hh
///
/// Morphological attribute closing.
///
/// \todo How to pass dynamic data (e.g., k of accu::rank) to the routine?
///
/// \todo Add extension::adjust_fill.

# include <mln/morpho/includes.hh>
# include <mln/canvas/morpho/algebraic_union_find.hh>
# include <mln/level/sort_psites.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace morpho
  {

    /// Morphological attribute closing.
    template <typename A, typename I, typename N>
    mln_concrete(I)
    closing_attribute(const Image<I>& input, const Neighborhood<N>& nbh,
		      mln_result(A) lambda);



# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {


      // Generic version.

      namespace generic
      {

	template <typename I, typename A_>
	struct closing_attribute_functor_t
	{
	  // requirements from mln::canvas::morpho::algebraic_union_find

	  typedef A_ A;
	  typedef mln_psite(I) P;
	  typedef p_array<P> S;

	  mln_result(A) lambda;
	  const S s;

	  void init()
	  {
	    // FIXME: border::fill(input, mln_max(mln_value(I)));
	  }

	  bool is_active(const A& attr) const
	  {
	    return attr.to_result() < lambda;
	  }

	  void inactivate(A& attr)
	  {
	    attr.set_value(lambda);
	  }

	  // end of requirements

	  closing_attribute_functor_t(const Image<I>& input, mln_result(A) lambda)
	    : lambda(lambda),
	      s(level::sort_psites_increasing(exact(input)))
	  {
	  }

	};


	template <typename A, typename I, typename N>
	inline
	mln_concrete(I)
	closing_attribute(const Image<I>& input, const Neighborhood<N>& nbh,
			  mln_result(A) lambda)
	{
	  trace::entering("morpho::impl::generic::closing_attribute");
	
	  mln_precondition(exact(input).is_valid());
	
	  typedef closing_attribute_functor_t<I, A> F;
	  F f(input, lambda);
	  mln_concrete(I) output = canvas::morpho::impl::generic::algebraic_union_find(input, nbh, f);
	
	  mln_postcondition(output >= input);
	
	  trace::exiting("morpho::impl::generic::closing_attribute");
	  return output;
	}


      } // end of namespace mln::morpho::impl::generic



      // "Fastest" version.


      template <typename I, typename A_>
      struct closing_attribute_fastest_functor_t
      {
	// requirements from mln::canvas::morpho::algebraic_union_find

	typedef A_ A;
	typedef mln_psite(I) P;
	typedef p_array<P> S;

	mln_result(A) lambda;
	const S s;

	void init()
	{
	  // FIXME: border::fill(input, mln_max(mln_value(I)));
	}

	bool is_active(const A& attr) const
	{
	  return attr.to_result() < lambda;
	}

	void inactivate(A& attr)
	{
	  attr.set_value(lambda);
	}

	// end of requirements

	closing_attribute_fastest_functor_t(const Image<I>& input, mln_result(A) lambda)
	  : lambda(lambda),
	    s(level::sort_psites_increasing(exact(input)))
	{
	}

      };


      template <typename A, typename I, typename N>
      inline
      mln_concrete(I)
      closing_attribute_fastest(const Image<I>& input, const Neighborhood<N>& nbh,
				mln_result(A) lambda)
      {
	trace::entering("morpho::impl::closing_attribute_fastest");
	
	mln_precondition(exact(input).is_valid());
	
	typedef impl::closing_attribute_fastest_functor_t<I, A> F;
	F f(input, lambda);
	mln_concrete(I) output = canvas::morpho::impl::algebraic_union_find_fastest(input, nbh, f);
	
	mln_postcondition(output >= input);
	
	trace::exiting("morpho::impl::closing_attribute_fastest");
	return output;
      }


    } // end of namespace mln::morpho::impl



    // Dispatch.

    namespace internal
    {

      template <typename A, typename I, typename N>
      inline
      mln_concrete(I)
      closing_attribute_dispatch(trait::image::speed::any,
				 const Image<I>& input, const Neighborhood<N>& nbh,
				 mln_result(A) lambda)
      {
	return impl::generic::closing_attribute<A>(input, nbh, lambda);
      }

//       template <typename A, typename I, typename N>
//       inline
//       mln_concrete(I)
//       closing_attribute_dispatch(trait::image::speed::fastest,
// 				 const Image<I>& input, const Neighborhood<N>& nbh,
// 				 mln_result(A) lambda)
//       {
// 	return impl::closing_attribute_fastest<A>(input, nbh, lambda);
//       }

      template <typename A, typename I, typename N>
      inline
      mln_concrete(I)
      closing_attribute_dispatch(const Image<I>& input, const Neighborhood<N>& nbh,
				 mln_result(A) lambda)
      {
	return closing_attribute_dispatch<A>(mln_trait_image_speed(I)(),
					     input, nbh, lambda);
      }

    } // end of namespace internal



    // Facade.

    template <typename A, typename I, typename N>
    inline
    mln_concrete(I)
    closing_attribute(const Image<I>& input, const Neighborhood<N>& nbh,
		      mln_result(A) lambda)
    {
      trace::entering("morpho::closing_attribute");

      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output = internal::closing_attribute_dispatch<A>(input, nbh, lambda);

      mln_postcondition(output >= input);

      trace::exiting("morpho::closing_attribute");
      return output;
    }


    // Facade.


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_CLOSING_ATTRIBUTE_HH
