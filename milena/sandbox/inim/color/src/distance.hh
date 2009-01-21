/*! \file src/distance.hh
 *
 */

#ifndef SRC_DISTANCE_HH
# define SRC_DISTANCE_HH

# include "graph.hh"

# include <cmath>


/// Manhatan distance
inline
unsigned distance(const mln::value::rgb8& lhs,
                  const mln::value::rgb8& rhs)
{
  return abs(lhs.red() - rhs.red()) +
    abs(lhs.green() - rhs.green()) +
    abs(lhs.blue() - rhs.blue());
}


/// Store the distance between two points on edge
/// FIXME documentation
template <typename I, typename N, typename O>
void distance(const I& rgb_graph,
              const N& nbh,
              O& gl_graph)
{
  mln_piter(I) p(rgb_graph.domain());
  mln_niter(N) n(nbh, p);

  for_all(p)
  {
    mln::value::rgb8 v1;
    mln::value::rgb8 v2;

    n.start();
    assert(n.is_valid() && rgb_graph.has(n));
    v1 = rgb_graph(n);
    n.next();
    assert(n.is_valid() && rgb_graph.has(n));
    v2 = rgb_graph(n);

    gl_graph(p) = distance(v1, v2);
  }
}



#endif // !SRC_DISTANCE_HH
