// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LABELING_VALUE_SPE_HH
# define MLN_LABELING_VALUE_SPE_HH

/// \file
///
/// Specialization for mln::labeling::value.
///
/// \todo Re-activate the fastest version in dispatch...

# ifndef MLN_LABELING_VALUE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LABELING_VALUE_HH

# include <mln/extension/adjust_fill.hh>
# include <mln/value/other.hh>



namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the image objects at a given
     * value.
     *
     * \param[in]  input    The input image.
     * \param[in]  val      The value to consider for the labeling.
     * \param[in]  nbh      The connexity of the value components.
     * \param[out] nlabels  The number of labels.
     * \return              The label image.
     */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    value(const Image<I>& input, const mln_value(I)& val,
	  const Neighborhood<N>& nbh, L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Fwd decl of the Generic version.

      namespace generic
      {

	template <typename I, typename N, typename L>
	mln_ch_value(I, L)
	value(const Image<I>& input, const mln_value(I)& val,
	      const Neighborhood<N>& nbh,
	      L& nlabels);

      } // end of namespace mln::labeling::impl::generic

      /*

      // Fastest functor.

      template <typename I_, typename N_, typename L_>
      struct value_fastest_functor
      {
	// requirements from mln::canvas::labeling:

	typedef I_ I;
	typedef N_ N;
	typedef L_ L;
        typedef mln_domain(I) S;

	const I& input;
	const N& nbh;
        const S& s;

	bool handles(unsigned p) const         { return input.element(p) == val; }
	bool equiv(unsigned n, unsigned) const { return input.element(n) == val; }

 	void init()                          {}
	bool labels(unsigned) const          { return true;  }
	void do_no_union(unsigned, unsigned) {}
	void init_attr(unsigned)             {}
	void merge_attr(unsigned, unsigned)  {}

	// end of requirements

	const mln_value(I_)& val;

	value_fastest_functor(const I_& input, const mln_value(I_)& val,
			      const N_& nbh)
	  : input(input),
	    nbh(nbh),
	    s(input.domain()),
	    val(val)
	{}
      };


      // Fastest implementation.

      template <typename I, typename N, typename L>
      mln_ch_value(I, L)
      value_fastest(const Image<I>& input, const mln_value(I)& val,
		    const Neighborhood<N>& nbh,
		    L& nlabels)
      {
	mln_trace("labeling::impl::value_fastest");

	// FIXME: HERE

	extension::adjust_fill(input, nbh.delta(), value::other(val));

	typedef value_fastest_functor<I,N,L> F;
	F f(input, val, nbh);
	canvas::labeling_fastest<F> run(f);

	nlabels = run.nlabels;
	// FIXME: Handle run.status

	return run.output;
      }

      */

    } // end of namespace mln::labeling::impl

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_VALUE_SPE_HH
