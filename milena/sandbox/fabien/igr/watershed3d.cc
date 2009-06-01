#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/data/transform.hh>
#include <mln/data/stretch.hh>

#include <mln/labeling/mean_values.hh>

#include <mln/convert/to_fun.hh>

#include <mln/make/graph.hh>

#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/fun/l2l/wrap.hh>

#include <mln/core/var.hh>

#include <mln/core/routine/extend.hh>

#include <mln/util/graph.hh>

#include <mln/essential/2d.hh>
#include <mln/core/alias/vec3d.hh>
#include <mln/debug/draw_graph.hh>
#include <mln/util/graph.hh>
#include <mln/accu/center.hh>
#include <mln/io/dump/all.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb16.hh>
#include <mln/accu/compute.hh>
#include <mln/core/alias/dpoint2d.hh>
#include <mln/draw/box.hh>
#include <mln/data/stretch.hh>
#include <mln/fun/v2v/id.hh>
#include <mln/fun/l2l/wrap.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/extension/adjust_fill.hh>
#include <mln/extract/all.hh>
#include <mln/make/region_adjacency_graph.hh>



// Given a color image and a wshed image, computes the component graph.
// Vertex values are computed thanks to a RGB image.

namespace mln
{

  template <typename V>
  value::int_u8 dist(const V& c1, const V& c2)
  {
    unsigned d = math::diff_abs(c1.red(), c2.red());
    unsigned d_;
    d_ = math::diff_abs(c1.green(), c2.green());

    if (d_ > d)
      d = d_;

    d_ = math::diff_abs(c1.blue(), c2.blue());

    if (d_ > d)
      d = d_;
    return d;
  }


  // ima_v, image on graph vertices; value = mean color per vertex (watershed basin)
  template <typename I>
  inline
  pw::image<fun::i2v::array<value::int_u8>,
	    p_edges<util::graph, fun::i2v::array<mln_site(I)> > >
  make_edge_graph_image(const I& ima_v, const util::graph& g)
  {
    // edge sites.
    typedef fun::i2v::array<mln_site(I)> edge_site_t;
    edge_site_t edge_site(g.e_nmax(), literal::origin);
    typedef p_edges<util::graph, edge_site_t > pe_t;
    pe_t pe(g, edge_site);

    // edge values
    typedef fun::i2v::array<value::int_u8> edge_values_t;
    edge_values_t edge_values(g.e_nmax());

    // image on graph edges
    typedef pw::image<edge_values_t, pe_t> ima_e_t;
    ima_e_t ima_e = (edge_values | pe);

    mln_piter(ima_e_t) e(ima_e.domain());
    for_all(e) // in ima_e
      ima_e(e) = math::diff_abs(ima_v.function()(e.element().v1()), ima_v.function()(e.element().v2()));

    return ima_e;
  }


  template <typename I, typename J>
  inline
  util::array<mln_value(I)>
  mean_color_values(const I& input, const J& w, mln_value(J) nbasins)
  {
    // Cf. sandbox/theo/color/segment_rgb_pixels.cc

    util::array<float> m_3f = labeling::compute(accu::mean<mln_value(I)>(),
						input, // input color image
						w, // watershed labeling
						nbasins);
    m_3f[0] = literal::zero;

    util::array<mln_value(I)> m;
    convert::from_to(m_3f, m);
    m[0] = 150u;

    /*io::ppm::save(data::transform(w,
	  convert::to< fun::i2v::array<mln_value(I)> >(m)),
	"wst_rag_wshd_color.ppm");*/

    return m;
  }


  template <typename I, typename J>
  pw::image<fun::i2v::array<mln_value(I)>, p_vertices<util::graph, fun::i2v::array<mln_site(I)> > >
  make_vertex_graph_image(const util::graph& g, const I&input, const J& w, const mln_value(J)& nbasins)
  {
    typedef util::array<mln_site(I)> vertex_sites_t;
    vertex_sites_t site_values;
    convert::from_to(labeling::compute(accu::center<mln_site(I)>(), w, nbasins),
				       site_values);

    typedef fun::i2v::array<mln_site(J)> f_sites_t;
    f_sites_t sites;
    convert::from_to(site_values, sites);

    // p_vertices
    typedef p_vertices<util::graph, f_sites_t> S;
    S pv(g, sites);


    typedef fun::i2v::array<mln_value(I)> vertex_values_t;
    vertex_values_t vertex_values;
    convert::from_to(mean_color_values(input, w, nbasins), vertex_values);

    mln_VAR(ima_v, (vertex_values | pv));

    return ima_v;
  }


  template <typename I, typename V>
  struct edge_to_color : Function_v2v< edge_to_color<I,V> >
  {
    typedef V result;

    edge_to_color(const I& ima)
      : ima_(ima)
    {
    }

    V
    operator()(const unsigned& e) const
    {
      return convert::to<V>(ima_.function()(e));
    }

    const I& ima_;
  };

  template <typename V>
  inline
  unsigned
  find_root(util::array<V>& parent, const unsigned& x)
  {
    if (parent[x] == x)
      return x;
    else
      return parent[x] = find_root(parent, parent[x]);
  }


