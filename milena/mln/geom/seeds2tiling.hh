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

#ifndef MLN_GEOM_SEEDS2TILING_HH
# define MLN_GEOM_SEEDS2TILING_HH

/*! \file mln/geom/seeds2tiling.hh
 *
 *  \brief Definition of a function which takes a labeled image and
 * returns an image which has blew up its labeled values.
 */

# include <map> 

# include <mln/core/queue_p.hh>
# include <mln/core/clone.hh>
# include <mln/accu/mean.hh>
# include <mln/estim/min_max.hh>
# include <mln/metal/vec.hh>


namespace mln
{
  namespace geom
  {

  template <typename I, typename N>
  I seeds2tiling (Image<I>& ima_, const Neighborhood<N>& nbh);

# ifndef MLN_INCLUDE_ONLY


  template <typename I, typename N>
  I
  seeds2tiling (Image<I>& ima_,
		const Neighborhood<N>& nbh)
  {
    I& ima = exact(ima_);
    I out = clone(ima_);
    queue_p<mln_psite(I)> q;

    // Init.
    {
      mln_piter(I) p(ima.domain());
      mln_niter(N) n(nbh, p);

      for_all(p) if (ima(p) == 0)
	for_all(n) if (ima(n) != 0)
	  {
	    q.push(p);
	    break;
	  }
    }

//     // Body.
//     {
//       while (! q.empty())
// 	{
// 	  mln_psite(I) p = q.front();
// 	  q.pop();
// 	  mln_invariant(ima(p) == 0);

// 	  mln_niter(N) n(nbh, p);
// 	  for_all(n) if (ima.has(n))
// 	    if (out(n) != 0)
// 	      out(p) = out(n);
// 	    else
// 	      if (! q.has(n))
// 		q.push(n);
// 	}
//     }

    // Body: alternative version.
    {
      while (! q.empty())
	{
	  mln_psite(I) p = q.front();
	  q.pop();
	  if (out(p) != 0) // p has already been processed so ignore
	    continue;

	  mln_niter(N) n(nbh, p);
	  for_all(n) if (ima.has(n))
	    if (out(n) != 0)
	      out(p) = out(n);
	    else
	      q.push_force(n); // n may already be in the queue,
	                       // yet we then queue again this psite
	}
    }

    return out;
  }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_SEEDS2TILING_HH
