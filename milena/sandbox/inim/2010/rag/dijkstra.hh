#ifndef DIJKSTRA_HH
# define DIJKSTRA_HH

# include <algorithm>
# include <vector>
# include <mln/core/concept/graph.hh>
# include <mln/util/array.hh>

namespace mln
{
  struct dijkstra_t
  {
    template <typename G, typename F>
    void operator()(const Graph<G>& g_, const util::vertex_id_t source,
                    const F& distance_calc)
    {
      const G& g = exact(g_);
      mln_assertion(source < g.v_nmax());

      previous.reserve(g.v_nmax());
      dist.reserve(g.v_nmax());
      for(unsigned i = 0; i < g.v_nmax(); ++i)
      {
        dist[i] = -1;
        previous[i] = -1;
      }

      dist[source] = 0;
      std::set<util::vertex_id_t> vertices;

      mln_vertex_iter(G) v(g);
      for_all(v)
        vertices.insert(v.id());

      while (!vertices.empty())
      {
        unsigned best_vertice = *vertices.begin();
        for (std::set<util::vertex_id_t>::const_iterator i = vertices.begin();
             i != vertices.end();
             ++i)
          if (dist[best_vertice] > dist[*i])
            best_vertice = *i;

        if (dist[best_vertice] == static_cast<unsigned>(-1))
          break;

        util::vertex<G> current_v = g.vertex(best_vertice);
        vertices.erase(best_vertice);

        for (unsigned i = 0; i < current_v.nmax_nbh_vertices(); ++i)
        {
          unsigned d = dist[current_v.id()] +
            distance_calc(current_v.id(), current_v.ith_nbh_vertex(i),
                          current_v.ith_nbh_edge(i));

          if (d < dist[current_v.ith_nbh_vertex(i)])
          {
            dist[current_v.ith_nbh_vertex(i)] = d;
            previous[current_v.ith_nbh_vertex(i)] = current_v.id();
          }
        }
      }
    }

    std::vector<int> previous;
    std::vector<unsigned> dist;
  };

  dijkstra_t dijkstra;
}
#endif // !DIJKSTRA_HH
