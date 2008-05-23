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

#ifndef MLN_LABELING_REGIONAL_MAXIMA_HH
# define MLN_LABELING_REGIONAL_MAXIMA_HH

/*! \file mln/labeling/regional_maxima.hh
 *
 * \brief Connected component labeling of the regional maxima of an
 * image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling.hh>
# include <mln/level/fill.hh>
# include <mln/level/sort_psites.hh>


namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the regional maxima of an
     * image.
     *
     * \param[in]  input    The input image.
     * \param[in]  nbh      The connexity of the regional maxima.
     * \param[out] nlabels  The number of labeled regions.
     * \return              The label image.
     *
     */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
      regional_maxima(const Image<I>& input, const Neighborhood<N>& nbh,
		      L& nlabels);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic functor.

      template <typename I_, typename N_, typename L_>
      struct regional_maxima_functor
      {
	typedef mln_psite(I_) P;

	// requirements from mln::canvas::labeling:

	typedef I_ I;
	typedef N_ N;
	typedef L_ L;
	typedef p_array<P> S;

	const I& input;
	const N& nbh;
	S s;

 	void init()                              { level::fill(attr, true); }
	bool handles(const P&) const             { return true; }
	bool labels(const P& p) const            { return attr(p); }
	bool equiv(const P& n, const P& p) const { return input(n) ==
                                                          input(p); }
	void do_no_union(const P& n, const P& p) { mln_invariant(input(n) >
								 input(p));
	                                           attr(p) = false; }
	void init_attr(const P&)                 {}
	void merge_attr(const P& r, const P& p)  { attr(p) = attr(p) &&
	                                           attr(r); }

	// end of requirements

	mln_ch_value(I, bool) attr;

	regional_maxima_functor(const I_& input, const N_& nbh)
	  : input(input),
	    nbh(nbh),
	    s(level::sort_psites_decreasing(input))
	{
	  initialize(attr, input);
	}
      };


      // Generic implementation.

      namespace generic
      {

	template <typename I, typename N, typename L>
	mln_ch_value(I, L)
	regional_maxima_(const I& input, const N& nbh,
			 L& nlabels)
	{
	  trace::entering("labeling::impl::generic::regional_maxima_");

	  typedef impl::regional_maxima_functor<I,N,L> F;
	  F f(input, nbh);
	  canvas::labeling<F> run(f);
	  nlabels = run.nlabels;

	  trace::exiting("labeling::impl::generic::regional_maxima_");
	  return run.output;
	}

      } // end of namespace mln::labeling::impl::generic


    } // end of namespace mln::labeling::impl



    // Facade.

    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
      regional_maxima(const Image<I>& input_, const Neighborhood<N>& nbh_,
		      L& nlabels)
    {
      trace::entering("labeling::regional_maxima");
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mln_precondition(input.has_data());

      // Calls the only (generic) impl.
      mln_ch_value(I, L) output =
	impl::generic::regional_maxima_(input, nbh, nlabels);

      trace::exiting("labeling::regional_maxima");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_REGIONAL_MAXIMA_HH
