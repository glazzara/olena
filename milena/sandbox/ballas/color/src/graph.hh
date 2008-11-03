/*! \file src/graph.hh
 *
 */

#ifndef SRC_GRAPH_HH
# define SRC_GRAPH_HH

# include <mln/value/int_u8.hh>
# include <mln/value/int_u16.hh>
# include <mln/value/rgb8.hh>

# include <mln/level/fill.hh>

# include <mln/core/image/image2d.hh>


// Neighborhood functions
inline
bool is_row_odd(const mln::point2d& p)
{
  return p.row() % 2;
}
inline
bool is_cell(const mln::point2d& p)
{
  return p.row() % 2 == 0 && p.col() % 2 == 0;
}
inline
bool is_edge(const mln::point2d& p)
{
  return (p.row() % 2 + p.col() % 2) == 1;
}
inline
bool is_point(const mln::point2d& p)
{
  return p.row() % 2 && p.col() % 2;
}
inline
bool is_not_edge(const mln::point2d& p)
{
  return ! is_edge(p);
}

// Graph image creation function
// FIXME: add exact conversion....
// FIXME: check that the input image is in 2 dimension
template <typename I>
void
create_graph(const I& ima, I& graph, mln_value(I) val)
{
  graph = I(ima.nrows() * 2 -1, ima.ncols() * 2 - 1);

  mln::level::fill(graph, val);

  mln_piter(I) p(ima.domain());
  for_all(p)
    graph.at(p.row() * 2, p.col() * 2) = ima(p);
}


#endif // !SRC_GRAPH_HH
