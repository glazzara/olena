#include <mln/essential/2d.hh>

#include <mln/core/image/edge_image.hh>

#include <mln/core/alias/dpoint2d.hh>
#include <mln/core/alias/vec3d.hh>

#include <mln/accu/center.hh>
#include <mln/accu/compute.hh>

#include <mln/debug/draw_graph.hh>

#include <mln/draw/box.hh>

#include <mln/extract/all.hh>

#include <mln/fun/v2v/id.hh>

#include <mln/io/dump/all.hh>

#include <mln/labeling/mean_values.hh>

#include <mln/level/stretch.hh>

#include <mln/morpho/elementary/dilation.hh>

#include <mln/util/graph.hh>

#include <mln/value/label_16.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/hsl.hh>

#include <mln/make/region_adjacency_graph.hh>
#include <mln/make/edge_image.hh>
#include <mln/make/vertex_image.hh>
#include <mln/make/p_vertices_with_mass_centers.hh>


// Given a color image and a wshed image, computes the component graph.
// Vertex values are computed thanks to a HSL image.


namespace mln
{


  struct hsl2rgb : Function_v2v<hsl2rgb>
  {
    typedef value::rgb8 result;

    value::rgb8 operator()(const value::hsl_f& v) const
    {
       value::rgb8 res((v.hue() / 360.0f) * 255.0f,
			 v.sat() * 255.0f,
			 v.lum());
       return res;
    }
  };


  struct dist : Function_vv2v<dist>
  {
    typedef value::int_u8 result;

    value::int_u8 operator()(const value::rgb8& c1, const value::rgb8& c2) const
    {
      unsigned d = 0;
      d += (math::diff_abs(c1.red(), c2.red()) + 2) / 3;
      d += (math::diff_abs(c1.green(), c2.green()) + 2) / 3;
      d += (math::diff_abs(c1.blue(), c2.blue()) + 2) / 3;
      if (d > 255u)
	d = 255;
      return d;
    }

  };



  // ima_v, image on graph vertices; value = mean color per vertex (watershed basin)
  template <typename I>
  inline
  edge_image<mln_site(I),value::int_u8>
  make_edge_graph_image(const I& ima_v, const util::graph& g)
  {

    // edge sites.
    typedef fun::i2v::array<mln_site(I)> edge_site_t;
    edge_site_t edge_site(g.e_nmax(), literal::origin);

    // image on graph edges
    typedef edge_image<mln_site(I),value::int_u8> ima_e_t;
    ima_e_t ima_e = make::edge_image(ima_v, edge_site, dist());

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

    return m;
  }

  template <typename I, typename J>
  vertex_image<mln_site(I), mln_value(I)>
  make_vertex_graph_image(const util::graph& g, const I&input,
			  const J& w, const mln_value(J)& nbasins)
  {
    /// Sites
    typedef fun::i2v::array<mln_site(J)> vertex_sites_t;
    typedef p_vertices<util::graph, vertex_sites_t> S;
    S pv = make::p_vertices_with_mass_centers(w, nbasins, g);

    /// Values
    typedef fun::i2v::array<mln_value(I)> vertex_values_t;
    vertex_values_t vertex_values;
    convert::from_to(mean_color_values(input, w, nbasins), vertex_values);

    vertex_image<mln_site(I), mln_value(I)> ima_v(pv, vertex_values);

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
			 unsigned box_size)
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


int main(int argc, char *argv[])
{
  using namespace mln;

  using value::int_u16;
  using value::int_u8;
  using value::label_16;
  using value::label_8;
  using value::rgb16;
  using value::rgb8;
  using value::hsl_f;

//  typedef image2d<rgb8> I;
  typedef image2d<rgb8> K;
  typedef image2d<hsl_f> I;
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

  K input;
  io::ppm::load(input, argv[1]);
  I input_ = level::convert(hsl_f(), input);
  input = level::transform(input_, hsl2rgb());

  J vol;
  io::dump::load(vol, argv[2]);

  label_16 nbasins = atoi(argv[3]);
  std::cout << "nbasins = " << nbasins << std::endl;

  J vol2;
  unsigned nbasins2;
  fun::i2v::array<label_16> f;
  {
    util::graph g = make::region_adjacency_graph(vol, c4(), nbasins);

    mln_VAR(ima_v, make_vertex_graph_image(g, input, vol, nbasins));
    mln_VAR(ima_e, make_edge_graph_image(ima_v, g));

    //DEBUG
    io::ppm::save(make_debug_graph_image(input, ima_v, ima_e, box_size),
	"wst_rag_graph_image.ppm");


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

    f = fun::i2v::array<label_16>(parent.nelements());
    std::vector<unsigned> new_label(parent.nelements(), 0);
    nbasins2 = 0;
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
  }
  //  debug::println(vol);
  vol2 = level::transform(vol, f);
  //  debug::println(vol2);
  util::graph g2 = make::region_adjacency_graph(vol2, c4(), nbasins2);

  // Compute values distance on the HSL Image.
  mln_VAR(ima_v2, make_vertex_graph_image(g2, input, vol2, nbasins2));
  mln_VAR(ima_e2, make_edge_graph_image(ima_v2, g2));

  mln_VAR(vol2_,
      morpho::elementary::dilation(extend(vol2 | (pw::value(vol2) == 0u),
	  vol2),
	c8()));

  data::fill((vol2 | (pw::value(vol2) == 0u)).rw(), vol2_);

  io::ppm::save(labeling::mean_values(input, vol2, nbasins2),
      "wst_rag_mean_colors.ppm");
  io::ppm::save(make_debug_graph_image(input, ima_v2, ima_e2, box_size),
      "wst_rag_graph_image2.ppm");

}
