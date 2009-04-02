#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/box.hh>
#include <mln/data/fill.hh>
#include <mln/core/alias/dpoint2d.hh>
#include <mln/core/macros.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_edges.hh>
#include <mln/core/site_set/p_vertices.hh>
#include <mln/io/essential.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/make/rag_and_labeled_wsl.hh>
#include <mln/util/graph.hh>
#include <mln/util/couple.hh>
#include <mln/debug/colorize.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/center.hh>
#include <mln/accu/median_h.hh>
#include <mln/accu/mean.hh>
#include <mln/pw/cst.hh>
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

} // end of namespace mln



int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace value;

  if (argc < 2)
  {
    std::cout << argv[0] << " <input.pgm> " << std::endl;
    return 1;
  }

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  image2d<int_u8> grad = morpho::elementary::gradient_internal(input, c4());
  io::pgm::save(grad, "binarization_grad.pgm");

  image2d<int_u8> clo_vol = morpho::closing::volume(grad, c4(), 500);
  io::pgm::save(clo_vol, "binarization_clo.pgm");

  label_16 nbasins;
  image2d<label_16> wst = morpho::watershed::flooding(clo_vol, c4(), nbasins);
  io::ppm::save(debug::colorize(rgb8(), wst, nbasins), "binarization_wst.ppm");


  util::couple<util::graph, image2d<label_16> >
    rag_data = make::rag_and_labeled_wsl(wst, c4(), nbasins);

  io::pgm::save(rag_data.second(), "binarization_lwsl.pgm");
  io::ppm::save(debug::colorize(rgb8(), rag_data.second(), nbasins), "binarization_lwsl.ppm");

  p_vertices<util::graph, fun::i2v::array<point2d> >
    pv = make::common_pvertices(wst, nbasins, rag_data.first());

  pw::image<fun::i2v::array<int_u8>,
	    p_vertices<util::graph, fun::i2v::array<point2d> > >
    ima_v = make::vertex_image(accu::mean<int_u8>(), pv, input, wst, nbasins);

  pw::image<fun::i2v::array<int_u8>,
	    p_edges<util::graph,pw::cst_<literal::origin_t> > >
    ima_e = make::edge_image(ima_v,
			     make::dummy_pedges(rag_data.first(),literal::origin),
			     edge_values());


  io::pgm::save(make::debug_graph_image(input,ima_v, ima_e, 4, 0), "binarization_graph.ppm");

  // FIXME: we should use the following line instead. We cannot since currently
  // graph edges are numbered from 0 and we must use mln_max() - 1 as
  // background id.
  //
  //label_16 nelts = rag.first().e_nmax();
  label_16 nelts = mln_max(label_16).prev();

  util::array<int_u8> median_grad = labeling::compute(accu::meta::median_h(),
						      grad,
						      rag_data.second(),
						      nelts);
  fun::i2v::array<int_u8> f;
  convert::from_to(median_grad, f);

  image2d<int_u8> med = level::transform(rag_data.second(), f);

  io::pgm::save(med, "binarization_median_grad.pgm");
}
