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

/// \file tests/core/complex.cc
/// \brief Test of mln::complex.

#include <iostream>

#include <mln/core/complex.hh>


int main()
{
  using namespace mln;

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
    << "  number of 2-faces: c.nfaces(2) = " << c.nfaces(2) << std::endl;

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
}
