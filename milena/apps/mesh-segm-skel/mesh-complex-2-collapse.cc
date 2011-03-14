// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file
/// \brief A program computing the ultimate 2-collapse of a triangle
/// mesh.

#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/mutable_extension_ima.hh>
#include <mln/core/routine/mutable_extend.hh>
#include <mln/data/paste.hh>

#include <mln/topo/is_n_face.hh>
#include <mln/topo/is_simple_pair.hh>
#include <mln/topo/detach_pair.hh>
#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/io/vtk/load.hh>
#include <mln/io/vtk/save.hh>


int
main(int argc, char* argv[])
{
  if (argc != 3)
    {
      std::cerr << "usage: " << argv[0] << " input.vtk output.vtk"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  std::string output_filename = argv[2];

  /*----------------------.
  | Complex input image.  |
  `----------------------*/

  // Image type.
  typedef mln::bin_2complex_image3df ima_t;
  // Dimension of the image (and thus of the complex).
  static const unsigned D = ima_t::dim;
  // Geometry of the image.
  typedef mln_geom_(ima_t) G;

  ima_t ima;
  mln::io::vtk::load(ima, input_filename);

  /*-------------.
  | 2-collapse.  |
  `-------------*/

  // ------------------------------- //
  // Image restricted to triangles.  //
  // ------------------------------- //

  // Predicate type: is a face a triangle (2-face)?
  typedef mln::topo::is_n_face<mln_psite_(ima_t), D> is_a_triangle_t;
  is_a_triangle_t is_a_triangle;
  // Surface image type, of which domain is restricted to triangles.
  typedef mln::image_if<ima_t, is_a_triangle_t> triangle_only_ima_t;
  // Surface image type, of which iteration (not domain) is restricted
  // to triangles.
  typedef mln::mutable_extension_ima<triangle_only_ima_t, ima_t>
    triangle_ima_t;

  // ------------------------ //
  // Simple point predicate.  //
  // ------------------------ //

  // Neighborhood type returning the set of (n-1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_lower_neighborhood<D, G> lower_adj_nbh_t;
  lower_adj_nbh_t lower_adj_nbh;
  // Neighborhood type returning the set of (n+1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_higher_neighborhood<D, G> higher_adj_nbh_t;
  higher_adj_nbh_t higher_adj_nbh;
  // Predicate type: is a triangle (2-face) simple?
  typedef mln::topo::is_simple_pair< triangle_ima_t,
                                     lower_adj_nbh_t,
                                     higher_adj_nbh_t >
    is_simple_triangle_t;
  is_simple_triangle_t is_simple_triangle(lower_adj_nbh, higher_adj_nbh);

  // ------------------------------- //
  // Simple point detach procedure.  //
  // ------------------------------- //

  // Functor detaching a cell.
  typedef mln::topo::detach_pair< triangle_ima_t,
                                  lower_adj_nbh_t,
                                  higher_adj_nbh_t > detach_t;
  detach_t detach(lower_adj_nbh, higher_adj_nbh);

  // ------------------------ //
  // Thinning by 2-collapse.  //
  // ------------------------ //

  /// Adjacent triangles are connected by shared edges.
  typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  mln_concrete_(ima_t) skel;
  mln::initialize(skel, ima);
  mln::data::paste
    (mln::topo::skeleton::breadth_first_thinning
     (mln::mutable_extend((ima | is_a_triangle).rw(), ima),
      nbh,
      is_simple_triangle,
      detach)
     /* Before pasting the result of the computation into SKEL,
        re-expand its domain to the initial site set, to ensure data
        from all faces (i.e., both the 2-faces, directly processed;
        and the 1-faces from the extension, undirectly processed).  */
     | ima.domain(),
     skel);

  /*---------.
  | Output.  |
  `---------*/

  mln::io::vtk::save(skel, output_filename);
}
