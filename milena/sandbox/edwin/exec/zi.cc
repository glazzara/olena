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
#include <mln/core/var.hh>

#include <mln/accu/stat/mean.hh>
#include <mln/accu/center.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_8.hh>
#include <mln/data/convert.hh>
#include <mln/util/graph.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/relabel.hh>

#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/make/influence_zone_adjacency_graph.hh>
#include <mln/make/edge_image.hh>
#include <mln/make/vertex_image.hh>

#include <mln/morpho/closing/height.hh>

#include <mln/math/max.hh>
#include <mln/norm/l1.hh>

#include "color_distance.hh"
#include "zi.hh"
#include <iostream>

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
  template <typename P>
  inline
  value::int_u8
  distance(const P& p1, const P& p2)
  {
    return convert::to<value::int_u8>(norm::l1_distance(p1.to_vec(), p2.to_vec()));
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
      std::pair<L, L> p((value::label_8)e.v1(), (value::label_8)e.v2());
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

  if (argc < 4)
    usage(argv);

  const char* finput = argv[1];
  const char* fsource = argv[2];
  const unsigned lambda = atoi(argv[3]);
  const char* foutput = argc > 4 ? argv[4] : "zi.pgm";

  // Image loadin'
  image2d<bool> input;
  io::pbm::load(input, finput);

  image2d<value::rgb8> source;
  io::ppm::load(source, fsource);


  // I.Z
  typedef value::label_8 L;

  typedef image2d<L> I;
  I labels, iz, out;
  image2d<unsigned> dmap;
  L nlabel;
  typedef std::map< std::pair<L,L>, unsigned > map_t;

  labels = labeling::blobs(input, c4(), nlabel);
  initialize(iz, input);
  initialize(dmap, input);

  map_t d = distances(labels, c4(), dmap, iz);

  io::pgm::save(data::convert(int_u8(), dmap), "dmap.pgm");
  io::pgm::save(iz, foutput);

  // I.Z Graph
  util::graph izg = make::influence_zone_adjacency_graph(iz, c4(), nlabel);


  // Valuation of color distance
  /*
  util::array<value::rgb8> mean_colors;
  convert::from_to(labeling::compute(accu::stat::mean<value::rgb8>(), source, labels, nlabel),
  mean_colors);

  typedef vertex_image<void, value::rgb8, util::graph> V;
  V v_ima = make::vertex_image(izg, mean_colors);

  typedef edge_image<void, value::int_u8, util::graph> E;
  E e_ima = make::edge_image(v_ima, convert::tofun(dist_mean));
  */

  // Valuation of geometric distance
  /*
  util::array<mln_psite_(I)> mass_centers;
  convert::from_to(labeling::compute(accu::center<mln_psite_(I)>(), labels, nlabel),
		   mass_centers);

  typedef vertex_image<void, mln_psite_(I), util::graph> V;
  V v_ima = make::vertex_image(izg, mass_centers);

  typedef edge_image<void, value::int_u8, util::graph> E;
  E e_ima = make::edge_image(v_ima, convert::tofun(distance<mln_psite_(I)>));
  */

  // Valuation of distance between bounding boxes
  /*
  util::array< box<mln_psite_(I)> > bboxes;
  convert::from_to(labeling::compute(accu::shape::bbox<mln_psite_(I)>(), labels, nlabel),
		   bboxes);

  typedef vertex_image<void, box<mln_psite_(I)>, util::graph> V;
  V v_ima = make::vertex_image(izg, bboxes);

  typedef edge_image<void, value::int_u8, util::graph> E;
  E e_ima = make::edge_image(v_ima, convert::tofun(distance_box));
  */

  // Valuation of components geodesic distance.
  typedef edge_image<void, unsigned, util::graph> E;
  E e_ima = make::edge_image(izg, distance_t<util::graph, value::label_8>(d, izg));


  // Closing
  e_ima = morpho::closing::height(e_ima, E::nbh_t (), lambda);


  // WST.
  typedef vertex_image<void, L, util::graph> V;
  p_vertices<util::graph> pv(izg);
  V v_ima_labels(pv);

  typedef edge_image<void, L, util::graph> e_ima_wst_t;
  L nbasins;
  e_ima_wst_t e_ima_wst =
    morpho::watershed::flooding(e_ima, E::nbh_t (), nbasins);

  { // Label nodes with edge wst labels.
    mln_vertex_iter_(util::graph) v(izg);
    mln_vertex_nbh_edge_iter_(util::graph) n(v);
    for_all(v)
      for_all(n)
      {
	if (e_ima_wst(n))
	  {
	    v_ima_labels(v) = e_ima_wst(n);
	    break;
	  }
      }
  }

  // Relabeling
  util::array<L> tr(nlabel);
  mln_piter_(V) v(v_ima_labels.domain());
  for_all(v)
  {
    std::cout << v.id() << " : " << v_ima_labels(v) << std::endl;
    tr[v.id()] = v_ima_labels(v);
  }

  //print distance
  for (map_t::const_iterator it = d.begin(); it != d.end(); ++it)
    std::cout << it->first.first << ", "
	      << it->first.second << " -> "
	      << it->second << std::endl;

  labeling::relabel_inplace(iz, nlabel, tr);

  io::pgm::save(iz, "zi_wst.pgm");
}

