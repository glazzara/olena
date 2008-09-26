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

/// \file tests/topo/complex.cc
/// \brief Test of mln::complex.

#include <iostream>

#include <mln/topo/complex.hh>

using namespace mln;


// Forward declaration.
template <unsigned N, unsigned D> void test_faces_iter(topo::complex<D>& c);
template <unsigned N, unsigned D> void test_complex_faces_iter(topo::complex<D>& c);


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

  topo::complex<D> c;

  // 0-faces (points).
  topo::face_handle<0, D> v0 = c.add_face();
  topo::face_handle<0, D> v1 = c.add_face();
  topo::face_handle<0, D> v2 = c.add_face();
  topo::face_handle<0, D> v3 = c.add_face();
 
  // 1-faces (segments).
  topo::face_handle<1, D> e0 = c.add_face(v0 + v1);
  topo::face_handle<1, D> e1 = c.add_face(v0 + v2);
  topo::face_handle<1, D> e2 = c.add_face(v1 + v2);
  topo::face_handle<1, D> e3 = c.add_face(v0 + v3);
  topo::face_handle<1, D> e4 = c.add_face(v2 + v3);

  // 2-faces (triangles).
  topo::face_handle<2, D> t0 = c.add_face(e0 + e1 + e2);
  topo::face_handle<2, D> t1 = c.add_face(e1 + e3 + e4);

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
  const topo::face_data<1, D>& face1 = e0.face_data();

  // Any-face handle.
  topo::any_face_handle<D> af(e0);
  // Get the face data from (``dynamic'') face handle AF.
  const topo::face_data<1, D>& face2 = af.face_data<1>();

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
  mln_fwd_citer_(topo::complex<D>) fwd_f(c);
  mln_bkd_citer_(topo::complex<D>) bkd_f(c);
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

  /*------------------------------.
  | Iterators on adjacent faces.  |
  `------------------------------*/

  /* FIXME: Write and exercise more iterators (see
     milena/tests/core/complex_image.cc) and ticket #162
     (https://trac.lrde.org/olena/ticket/162) */

  /* Iterate on the the set of (n-1)-faces adjacent to AF.

     Note: this can be solved with iterators where the dimension can
     be either static or dynamic.

     Let's start with a dynamic one, as it would allow us to
     write something like this:

       mln_fwd_citer_(topo::complex<D>) f(c);
       mln_fwd_lower_nciter(topo::complex<D>) n(c);
       for_all(c)
         for_all(n)
           // ...

     (Note: we might want to get rid of the name `citer', and use
     `fiter' everywhere.).

     A static version might be useful (and more efficient) too.

     Likewise, our iterators on n-faces (both faces_piter and
     complex_faces_piter) use a static `n'.  We should also have
     n-faces iterators where n could be dynamic.

     But first, we need to clarify (existing) names.

     -----------------------------------------------------------------
     Current name             New name	           Definition                
     -----------------------------------------------------------------
     complex<D>               (n/a)                General complex
			                     
     face<N, D>               face_data<N, D>      Face data                 
     face_handle<N, D>        n_face<N, D>         (Static) n-face handle    
     faces_set<N, D>          n_faces_set<N, D>    Set of face handles       
     any_face_handle<D>       face<D>	           Dynamic face handle       
			                     
     complex_fwd_iter_<D>(c)  face_fwd_iter<D>(c)  | Iterators on all
     complex_bkd_iter_<D>(c)  face_bkd_iter<D>(c)  | faces of c

     complex_iter_base_<F, E> complex_iter_base<F, E>
                                                   | Factoring base
                                                   | class.
     -----------------------------------------------------------------

     (Note: get rid of trailing underscores, even for entities in
     mln::internal::.)

     Next, write these:

     -----------------------------------------------------------------
     Name                               Definition
     -----------------------------------------------------------------
     n_faces_fwd_iter<D>(c)             | Iterators on n-faces,
     n_faces_bkd_iter<D>(c)             | n being dynamic

     adj_lower_faces_fwd_iter<D>(c, f)  | Iterators on the adjacent
     adj_lower_faces_bkd_iter<D>(c, f)  | (lower) (n-1)-faces of the
                                        | n-face f of the complex c,
                                        | n being dynamic

     adj_higher_faces_fwd_iter<D>(c, f) | Iterators on the adjacent
     adj_higher_faces_bkd_iter<D>(c, f) | (higher) (n+1)-faces of the
                                        | n-face f of the complex c,
                                        | n being dynamic

     adj_lower_dim_connected_n_faces_fwd_iter<D>(c, f)
     adj_lower_dim_connected_n_faces_bkd_iter<D>(c, f)
     (FIXME: These names are admittedly too long.)
                                        | Iterators on the the set of
                                        | n-faces sharing an adjacent
                                        | (n-1)-face with f, n being
                                        | dynamic

     adj_higher_dim_connected_n_faces_fwd_iter<D>(c, f)
     adj_higher_dim_connected_n_faces_bkd_iter<D>(c, f)
     (FIXME: These names are admittedly too long.)
                                        | Iterators on the the set of
                                        | n-faces sharing an adjacent
                                        | (n+1)-face with f, n being
                                        | dynamic

     cell_fwd_iter<D>(c, f)             | Iterators on the set of the
     cell_bkd_iter<D>(c, f)             | faces in the « cell »
                                        | including p, i.e. the set of
                                        | all m-faces adjacent to p,
                                        | where m is in [0, n-1];
                                        | this set is name « f-hat »

     cell_boundary_fwd_iter<D>(c, f)    | Likewise, but excluding p;
     cell_boundary_bkd_iter<D>(c, f)    | this set is named « p-hat* »
     -----------------------------------------------------------------

     We could also have generic iterators based on predicated, and
     even use them to provide first (non efficient) implementations of
     the iterators from the previous list.

     -----------------------------------------------------------------
     Name                               Definition
     -----------------------------------------------------------------
     generic_face_fwd_iter<D>(c, pred)  | Generic face iterators on c
     generic_face_bkd_iter<D>(c, pred)  | using predicate pred to
                                        | define the iterated subset.
     -----------------------------------------------------------------

     I'm unsure about the following existing iterators; should we keep
     them?  What are they good for, except testing our code?

     -----------------------------------------------------------------
     Current name                       Definition
     -----------------------------------------------------------------
     complex_faces_fwd_iter_<N, D>      Iterators on N-faces, N being
     complex_faces_fwd_iter_<N, D>      static, acting as proxies of
                                        face_handle<N, D>'s.

     faces_fwd_iter_<N, D>              Iterators on N-faces, N being
     faces_fwd_iter_<N, D>	        static, acting as proxies of
                                        any_face_handle<N, D>'s. 
     -----------------------------------------------------------------
  */
}


template <unsigned N, unsigned D>
void
test_faces_iter(topo::complex<D>& c)
{
  std::cout << "test_faces_iter<" << N << ", " << D << ">:"
	    << std::endl;
  mln_fwd_fiter(N, topo::complex<D>) fwd_nf(c);
  mln_bkd_fiter(N, topo::complex<D>) bkd_nf(c);
  for_all_2(fwd_nf, bkd_nf)
    std::cout << fwd_nf << ' ' << bkd_nf << std::endl;
  std::cout << std::endl;
}

template <unsigned N, unsigned D>
void
test_complex_faces_iter(topo::complex<D>& c)
{
  std::cout << "test_complex_faces_iter<" << N << ", " << D << ">:"
	    << std::endl;
  /* FIXME: Provide sugar.  Maybe redefined mln_fwd_fiter and
     mln_bkd_fiter so that they expand as complex_faces_iters (instead
     of faces_iters).  */
  topo::complex_faces_fwd_iter_<N, D> fwd_ncf(c);
  topo::complex_faces_bkd_iter_<N, D> bkd_ncf(c);
  for_all_2(fwd_ncf, bkd_ncf)
    std::cout << fwd_ncf << ' ' << bkd_ncf << std::endl;
  std::cout << std::endl;
}
