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

#ifndef MLN_MORPHO_OPENING_ATTRIBUTE_HH
# define MLN_MORPHO_OPENING_ATTRIBUTE_HH

/// \file mln/morpho/opening_attribute.hh
///
/// Morphological attribute opening.
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

    /// Morphological attribute opening.
    template <typename A,
	      typename I, typename N>
    mln_concrete(I)
    opening_attribute(const Image<I>& input, const Neighborhood<N>& nbh,
		      mln_result(A) lambda);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename A_>
      struct opening_attribute_t
      {
	// requirements from mln::canvas::morpho::algebraic_union_find

	typedef A_ A;
	typedef mln_psite(I) P;
	typedef p_array<P> S;

	mln_result(A) lambda;
	const S s;

	inline
	void init()
	{
	  // FIXME: border::fill(input, mln_max(mln_value(I)));
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
	opening_attribute_t(const Image<I>& input, mln_result(A) lambda)
	  : lambda(lambda),
	    s(level::sort_psites_decreasing(exact(input)))
	{
	}

      };

    } // end of namespace mln::morpho::impl


    // Facade.

    template <typename A, typename I, typename N>
    inline
    mln_concrete(I)
    opening_attribute(const Image<I>& input,
		      const Neighborhood<N>& nbh, mln_result(A) lambda)
    {
      trace::entering("morpho::opening_attribute");

      mln_precondition(exact(input).is_valid());

      typedef impl::opening_attribute_t<I, A> F;
      F f(input, lambda);
      mln_concrete(I) output = canvas::morpho::algebraic_union_find(input, nbh, f);

      mln_postcondition(output <= input);

      trace::exiting("morpho::opening_attribute");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_OPENING_ATTRIBUTE_HH
