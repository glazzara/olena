#include <mln/core/image2d_b.hh>
#include <mln/core/point2d.hh>
#include <mln/debug/println.hh>
#include "graph.hh"
#include "mesh_p.hh"
#include "mesh_psite.hh"
#include "draw_mesh.hh"

using namespace mln;

int
main (void)
{
  util::Graph<void>		g;

  g.add_node ();
  g.add_node ();
//   g.add_node ();
//   g.add_node ();

  g.add_edge (0, 1);
  //  g.add_edge (0, 2);
//   g.add_edge (0, 2);
//   g.add_edge (2, 3);
//   g.add_edge (0, 3);

  g.coherence ();
  //  g.print_debug ();

  std::vector<point2d>		v;
  v.push_back (make::point2d (10,10));
  v.push_back (make::point2d (19,6));
//   v.push_back (make::point2d (1,15));
//   v.push_back (make::point2d (16,5));
  image2d_b<int>		ima (20, 20, 1);

  Mesh_p<point2d>		m(g, v);

  draw::mesh (ima, m, 7, 1);
  debug::println (ima);
}
