#include "graph.hh"

using namespace mln;

int
main (void)
{
  mln::util::Graph<void>		g (0,0);

  g.add_node ();
  g.add_node ();
  g.add_node ();
  g.add_edge (0,1);
  g.coherce ();
  g.print ();

//  image2d_b<int>		out (5, 5, 1);

  //  debug::println_with_border (out);

}
