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
/// \brief Exercise mln::complex.

#include <algorithm>
#include <iterator>
#include <iostream>

#include <mln/topo/complex.hh>

using namespace mln;

/* FIXME: Split this test (and maybe factor common parts, like the
   construction of the complex), since it exercises too many features
   in a single file.  */

// Forward declarations.
template <unsigned N, unsigned D>
void test_static_n_face_iter(topo::complex<D>& c);
// FIXME: Disabled (moved to the attic).
#if 0
template <unsigned N, unsigned D>
void test_faces_iter(topo::complex<D>& c);
#endif


int main()
{
  /* A 2-d (simplicial) complex and its adjacency graph.

              v0      e3     v3
                o-----------o                v0----e3----v3      
               / \ ,-----. /                / \    |    /   
              / . \ \ t1/ /                /   \   t1  /    
          e0 / / \ e1\ / / e4             e0.  ,e1´  `e4  
            / /t0 \ \ ' /                /   t0  \   /      
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
  topo::n_face<0, D> v0 = c.add_face();
  topo::n_face<0, D> v1 = c.add_face();
  topo::n_face<0, D> v2 = c.add_face();
  topo::n_face<0, D> v3 = c.add_face();
 
  // 1-faces (segments).
  topo::n_face<1, D> e0 = c.add_face(-v1 + v0);
  topo::n_face<1, D> e1 = c.add_face(-v0 + v2);
  topo::n_face<1, D> e2 = c.add_face(-v2 + v1);
  topo::n_face<1, D> e3 = c.add_face(-v0 + v3);
  topo::n_face<1, D> e4 = c.add_face(-v3 + v2);

  // 2-faces (triangles).
  topo::n_face<2, D> t0 = c.add_face( e0 + e1 + e2);
  topo::n_face<2, D> t1 = c.add_face(-e1 + e3 + e4);

  std::cout << c << std::endl;

  std::cout
    << "Using ``static'' manipulators." << std::endl
    << "  number of 0-faces: c.nfaces_of_static_dim<0>() = "
    << c.nfaces_of_static_dim<0>() << std::endl
    << "  number of 1-faces: c.nfaces_of_static_dim<1>() = "
    << c.nfaces_of_static_dim<1>() << std::endl
    << "  number of 2-faces: c.nfaces_of_static_dim<2>() = "
    << c.nfaces_of_static_dim<2>() << std::endl
    << "  total number of faces: c.nfaces() = " << c.nfaces() << std::endl
    << std::endl;

  std::cout
    << "Using ``dynamic'' manipulators." << std::endl
    << "  number of 0-faces: c.nfaces_of_dim(0) = "
    << c.nfaces_of_dim(0) << std::endl
    << "  number of 1-faces: c.nfaces_of_dim(1) = "
    << c.nfaces_of_dim(1) << std::endl
    << "  number of 2-faces: c.nfaces_of_dim(2) = "
    << c.nfaces_of_dim(2) << std::endl
    << std::endl;

  /*-------------------.
  | Handles and data.  |
  `-------------------*/

  // Get the face data from (``static'') face handle E0.
  const topo::face_data<1, D>& face1 = e0.data();

  /* FIXME: Rename AF (everywhere) as `any-face handles' have been
     renamed to `face'.  */
  // Any-face handle.
  topo::face<D> af(e0);
  // Get the face data from (``dynamic'') face handle AF.
  const topo::face_data<1, D>& face2 = af.data<1>();

  mln_assertion(&face1 == &face2);

  /*-----------------.
  | Adjacent faces.  |
  `-----------------*/

  // Adjacent lower-dimension faces of AF.
  std::vector< topo::algebraic_face<D> > af_lower_dim_adj_faces =
    af.lower_dim_adj_faces();
  std::cout << "lower-dimension faces adjacent to " << af << ":" << std::endl;
  std::copy (af_lower_dim_adj_faces.begin(), af_lower_dim_adj_faces.end(),
	     std::ostream_iterator< topo::face<D> > (std::cout, "\n"));
  std::cout << std::endl;

  // Adjacent higher-dimension faces of AF.
  std::vector< topo::algebraic_face<D> > af_higher_dim_adj_faces =
    af.higher_dim_adj_faces();
  std::cout << "higher-dimension faces adjacent to " << af << ":" << std::endl;
  std::copy (af_higher_dim_adj_faces.begin(), af_higher_dim_adj_faces.end(),
	     std::ostream_iterator< topo::face<D> > (std::cout, "\n"));
  std::cout << std::endl;


  /*------------.
  | Iteration.  |
  `------------*/

  // FIXME: Possibly split this test (create a test for iterators).

  // --------------- //
  // Iterator on C.  //
  // --------------- //

  // Iterators on a complex (not complex_image), or more precisely on
  // (all) the faces of complex C.
  topo::face_fwd_iter<D> fwd_f(c);
  topo::face_bkd_iter<D> bkd_f(c);
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

  // Dynamic version.
  for (unsigned n = 0; n <= D; ++n)
    {
      topo::n_face_fwd_iter<D> fwd_nf(c, n);
      topo::n_face_fwd_iter<D> bkd_nf(c, n);
      std::cout << "test (dynamic) n_face_iters (n = " << n << "):"
		<< std::endl;
      for_all_2(fwd_nf, bkd_nf)
	std::cout << fwd_nf << ' ' << bkd_nf << std::endl;
      std::cout << std::endl;
    }

  // Static version.
  test_static_n_face_iter<0>(c);
  test_static_n_face_iter<1>(c);
  test_static_n_face_iter<2>(c);

// FIXME: Disabled (moved to the attic).
# if 0
  /* Using faces_{fwd,bkd}_iter_<N, D>, which are proxies to
     n_faces<N, D>.  */
  test_faces_iter<0>(c);
  test_faces_iter<1>(c);
  test_faces_iter<2>(c);
#endif

  /*------------------------------.
  | Iterators on adjacent faces.  |
  `------------------------------*/

  /* FIXME: Write and exercise more iterators (see
     milena/tests/core/complex_image.cc) and ticket #162
     (https://trac.lrde.org/olena/ticket/162) */

  // ------------------------------------------------------------ //
  // Iterate on the the set of (n-1)- and/or (n+1)-faces adjacent //
  // to the faces of C.                                           //
  // ------------------------------------------------------------ //

  /*   Note: this can be solved with iterators where the dimension can
     be either static or dynamic.  For the moment, our iterators are
     dynamic w.r.t. the dimensions of the faces (both the reference
     face and the iterated ones).

     Static versions might be useful, and more efficient too, when
     flexibility is not needed.  */

  // Iterate on the the set of (n-1)-faces adjacent to the faces of C.
  topo::adj_lower_face_fwd_iter<D> fwd_alf(fwd_f);
  topo::adj_lower_face_bkd_iter<D> bkd_alf(fwd_f);
  for_all(fwd_f)
  {
    std::cout << "Lower-dimension faces adjacent to " << fwd_f << ": "
	      << std::endl;
    for_all_2(fwd_alf, bkd_alf)
      std::cout << "  " << fwd_alf << '\t' << bkd_alf << std::endl;
  }
  std::cout << std::endl;

  // Iterate on the the set of (n+1)-faces adjacent to the faces of C.
  topo::adj_higher_face_fwd_iter<D> fwd_ahf(fwd_f);
  topo::adj_higher_face_bkd_iter<D> bkd_ahf(fwd_f);
  for_all(fwd_f)
  {
    std::cout << "Higher-dimension faces adjacent to " << fwd_f << ": "
	      << std::endl;
    for_all_2(fwd_ahf, bkd_ahf)
      std::cout << "  " << fwd_ahf << '\t' << bkd_ahf << std::endl;
  }
  std::cout << std::endl;

  // Iterate on the the set of (n+1)-faces *and* (n+1)-faces adjacent
  // to the faces of C.
  topo::adj_lower_higher_face_fwd_iter<D> fwd_alhf(fwd_f);
  topo::adj_lower_higher_face_bkd_iter<D> bkd_alhf(fwd_f);
  for_all(fwd_f)
  {
    std::cout << "Lower- and higher-dimension faces adjacent to " << fwd_f
	      << ": " << std::endl;
    for_all_2(fwd_alhf, bkd_alhf)
      std::cout << "  " << fwd_alhf << '\t' << bkd_alhf << std::endl;
  }
  std::cout << std::endl;


  // ------------------------------------------------------- //
  // Iterators on the the set of n-faces sharing an adjacent //
  // (n-1)-face or (n+1)-face with f, n being dynamic.       //
  // ------------------------------------------------------- //

  // For each face F of C, iterate on the the set of n-faces sharing
  // adjacent (n-1)-faces with F.
  topo::adj_lower_dim_connected_n_face_fwd_iter<D> fwd_aldcf(fwd_f);
  topo::adj_lower_dim_connected_n_face_bkd_iter<D> bkd_aldcf(fwd_f);
  for_all(fwd_f)
  {
    std::cout << "Lower-dim-connected faces adjacent to " << fwd_f
	      << ": " << std::endl;
    for_all_2(fwd_aldcf, bkd_aldcf)
      std::cout << "  " << fwd_aldcf << '\t' << bkd_aldcf << std::endl;
  }
  std::cout << std::endl;

  // For each face F of C, iterate on the the set of n-faces sharing
  // adjacent (n+1)-faces with F.
  topo::adj_higher_dim_connected_n_face_fwd_iter<D> fwd_ahdcf(fwd_f);
  topo::adj_higher_dim_connected_n_face_bkd_iter<D> bkd_ahdcf(fwd_f);
  for_all(fwd_f)
  {
    std::cout << "Higher-dim-connected faces adjacent to " << fwd_f
	      << ": " << std::endl;
    for_all_2(fwd_ahdcf, bkd_ahdcf)
      std::cout << "  " << fwd_ahdcf << '\t' << bkd_ahdcf << std::endl;
  }
  std::cout << std::endl;

  // For each face F of C, and for M in [0, D], iterate on the the set
  // of M-faces transitively adjacent to F.
  topo::adj_m_face_fwd_iter<D> fwd_amf(fwd_f, 0);
  topo::adj_m_face_bkd_iter<D> bkd_amf(fwd_f, 0);
  for_all(fwd_f)
  {
    for (unsigned m = 0; m <= D; ++m)
      {
	fwd_amf.set_m(m);
	bkd_amf.set_m(m);
	std::cout << m << "-faces (transitively) adjacent to " << fwd_f
		  << ": "  << std::endl;
	for_all_2(fwd_amf, bkd_amf)
	  std::cout << "  " << fwd_amf << '\t' << bkd_amf << std::endl;
      }
    std::cout << std::endl;
  }


  /* Next, write these:

     -----------------------------------------------------------------
     Name                               Definition
     -----------------------------------------------------------------
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


     I'm unsure about the old iterators on n-faces (with a static n)
     moved to the attic; should we keep them?  What are they good for,
     except testing our code?  At least, we should find better names
     for them, as we might confuse them with new iterators.

     -----------------------------------------------------------------
     Current name                       Definition
     -----------------------------------------------------------------
     complex_faces_fwd_iter_<N, D>      Iterators on N-faces, N being
     complex_faces_fwd_iter_<N, D>      static, acting as proxies of
                                        n_face<N, D>'s.

     faces_fwd_iter_<N, D>              Iterators on N-faces, N being
     faces_fwd_iter_<N, D>              static, acting as proxies of
                                        face<D>'s. 
     -----------------------------------------------------------------

     See also https://trac.lrde.org/olena/wiki/Olena/ComplexBasedImages  */


  /*------------------.
  | Other iterators.  |
  `------------------*/

  // For each face, iterate on itself.  (This iterator is not
  // interesting as-is, but is useful when combined with others,
  // e.g. in topo::centered_iter_adapter).
  std::cout << "Center-only iterator:" << std::endl;
  topo::center_only_iter<D> center(fwd_f);
  for_all(fwd_f)
    for_all(center)
      std::cout << "  " << center << std::endl;
  std::cout << std::endl;
}


template <unsigned N, unsigned D>
void
test_static_n_face_iter(topo::complex<D>& c)
{
  std::cout << "test_static_n_face_iter<" << N << ", " << D << ">:"
	    << std::endl;
  /* FIXME: Provide sugar.  Maybe redefined mln_fwd_fiter and
     mln_bkd_fiter so that they expand as complex_faces_iters (instead
     of faces_iters).  */
  topo::static_n_face_fwd_iter<N, D> fwd_nf(c);
  topo::static_n_face_bkd_iter<N, D> bkd_nf(c);
  for_all_2(fwd_nf, bkd_nf)
    std::cout << fwd_nf << ' ' << bkd_nf << std::endl;
  std::cout << std::endl;
}

// FIXME: Disabled (moved to the attic).
# if 0
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
#endif
