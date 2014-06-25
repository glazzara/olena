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

/// \file
///
/// Test of mln::complex_image with morphological filters.

#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>

// FIXME: Include these elsewhere? (In complex_image.hh?)
#include <mln/core/image/complex_windows.hh>
#include <mln/core/image/complex_window_piter.hh>

#include <mln/debug/iota.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/dilation.hh>

/* FIXME: Factor common parts with
   milena/tests/core/image/complex_image.cc  */

// Forward declaration.
template <typename I, typename W>
void test_morpho(const mln::Image<I>& ima, const mln::Window<W> win,
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


  /*-------------------------.
  | Complex-based site set.  |
  `-------------------------*/

  typedef point2d P;
  typedef geom::complex_geometry<D, P> G;
  G geom;
  geom.add_location(point2d(0,1)); // 0-face #0.
  geom.add_location(point2d(2,0)); // 0-face #1.
  geom.add_location(point2d(2,2)); // 0-face #2.
  geom.add_location(point2d(0,3)); // 0-face #3. 
  p_complex<D, G> pc(c, geom);


  /*----------------------.
  | Complex-based image.  |
  `----------------------*/

  using mln::value::int_u8;

  // An image type built on a 2-complex with mln::int_u8 values on
  // each face.
  typedef complex_image<D, G, int_u8> ima_t;
  ima_t ima(pc);
  // Initialize values.
  debug::iota(ima);

  /* Values of IMA.

	      v0      e3     v3             1      8      4
		o-----------o                o-----------o
	       / \ ,-----. /                / \ ,-----. /
	      / . \ \ t1/ /                / . \ \ 11/ /
	  e0 / / \ e1\ / / e4           5 / / \ 6 \ / / 9
	    / /t0 \ \ ' /                / /10 \ \ ' /
	   / `-----' \ /                / `-----' \ /
	  o-----------o                o-----------o
       v1      e2      v2	     2      7       3

  */

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    std::cout << "ima (" << p << ") = " << ima(p) << std::endl;
  std::cout << std::endl << std::endl;

  /*---------------------------------------------------.
  | Morphological operations on complex-based images.  |
  `---------------------------------------------------*/

  test_morpho(ima, complex_lower_window_p<D, G>(),
	      "lower-dimension faces");
  test_morpho(ima, complex_higher_window_p<D, G>(),
	      "higher-dimension faces");
  test_morpho(ima, complex_lower_higher_window_p<D, G>(),
	      "lower- and higer-dimension faces");

  test_morpho(ima, complex_lower_dim_connected_n_face_window_p<D, G>(),
	      "lower-dimension connected n-faces");
  test_morpho(ima, complex_higher_dim_connected_n_face_window_p<D, G>(),
	      "higher-dimension connected n-faces");

  /* FIXME: Exercise elementary erosion/dilation (with neighborhoods)
     when available.  */
}


template <typename I, typename W>
void
test_morpho(const mln::Image<I>& ima_, const mln::Window<W> win,
	    const std::string& comment)
{
  const I& ima = exact(ima_);
  mln_assertion(ima.is_valid());
  mln_piter(I) p(ima.domain());

  mln_concrete(I) ima_dil = mln::morpho::dilation(ima, win);
  // Manual iteration over the domain of IMA_DIL.
  std::cout << "Dilation using " << comment << ":" << std::endl;
  for_all (p)
    std::cout << "  ima_dil (" << p << ") = " << ima_dil(p) << std::endl;
  std::cout << std::endl;

  std::cout << "Erosion using " << comment  << ":" << std::endl;
  mln_concrete(I) ima_ero = mln::morpho::erosion(ima, win);
  // Manual iteration over the domain of IMA_ERO.
  for_all (p)
    std::cout << "  ima_ero (" << p << ") = " << ima_ero(p) << std::endl;
  std::cout << std::endl << std::endl;
}
