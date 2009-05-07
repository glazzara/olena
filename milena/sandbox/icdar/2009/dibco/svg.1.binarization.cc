#include <mln/core/var.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_queue.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/image_if.hh>

#include <mln/data/fill.hh>

#include <mln/core/image/edge_image.hh>
#include <mln/core/image/vertex_image.hh>
#include <mln/core/site_set/p_edges.hh>
#include <mln/core/site_set/p_vertices.hh>

#include <mln/io/essential.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/histo/compute.hh>

#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/closing/height.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/make/rag_and_labeled_wsl.hh>
#include <mln/util/graph.hh>
#include <mln/util/couple.hh>
#include <mln/labeling/colorize.hh>
#include <mln/fun/i2v/array.hh>

#include <mln/labeling/compute.hh>
#include <mln/labeling/regional_minima.hh>

# include <mln/linear/gaussian_1d.hh>

#include <mln/accu/center.hh>
#include <mln/accu/median_h.hh>
#include <mln/accu/mean.hh>

#include <mln/math/abs.hh>
#include <mln/literal/colors.hh>
#include <mln/debug/draw_graph.hh>





namespace mln
{

  struct edge_values : Function_vv2v<edge_values>
  {
    typedef value::int_u8 result;
    typedef void mutable_result;

    value::int_u8 operator()(const value::int_u8& v1,
			     const value::int_u8& v2) const
    { return math::abs(v1 - v2); }

  };


  template <typename I, typename V>
  class edge_to_color : public Function_p2v< edge_to_color<I,V> >
  {
  public:
    typedef V result;

    edge_to_color(const I& ima_e) : ima_e_(ima_e)
    {}

    V
    operator()(const unsigned& e) const
    { return convert::to<V>(ima_e_.function()(e)); }

  private:
    const I& ima_e_;
  };


  namespace make
  {

    template <typename G, typename P>
    p_edges<G,pw::cst_<P> >
    dummy_pedges(const Graph<G>& g_, const P& dummy_value)
    {
      trace::entering("dummy_pedges");

      const G& g = exact(g_);
      mln_precondition(g.is_valid());
      p_edges<G,pw::cst_<P> > pe(g, pw::cst(dummy_value));

      trace::exiting("dummy_pedges");
      return pe;
    }

    template <typename FVV, typename G, typename V2P, typename E2P, typename FVE>
    edge_image<mln_result(FVE),mln_result(E2P)>
    edge_image(const pw::image<FVV,p_vertices<G,V2P> >& vertex_image,
	       const p_edges<G,E2P>& pe,
	       const Function_vv2v<FVE>& edge_value_)
    {
      trace::entering("edge_image");

      const FVE& edge_value = exact(edge_value_);
      mln_precondition(vertex_image.is_valid());
      mln_precondition(pe.is_valid());

      typedef fun::i2v::array<mln_result(FVE)> edge_values_t;
      edge_values_t edge_values(pe.nsites());

      // image on graph edges
      typedef mln::edge_image<mln_result(FVE),mln_result(E2P)> ima_e_t;
      ima_e_t ima_e(pe, edge_values);

      mln_piter(ima_e_t) e(ima_e.domain());
      for_all(e)
	ima_e(e) = edge_value(vertex_image.function()(e.element().v1()),
			      vertex_image.function()(e.element().v2()));

      trace::exiting("edge_image");
      return ima_e;
    }


    template <typename W, typename G>
    inline
    p_vertices<util::graph, fun::i2v::array<mln_site(W)> >
    common_pvertices(const Image<W>& wst, const mln_value(W)& nbasins,
		     const Graph<G>& g_)
    {
      trace::entering("common_pvertices");

      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      typedef fun::i2v::array<mln_site(W)> vertex_sites_t;
      vertex_sites_t vertex_sites;
      convert::from_to(labeling::compute(accu::center<mln_site(W)>(), wst, nbasins),
	  vertex_sites);

      p_vertices<util::graph, vertex_sites_t> pv(g, vertex_sites);

      trace::exiting("common_pvertices");
      return pv;
    }


