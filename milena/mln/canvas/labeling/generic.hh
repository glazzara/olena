// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CANVAS_LABELING_GENERIC_HH
# define MLN_CANVAS_LABELING_GENERIC_HH

/// \file
///
/// Generic Connected component labeling of the object part in a
/// binary image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/site_set.hh>

# include <mln/data/fill.hh>

namespace mln
{

  namespace canvas
  {

    namespace labeling
    {

      namespace impl
      {

	namespace generic
	{

          /*! \brief Generic Connected component labeling of the
	   * object part in a binary image.
	   *
	   * \ingroup modcanvaslabeling
	   */
	  template <typename I, typename N, typename L,
		    typename S, typename F>
	  mln_ch_value(I, L)
	  labeling(const Image<I>& input_, const Neighborhood<N>& nbh_,
		   L& nlabels, const Site_Set<S>& s_, F& f);


# ifndef MLN_INCLUDE_ONLY

	  template <typename I>
	  static inline
	  mln_psite(I)
	  find_root(I& parent, const mln_psite(I)& x)
	  {
	    if (parent(x) == x)
	      return x;
	    else
	      return parent(x) = find_root(parent, parent(x));
	  }



	  template <typename I, typename N, typename L,
		    typename S, typename F>
	  mln_ch_value(I, L)
	  labeling(const Image<I>& input_, const Neighborhood<N>& nbh_,
		   L& nlabels, const Site_Set<S>& s_, F& f)
	  {
	    trace::entering("canvas::labeling::impl::generic::labeling");

	    // FIXME: Test?!

	    const I& input = exact(input_);
	    const N& nbh   = exact(nbh_);
	    const S& s     = exact(s_);

	    // Local type.
	    typedef mln_psite(I) P;

	    // Auxiliary data.
	    mln_ch_value(I, bool) deja_vu;
	    mln_ch_value(I, P)    parent;

	    // Output.
	    mln_ch_value(I, L) output;
	    bool status; // FIXME: Is-it useful?

	    // Initialization.
	    {
	      initialize(deja_vu, input);
	      mln::data::fill(deja_vu, false);

	      initialize(parent, input);

	      initialize(output, input);
	      mln::data::fill(output, L(literal::zero));
	      nlabels = 0;

	      f.init(); // <-- f.init() - Client initialization.
	    }

	    // First Pass.
	    {
	      mln_bkd_piter(S) p(s);  // Backward.
	      mln_niter(N) n(nbh, p);
	      for_all(p) if (f.handles(p)) // <-- f.handles()
	      {
		// Make-Set.
		parent(p) = p;
		f.init_attr(p);

		for_all(n)
		  if (input.domain().has(n) && deja_vu(n))
		  {
		    if (f.equiv(n, p)) // <-- f.equiv()
		    {
		      // Do-Union.
		      P r = find_root(parent, n);
		      if (r != p)
		      {
			parent(r) = p;
			f.merge_attr(r, p); // <-- f.merge_attr()
		      }
		    }
		    else
		      f.do_no_union(n, p); // <-- f.do_no_union()
		  }
		deja_vu(p) = true;
	      }
	    }

	    // Second Pass.
	    {
	      mln_fwd_piter(S) p(s); // Forward.
	      for_all(p) if (f.handles(p)) // <-- f.handles()
	      {
		if (parent(p) == p) // if p is root
		{
		  if (f.labels(p)) // <-- f.labels()
		  {
		    if (nlabels == mln_max(L))
		    {
		      status = false;
		      trace::warning("labeling aborted! Too many labels \
					    for this label type: nlabels > \
					    max(label_type).");

		      return output;
		    }
		    output(p) = ++nlabels;
		  }
		}
		else
		  output(p) = output(parent(p));
	      }
	      status = true;
	    }

	    trace::exiting("canvas::labeling::impl::generic::labeling");
	    return output;
	  }

# endif // ! MLN_INCLUDE_ONLY

	} // end of namespace mln::canvas::labeling::impl::generic

      } // end of namespace mln::canvas::labeling::impl

    } // end of namespace mln::canvas::labeling

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_GENERIC_HH
