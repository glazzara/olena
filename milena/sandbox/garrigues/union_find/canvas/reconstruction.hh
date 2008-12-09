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

#ifndef MLN_CANVAS_ON_SET_HH
# define MLN_CANVAS_ON_SET_HH

# include <mln/core/image/image2d.hh>
# include <mln/level/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>
# include <mln/accu/count.hh>
# include <mln/util/pix.hh>

# include <mln/core/alias/neighb2d.hh>

# include <mln/io/pbm/save.hh>
# include <mln/io/pbm/load.hh>

namespace mln
{


  namespace canvas
  {

    namespace morpho
    {

      template <typename I>
      static inline
      mln_psite(I)
      find_root(I& parent,
		const mln_psite(I)& x)
      {
	if (parent(x) == x)
	  return x;
	else
	  return parent(x) = find_root(parent, parent(x));
      }

      template <typename N, typename F, typename I, typename O>
      void
      reconstruction(const Image<I>& mask_,
		     const Neighborhood<N>& nbh_,
		     F& f,
		     O& output)
      {
	trace::entering("canvas::morpho::reconstruction");

	const N& nbh = exact(nbh_);
	const I& mask = exact(mask_);

	mln_precondition(mask.domain() == output.domain());

	// Local type.
	typedef typename F::P P;

	// Auxiliary data.
	mln_ch_value(O, bool)  deja_vu;
	mln_ch_value(O, P)     parent;

	// init
	{
	  initialize(deja_vu, mask);
	  initialize(parent, mask);

	  mln::level::fill(deja_vu, false);
	  f.set_default_output(); // Client initialization.
	}

	// first pass
	{
	  mln_fwd_piter(I) p(mask.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	  {
	    if (f.is_in_D(p))
	    {
	      // Make set.
	      parent(p) = p;
	      f.init(p);

	      for_all(n) if (mask.domain().has(n))
	      {
		if (deja_vu(n) && f.is_in_D(n))
		{
		  //do_union(n, p);
		  P r = find_root(parent, n);
		  if (r != p)
		  {
		    f.merge(r, p);
		    parent(r) = p;
		  }
		}
		else
		  // Visit boundaries of D.
		  f.visit_ext_border(n, p);
	      }
	      deja_vu(p) = true;
	    }

	  }
	}

	// second pass
	{
	  mln_bkd_piter(I) p(mask.domain());
	  for_all(p)
	    if (f.is_in_D(p))
	      if (parent(p) != p) // if p is not a root.
		output(p) = output(parent(p));
	}

	trace::exiting("canvas::morpho::reconstruction");
      }


    } // end of namespace mln::canvas::morpho

  } // end of namespace mln::canvas

} // end of namespace mln.

#endif // ! MLN_CANVAS_ON_SET_HH
