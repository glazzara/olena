#include "graph.hh"
#include "mesh_p.hh"
#include "mesh_psite.hh"

using namespace mln;

int
main (void)
{
  util::Graph<void>		g;

  g.add_node ();
  g.add_node ();
  g.add_node ();
  g.add_node ();

  g.add_edge (0, 1);
  g.add_edge (1, 2);
  g.add_edge (0, 2);
  g.add_edge (2, 3);
  g.add_edge (0, 3);

  g.coherence ();
  g.print_debug ();

  //  image2d_b<int>		out (2, 2, 1);



}