    template <typename A, typename G, typename F, typename I, typename W>
    pw::image<fun::i2v::array<mln_value(I)>, p_vertices<G,F> >
    vertex_image(const Accumulator<A>& accu,
		 const p_vertices<G,F>& pv,
		 const Image<I>& input_,
		 const Image<W>& wst_,
		 const mln_value(W)& nbasins)
    {
      trace::entering("vertex_image");

      const I& input = exact(input_);
      const W& wst = exact(wst_);
      mln_precondition(input.is_valid());
      mln_precondition(wst.is_valid());
      mln_precondition(pv.is_valid());

      typedef fun::i2v::array<mln_value(I)> vertex_values_t;
      vertex_values_t vertex_values;
      convert::from_to(labeling::compute(accu, input, wst, nbasins),
		       vertex_values);

      pw::image<fun::i2v::array<mln_value(I)>,
	p_vertices<util::graph, fun::i2v::array<mln_site(I)> > >
	  ima_vertex = vertex_values | pv;

      trace::exiting("vertex_image");
      return ima_vertex;
    }


    template <typename I, typename V, typename E>
    inline
    mln_concrete(I)
    debug_graph_image(const Image<I>& input_,
		      const Image<V>& ima_v_, const Image<E>& ima_e_,
		      unsigned box_size, const mln_value(I)& bg)
    {
      trace::entering("debug_graph_image");

      const I& input = exact(input_);
      const V& ima_v = exact(ima_v_);
      const E& ima_e = exact(ima_e_);

      mln_precondition(input.is_valid());
      mln_precondition(ima_v.is_valid());
      mln_precondition(ima_e.is_valid());

      mln_concrete(I) output;
      initialize(output, input);

      data::fill(output, bg);
      debug::draw_graph(output, ima_v.domain(),
	  pw::cst(150u),
	  edge_to_color<E, mln_value(I)>(ima_e));

      dpoint2d tl(-box_size,-box_size);
      dpoint2d br(box_size,box_size);
      mln_piter(V) p(ima_v.domain());
      for_all(p)
      {
	box<mln_site(I)> b(p + tl, p + br);
	b.crop_wrt(output.domain());
	data::fill((output | b).rw(), convert::to<mln_value(I)>(ima_v(p)));
      }

      trace::exiting("debug_graph_image");
      return output;
    }

  } // end of namespace mln::make

  int
  find_threshold(const histo::array<value::int_u8>& h)
  {

    const float sigma = 5; // FIXME: hard-coded!
  
    util::array<point1d> c;
    value::label_8 n;

    {
      image1d<unsigned> h_, hs_;
      image1d<value::label_8> l;

      convert::from_to(h, h_);
      hs_ = linear::gaussian_1d(h_, sigma, 0);
      l = labeling::regional_minima(hs_, c2(), n);
     
      {
// 	debug::println("l", l);
// 	debug::histo(h, "tmp_h.txt");
	
// 	std::ofstream file("tmp_hs.txt");
// 	mln_piter_(box1d) p(h_.domain());
// 	for_all(p)
// 	  file << p.ind() << ' ' << hs_(p) << std::endl;
// 	file.close();
      }

      accu::center<point1d, point1d> a;
      c = labeling::compute(a, l, n);

      c[0] = point1d(0); // Force a neutral value for the non-label value (0).

      // std::cout << "c = " << c << std::endl;
    }


    int threshold;

    {
      std::vector<int> v;
      for (unsigned i = 0; i <= n; ++i)
	v.push_back(c[i].ind());
      std::sort(v.begin(), v.end());

      for (unsigned i = 0; i <= n; ++i)
	if (v[i] != 0)
	  {
	    threshold = v[i];
	    break;
	  }

      std::cout << "threshold = " << threshold << std::endl;
    }

    return threshold;
  }


  image2d<bool> contours(const image2d<value::int_u8>& f, unsigned& threshold)
  {
    using value::int_u8;
    typedef int_u8 V;
    histo::array<V> h;
    
    image2d<int_u8> g;
    g = morpho::elementary::gradient_internal(f, c4());
    g = morpho::closing::height(g, c4(), 3);  // FIXME: hard-coded!
    
    unsigned nbasins;
    image2d<unsigned> w = morpho::watershed::flooding(g, c4(), nbasins);

    h = histo::compute(g | (pw::value(w) == pw::cst(0)));
    
    threshold = find_threshold(h);

    return duplicate((pw::value(g) > pw::cst(threshold)) | f.domain());
  }

} // end of namespace mln



