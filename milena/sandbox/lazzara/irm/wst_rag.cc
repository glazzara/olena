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
#include <mln/canvas/morpho/algebraic_union_find.hh>
#include <mln/fun/v2v/id.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/labeling/mean_values.hh>

#include <mln/extract/all.hh>

namespace mln
{


  template <typename I, typename N>
  util::graph
  make_graph(const I& w, const N& nbh, const mln_value(I)& nbasins)
  {

    image2d<bool> adj(box2d(nbasins.next(), nbasins.next()));
    data::fill(adj, false);

    typedef mln_value(I) L;
    L l1, l2;
    mln_piter(I) p(w.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      if (w(p) != 0u)
	continue;
      // p is in the watershed line.
      l1 = l2 = 0;
      for_all(n)
	if (w.has(n) && w(n) != 0u)
	{
	  if (l1 == 0u) // First label to be stored.
	    l1 = w(n);
	  else
	    if (w(n) != l1) // Useless: && l2 == 0)
	    { // Second label to be stored.
	      mln_invariant(l2 == 0u);
	      l2 = w(n);
	      break;
	    }
	}
      if (l2 == 0u || l1 == 0u)
	continue;
      if (l2 < l1)
	std::swap(l1, l2);

      // adjacency l1 l2
      adj(point2d(l1,l2)) = true;
      adj(point2d(l2,l1)) = true;
    }

    // Construct graph.
    util::graph g;
    g.add_vertices(nbasins.next());
    for (unsigned i = 1; i < geom::nrows(adj); ++i)
      for (unsigned j = 1; j < i; ++j)
	if (adj(point2d(i,j)))
	  g.add_edge(i, j);

    return g;
  }




  template <typename V>
  value::int_u8 dist(const V& c1, const V& c2)
  {
    unsigned d = 0;
    d += (math::diff_abs(c1.red(), c2.red()) + 2) / 3;
    d += (math::diff_abs(c1.green(), c2.green()) + 2) / 3;
    d += (math::diff_abs(c1.blue(), c2.blue()) + 2) / 3;
    if (d > 255)
      d = 255;
    return d;
  }


  template <typename I>
  inline
  pw::image<fun::i2v::array<value::int_u8>, p_edges<util::graph, fun::i2v::array<mln_site(I)> > >
  make_edge_graph_image(const I& ima_v, const util::graph& g) // image on graph vertices; value = mean color per vertex (watershed basin)
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
  pw::image<fun::i2v::array<mln_value(I)>, p_vertices<util::graph, fun::i2v::array<mln_site(I)> > >
  make_vertex_graph_image(const util::graph& g, const I&input,
      const J& w, const mln_value(J)& nbasins)
  {
    typedef util::array<mln_site(I)> vertex_sites_t;
    vertex_sites_t site_values;
    convert::from_to(labeling::compute(accu::center<mln_site(I)>(), w, nbasins), site_values);

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
  make_debug_graph_image(const I& input, const V& ima_v, const E& ima_e, unsigned box_size)
  {
    image2d<mln_value(I)> ima;
    initialize(ima, input);

    data::fill(ima, literal::white);
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


int main(int, char *argv[])
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

  unsigned box_size = atoi(argv[3]);

  I input;
  io::ppm::load(input, argv[1]);
//  image2d<rgb16> input = level::convert(rgb16(), input_);

  J vol;
  io::dump::load(vol, argv[2]);

  label_16 nbasins = 898;
  std::cout << "nbasins = " << nbasins << std::endl;

  util::graph g = make_graph(vol, c4(), nbasins);

  mln_VAR(ima_v, make_vertex_graph_image(g, input, vol, nbasins));
  mln_VAR(ima_e, make_edge_graph_image(ima_v, g));

  //DEBUG
//  io::ppm::save(make_debug_graph_image(input, ima_v, ima_e, box_size), "wst_rag_graph_image.ppm");


  mln_piter_(ima_e_t) e(ima_e.domain());
  util::array<label_16> parent(g.v_nmax());
  for (unsigned i = 0; i < parent.nelements(); ++i)
    parent[i] = i;

  for_all(e)
  {
    unsigned v1 = e.element().v1();
    unsigned v2 = e.element().v2();
    if (ima_e(e) <= (unsigned)(atoi(argv[4]))
	&& find_root(parent, v1) != find_root(parent, v2))
      parent[find_root(parent, v1)] = find_root(parent, v2);
  }

  fun::i2v::array<label_16> f(parent.nelements());
  std::vector<unsigned> new_label(parent.nelements(), 0);
  unsigned nbasins2 = 0;
  for (unsigned i = 0; i < parent.nelements(); ++i)
  {
    unsigned p = find_root(parent, i);
    if (new_label[p] == 0)
      new_label[p] = nbasins2++;
    f(i) = new_label[p];
  }
  mln_invariant(f(0) == 0);
  --nbasins2; //nbasins2 does not count the basin with label 0.
  std::cout << "nbasins2 = " << nbasins2 << std::endl;

  J vol2 = level::transform(vol, f);
  util::graph g2 = make_graph(vol2, c4(), nbasins2);
  mln_VAR(ima_v2, make_vertex_graph_image(g2, input, vol2, nbasins2));
  mln_VAR(ima_e2, make_edge_graph_image(ima_v2, g2));

  J vol2_ = morpho::elementary::dilation(vol2, c4());
  io::ppm::save(labeling::mean_values(input, vol2_, nbasins2), "wst_rag_mean_colors.ppm");
  io::ppm::save(make_debug_graph_image(input, ima_v2, ima_e2, box_size), "wst_rag_graph_image2.ppm");
}
