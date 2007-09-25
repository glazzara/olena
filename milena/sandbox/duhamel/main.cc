#include "graph.hh"
//#include "mesh_p.hh"
//#include "mesh_psite.hh"

using namespace mln;

int
main (void)
{
  util::Graph<void>		g;

  g.add_node ();
  g.add_node ();
  g.add_node ();
  g.add_edge (0,1);
  g.coherence ();
  g.print_debug ();

//  image2d_b<int>		out (5, 5, 1);

  //  debug::println_with_border (out);

}