  template <typename I, typename V, typename E>
  inline
  image3d<mln_value(I)>
  make_debug_graph_image(const I& input,
			 const V& ima_v, const E& ima_e,
			 unsigned box_size, const value::int_u12& bg)
  {
    image3d<mln_value(I)> ima;
    initialize(ima, input);

    data::fill(ima, bg);
    debug::draw_graph(ima, ima_v.domain(),
		      pw::cst(150u),
		      edge_to_color<E, mln_value(I)>(ima_e));

    dpoint3d tl(-box_size,-box_size, -box_size);
    dpoint3d br(box_size,box_size, -box_size);
    mln_piter(V) p(ima_v.domain());
    for_all(p)
    {
      box3d b(p + tl, p + br);
      b.crop_wrt(ima.domain());
      data::fill((ima | b).rw(), convert::to<mln_value(I)>(ima_v(p)));
    }

    return ima;
  }

}


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::rgb8;
  using value::label_16;
  using value::label_32;

  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dcm> <closure_lambda> <box_size> <dist_max>"
	      << std::endl;
    return 1;
  }

  unsigned closure_lambda = atoi(argv[2]);
  unsigned box_size = atoi(argv[3]);
  unsigned dist_max = atoi(argv[4]);

  image3d<int_u12> dcm;
  io::dicom::load(dcm, argv[1]);

  // Debug
  assert(mln_max(label_32) == 4294967295);
  io::dump::save(data::stretch(int_u8(), dcm), "wsd_01_src.dump");

  // Gradient
  image3d<int_u12> grad = morpho::elementary::gradient(dcm, c6());

  // Closure
  image3d<int_u12> clo = morpho::closing::volume(grad, c6(), closure_lambda);

  // Watershed
  label_32 nbasins;
  image3d<label_32> wshed = morpho::meyer_wst(clo, c6(), nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  // Debug
  io::dump::save(data::stretch(int_u8(), clo), "wsd_02.dump");
  io::dump::save(data::transform(wshed, fun::l2l::wrap<int_u8>()), "wsd_03.dump");

  //mln_VAR(vol2_, morpho::elementary::dilation(extend(wshed | (pw::value(wshed) == 0u), wshed), c26()));
  //data::fill((wshed | (pw::value(wshed) == 0u)).rw(), vol2_);

  io::dump::save(data::transform(wshed, fun::l2l::wrap<int_u8>()), "wsd_04.dump");

  /// Graph
  trace::quiet = false;
  util::graph g = make::region_adjacency_graph(wshed, c6(), nbasins);
  // Build graph images and compute distance values with a RGB image.
  /*mln_VAR(ima_v, make_vertex_graph_image(g, dcm, wshed, nbasins));
  mln_VAR(ima_e, make_edge_graph_image(ima_v, g));

  /// Try to merge vertices.
  mln_piter_(ima_e_t) e(ima_e.domain());
  util::array<label_32> parent(g.v_nmax());
  for (unsigned i = 0; i < parent.nelements(); ++i)
    parent[i] = i;

  for_all(e)
  {
    unsigned v1 = e.element().v1();
    unsigned v2 = e.element().v2();
    if (ima_e(e) <= dist_max && find_root(parent, v1) != find_root(parent, v2))
      parent[find_root(parent, v1)] = find_root(parent, v2);
  }

  fun::i2v::array<label_32> f(parent.nelements());
  std::vector<unsigned> new_label(parent.nelements(), 0);
  unsigned nbasins2 = 0;
  for (unsigned i = 0; i < parent.nelements(); ++i)
  {
    unsigned p = find_root(parent, i);
    mln_assertion(parent[p] == find_root(parent, i));
    if (new_label[p] == 0)
      new_label[p] = nbasins2++;
    f(i) = new_label[p];
  }
  mln_invariant(f(0) == 0u);
  --nbasins2; // nbasins2 does not count the basin with label 0.
  image3d<label_32> wsd2 = data::transform(wshed, f);

  io::dump::save(data::transform(wsd2, fun::l2l::wrap<int_u8>()), "wsd_05.dump");

  /// Reconstruct a graph from the simplified image.
  util::graph g2 = make::graph(wsd2, c6(), nbasins2);

  // Compute distance values with a RGB image.
  mln_VAR(ima_v2, make_vertex_graph_image(g2, dcm, wsd2, nbasins2));
  mln_VAR(ima_e2, make_edge_graph_image(ima_v2, g2));

  mln_VAR(wsd2_, morpho::elementary::dilation(extend(wsd2 | (pw::value(wsd2) == 0u), wsd2), c26()));

  data::fill((wsd2 | (pw::value(wsd2) == 0u)).rw(), wsd2_);

  io::dump::save(data::transform(labeling::mean_values(dcm, wsd2, nbasins2), fun::l2l::wrap<int_u8>()), "wsd_06_mean_colors.dump");
  //io::dump::save(data::stretch(int_u8(), make_debug_graph_image(dcm, ima_v2, ima_e2, box_size, 4095)), "wsd_07_graph_image2_white.dump");
  //io::dump::save(data::stretch(int_u8(), make_debug_graph_image(dcm, ima_v2, ima_e2, box_size, 0)), "wsd_08_graph_image2_black.dump");
  io::dump::save(data::transform(wsd2, fun::l2l::wrap<int_u8>()), "wsd_99_result.dump");*/
}
