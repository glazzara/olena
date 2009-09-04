// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
        alpha-connected component (alpha-cc) described by Pierre Soille.

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
        the (alpha, omega)-connected component (alpha-omega cc)
        described by Pierre Soille.  */

#include <cstdio>

#include <set>
#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>

#include <mln/pw/all.hh>

// From Théo's sandbox.
#include <theo/cplx2d.hh>

// From Alexandre Abraham's sandbox.
#include <alexandre/topo_wst.hh>

#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/accu/stat/min.hh>
#include <mln/accu/stat/max.hh>

#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>


int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " input.pbm" << std::endl;
      std::exit(1);
    }

  using namespace mln;
  using mln::value::int_u8;

  // Load an image.
  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  // Double its resolution.
  image2d<int_u8> f(input.nrows() * 2, input.ncols() * 2);
  mln_piter_(image2d<int_u8>) p(f.domain());
  for_all(p)
  {
    /* This conversion from ``piter'' to ``point'' is required, since
       an iterator does not expose the interface of the underlying
       point (among which the methods row(), col(), etc.).  */
    point2d p_ = p;
    point2d q(p_.row() / 2, p_.col() / 2);
    f(p) = input(q);
  }
  debug::println(f);

  // Compute the associated line graph gradient.
  mln_VAR(g, cplx2d::f_to_g(f) );
  debug::println("g:", g);

  // Compute a topological watershed transform on this gradient.
  typedef morpho::topo_wst<g_t, cplx2d::dbl_neighb2d> tree_t;
  tree_t tree(g, cplx2d::e2e());
  tree.go();
  mln_VAR(w, morpho::topo_watershed(tree));
  debug::println("w:", w);

  // Computing the set of values of W.
  // FIXME: Milena may provide something simpler than this.
  std::set<int_u8> values;
  mln_piter_(w_t) p2(w.domain());
  for_all(p2)
    values.insert (w(p2));

  // Thresholding for each value of W.
  for (std::set<int_u8>::const_iterator alpha = values.begin();
       alpha != values.end(); ++alpha)
    {
      mln_VAR(alpha_cc, w | (pw::value(w) > pw::cst(*alpha)));
      /* FIXME: There should be variants of debug::println allowing
	 the user to pass an optional ``support'' larger than the
	 actual domain of the image.  For now, use a low-level routine
	 as a workaround.  */
      std::cout << *alpha << "-cc:" << std::endl;
      debug::impl::println(w.unmorph_().domain(), alpha_cc);
    }


  // Compute attributes on the components of the topological watershed (W).
  typedef p_array<tree_t::site> sites_t;
  sites_t sites = data::sort_psites_decreasing(w);

  /* FIXME: Of course, we'd like to be able to reuse the component tree
     within TREE instead of rebuilding a morpho::tree::data...  This
     requires some changes in the topological WST implementation to
     make its component tree structure compatible with
     morpho::tree::data.  */
  morpho::tree::data<w_t, sites_t> t(w, sites, cplx2d::e2e());

  // Height (max-min) on the line graph WST, but with min and max
  // values computed on vertices.
  mln_ch_value_(w_t, accu::stat::min<int_u8>) init_min_val;
  initialize (init_min_val, w);
  mln_ch_value_(w_t, accu::stat::max<int_u8>) init_max_val;
  initialize (init_max_val, w);

  mln_fwd_piter_(w_t) e(w.domain());
  mln_niter_(cplx2d::dbl_neighb2d) v(cplx2d::e2p(), e);
  for_all(e)
  {
    // Compute the min and max values on vertices (pixels) adjacent to edge E.
    for_all(v)
    {
      /* Unfortunately, the data structure G does not record any
	 information from the image F (i.e., the values on
	 vertices/pixels).  We have to convert the coordinates of V
	 to its equivalent in F's domain to get the values on
	 vertices.

	 In addition, note that an explicit `to_site()' conversion
	 is required here, since an iterator does not expose the
	 interface of the underlying point (among which the methods
	 row(), col(), etc.).  */
      point2d v_(v.to_site().row() / 2, v.to_site().col() / 2);
      init_min_val(e).take(f(v_));
      init_max_val(e).take(f(v_));
    }
  }
  accu::stat::min<int_u8> min_accu;
  mln_ch_value_(w_t, int_u8) min_val =
    morpho::tree::compute_attribute_image_from(min_accu, t, init_min_val);
  accu::stat::max<int_u8> max_accu;
  mln_ch_value_(w_t, int_u8) max_val =
    morpho::tree::compute_attribute_image_from(max_accu, t, init_max_val);
  
  mln_ch_value_(w_t, int_u8) height;
  initialize(height, w);
  for_all(e)
    height(e) = max_val(e) - min_val(e);
  debug::println(height);

  // Thresholding for the first integer values with a condition on HEIGHT.
  for (unsigned alpha = 0; alpha <= 6; ++alpha)
    {
      mln_VAR(alpha_alpha_cc,
	      w | (pw::value(w) > pw::cst(alpha)
		   || (pw::value(height) > pw::cst(alpha))));
      /* FIXME: There should be variants of debug::println allowing
	 the user to pass an optional ``support'' larger than the
	 actual domain of the image.  For now, use a low-level routine
	 as a workaround.  */
      std::cout << "(" << alpha << ", " << alpha << ")-cc:" << std::endl;
      debug::impl::println(w.unmorph_().domain(), alpha_alpha_cc);
    }
}
