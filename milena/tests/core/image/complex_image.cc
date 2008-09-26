// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/core/image/complex_image.cc
/// \brief Test of mln::complex_image.

#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>


int main()
{
  using namespace mln;

  /*----------.
  | Complex.  |
  `----------*/

  /* A 2-d (simplicial) complex and its adjacency graph.

              v0      e3     v3
                o-----------o                v0----e3----v3      
               / \ ,-----. /                / \    |    /   
              / . \ \ t2/ /                /   \   t2  /    
          e0 / / \ e1\ / / e4             e0.  ,e1´  `e4  
            / /t1 \ \ ' /                /   t1  \   /      
           / `-----' \ /                /    |    \ /       
          o-----------o                v1----e2----v2
       v1      e2      v2

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

  
  /*---------------------.
  | Complex-based pset.  |
  `---------------------*/

  // A pset.
  p_complex<D, point2d> pc(c);
  topo::any_face_handle<D> af(e0);
  // An associated psite.
  complex_psite<D, point2d> cs(pc, af);


  /*--------------------.
  | Faces-based psets.  |
  `--------------------*/

  /* FIXME: Not that p_faces have become less interesting since the
     introduction of p_complex_faces_{fwd,bkd}_piter_.  Keep this part
     of the test?  */

  // Pset of 0-faces.
  p_faces<0, D, point2d> pf0(c);
  // Pset of 1-faces.
  p_faces<1, D, point2d> pf1(c);
  // Pset of 2-faces.
  p_faces<2, D, point2d> pf2(c);

  // Some psites on faces.
  faces_psite<0, D, point2d> fs0(pf0, v0);
  faces_psite<1, D, point2d> fs1(pf1, e0);
  faces_psite<2, D, point2d> fs2(pf2, t0);


  /*----------------------.
  | Complex-based image.  |
  `----------------------*/

  using mln::value::int_u8;

  // An image type built on a 2-complex with mln::int_u8 values on
  // each face.
  typedef complex_image<D, point2d, int_u8> ima_t;

  // Values.
  metal::vec<D + 1, std::vector< int_u8 > > values;
  // Assign 0 to 0-faces, 1 to 1-faces and 2 to 2-faces.
  for (unsigned d = 0; d <= D; ++d)
    for (unsigned n = 0; n < pc.cplx().nfaces(d); ++n)
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

  // We need to instantiate a (non temporary) object on the stack, so
  // that it can be referenced later by face_psites held by iterators
  // (as for windows and neighborhoods).

  // FIXME: Sugar the name of the iterator.
  p_complex_faces_fwd_piter_<0, D, point2d> f0p(ima.domain());
  for_all(f0p)
    std::cout << "ima(" << f0p << ") = " << ima(f0p) << std::endl;


  /* FIXME: Implement other psite iterators, for instance:

     - iterators on N-faces with N in a subset of [0, D];
     - etc.  */

  // ---------------------------------------- //
  // Iterators on windows and neighborhoods.  //
  // ---------------------------------------- //

  /* FIXME: Implement windows (and neighborhoods) and corresponding
     iterators for complex-based images.

     For a given (fixed) dimension N and a psite P on a N-face,
     implement windows returning

     - the set of (N-1)-faces adjacent to P (using p_faces and
       faces_psite?);
     - the set of (N+1)-faces adjacent to P (using p_faces and
       faces_psite?);

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
