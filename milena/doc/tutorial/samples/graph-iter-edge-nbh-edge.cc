// Iterator on edges.
mln_edge_iter(util::graph) e(g);

// Iterator on edges adjacent to e.
mln_edge_nbh_edge_iter ne(g, e);

// Prints the graph
// List all adjacent edges for each edge.
for_all(e)
{
  std::cout << e << " : ";
  for_all(ne)
    std::cout << ne << " ";
  std::cout << std::endl;
}
