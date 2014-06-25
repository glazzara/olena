// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <algorithm>
#include <iterator>
#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>

/* FIXME: Split this test (and maybe factor common parts, like the
   construction of the complex), since it exercises too many features
   in a single file.  */


int main()
{
  using namespace mln;

  /*----------.
  | Complex.  |
  `----------*/

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


  const unsigned D = 2;

  topo::complex<D> c;

  // 0-faces (points).
  topo::n_face<0, D> v0 = c.add_face();
  topo::n_face<0, D> v1 = c.add_face();
  topo::n_face<0, D> v2 = c.add_face();
  topo::n_face<0, D> v3 = c.add_face();
 
  // 1-faces (segments).
  topo::n_face<1, D> e0 = c.add_face(v0 + v1);
  topo::n_face<1, D> e1 = c.add_face(v0 + v2);
  topo::n_face<1, D> e2 = c.add_face(v1 + v2);
  topo::n_face<1, D> e3 = c.add_face(v0 + v3);
  topo::n_face<1, D> e4 = c.add_face(v2 + v3);

  // 2-faces (triangles).
  topo::n_face<2, D> t0 = c.add_face(e0 + e1 + e2);
  topo::n_face<2, D> t1 = c.add_face(e1 + e3 + e4);


  /*------------------------------.
  | Complex geometry (location).  |
  `------------------------------*/

  typedef point2d P;
  typedef geom::complex_geometry<D, P> G;
  G geom;
  geom.add_location(point2d(0,1)); // 0-face #0.
  geom.add_location(point2d(2,0)); // 0-face #1.
  geom.add_location(point2d(2,2)); // 0-face #2.
  geom.add_location(point2d(0,3)); // 0-face #3. 


  /*---------------------.
  | Complex-based pset.  |
  `---------------------*/

  // A pset.
  p_complex<D, G> pc(c, geom);

  // An iterator on this pset.
  p_complex_fwd_piter_<D, G> p(pc);
  for_all(p)
  {
    std::cout << p << ": ";
    // Print site(s).
    mln_site_(G) s(p);
    std::copy (s.sites.begin(), s.sites.end(),
	       std::ostream_iterator<P>(std::cout, " "));
    std::cout << std::endl;
  }
}
