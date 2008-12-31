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

    namespace impl
    {

      template <typename I, typename A_>
      struct closing_attribute_t
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

	closing_attribute_t(const Image<I>& input, mln_result(A) lambda)
	  : lambda(lambda),
	    s(level::sort_psites_increasing(exact(input)))
	{
	}

      };

    } // end of namespace mln::morpho::impl


    template <typename A, typename I, typename N>
    inline
    mln_concrete(I)
    closing_attribute(const Image<I>& input, const Neighborhood<N>& nbh,
		      mln_result(A) lambda)
    {
      trace::entering("morpho::closing_attribute");

      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output;
      initialize(output, input);

      typedef impl::closing_attribute_t<I, A> F;
      F f(input, lambda);
      canvas::morpho::algebraic_union_find(input, nbh, f, output);

      mln_postcondition(output >= input);

      trace::exiting("morpho::closing_attribute");
      return output;
    }


    /* -----------------------------------------------------------

       Old code below.

    namespace impl
    {

      template <typename A_,
		typename I_, typename N_, typename O_>
      struct closing_attribute_t_f
      {
	typedef mln_psite(I_) P;

	// requirements from mln::canvas::morpho::algebraic_union_find

	typedef A_ A;
	typedef I_ I;
	typedef N_ N;
	typedef O_ O;
	typedef p_array<P> S;
	typedef util::pix<I> pix_t;

	const I& input;
	const N& nbh;
	mln_result(A) lambda;
	O& output;

	const S s;

	inline
	void init()
	{
	  // FIXME: border::fill(input, mln_max(mln_value(I)));
	  // FIXME: Shouldn't it be
	  //   border::fill(input, mln_max(mln_value(I)))
	  //   instead?
	}

	inline
	bool is_active(const A& attr) const
	{
	  return attr.to_result() < lambda;
	}

	inline
	void inactivate(A& attr)
	{
	  attr.set_value(lambda);
	}

	// end of requirements

	inline
	closing_attribute_t_f(const I_& input, const N_& nbh,
			      mln_result(A) lambda, O_& output)
	  : input(input), nbh(nbh), lambda(lambda), output(output),
	    s(level::sort_psites_increasing(input))
	{
	}

      };

    } // end of namespace mln::morpho::impl


    template <typename A,
	      typename I, typename N, typename O>
    inline
    void closing_attribute_f(const Image<I>& input_,
			     const Neighborhood<N>& nbh_, mln_result(A) lambda,
			     Image<O>& output_)
    {
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      O& output = exact(output_);
      mln_precondition(output.domain() == input.domain());

      typedef impl::closing_attribute_t_f<A,I,N,O> F;
      F f(input, nbh, lambda, output);
      canvas::morpho::algebraic_union_find_f<F> run(f);

      mln_postcondition(output >= input);
    }

    */

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_CLOSING_ATTRIBUTE_HH
