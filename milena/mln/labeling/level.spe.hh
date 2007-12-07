// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_LABELING_LEVEL_SPE_HH
# define MLN_LABELING_LEVEL_SPE_HH

/*! \file mln/labeling/level.spe.hh
 *
 * \brief Specialization for mln::labeling::level.
 */

# include <mln/border/adjust.hh>
# include <mln/border/fill.hh>
# include <mln/value/other.hh>



namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the image objects at a given
     * level.
     *
     * \param[in]  input The input image.
     * \param[in]  val   The level to consider for the labeling.
     * \param[in]  nbh   The neighborhood.
     * \param[out] nlabels The number of labels.
     * \return  The label image.
     */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
      level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	    L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Fwd decl of the Generic version.

      namespace generic
      {

	template <typename I, typename N, typename L>
	mln_ch_value(I, L)
	  level_(const I& input, const mln_value(I)& val, const N& nbh,
		 L& nlabels);

      } // end of namespace mln::labeling::impl::generic



      // Fastest functor.

      template <typename I_, typename N_, typename L_>
      struct level_fastest_functor
      {
	// requirements from mln::canvas::labeling:

	typedef I_ I;
	typedef N_ N;
	typedef L_ L;
        typedef mln_pset(I) S;

	const I& input;
	const N& nbh;
        const S& s;

	bool handles(unsigned p) const         { return input[p] == val; }
	bool equiv(unsigned n, unsigned) const { return input[n] == val; }

 	void init()                          {}
	bool labels(unsigned) const          { return true;  }
	void do_no_union(unsigned, unsigned) {}
	void init_attr(unsigned)             {}
	void merge_attr(unsigned, unsigned)  {}

	// end of requirements

	const mln_value(I_)& val;

	level_fastest_functor(const I_& input, const mln_value(I_)& val, const N_& nbh)
	  : input(input),
	    nbh(nbh),
	    s(input.domain()),
	    val(val)
	{}
      };


      // Fastest routine.

      template <typename I, typename N, typename L>
      mln_ch_value(I, L)
	level_fastest_(const I& input, const mln_value(I)& val, const N& nbh,
		       L& nlabels)
      {
	trace::entering("labeling::impl::level_fastest_");

	border::adjust(input, nbh.delta());
	border::fill(input, value::other(val));

	typedef level_fastest_functor<I,N,L> F;
	F f(input, val, nbh);
	canvas::labeling_fastest<F> run(f);

	nlabels = run.nlabels;
	// FIXME: Handle run.status

	trace::exiting("labeling::impl::level_fastest_");
	return run.output;
      }


      // Disjunction between "fastest" and "not fastest".

      template <typename I, typename N, typename L>
      mln_ch_value(I, L)
	level_(trait::image::speed::any,
	       const I& input, const mln_value(I)& val, const N& nbh,
	       L& nlabels)
      {
	return generic::level_(input, val, nbh, nlabels);
      }

      template <typename I, typename N, typename L>
      mln_ch_value(I, L)
	level_(trait::image::speed::fastest,
	       const I& input, const mln_value(I)& val, const N& nbh,
	       L& nlabels)
      {
	return level_fastest_(input, val, nbh, nlabels);
      }


    } // end of namespace mln::labeling::impl


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_LEVEL_SPE_HH
