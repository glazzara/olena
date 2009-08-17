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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/accu/stat/mean.hh>
#include <mln/accu/center.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_8.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/data/sort_psites.hh>

#include <mln/util/graph.hh>
#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/make/influence_zone_adjacency_graph.hh>
#include <mln/make/edge_image.hh>
#include <mln/make/vertex_image.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/relabel.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/tree/components.hh>
#include <mln/morpho/tree/propagate_representative.hh>
#include <mln/morpho/tree/propagate.hh>

#include <mln/math/max.hh>
#include <mln/math/diff_abs.hh>

#include "score.hh"
#include "color_distance.hh"
#include "zi.hh"
#include <iostream>
#include <cmath>

namespace mln
{

  namespace convert
  {

    // C-binary function to vv2v. (convert::to_fun...en binaire)

    template <typename R, typename V1, typename V2>
    struct cfun_vv2v : Function_vv2v< cfun_vv2v<R, V1, V2> >
    {
      typedef R result;

      cfun_vv2v( R(*f_)(V1,V2) ) :
      f (f_)
      {
      }

      R operator() (V1 arg1, V2 arg2) const
      {
	return f(arg1, arg2);
      }

      R (*f) (V1, V2);
    };


    template <typename R, typename V1, typename V2>
    inline
    cfun_vv2v<R, V1, V2>
    tofun(R (*from) (V1, V2))
    {
      cfun_vv2v<R, V1, V2> f(from);
      return f;
    }

  }

  // distance entre les centres de masses.
  inline
  float
  orientation(const point2d& p1, const point2d& p2)
  {
    return atan((float)(p2.col() - p1.col()) / (float)(p2.row() - p1.row()));
  }


  // distance entre les boites englobantes.
  inline
  value::int_u8
  distance_box(const box<point2d>& b1, const box<point2d>& b2)
  {
    const box<point2d> *btop, *bdown, *bleft, *bright;

    btop = b1.pmin().row() < b2.pmin().row() ? &b1 : &b2;
    bdown = b1.pmax().row() > b2.pmax().row() ? &b1 : &b2;
    bleft = b1.pmin().col() < b2.pmin().col() ? &b1 : &b2;
    bright = b1.pmax().col() > b2.pmax().col() ? &b1 : &b2;

    int w_dist = (bright->pmax().col() - bleft->pmin().col()) -
      (bright->pmax().col() - bright->pmin().col()) -
      (bleft->pmax().col() - bleft->pmin().col());

    int h_dist = (bdown->pmax().row() - btop->pmin().row()) -
      (bdown->pmax().row() - bdown->pmin().row()) -
      (btop->pmax().row() - btop->pmin().row());

    return math::max(math::max(w_dist, h_dist), 0);
  }


  // distance geodesic entre les composantes.
  template <typename G, typename L>
  struct distance_t : Function_v2v< distance_t<G, L> >
  {
    typedef unsigned result;
    typedef util::vertex_id_t V;
    typedef util::edge_id_t E;

    distance_t(const std::map< std::pair<L, L>, unsigned >& d_,
	       const G& graph)
      : d(d_),
	g(graph)
    {
    }

    unsigned
    operator () (const E& e_id) const
    {
      typename G::edge_t e = g.edge(e_id);
      std::pair<L, L> p(static_cast<L>(e.v1()), static_cast<L>(e.v2()));
      return d.find(p)->second;
    }

  private:
    const std::map< std::pair<L, L>, unsigned >& d;
    const G& g;
  };

}



void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " in:mask.pbm in:source.ppm [out:zi.pgm]"
	    << "Compute the SKIZ from a binary image (saved in iz.pgm)."
	    << ""
	    << std::endl;


  abort();
}



