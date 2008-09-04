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

/// \file tests/core/complex_image.cc
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

  complex<D> c;

  // 0-faces (points).
  face_handle<0, D> v0 = c.add_face();
  face_handle<0, D> v1 = c.add_face();
  face_handle<0, D> v2 = c.add_face();
  face_handle<0, D> v3 = c.add_face();
 
  // 1-faces (segments).
  face_handle<1, D> e0 = c.add_face(v0 + v1);
  face_handle<1, D> e1 = c.add_face(v0 + v2);
  face_handle<1, D> e2 = c.add_face(v1 + v2);
  face_handle<1, D> e3 = c.add_face(v0 + v3);
  face_handle<1, D> e4 = c.add_face(v2 + v3);

  // 2-faces (triangles).
  face_handle<2, D> t0 = c.add_face(e0 + e1 + e2);
  face_handle<2, D> t1 = c.add_face(e1 + e3 + e4);

  
  /*---------------------.
  | Complex-based pset.  |
  `---------------------*/

  // A pset.
  p_complex<D, point2d> pc(c);
  /* An any-face handle (on E0).

     Note that AF is built on `e0_', not `e0' (e0_ is built on
     `pc.cplx()', not `c'), since the p_complex `pc' makes a copy of
     the complex `c', and crossed-ownership tests doesn't work.  I.e.,

       pc.has(e0);

     is false.


     FIXME: This might be a problem, since `pc.cplx()' and `c'
     represent the same complex, in two different memory location (but
     the former is controlled by PC, while the latter can be modified,
     or even destroyed).  This is a common problem for ``big'' values
     that we don't want to manipulate by value (copy), or when we
     don't want to use expensive, deep comparisons of pset to ensure
     consistency.  Here (and in graph-based images), we choose to
     create a copy of the pset once, and manipulate it with a
     tracked_ptr, to ensure both

     1. perfect control of the lifetime of the pset (here, you can
        delete `c', and `pc' will still be valid);

     2. no pset duplication when creating new images based on it.


     I (Roland) don't see elegant solutions here.  A possiblity would
     be to disconnect a face_handle from its complex (currently, a
     face_handle is a bit like a Trivial Iterator from the C++
     Standard Library), but this means relaxed dynamic checks, and
     more obscure errors.

     At least, we could have better error messages, i.e., something
     like

       mln/core/image/complex_image.hh 267:
       mln::complex_image<D, P, V>::operator(): Uncompatible p_complex.

     instead of

       mln/core/image/complex_image.hh:267:
       typename mln::complex_image<D, P, V>::lvalue
       mln::complex_image<D, P, V>::operator()(const mln::complex_psite<D, P>&)
         [with unsigned int D = 2u,
                            P = mln::point_<mln::grid::square, int>,
                            V = mln::value::int_u<8u>]:
       Assertion `this->data_->pc_.has(p)' failed.

     (which looks even uglier in the original, non-indented version).

     Ask Akim for his improved versions of abort() and assert().  */
  face_handle<1, D> e0_(pc.cplx(), 0);
  any_face_handle<D> af(e0_);
  // An associated psite.
  complex_psite<D, point2d> cs(af);


  /*--------------------.
  | Faces-based psets.  |
  `--------------------*/

  // Pset of 0-faces.
  p_faces<0, D, point2d> pf0(c);
  // Pset of 1-faces.
  p_faces<1, D, point2d> pf1(c);
  // Pset of 2-faces.
  p_faces<2, D, point2d> pf2(c);

  // Some psites on faces.
  faces_psite<0, D, point2d> fs0(v0);
  faces_psite<1, D, point2d> fs1(e0);
  faces_psite<2, D, point2d> fs2(t0);


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

  // Check the value associated to edge E0_ (through complex psite CS).
  mln_assertion(ima(cs) == 1u);


  /*--------------------------------.
  | Complex-based image iterators.  |
  `--------------------------------*/

  mln_fwd_piter_(ima_t) fp(ima.domain());
  for_all(fp)
    std::cout << "ima(" << fp << ") = " << ima(fp) << std::endl;

  std::cout << std::endl;

  mln_bkd_piter_(ima_t) bp(ima.domain());
  for_all(bp)
    std::cout << "ima(" << bp << ") = " << ima(bp) << std::endl;


  /* FIXME: Implement other psite iterators, for instance:

     - iterators on N-faces with N fixed in [0, D] (using p_faces
       and faces_psite?)
     - iterators on N-faces with N in a subset of [0, D];
     - etc.  */

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
       if there is a sequence (M1, M2, ..., Mn) of faces so that
       - M1 is an (M+1)-face adjacent to M ;
       - M2 is an (M+2)-face adjacent to M1 ;
       - and so on;
       - Mn is an (N-1)-face adjacent to N.

     - what else?

     We might want to look at operators on (simplicial?) complexes
     like star, link, etc. and possibly implement them.  */
}
