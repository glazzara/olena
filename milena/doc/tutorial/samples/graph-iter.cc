// Function which maps sites to data.
viota_t viota;

// Iterator on vertices.
mln_vertex_iter(util::graph) v(g);

// Prints each vertex and its associated value.
for_all(v)
{
  std::cout << v << " : " << viota(v) << std::endl;
}
