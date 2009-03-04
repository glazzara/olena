#include <mln/essential/2d.hh>
#include <mln/core/alias/vec3d.hh>
#include <mln/debug/draw_graph.hh>
#include <mln/util/graph.hh>
#include <mln/accu/center.hh>
#include <mln/io/dump/all.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb16.hh>
#include <mln/accu/compute.hh>
#include <mln/core/alias/dpoint2d.hh>
#include <mln/draw/box.hh>
#include <mln/level/stretch.hh>
#include <mln/fun/v2v/id.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/extension/adjust_fill.hh>
#include <mln/extract/all.hh>
#include <mln/make/region_adjacency_graph.hh>
#include <mln/make/graph.hh>

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
      ima_e(e) = dist(ima_v.function()(e.element().v1()),
			    ima_v.function()(e.element().v2()));

    return ima_e;
  }


  template <typename I, typename J>
  inline
  util::array<mln_value(I)>
  mean_color_values(const I& input, const J& w, mln_value(J) nbasins)
  {
    // Cf. sandbox/theo/color/segment_rgb_pixels.cc

    util::array<vec3d_f> m_3f = labeling::compute(accu::mean<mln_value(I)>(),
						  input, // input color image
						  w, // watershed labeling
						  nbasins);
    m_3f[0] = literal::zero;

    util::array<mln_value(I)> m;
    convert::from_to(m_3f, m);
    m[0] = literal::yellow;

    io::ppm::save(level::transform(w,
	  convert::to< fun::i2v::array<mln_value(I)> >(m)),
	"wst_rag_wshd_color.ppm");

    return m;
  }


  template <typename I, typename J>
  pw::image<fun::i2v::array<mln_value(I)>,
	    p_vertices<util::graph, fun::i2v::array<mln_site(I)> > >
  make_vertex_graph_image(const util::graph& g, const I&input,
      const J& w, const mln_value(J)& nbasins)
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
    convert::from_to(mean_color_values(input, w, nbasins),
		     vertex_values);

    mln_VAR(ima_v, (vertex_values | pv));

    return ima_v;
  }


  template <typename I, typename V>
  struct edge_to_color : Function_p2v< edge_to_color<I,V> >
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
  image2d<mln_value(I)>
  make_debug_graph_image(const I& input,
			 const V& ima_v, const E& ima_e,
			 unsigned box_size, const value::rgb8& bg)
  {
    image2d<mln_value(I)> ima;
    initialize(ima, input);

    data::fill(ima, bg);
    debug::draw_graph(ima, ima_v.domain(),
		      pw::cst(mln_value(I)(literal::green)),
		      edge_to_color<E, mln_value(I)>(ima_e));

    dpoint2d tl(-box_size,-box_size);
    dpoint2d br(box_size,box_size);
    mln_piter(V) p(ima_v.domain());
    for_all(p)
    {
      box2d b(p + tl, p + br);
      b.crop_wrt(ima.domain());
      data::fill((ima | b).rw(), convert::to<mln_value(I)>(ima_v(p)));
    }

    return ima;
  }

}


int main(int argc, char *argv[])
{
  using namespace mln;

  using value::int_u16;
  using value::int_u8;
  using value::label_16;
  using value::label_8;
  using value::rgb16;
  using value::rgb8;

  typedef image2d<rgb8> I;
  typedef image2d<label_16> J;

  if (argc < 6)
  {
    std::cout << argv[0] << " <input.ppm> <wsh-label16.dump> <nbasins> <box_size> <dist_max>"
			 << std::endl << std::endl;

    std::cout << "box_size: size of the component mean color box." << std::endl;
    std::cout << "dist_max: merge two vertices if the associted edge value <= dist_max"
	      << std::endl;

    return 1;
  }

  unsigned box_size = atoi(argv[4]);

  I input;
  io::ppm::load(input, argv[1]);

  J wsd;
  io::dump::load(wsd, argv[2]);

  label_16 nbasins = atoi(argv[3]);
  std::cout << "nbasins = " << nbasins << std::endl;

  /// Build graph
  util::graph g = make::graph(wsd, c4(), nbasins);
  // Build graph images and compute distance values with a RGB image.
  mln_VAR(ima_v, make_vertex_graph_image(g, input, wsd, nbasins));
  mln_VAR(ima_e, make_edge_graph_image(ima_v, g));


  //DEBUG
  io::ppm::save(make_debug_graph_image(input, ima_v, ima_e, box_size, literal::white),
				       "wst_rag_graph_image_white.ppm");
  io::ppm::save(make_debug_graph_image(input, ima_v, ima_e, box_size, literal::black),
				       "wst_rag_graph_image_black.ppm");


  /// Try to merge vertices.
  mln_piter_(ima_e_t) e(ima_e.domain());
  util::array<label_16> parent(g.v_nmax());
  for (unsigned i = 0; i < parent.nelements(); ++i)
    parent[i] = i;

  for_all(e)
  {
    unsigned v1 = e.element().v1();
    unsigned v2 = e.element().v2();
    if (ima_e(e) <= (unsigned)(atoi(argv[5]))
	&& find_root(parent, v1) != find_root(parent, v2))
      parent[find_root(parent, v1)] = find_root(parent, v2);
  }

  fun::i2v::array<label_16> f(parent.nelements());
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
  --nbasins2; //nbasins2 does not count the basin with label 0.
  std::cout << "nbasins2 = " << nbasins2 << std::endl;
  J wsd2 = level::transform(wsd, f);


  /// Reconstruct a graph from the simplified image.
  util::graph g2 = make::graph(wsd2, c4(), nbasins2);
  // Compute distance values with a RGB image.
  mln_VAR(ima_v2, make_vertex_graph_image(g2, input, wsd2, nbasins2));
  mln_VAR(ima_e2, make_edge_graph_image(ima_v2, g2));

  mln_VAR(wsd2_,
      morpho::elementary::dilation(extend(wsd2 | (pw::value(wsd2) == 0u),
					  wsd2),
				    c8()));

  data::fill((wsd2 | (pw::value(wsd2) == 0u)).rw(), wsd2_);

  io::ppm::save(labeling::mean_values(input, wsd2, nbasins2),
		"wst_rag_mean_colors.ppm");
  io::ppm::save(make_debug_graph_image(input, ima_v2, ima_e2, box_size, literal::white),
		"wst_rag_graph_image2_white.ppm");
  io::ppm::save(make_debug_graph_image(input, ima_v2, ima_e2, box_size, literal::black),
		"wst_rag_graph_image2_black.ppm");
}
