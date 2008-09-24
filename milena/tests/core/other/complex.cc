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

/// \file tests/core/other/complex.cc
/// \brief Test of mln::complex.

#include <iostream>

#include <mln/core/complex.hh>

using namespace mln;


// Forward declaration.
template <unsigned N, unsigned D> void test_faces_iter(complex<D>& c);
template <unsigned N, unsigned D> void test_complex_faces_iter(complex<D>& c);


int main()
{
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

       v = vertex   (0-face)
       e = edge     (1-face)
       t = triangle (2-face)
  */


  const unsigned D = 2;

  /*-----------------------.
  | Complex construction.  |
  `-----------------------*/

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

  std::cout << c << std::endl;

  std::cout
    << "Using ``static'' manipulators." << std::endl
    << "  number of 0-faces: c.nfaces<0>() = " << c.nfaces<0>() << std::endl
    << "  number of 1-faces: c.nfaces<1>() = " << c.nfaces<1>() << std::endl
    << "  number of 2-faces: c.nfaces<2>() = " << c.nfaces<2>() << std::endl
    << "  total number of faces: c.nfaces() = " << c.nfaces() << std::endl
    << std::endl;

  std::cout
    << "Using ``dynamic'' manipulators." << std::endl
    << "  number of 0-faces: c.nfaces(0) = " << c.nfaces(0) << std::endl
    << "  number of 1-faces: c.nfaces(1) = " << c.nfaces(1) << std::endl
    << "  number of 2-faces: c.nfaces(2) = " << c.nfaces(2) << std::endl
    << std::endl;

  /*-------------------.
  | Handles and data.  |
  `-------------------*/

  // Get the face data from (``static'') face handle E0.
  const face<1, D>& face1 = e0.to_face();

  // Any-face handle.
  any_face_handle<D> af(e0);
  // Get the face data from (``dynamic'') face handle AF.
  const face<1, D>& face2 = af.to_face<1>();

  mln_assertion(&face1 == &face2);

  /*------------.
  | Iteration.  |
  `------------*/

  // FIXME: Possibly split this test (create a test for iterators).

  // --------------- //
  // Iterator on C.  //
  // --------------- //

  // Iterators on a complex (not complex_image), or more precisely on
  // (all) the faces of complex C.
  mln_fwd_citer_(complex<D>) fwd_f(c);
  mln_bkd_citer_(complex<D>) bkd_f(c);
  for_all_2(fwd_f, bkd_f)
    std::cout << fwd_f << ' ' << bkd_f << std::endl;
  std::cout << std::endl;

  // -------------------------- //
  // Iterator on n-faces of C.  //
  // -------------------------- //

  /* FIXME: There's probably useless code here; I (Roland) think that
     faces_{fwd,bkd}_iter<N, D> won't be really useful to work with
     actual complex processing since they are not really flexible ---
     but I'm not sure.  */

  /* Using faces_{fwd,bkd}_iter_<N, D>, which are proxies to
     face_handles<N, D>.  */
  test_faces_iter<0>(c);
  test_faces_iter<1>(c);
  test_faces_iter<2>(c);

  /* Using complex_faces_{fwd,bkd}_iter_<N, D>, which are proxies to
     any_face_handles<N>.  */
  test_complex_faces_iter<0>(c);
  test_complex_faces_iter<1>(c);
  test_complex_faces_iter<2>(c);


  /* FIXME: Exercice more iterators (see
     milena/tests/core/complex_image.cc) and ticket #162
     (https://trac.lrde.org/olena/ticket/162) */
  // ...
}


template <unsigned N, unsigned D>
void
test_faces_iter(complex<D>& c)
{
  std::cout << "test_faces_iter<" << N << ", " << D << ">:"
	    << std::endl;
  mln_fwd_fiter(N, complex<D>) fwd_nf(c);
  mln_bkd_fiter(N, complex<D>) bkd_nf(c);
  for_all_2(fwd_nf, bkd_nf)
    std::cout << fwd_nf << ' ' << bkd_nf << std::endl;
  std::cout << std::endl;
}

template <unsigned N, unsigned D>
void
test_complex_faces_iter(complex<D>& c)
{
  std::cout << "test_complex_faces_iter<" << N << ", " << D << ">:"
	    << std::endl;
  /* FIXME: Provide sugar.  Maybe redefined mln_fwd_fiter and
     mln_bkd_fiter so that they expand as complex_faces_iters (instead
     of faces_iters).  */
  complex_faces_fwd_iter_<N, D> fwd_ncf(c);
  complex_faces_bkd_iter_<N, D> bkd_ncf(c);
  for_all_2(fwd_ncf, bkd_ncf)
    std::cout << fwd_ncf << ' ' << bkd_ncf << std::endl;
  std::cout << std::endl;
}