int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace value;

  if (argc != 4)
  {
    std::cout << argv[0] << " input.pgm lambda_grad lambda_graph" << std::endl;
    return 1;
  }

  int lambda_grad  = std::atoi(argv[2]);
  int lambda_graph = std::atoi(argv[3]);


  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  image2d<int_u8> grad = morpho::elementary::gradient_internal(input, c4());
//   io::pgm::save(grad, "binarization_grad.pgm");

  image2d<int_u8> clo_vol = morpho::closing::volume(grad, c4(), lambda_grad);
//   io::pgm::save(clo_vol, "binarization_clo.pgm");

  label_16 nbasins;
  image2d<label_16> wst = morpho::watershed::flooding(clo_vol, c4(), nbasins);
//   io::ppm::save(labeling::colorize(rgb8(), wst, nbasins), "binarization_wst.ppm");


  util::couple<util::graph, image2d<label_16> >
    rag_data = make::rag_and_labeled_wsl(wst, c4(), nbasins);

//   io::pgm::save(rag_data.second(), "binarization_lwsl.pgm");


  unsigned threshold;
  contours(input, threshold);


  typedef accu::median_h<int_u8> A;
  typedef fun::i2v::array<point2d> F;


  util::array<int_u8> basin_med = labeling::compute(A(), input, wst, nbasins);
  basin_med[0] = 0;

  {
    util::graph& gr = rag_data.first();

    fun::i2v::array<int_u8> f_med;
    convert::from_to(basin_med, f_med);

    // io::pgm::save(level::transform(wst, f_med), "basin_med.pgm");

    p_vertices<util::graph, fun::i2v::array<point2d> >
      pv = make::common_pvertices(wst, nbasins, rag_data.first());

    typedef vertex_image<point2d, int_u8> med_t;
    med_t med(pv, f_med);

    int_u8
      object     = 255,
      background = 0,
      unknown    = 128;

    med_t out;
    initialize(out, med);
    data::fill(out, unknown);

    typedef med_t::nbh_t N;
    N nbh;

    // Initialization.

    mln_piter_(med_t) p(med.domain());
    mln_niter_(N) n(nbh, p);
    for_all(p)
      for_all(n)
      {
	int d = math::diff_abs(med(p), med(n));
// 	if (2 * d > 3 * threshold)
	if (d > threshold)
	  {
	    if (med(p) < med(n))
	      {
		out(p) = object;
		out(n) = background;
	      }
	    else
	      {
		out(n) = object;
		out(p) = background;
	      }
	    break;
	  }
      }
    
    io::pgm::save(level::transform(wst, out.function()),
		  "temp_out.pgm");

    // Enqueuing.

    typedef mln_psite_(med_t) P;
    p_queue<P> q;

    for_all(p)
    {
      if (out(p) != unknown)
	continue;
      for_all(n)
	if (out(n) != unknown)
	  {
	    q.insert(p);
	    break;
	  }
    }

    // Propagation.

    P p_;
    mln_niter_(N) n_(nbh, p_);
    while (! q.is_empty())
      {
	p_ = q.pop_front();

	if (out(p_) != unknown)
	  continue; // std::cerr << "oops! (1)" << std::endl;

	unsigned d = 256, id;
	for_all(n_)
	{
	  if (out(n_) == unknown)
	    {
	      q.insert(n_);
	      continue;
	    }
	  unsigned d_ = math::diff_abs(med(p_), med(n_));
	  if (d_ < d)
	    {
	      d = d_;
	      id = out(n_);
	    }
	}
	if (d == 256)
	  std::cerr << "oops! (2)" << std::endl;

	out(p_) = id; // object or background.
      }

    io::pgm::save(level::transform(wst, out.function()),
		  "temp_out_2.pgm");

  }

}
