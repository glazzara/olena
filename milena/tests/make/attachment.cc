// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Exercise mln::make::attachment.

#include <mln/make/attachment.hh>

#include "tests/core/image/complex_image.hh"


int
main()
{
  typedef mln::bin_2complex_image2d I;
  typedef mln_psite_(I) P;

  // Neighborhood (n-face to (n+1)-faces relationship) required by
  // mln::is_facet.
  mln::complex_lower_higher_neighborhood<I::dim, mln_geom_(I)> adj_nbh;

  /* A 2-d (simplicial) complex and its adjacency graph.

       c   0     1     2     3
     r .------------------------
       |        v0      e3     v3
     0 |         o-----------o                v0----e3----v3      
       |        / \ ,-----. /                / \    |    /   
       |       / . \ \ t1/ /                /   \   t1  /    
     1 |   e0 / / \ e1\ / / e4             e0.  ,e1'  `e4  
       |     / /t0 \ \ ' /                /   t0  \   /      
       |    / `-----' \ /                /    |    \ /       
     2 |   o-----------o                v1----e2----v2
       | v1     e2      v2

       v = vertex
       e = edge
       t = triangle
  */
  I ima = make_test_bin_2complex_image2d();

  // A few faces.
  P v0(ima.domain(), 0, 0);
  P v2(ima.domain(), 0, 2);
  P e1(ima.domain(), 1, 1);
  P t0(ima.domain(), 2, 0);

  // The attachment of the cell corresponding to T0 from IMA yields a
  // set containing E1, V0 and V1.
  mln::p_set<P> attach_t0 = mln::make::attachment(ima, t0, adj_nbh);
  mln_assertion(attach_t0.nsites() == 3);
  mln_assertion(attach_t0.has(e1));
  mln_assertion(attach_t0.has(v0));
  mln_assertion(attach_t0.has(v2));
}
