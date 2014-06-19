// Copyright (C) 2009, 2013, 2014 EPITA Research and Development
// Laboratory (LRDE).
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

/** \file constrained-connectivity.cc
   
    \brief A topological watershed-based implementation of Pierre
    Soille's constrained connectivity segmentation.
   
    Reference:

      Pierre Soille.  Constrained Connectivity for Hierarchical Image
      Partitioning and Simplification.  In: IEEE Transactions on
      Pattern Analysis and Machine Intelligence, vol. 30, no. 7, July
      2008, pages 1132-1145.


    The scenario is as follows.

    Part 1.
    \li Load a 2D image.
    \li Double its resolution (a pixel becomes a square of four pixels).
    \li Compute a line graph from this image and 
    \li Assign values of a gradient to the edges (e.g., |v1 - v2|).
    \li Compute a topological watershed transform on this gradient.
    \li Thresholding the values of this watershed gives
        alpha-connected components (alpha-cc) described by Pierre Soille.

    Part 2.
    \li For each edge of the watershed (line graph-based) image,
        compute the min and max value of the adjacent vertices (pixels).
    \li Using the component tree corresponding to the previously
        computed topological watershed transform, propagate and
        compute the min and max value for each component of the
        watershed.
    \li Create an image of ``heights'' by subtracting the image of min
        values from the image of max values.
    \li Thresholding the watershed image (using a parameter alpha)
        <em>and</em> the height image (using a parameter omega) gives
        the (alpha, omega)-connected components (alpha-omega cc)
        described by Pierre Soille.  */

#include <cstdio>

#include <set>
#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>

#include <mln/pw/all.hh>

#include <mln/fun/vv2v/diff_abs.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/neighb2d.hh>

#include <mln/morpho/watershed/topological.hh>

#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/accu/stat/min.hh>
#include <mln/accu/stat/max.hh>

#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>


int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " input.pgm" << std::endl;
      std::exit(1);
    }

  using namespace mln;
  using mln::value::int_u8;

  // Load an image.
  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  // Double its resolution.
  image2d<int_u8> f_(input.nrows() * 2, input.ncols() * 2);
  mln_piter_(image2d<int_u8>) p_ima(f_.domain());
  for_all(p_ima)
  {
    /* This conversion from a ``piter'' type to point2d is required,
       since an iterator does not expose the interface of the
       underlying point (among which the methods row(), col(),
       etc.).  */
    point2d p_ima_ = p_ima;
    point2d p_f(p_ima_.row() / 2, p_ima_.col() / 2);
    f_(p_ima) = input(p_f);
  }
  debug::println("f_:", f_);

  image_if<image2d<int_u8>, world::inter_pixel::is_pixel> f =
    world::inter_pixel::immerse(f_);
  debug::println("f:", f);

  // Compute the associated line graph gradient.
  typedef image_if<image2d<int_u8>, world::inter_pixel::is_separator> g_t;
  g_t g = world::inter_pixel::compute (f, fun::vv2v::diff_abs<int_u8>());

  debug::println("g:", g);

  // Compute a topological watershed transform on this gradient.
  typedef morpho::watershed::topo_wst<g_t, world::inter_pixel::dbl_neighb2d> tree_t;
  tree_t tree(g, world::inter_pixel::e2e());
  tree.go();
  typedef image_if<image2d<int_u8>, world::inter_pixel::is_separator> w_t;
  w_t w = morpho::watershed::topological(tree);
  debug::println("w:", w);

  // Computing the set of values of W.
  // FIXME: Milena may provide something simpler than this.
  std::set<int_u8> values;
  mln_piter_(w_t) p2(w.domain());
  for_all(p2)
    values.insert(w(p2));

  // Thresholding W for each value of the image.
  for (std::set<int_u8>::const_iterator alpha = values.begin();
       alpha != values.end(); ++alpha)
    {
      typedef image_if< w_t,
                        fun::greater_v2b_expr_< pw::value_<w_t>,
                                                pw::cst_<int_u8> > >
        alpha_cc_t;
      alpha_cc_t alpha_cc = w | (pw::value(w) > pw::cst(*alpha));
      std::cout << *alpha << "-cc:" << std::endl;
      /* FIXME: There should be variants of debug::println allowing
	 the user to pass an optional ``support'' larger than the
	 actual domain of the image.  For now, use a low-level routine
	 as a workaround.  */
      debug::impl::println(w.unmorph_().domain(), alpha_cc);
    }


  // Compute the height (max - min) of connected components on the line
  // graph-based watershed, but with min and max values computed on
  // vertices.

  /* FIXME: Of course, we'd like to be able to reuse the component
     tree within TREE instead of rebuilding a morpho::tree::data...
     This requires some changes in the topological WST implementation,
     to make its component tree structure compatible with
     morpho::tree::data.  */
  typedef p_array<tree_t::psite> psites_t;
  psites_t psites = data::sort_psites_decreasing(w);
  morpho::tree::data<w_t, psites_t> t(w, psites, world::inter_pixel::e2e());

  // Create initial images for min and max values on psites (not components).
  mln_ch_value_(w_t, accu::stat::min<int_u8>) init_min_val;
  initialize (init_min_val, w);
  mln_ch_value_(w_t, accu::stat::max<int_u8>) init_max_val;
  initialize (init_max_val, w);

  /* Compute the min and max values on vertices (pixels) adjacent to
     edge E.

     Unfortunately, the data structure G does not record any
     information from the image F (i.e., the values on
     vertices/pixels).  We have to convert the coordinates of V to its
     equivalent in F's domain to get the values on vertices.  */
  mln_piter_(w_t) e(w.domain());
  mln_niter_(world::inter_pixel::dbl_neighb2d)
    v_g(world::inter_pixel::e2v(), e);
  for_all(e)
    for_all(v_g)
    {
      // Same remark as above avour piter to point2d conversions.
      point2d v_g_ = v_g;
      point2d v_f(v_g_.row() / 2, v_g_.col() / 2);
      init_min_val(e).take(f_(v_f));
      init_max_val(e).take(f_(v_f));
    }
  // Attribute images of min and max values on components.
  accu::stat::min<int_u8> min_accu;
  mln_ch_value_(w_t, int_u8) min_val =
    morpho::tree::compute_attribute_image_from(min_accu, t, init_min_val);
  accu::stat::max<int_u8> max_accu;
  mln_ch_value_(w_t, int_u8) max_val =
    morpho::tree::compute_attribute_image_from(max_accu, t, init_max_val);
  // Attribute image of components' height.
  typedef mln_ch_value_(w_t, int_u8) height_t;
  height_t height;
  initialize(height, w);
  for_all(e)
    height(e) = max_val(e) - min_val(e);
  debug::println(height);

  // Thresholding W using first integer values with a condition on HEIGHT.
  for (unsigned alpha = 0; alpha <= 6; ++alpha)
    {
      typedef image_if< w_t,
                        fun::or__v2b_expr_<
                          fun::greater_v2b_expr_< pw::value_<w_t>,
                                                  pw::cst_<unsigned> >,
                          fun::greater_v2b_expr_< pw::value_<height_t>,
                                                  pw::cst_<unsigned> > > >
        alpha_alpha_cc_t;
      alpha_alpha_cc_t alpha_alpha_cc =
        w | (pw::value(w) > pw::cst(alpha)
             || (pw::value(height) > pw::cst(alpha)));
      std::cout << "(" << alpha << ", " << alpha << ")-cc:" << std::endl;
      // FIXME: Same remark as above about println.
      debug::impl::println(w.unmorph_().domain(), alpha_alpha_cc);
    }
}
