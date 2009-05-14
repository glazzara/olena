# include <iostream>
# include <cstdlib> // rand
# include <numeric>

# include <mln/io/ppm/save.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/label_8.hh>
# include <mln/value/label_16.hh>

# include <mln/morpho/watershed/flooding.hh>
# include <mln/morpho/watershed/superpose.hh>
# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/closing/area.hh>
# include <mln/morpho/opening/area.hh>
# include <mln/morpho/elementary/gradient.hh>
# include <mln/level/transform.hh>

# include <mln/make/region_adjacency_graph.hh>
# include <mln/util/graph.hh>

# include <mln/core/site_set/p_vertices.hh>
# include <mln/core/var.hh>
# include <mln/fun/i2v/array.hh>
# include <mln/make/p_vertices_with_mass_centers.hh>
# include <mln/literal/grays.hh>
# include <mln/debug/draw_graph.hh>
# include <mln/draw/line.hh>

# include <mln/core/concept/function.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <mln/norm/l2.hh>
# include <mln/logical/not.hh>

# include "center_weight.hh"
# include "p_vertices_with_accu.hh"

# include <mln/core/image/extended.hh>

# include <mln/canvas/browsing/depth_first_search.hh>

# include "dijkstra.hh"

namespace mln
{
  using value::int_u8;
  using value::label_16;

  typedef double distance_t;
  typedef double angle_t;
  typedef util::couple<distance_t, angle_t> edge_value_t;

  template <typename T>
  distance_t distance(T a, T b)
  {
    return (a[0] - b[0]) * (a[0] - b[0]) +
      (a[1] - b[1]) * (a[1] - b[1]);
  }

  template <typename G, typename VV>
  inline
  p_edges<G, fun::i2v::array<edge_value_t> >
  p_edges_for_lines(const Graph<G>& g_,
                    const VV& vertices_values)
  {
    trace::entering("make::p_edges_for_lines");

    // const W& wst = exact(wst_);
    const G& g = exact(g_);
    mln_precondition(g.is_valid());

    typedef fun::i2v::array<edge_value_t> edges_value_t;
    edges_value_t edge_values(g.e_nmax());

    mln_edge_iter(G) e(g);
    for_all(e)
    {
      distance_t d = distance(vertices_values(e.v1()).first(), vertices_values(e.v2()).first());
      distance_t y = std::abs(vertices_values(e.v1()).first()[0] -
                              vertices_values(e.v2()).first()[0]); // We lose genericity.
      distance_t x = (vertices_values(e.v1()).first()[1] -
                      vertices_values(e.v2()).first()[1]);
      // assert(x != 0);
      angle_t a =  std::abs(atan(y / (x + 1)));

      edge_values(e.id()) = edge_value_t(d, a);
    }

    p_edges<G, edges_value_t> pe(g, edge_values);

    trace::exiting("make::p_edges_for_lines");
    return pe;
  }

  template <typename I, typename G, typename EV, typename VV>
  void draw_edges(Image<I>& input_, const Graph<G>& g_,
                  const EV& edges_values, const VV& vertices_values)
  {
    I& input = exact(input_);
    const G& g = exact(g_);
    (void) edges_values;

    mln_edge_iter(G) e(g);
    for_all(e)
    {
      // int val = vertices_values(e.v1()).second() + vertices_values(e.v2()).second() + 1;
      // std::cerr << edges_values.function()(e.id()).second() << std::endl;
      // if (edges_values.function()(e.id()).second() < 0.7)
      mln_VAR(val, edges_values.function()(e.id()));
      int value = std::accumulate(val.begin(), val.end(), 0);
      draw::line(input, vertices_values(e.v1()).first(),
                 vertices_values(e.v2()).first(), value + 1);
    }
  }

#define QRT(a) (a * a)
  template <typename VV, typename EV>
  struct dist_calc_t
  {
    dist_calc_t(const VV& vertices_values,
                const EV& edges_values)
      : vertices_values(vertices_values), edges_values(edges_values)
    {}

    unsigned operator()(unsigned from_s, unsigned to_s, unsigned edge) const
    {
      unsigned rot = 0;
      // Check whether the run is backward.
      if (vertices_values(from_s).first()[1] > vertices_values(to_s).first()[1])
        rot = 1;

      return edges_values.function()(edge).first() *
        (rot + std::abs(edges_values.function()(edge).second()) / 90);
    }

    const VV& vertices_values;
    const EV& edges_values;
  };


  template <typename G, typename VV, typename EV>
  p_edges<G, fun::i2v::array<std::set<unsigned> > > init_paths(const Graph<G>& g_,
                                                               const VV& vertices_values,
                                                               const EV& edges_values)
  {
    const G& g = exact(g_);
    typedef fun::i2v::array<std::set<unsigned> > edges_value_t;

    edges_value_t label_values(g.e_nmax());
    mln_vertex_iter(G) v(g);

    std::vector<unsigned> initial;
    std::vector<unsigned> final;

    dist_calc_t<VV, EV> dist_calc(vertices_values, edges_values);

    for_all(v)
    {
      if (vertices_values(v.id()).second() == 1)
        initial.push_back(v.id());

      if (vertices_values(v.id()).second() == 2)
        final.push_back(v.id());
    }

    int id = 3;
    for (std::vector<unsigned>::const_iterator i = initial.begin();
         i != initial.end();
         ++i, ++id)
    {
      dijkstra(g, *i, dist_calc);
      unsigned min = 0;
      for (unsigned i = 0; i < final.size(); ++i)
      {
        if (dijkstra.dist[final[i]] < dijkstra.dist[final[min]])
          min = i;
      }

      unsigned current = final[min];
      while (dijkstra.previous[current] != -1)
      {
        util::vertex<G> current_v = g.vertex(dijkstra.previous[current]);

        for (unsigned i = 0; i < current_v.nmax_nbh_vertices(); ++i)
          if (current_v.ith_nbh_vertex(i) == current)
          {
            label_values(current_v.ith_nbh_edge(i)).insert(id);
            break;
          }

        current = dijkstra.previous[current];
      }
    }

    p_edges<G, edges_value_t> pe(g, label_values);
    return pe;
  }

  template <typename EV>
  unsigned decision(const EV& edge_values)
  {
    unsigned i = 0;
    unsigned res = 0;

    for (; i < edge_values.function().size(); ++i)
      if (edge_values.function()(i).size() != 0)
        res += edge_values.function()(i).size() - 1;
    return res;
  }



  template <typename I>
  mln_concrete(I) rag(Image<I>& input_)
  {
    I& input = exact(input_);
    typedef label_16 L;
    typedef mln_site(I) P;
    L n_basins;
    mln_concrete(I) grad;
    grad = morpho::closing::area(input, c4(), 20);
    image2d<L> wsed =
      morpho::watershed::flooding(grad, c4(), n_basins);

    io::ppm::save(morpho::watershed::superpose(input, wsed, literal::red), "wsd.ppm");
    util::graph g = make::region_adjacency_graph(wsed, c4(), n_basins);

    I not_input = logical::not_(input);
    accu::center_weight<I> accu(not_input);
    mln_VAR(vertices, make::p_vertices_with_accu(wsed, n_basins, g, accu));

    mln_VAR(edges, p_edges_for_lines(g, vertices));
    mln_VAR(paths, init_paths(g, vertices, edges));

    draw_edges(input, g, paths, vertices);
    std::cerr << "decision: " << decision(paths) << std::endl;

    return input;
  }

}
