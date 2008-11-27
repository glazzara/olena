#include <mln/essential/2d.hh>
#include <mln/util/graph.hh>
#include <doc/tutorial/tools/sample_utils.hh>

struct viota_t : public mln::Function_p2v< viota_t >
{
  typedef unsigned result;

  viota_t(unsigned size)
  {
    v_.resize(size);
    for(unsigned i = 0; i < size; ++i)
      v_[i] = 10 + i;
  }

  unsigned
  operator()(const mln::util::vertex<mln::util::graph>& v) const
  {
    return v_[v.id()];
  }

  protected:
    std::vector<result> v_;
};

int main()
{
  using namespace mln;

  util::graph g;

  for (unsigned i = 0; i < 5; ++i)
    g.add_vertex(); // Add vertex 'i';

  g.add_edge(0, 1); // Associated to edge 0.
  g.add_edge(1, 2); // Associated to edge 1.
  g.add_edge(1, 3); // Associated to edge 2.
  g.add_edge(3, 4); // Associated to edge 3.
  g.add_edge(4, 2); // Associated to edge 4.



  doc::begin_output();
  // \{
  // Function which maps sites to data.
  viota_t viota(g.v_nmax());

  // Iterator on vertices.
  mln_vertex_iter_(util::graph) v(g);

  // Prints each vertex and its associated value.
  for_all(v)
    std::cout << v << " : " << viota(v) << std::endl;
  // \}
  doc::end_output();


  {
    doc::begin_output();
    // \{
    // Iterator on vertices.
    mln_vertex_iter_(util::graph) v(g);

    // Iterator on v's edges.
    mln_vertex_nbh_edge_iter_(util::graph) e(v);

    // Prints the graph
    // List all edges for each vertex.
    for_all(v)
    {
      std::cout << v << " : ";
      for_all(e)
	std::cout << e << " ";
      std::cout << std::endl;
    }
    // \}
    doc::end_output();
  }



  {
    doc::begin_output();
    // \{
    // Iterator on edges.
    mln_edge_iter_(util::graph) e(g);

    // Iterator on edges adjacent to e.
    mln_edge_nbh_edge_iter_(util::graph) ne(e);

    // Prints the graph
    // List all adjacent edges for each edge.
    for_all(e)
    {
      std::cout << e << " : ";
      for_all(ne)
	std::cout << ne << " ";
      std::cout << std::endl;
    }
    // \}
    doc::end_output();
  }



  {
    doc::begin_output();
    // \{
    // Iterator on vertices.
    mln_vertex_iter_(util::graph) v(g);

    // Iterator on vertices adjacent to v.
    mln_vertex_nbh_vertex_iter_(util::graph) nv(v);

    // Prints the graph
    // List all adjacent edges for each edge.
    for_all(v)
    {
      std::cout << v << " : ";
      for_all(nv)
	std::cout << nv << " ";
      std::cout << std::endl;
    }
    // \}
    doc::end_output();
  }
}
