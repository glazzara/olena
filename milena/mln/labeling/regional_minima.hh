// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_LABELING_REGIONAL_MINIMA_HH
# define MLN_LABELING_REGIONAL_MINIMA_HH

/// \file
///
/// Connected component labeling of the regional minima of an image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling/sorted.hh>
# include <mln/data/fill.hh>
# include <mln/data/sort_psites.hh>


namespace mln
{

  namespace labeling
  {

    /*! \brief Connected component labeling of the regional minima of
      an image.

      \param[in]  input   The input image.
      \param[in]  nbh     The connexity of the regional minima.
      \param[out] nlabels The number of labeled regions.
      \return The label image.

      \ingroup labeling
     */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    regional_minima(const Image<I>& input, const Neighborhood<N>& nbh,
		    L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic functor.

      template <typename I>
      struct regional_minima_functor
      {
	typedef mln_psite(I) P;

	// requirements from mln::canvas::labeling:

	const I& input;

	// Generic implementation

	void init()                              { data::fill(attr, true); }
	bool handles(const P&) const             { return true; }
	bool labels(const P& p) const            { return attr(p); }
	bool equiv(const P& n, const P& p) const { return input(n) ==
                                                          input(p); }
	void do_no_union(const P& n, const P& p)
	{
	  // Avoid a warning about an undefined variable when NDEBUG
	  // is not defined.
	  (void)n;

	  mln_invariant(input(n) < input(p));
	  attr(p) = false;
	}

	void init_attr(const P&)                 {}
	void merge_attr(const P& r, const P& p)  { attr(p) = attr(p) &&
                                                   attr(r); }

	// Fastest implementation

	void init_()                              { data::fill(attr, true); }
	bool handles_(unsigned) const             { return true; }
	bool labels_(unsigned p) const            { return attr.element(p); }
	bool equiv_(unsigned n, unsigned p) const { return input.element(n) ==
                                                           input.element(p); }
	void do_no_union_(unsigned n, unsigned p)
	{
	  // Avoid a warning about an undefined variable when NDEBUG
	  // is not defined.
	  (void)n;

	  mln_invariant(input.element(n) < input.element(p));
	  attr.element(p) = false;
	}

	void init_attr_(unsigned)                 {}
	void merge_attr_(unsigned r, unsigned p)  { attr.element(p) = attr.element(p) &&
                                                   attr.element(r); }

	// end of requirements

	mln_ch_value(I, bool) attr;

	regional_minima_functor(const I& input)
	  : input(input)
	{
	  initialize(attr, input);
	}
      };


    } // end of namespace mln::labeling::impl



    // Facade.

    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    regional_minima(const Image<I>& input_, const Neighborhood<N>& nbh_,
		    L& nlabels)
    {
      mln_trace("labeling::regional_minima");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mln_precondition(input.is_valid());

      // FIXME: abort if L is not wide enough to encode the set of
      // minima.

      typedef impl::regional_minima_functor<I> F;
      F f(exact(input));
      mln_ch_value(I, L)
	output = canvas::labeling::sorted(input, nbh, nlabels, f, false);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_REGIONAL_MINIMA_HH
