#include <mln/core/var.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/site_set/p_queue.hh>

#include <mln/core/routine/extend.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/data/fill.hh>

#include <mln/core/site_set/p_edges.hh>
#include <mln/core/site_set/p_vertices.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>

#include <mln/io/essential.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/histo/compute.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/closing/height.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/make/rag_and_labeled_wsl.hh>
#include <mln/util/graph.hh>
#include <mln/util/couple.hh>
#include <mln/debug/colorize.hh>
#include <mln/fun/i2v/array.hh>

#include <mln/labeling/compute.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/wrap.hh>

#include <mln/level/transform.hh>
#include <mln/level/convert.hh>

#include <mln/linear/gaussian_1d.hh>

#include <mln/accu/center.hh>
#include <mln/accu/median_h.hh>
#include <mln/accu/mean.hh>

#include <mln/math/abs.hh>
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
    pw::image<fun::i2v::array<mln_result(FVE)>,p_edges<G,E2P> >
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
      typedef pw::image<edge_values_t, p_edges<G,E2P> > ima_e_t;
      ima_e_t ima_e = (edge_values | pe);

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


  // Distances.

  value::int_u8 dist(const value::int_u8& g1, const value::int_u8& g2)
  {
    return math::diff_abs(g1, g2);
  }

  value::int_u8 dist(const value::rgb8& c1, const value::rgb8& c2)
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


  // Functions.

  inline
  bool is_row_odd(const point2d& p)
  {
    return p.row() % 2;
  }

  inline
  bool is_square(const point2d& p)
  {
    return p.row() % 2 == 0 && p.col() % 2 == 0;
  }

  inline
  bool is_edge(const point2d& p)
  {
    return p.row() % 2 + p.col() % 2 == 1;
  }

  inline
  bool is_point(const point2d& p)
  {
    return p.row() % 2 && p.col() % 2;
  }


  // Neighborhoods.

  typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;
  
  const dbl_neighb2d& e2c()
  {
    static bool e2c_h[] = { 0, 1, 0,
			    0, 0, 0,
			    0, 1, 0 };
    static bool e2c_v[] = { 0, 0, 0,
			    1, 0, 1,
			    0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2c_h, e2c_v);
    return nbh;
  }

  const dbl_neighb2d& e2e()
  {
    static bool e2e_h[] = { 0, 0, 1, 0, 0,
			    0, 1, 0, 1, 0,
			    0, 0, 0, 0, 0,
			    0, 1, 0, 1, 0,
			    0, 0, 1, 0, 0 };
    static bool e2e_v[] = { 0, 0, 0, 0, 0,
			    0, 1, 0, 1, 0,
			    1, 0, 0, 0, 1,
			    0, 1, 0, 1, 0,
			    0, 0, 0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2e_h, e2e_v);
    return nbh;
  }


  // Transforms.

  template <typename T>
  image2d<T>
  image2full(const image2d<T>& input)
  {
    image2d<T> output(2 * input.nrows() - 1,
		      2 * input.ncols() - 1);
    for (int row = 0; row < input.nrows(); ++row)
      for (int col = 0; col < input.ncols(); ++col)
	opt::at(output, 2 * row, 2 * col) = opt::at(input, row, col);
    return output;
  }
  
  template <typename T>
  image2d<T>
  full2image(const image2d<T>& input)
  {
    image2d<T> output((input.nrows() + 1) / 2,
		      (input.ncols() + 1) / 2);
    for (int row = 0; row < input.nrows(); row += 2)
      for (int col = 0; col < input.ncols(); col += 2)
	opt::at(output, row / 2, col / 2) =
	  opt::at(input, row, col);
    return output;
  }


  // Display.

  template <typename I>
  I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);
    data::fill(output, bg);
    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row + i, col) = ima(p);
	}
    return output;
  }



  // Distance, stored on edges, of a couple of colors.

  template <typename I, typename N>
  image2d<value::int_u8>
  dist_on_edges(const I& input, const N& nbh)
  {
    image2d<value::int_u8> output;
    initialize(output, input);
    data::fill(output, 0);

    mln_piter(I) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      n.start();
      mln_value(I) c1 = input(n);
      n.next();
      mln_value(I) c2 = input(n);
      output(p) = dist(c1, c2);
    }

    // io::pgm::save(output, "temp_dist.pgm");

    return output;
  }


  value::int_u8 L_to_int_u8(unsigned l)
  {
    return l == 0 ?
      0 : // wshed line
      1 + (l - 1) % 255; // basin
  }



  // Quasi-Regional Minima Labeling.

  template <typename I>
  struct quasi_regional_minima_functor
  {
    typedef mln_psite(I) P;

    const I& input;
    unsigned threshold;

    void init()
    {
      data::fill(attr, true);
    }

    bool handles(const P&) const  { return true; }
    bool labels(const P& p) const { return attr(p); }

    bool equiv(const P& n, const P& p) const
    {
      if (attr(p) == false)
	return false;

      mln_invariant(input(n) <= input(p));
      if (input(n) > input(p))
	std::cout << "oups!" << std::endl;

      std::cout << "UNION:  n=" << n << " [" << input(n)
		<< "]  p=" << p << " [" << input(p)
		<< "]    equiv=" << (input(p) < input(n) + threshold)
		<< "         " << threshold
		<< std::endl;

//       return input(p) == input(n);
      return input(p) < input(n) + threshold;
    }

    void do_no_union(const P& n, const P& p)
    {
      if (input(n) > input(p))
	std::cout << "oups!" << std::endl;

      if (input(p) < input(n) + threshold)
	std::cout << "oups!" << std::endl;

      mln_invariant(input(p) >= input(n) + threshold);
      (void)n;
      attr(p) = false;

      std::cout << "INVALID:  n=" << n << " (" << input(n)
		<< ") p=" << p << " (" << input(p)
		<< std::endl;

    }
    
    void init_attr(const P&)
    {}

    void merge_attr(const P& r, const P& p)
    {
      attr(p) = attr(p) && attr(r);
    }
    
    mln_ch_value(I, bool) attr;
    
    quasi_regional_minima_functor(const I& input, unsigned threshold)
      : input(input),
	threshold(threshold)
    {
      initialize(attr, input);
    }
  };


  template <typename I, typename N, typename L>
  mln_ch_value(I, L)
    labeling__quasi_regional_minima(const Image<I>& input_, const Neighborhood<N>& nbh_,
				    unsigned threshold,
				    L& nlabels)
  {
    trace::entering("labeling__quasi_regional_minima");
    
    const I& input = exact(input_);
    const N& nbh = exact(nbh_);
    mln_precondition(input.is_valid());
    
    // FIXME: abort if L is not wide enough to encode the set of
    // minima.
    
    typedef quasi_regional_minima_functor<I> F;
    F f(exact(input), threshold);
    mln_ch_value(I, L) output = canvas::labeling_sorted(input, nbh, nlabels,
							f, false);

    trace::exiting("labeling__quasi_regional_minima");
    return output;
  }


} // end of namespace mln



