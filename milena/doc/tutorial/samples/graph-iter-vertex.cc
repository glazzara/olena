// Iterator on vertices.
mln_vertex_iter(util::graph) v(g);

// Iterator on v's edges.
mln_vertex_nbh_edge_iter(util::graph) e(g, v);

// Prints the graph
// List all edges for each vertex.
for_all(v)
{
  std::cout << v << " : ";
  for_all(e)
    std::cout << e << " ";
  std::cout << std::endl;
}
