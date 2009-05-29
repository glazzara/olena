#include <mln/core/site_set/p_vertices.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/var.hh>

#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>
#include <mln/pw/all.hh>

#include <vector>


// \{
template <typename S>
struct viota_t : public mln::Function_p2v< viota_t<S> >
{
  typedef unsigned result;

  viota_t(unsigned size)
  {
    v_.resize(size);
    for(unsigned i = 0; i < size; ++i)
      v_[i] = 10 + i;
  }

  unsigned
  operator()(const mln_psite(S)& p) const
  {
    return v_[p.v().id()];
  }

  protected:
    std::vector<result> v_;
};
// \}

int main()
{
  using namespace mln;

  // \{
  util::graph g;

  for (unsigned i = 0; i < 5; ++i)
    g.add_vertex(); // Add vertex 'i';
  // \}

  // \{
  g.add_edge(0, 1); // Associated to edge 0.
  g.add_edge(1, 2); // Associated to edge 1.
  g.add_edge(1, 3); // Associated to edge 2.
  g.add_edge(3, 4); // Associated to edge 3.
  g.add_edge(4, 2); // Associated to edge 4.
  // \}

  // \{
  typedef fun::i2v::array<point2d> F;
  F f(5); // We need to map 5 vertices.
  f(0) = point2d(0, 0);
  f(1) = point2d(2, 2);
  f(2) = point2d(0, 4);
  f(3) = point2d(4, 3);
  f(4) = point2d(4, 4);
  // \}

  // \{
  typedef p_vertices<util::graph, F> pv_t;
  pv_t pv(g, f);
  // \}

  // \{

  // Constructs an image
  viota_t<pv_t> viota(pv.nsites());
  mln_VAR(graph_vertices_ima, viota | pv);

  //Prints each vertex and its associated data.
  mln_piter_(graph_vertices_ima_t) p(graph_vertices_ima.domain());
  for_all(p)
    std::cout << "graph_vertices_ima(" << p << ") = "
	      << graph_vertices_ima(p) << std::endl;

  // \}
}

