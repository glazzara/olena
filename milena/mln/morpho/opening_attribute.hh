// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file mln/morpho/opening_attribute.hh
 *
 * \brief Morphological attribute opening.
 */

# include <mln/morpho/includes.hh>
# include <mln/canvas/morpho/algebraic_union_find.hh>
# include <mln/level/sort_points.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace morpho
  {

    /* FIXME: The neighborhood shall not be passed as argument, but
       bound to the input image.  We can also optionnaly provide a
       version of this function for regular-grid-based images where
       the neighborhood is replaced by a (user-provided) window.  */

    /// Morphological attribute opening.
    template <typename A,
	      typename I, typename N, typename O>
    void opening_attribute(const Image<I>& input, const Neighborhood<N>& nbh,
			   mln_result(A) lambda, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename A_,
		typename I_, typename N_, typename O_>
      struct opening_attribute_t
      {
	typedef mln_point(I_) P;

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
	opening_attribute_t(const I_& input, const N_& nbh,
			    mln_result(A) lambda, O_& output)
	  : input(input), nbh(nbh), lambda(lambda), output(output),
	    s(level::sort_points_decreasing(input))
	{
	}

      };

    } // end of namespace mln::morpho::impl


    template <typename A,
	      typename I, typename N, typename O>
    inline
    void opening_attribute(const Image<I>& input_,
			   const Neighborhood<N>& nbh_, mln_result(A) lambda,
			   Image<O>& output_)
    {
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      O& output = exact(output_);
      mln_precondition(output.domain() == input.domain());

      typedef impl::opening_attribute_t<A,I,N,O> F;
      F f(input, nbh, lambda, output);
      canvas::morpho::algebraic_union_find<F> run(f);

      mln_postcondition(output <= input);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_OPENING_ATTRIBUTE_HH
