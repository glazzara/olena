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

#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>

// FIXME: Include these elsewhere? (In complex_image.hh?)
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>

/* FIXME: Split this test (and maybe factor common parts, like the
   construction of the complex), since it exercises too many features
   in a single file.  */

// Forward declaration.
template <typename I, typename N>
void
test_neighborhood(const mln::Image<I>& ima_, const mln::Neighborhood<N> nbh,
		  const std::string& comment);


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
  topo::face<D> af(e0);
  // An associated psite.
  complex_psite<D, G> cs(pc, af);


  /*--------------------.
  | Faces-based psets.  |
  `--------------------*/

  /* FIXME: Not that p_faces have become less interesting since the
     introduction of p_complex_faces_{fwd,bkd}_piter_.  Keep this part
     of the test?  */

  // Pset of 0-faces.
  p_faces<0, D, G> pf0(c);
  // Pset of 1-faces.
  p_faces<1, D, G> pf1(c);
  // Pset of 2-faces.
  p_faces<2, D, G> pf2(c);

  // Some psites on faces.
  faces_psite<0, D, G> fs0(pf0, v0);
  faces_psite<1, D, G> fs1(pf1, e0);
  faces_psite<2, D, G> fs2(pf2, t0);


  /*----------------------.
  | Complex-based image.  |
  `----------------------*/

  using mln::value::int_u8;

  // An image type built on a 2-complex with mln::int_u8 values on
  // each face.
  typedef complex_image<D, G, int_u8> ima_t;

  // Values.
  metal::vec<D + 1, std::vector< int_u8 > > values;
  // Assign 0 to 0-faces, 1 to 1-faces and 2 to 2-faces.
  for (unsigned d = 0; d <= D; ++d)
    for (unsigned n = 0; n < pc.cplx().nfaces_of_dim(d); ++n)
      values[d].push_back(d);

  // Create and init an image based on PC.
  ima_t ima(pc, values);

  // Check the value associated to edge E0 (through complex psite CS).
  mln_assertion(ima(cs) == 1u);


  /*--------------------------------.
  | Complex-based image iterators.  |
  `--------------------------------*/

  // ---------------------------- //
  // Iterators on all the faces.  //
  // ---------------------------- //

  mln_fwd_piter_(ima_t) fp(ima.domain());
  for_all(fp)
    std::cout << "ima(" << fp << ") = " << ima(fp) << std::endl;
  std::cout << std::endl;

  mln_bkd_piter_(ima_t) bp(ima.domain());
  for_all(bp)
    std::cout << "ima(" << bp << ") = " << ima(bp) << std::endl;
  std::cout << std::endl;


  // ----------------------------------------------- //
  // Iterators on n-faces (with n fixed in [0, D]).  //
  // ----------------------------------------------- //

  // Dynamic version.
  for (unsigned n = 0; n <= D; ++n)
    {
      p_n_faces_fwd_piter<D, G> fwd_np(ima.domain(), n);
      p_n_faces_bkd_piter<D, G> bkd_np(ima.domain(), n);
      for_all_2(fwd_np, bkd_np)
	std::cout << "ima(" << fwd_np << ") = " << ima(fwd_np) << '\t'
		  << "ima(" << bkd_np << ") = " << ima(bkd_np)
		  << std::endl;
      std::cout << std::endl;
    }

  // Static version.
// FIXME: Disabled (moved to the attic).
# if 0
  // FIXME: Sugar the name of the iterator.
  p_complex_faces_fwd_piter_<0, D, G> f0p(ima.domain());
  for_all(f0p)
    std::cout << "ima(" << f0p << ") = " << ima(f0p) << std::endl;
#endif

  /* FIXME: Implement other psite iterators, for instance:

     - iterators on N-faces with N in a subset of [0, D];
     - etc.  */

  // ---------------------------- //
  // Iterators on neighborhoods.  //
  // ---------------------------- //


  test_neighborhood(ima, complex_lower_neighborhood<D, G>(),
		    "Lower-dimension faces");
  test_neighborhood(ima, complex_higher_neighborhood<D, G>(),
		    "Higher-dimension faces");
  test_neighborhood(ima, complex_lower_higher_neighborhood<D, G>(),
		    "Lower- and higer-dimension faces");

  test_neighborhood(ima,
		    complex_lower_dim_connected_n_face_neighborhood<D, G>(),
		    "Lower-dimension connected n-faces");
  test_neighborhood(ima,
		    complex_higher_dim_connected_n_face_neighborhood<D, G>(),
		    "Higher-dimension connected n-faces");

  /* FIXME: Implement other neighborhoods (and windows) and
     corresponding iterators for complex-based images.

     For a given (fixed) dimension N and a psite P on a N-face,
     implement windows returning

     - the set of N-faces sharing a (N-1)-face with P;
     - the set of N-faces sharing a (N-1)-face or (N-2)-face (by
       transitivity) with P (is it useful?);

     - the set of the ``cell'' including P (named ``P-hat'' in
       couprie.08.pami), i.e., the set of all M-faces adjacent to P,
       where M is in [0, N-1].

       In that definition, P is said adjacent to an M-face Q if
       there is a sequence (M1, M2, ..., Mk) of faces so that
       - M1 is an (N-1)-face adjacent to P ;
       - M2 is an (N-2)-face adjacent to M1 ;
       - and so on;
       - Mk is an (M+1)-face adjacent to Q.

     - what else?

     We might want to look at operators on (simplicial?) complexes
     like star, link, etc. and possibly implement them.

     See also https://trac.lrde.org/olena/ticket/162.  */
}


template <typename I, typename N>
void
test_neighborhood(const mln::Image<I>& ima_, const mln::Neighborhood<N> nbh,
		  const std::string& comment)
{
  const I& ima = exact(ima_);
  mln_fwd_piter(I) fp(ima.domain());

  mln_fwd_niter(N) fn(nbh, fp);
  mln_bkd_niter(N) bn(nbh, fp);
  for_all(fp)
  {
    std::cout << comment << " adjacent to " << fp << std::endl;
    for_all_2(fn, bn)
      {
	mln_assertion((fn.center() == static_cast<const mln_psite(I)&>(fp)));
	mln_assertion((bn.center() == static_cast<const mln_psite(I)&>(fp)));
	std::cout << "  " << fn << '\t' << bn << std::endl;
      }
  }
  std::cout << std::endl;
}
