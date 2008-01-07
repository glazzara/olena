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

#ifndef MLN_MAKE_VORONOI_HH
# define MLN_MAKE_VORONOI_HH

/*! \file mln/make/voronoi.hh
 *
 * \brief Routine to construct a voronoi mln::mesh_p.
 */

# include <vector>
# include <map>

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/mesh_p.hh>
# include <mln/accu/mean.hh>
# include <mln/estim/min_max.hh>

namespace mln
{

  namespace make
  {

    template <typename I, typename N>
    mesh_p<mln_psite(I)>
    voronoi (Image<I>& ima_,
	     Image<I>& orig_,
	     const Neighborhood<N>& nbh);

# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N>
    inline
    mesh_p<mln_psite(I)>
    voronoi (Image<I>& ima_,
	     Image<I>& orig_,
	     const Neighborhood<N>& nbh)
    {
      typedef metal::vec<2,float> X;
      typedef mln_value(I) V;
      typedef mln_psite(I) P;

      I& ima = exact(ima_);
      I& orig = exact(orig_);
      util::graph<void> gr;
      V min, max;
      estim::min_max (ima, min, max);
      unsigned nb = max - min + 1;
      std::vector<P> v(nb);
      std::vector< accu::mean_< X > > tab_mean (nb);
      std::map<std::pair<V, V>, bool> m;

      /// Take original point.
      {
	mln_piter(I) p(orig.domain());

	for_all(p)
	  {
	    if (orig(p) != 0)
	      {
		X x = mln_point(I)(p);
		tab_mean[orig(p) - min].take(x);
	      }
	  }
      }

      /// Compute links between link.
      {
	mln_piter(I) p(ima.domain());
	mln_niter(N) n(nbh, p);

	for_all(p)
	  {
	    for_all (n) if (ima.has(n))
	      if (ima(p) != ima(n))
		m[std::pair<V, V>(ima(p) - min, ima(n) - min)] = true;
	  }
      }
      
      /// Compute the center of label.
      for (unsigned i = 0; i < nb; ++i)
	{
	  gr.add_node ();

	  /// FIXME
 	  v[i] = make::point2d ((unsigned)tab_mean[i].to_result ()[0],
				(unsigned)tab_mean[i].to_result ()[1]);
	}

      typename std::map<std::pair<V, V>, bool>::const_iterator it = m.begin ();
      for (; it != m.end (); ++it)
	gr.add_edge((*it).first.first, (*it).first.second);

      mesh_p<P> res(gr, v);
      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_VORONOI_HH