int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc < 3)
    usage(argv);

  const char* finput = argv[1];
  const char* fsource = argv[2];
  const char* foutput = argc > 3 ? argv[3] : "zi.pgm";

  // Image loadin'
  image2d<bool> input;
  io::pbm::load(input, finput);

  image2d<value::rgb8> source;
  io::ppm::load(source, fsource);


  // I.Z
  typedef value::label_16 L;

  typedef image2d<L> I;
  I labels, iz, out;
  image2d<unsigned> dmap;
  L nlabel;
  typedef std::map< std::pair<L,L>, unsigned > map_t;

  labels = labeling::blobs(input, c4(), nlabel);
  initialize(iz, input);
  initialize(dmap, input);

  map_t d = distances(labels, c4(), dmap, iz);

  io::pgm::save(data::stretch(value::int_u8(), dmap), "dmap.pgm");
  io::pgm::save(iz, foutput);

  // I.Z Graph
  util::graph izg = make::influence_zone_adjacency_graph(iz, c4(), nlabel);


  // Valuation of color distance
  edge_image<void, value::int_u8> color_dist_ima;
  {
    util::array<value::rgb8> mean_colors;
    convert::from_to(labeling::compute(accu::stat::mean<value::rgb8>(), source, labels, nlabel),
		     mean_colors);

    typedef vertex_image<void, value::rgb8, util::graph> V;
    V v_ima = make::vertex_image(izg, mean_colors);

    color_dist_ima = make::edge_image(v_ima, convert::tofun(dist_mean));
  }

  // Valuation of orientation between mass centers.
  edge_image<void, float> orientation_ima;
  {
    util::array<mln_psite_(I)> mass_centers;
    convert::from_to(labeling::compute(accu::center<mln_psite_(I)>(), labels, nlabel),
		     mass_centers);

    typedef vertex_image<void, mln_psite_(I), util::graph> V;
    V v_ima = make::vertex_image(izg, mass_centers);

    orientation_ima = make::edge_image(v_ima, convert::tofun(orientation));
  }

  // Valuation of components geodesic distance.
  typedef edge_image<void, unsigned> E;
  E e_ima = make::edge_image(izg, distance_t<util::graph, L>(d, izg));

  // Component tree.
  typedef morpho::tree::data< E, p_array<mln_psite_(E)> > T;
  p_array< mln_psite_(E) > s = data::sort_psites_decreasing(e_ima);
  mln_ch_value_(E, bool) mask;
  T tree(e_ima, s, E::nbh_t ());

  // debug
  std::cout << izg;
  //
  {
    mln_node_piter_(T) n(tree);
    for_all(n)
    {
      (void) tree.f(n);
      (void) tree.f(tree.parent(n));
    }

  }

  std::cout << tree;
  {
    typedef mln_ch_value_(E, float) A;
    morpho::attribute::score_word<E> score(color_dist_ima, e_ima, orientation_ima);
    A attr_img = morpho::tree::compute_attribute_image(score, tree);

    // delta
    A delta_img;
    initialize(delta_img, attr_img);
    mln_up_node_piter_(T) n(tree);
    for_all(n)
      delta_img(n) = math::diff_abs(attr_img(tree.parent(n)), attr_img(n));

    // Max components retrieval.
    p_array<mln_psite_(E)> obj = morpho::tree::get_components(tree, delta_img);
    mask =  morpho::tree::set_value_to_components(tree, obj, true, false);

    { //debug
      std::cout << "edge:\tscore:\tdelta:" << std::endl;
      mln_piter_(p_array<mln_psite_(E)>) it(obj);
      for_all(it)
	std::cout << it << "\t" << attr_img(it) << "\t" << delta_img(it) << std::endl;
    }

  }

//   { // Label nodes with wst
//     mln_vertex_iter_(util::graph) v(izg);
//     mln_vertex_nbh_edge_iter_(util::graph) e(v);
//     for_all(v)
//     {
//       bool deja_vu = false;
//       for_all(e)
//       {
// 	{ // debug
// 	  std::pair<L, L> p(static_cast<L>(e.v1()), static_cast<L>(e.v2()));
// 	  std::cout << e.v1() << ", "
// 		    << e.v2() << " -> "
// 		    << e_ima(e) << std::endl;
// 	}

// 	if (e_ima_wst(e))
// 	  {
// 	    mln_assertion(!deja_vu || v_ima_labels(v) == e_ima_wst(e));
// 	    v_ima_labels(v) = e_ima_wst(e);
// 	    deja_vu = true;
// 	  }
//       }
//     }
//   }

  // Relabeling

  L nlabel_b;
  mln_ch_value_(E, L) e_ima_labels = labeling::blobs(mask, E::nbh_t (), nlabel_b);
  std::cout << "Labels (febore): " << nlabel << " (now): " << nlabel_b << std::endl;
  util::array<L> tr((unsigned)nlabel + 1);
  mln_edge_iter_(util::graph) e(izg);
  for_all(e)
  {
    if (e_ima_labels(e))
      {
	std::cout << e << " : " << e_ima_labels(e) << std::endl;
	tr[e.v1()] = e_ima_labels(e);
	tr[e.v2()] = e_ima_labels(e);
      }
  }
  tr[0] = 0;

  labeling::relabel_inplace(labels, nlabel, tr);

  io::pgm::save(iz, "zi_final.pgm");
}

