// Copyright (C) 2008, 2009, 2011 EPITA Research and Development Laboratory
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

/// \file
/// \brief Shared code for tests on complex-based images.

#ifndef TESTS_CORE_IMAGE_COMPLEX_IMAGE_HH
# define TESTS_CORE_IMAGE_COMPLEX_IMAGE_HH

# include <mln/core/image/complex_image.hh>
# include <mln/core/alias/complex_image.hh>
# include <mln/value/int_u8.hh>

# include <mln/data/fill.hh>

// FIXME: Keep this?  (See below.)
# include <mln/core/site_set/p_faces.hh>


inline
mln::int_u8_2complex_image2d
make_test_complex_image()
{
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
     1 |   e0 / / \ e1\ / / e4             e0.  ,e1´  `e4  
       |     / /t0 \ \ ' /                /   t0  \   /      
       |    / `-----' \ /                /    |    \ /       
     2 |   o-----------o                v1----e2----v2
       | v1     e2      v2

       v = vertex
       e = edge
       t = triangle
  */


  const unsigned D = 2;

  mln::topo::complex<D> c;

  // 0-faces (points).
  mln::topo::n_face<0, D> v0 = c.add_face();
  mln::topo::n_face<0, D> v1 = c.add_face();
  mln::topo::n_face<0, D> v2 = c.add_face();
  mln::topo::n_face<0, D> v3 = c.add_face();
 
  // 1-faces (segments).
  mln::topo::n_face<1, D> e0 = c.add_face(v0 + v1);
  mln::topo::n_face<1, D> e1 = c.add_face(v0 + v2);
  mln::topo::n_face<1, D> e2 = c.add_face(v1 + v2);
  mln::topo::n_face<1, D> e3 = c.add_face(v0 + v3);
  mln::topo::n_face<1, D> e4 = c.add_face(v2 + v3);

  // 2-faces (triangles).
  mln::topo::n_face<2, D> t0 = c.add_face(e0 + e1 + e2);
  mln::topo::n_face<2, D> t1 = c.add_face(e1 + e3 + e4);


  /*------------------------------.
  | Complex geometry (location).  |
  `------------------------------*/

  typedef mln::discrete_plane_2complex_geometry G;
  G geom;
  geom.add_location(mln::point2d(0,1)); // 0-face #0.
  geom.add_location(mln::point2d(2,0)); // 0-face #1.
  geom.add_location(mln::point2d(2,2)); // 0-face #2.
  geom.add_location(mln::point2d(0,3)); // 0-face #3. 


  /*---------------------.
  | Complex-based pset.  |
  `---------------------*/

  // A pset.
  mln::p_complex<D, G> pc(c, geom);
  mln::topo::face<D> af(e0);
  // An associated psite.
  mln::complex_psite<D, G> cs(pc, af);


  /*--------------------.
  | Faces-based psets.  |
  `--------------------*/

  /* FIXME: Not that p_faces have become less interesting since the
     introduction of p_complex_faces_{fwd,bkd}_piter_.  Keep this part
     of the test?  */

  /* FIXME: Move this parts out of this function? (into an existing
     test?)  */

  // Pset of 0-faces.
  mln::p_faces<0, D, G> pf0(c);
  // Pset of 1-faces.
  mln::p_faces<1, D, G> pf1(c);
  // Pset of 2-faces.
  mln::p_faces<2, D, G> pf2(c);

  // Some psites on faces.
  mln::faces_psite<0, D, G> fs0(pf0, v0);
  mln::faces_psite<1, D, G> fs1(pf1, e0);
  mln::faces_psite<2, D, G> fs2(pf2, t0);


  /*----------------------.
  | Complex-based image.  |
  `----------------------*/

  using mln::value::int_u8;

  // An image type built on a 2-complex with mln::int_u8 values on
  // each face.
  typedef mln::complex_image<D, G, int_u8> ima_t;

  // Values.
  mln::metal::vec<D + 1, std::vector< int_u8 > > values;
  // Assign 0 to 0-faces, 1 to 1-faces and 2 to 2-faces.
  for (unsigned d = 0; d <= D; ++d)
    for (unsigned n = 0; n < pc.cplx().nfaces_of_dim(d); ++n)
      values[d].push_back(d);

  // Create and init an image based on PC.
  ima_t ima(pc, values);

  // Check the value associated to edge E0 (through complex psite CS).
  mln_postcondition(ima(cs) == 1u);

  return ima;
}


/* FIXME: Quick and dirty implementation, based on
   `make_test_complex_image'.  Factor and revamp.  */
inline
mln::bin_2complex_image2d
make_test_bin_2complex_image2d()
{
  mln::int_u8_2complex_image2d model = make_test_complex_image();
  mln::bin_2complex_image2d ima;
  mln::initialize(ima, model);
  mln::data::fill(ima, true);
  return ima;
}

#endif // ! TESTS_CORE_IMAGE_COMPLEX_IMAGE_HH