int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace value;

  // trace::quiet = false;

  if (argc != 5)
    {
      std::cout << argv[0] << " input.pgm lambda echo output.pbm" << std::endl;
      return 1;
    }

  int lambda_grad  = std::atoi(argv[2]);
  int echo = std::atoi(argv[3]);


  if (echo != 0 && echo != 1)
    {
      std::cout << argv[0] << " input.pgm lambda echo output.pbm" << std::endl;
      return 1;
    }


  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  image2d<int_u8> input_ = image2full(input); // Larger to make room to edges.

  image2d<int_u8> grad_;
  {
    grad_ = dist_on_edges(extend(input_ | is_edge, pw::value(input_)),
			  e2c());
  }

  // io::pgm::save(grad_, "grad_.pgm");

  mln_VAR(grad, grad_ | is_edge);
  typedef grad_t E;

  grad = morpho::closing::volume(grad,
				 e2e(),
				 lambda_grad);

  // io::pgm::save(grad.unmorph_(), "grad.pgm");

  typedef label_16 L; // Label type.

  L nbasins;
  
  typedef mln_ch_value_(E, L) EL;

  EL wst = morpho::watershed::flooding(grad,
				       e2e(),
				       nbasins);

  std::cout << "n basins = " << nbasins << std::endl;


  if (echo)
    {
      io::ppm::save(display_edge(debug::colorize(rgb8(),
						 wst,
						 nbasins).unmorph_(),
				 literal::black,
				 3),
		    "temp_wst_edges.ppm");
    }


  image2d<L> w_all = wst.unmorph_();

  // edges -> squares
  mln_VAR(w_squares, w_all | is_square);
  data::paste(morpho::dilation(extend(w_squares, pw::value(w_all)),
			       c4().win()),
	      w_all);
  // edges -> points
  mln_VAR(w_points, w_all | is_point);
  data::paste(morpho::erosion(extend(w_points, pw::value(w_all)),
			      c4().win()),
	      w_all);

  image2d<L> ws = full2image(w_all);

  if (echo)
    {
      io::ppm::save(debug::colorize(rgb8(), ws, nbasins),
		    "temp_ws.ppm");
    }

  util::couple<util::graph, image2d<L> >
    rag_data = make::rag_and_labeled_wsl(w_all, c8(), nbasins);

  if (echo)
    io::pgm::save(labeling::wrap(int_u8(),
				 rag_data.second()),
		  "temp_lwsl.pgm");


  unsigned threshold;
  contours(input, threshold);


  typedef accu::median_h<int_u8> A;
  typedef fun::i2v::array<point2d> F;


  util::array<int_u8> basin_med = labeling::compute(A(), input, ws, nbasins);
  basin_med[0] = 0;



  if (echo)
    io::ppm::save(debug::colorize(rgb8(), w_all, nbasins),
		  "temp_w_all.ppm");



  // -----------------------------    R A G  --------------------------------


  util::graph& gr = rag_data.first();

  fun::i2v::array<int_u8> f_med;
  convert::from_to(basin_med, f_med);

  if (echo)
    {
      io::pgm::save(level::transform(w_all, f_med),
		    "temp_w_all.pgm");
      io::pgm::save(level::transform(ws, f_med), "temp_basin_med.pgm");
    }

  p_vertices<util::graph, fun::i2v::array<point2d> >
    pv = make::common_pvertices(ws, nbasins, rag_data.first());

  mln_VAR( med, f_med | pv );


  typedef graph_elt_neighborhood<util::graph, F> N;
  N nbh;

  threshold = 25; // FIXME

  L n_objects;
  mln_VAR( lab, labeling__quasi_regional_minima(med, nbh, threshold, n_objects) );


  io::pbm::save((pw::value(level::transform(ws,
					    lab.function())) != pw::cst(0))
		| input.domain(),
		argv[4]);


  /*

  int_u8
    object     = 255,
    background = 0,
    unknown    = 128;

  med_t out;
  initialize(out, med);
  data::fill(out, unknown);

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
    
  if (echo)
    {
      io::pgm::save(level::transform(ws, out.function()),
		    "temp_out.pgm");
    }


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

  if (echo)
    {
      io::pgm::save(level::transform(ws, out.function()),
		    "temp_out_2.pgm");
    }

  io::pbm::save((pw::value(level::transform(ws,
					    out.function())) == pw::cst(255))
		| input.domain(),
		argv[4]);

  */

}
