#include <mln/core/image2d_b.hh>
#include <mln/core/point2d.hh>
#include <mln/debug/println.hh>
#include "graph.hh"
#include "mesh_p.hh"
#include "mesh_psite.hh"
#include "draw_mesh.hh"
#include "mesh_image.hh"
#include <mln/core/interpolated.hh>

using namespace mln;

int
main (void)
{
 util::graph<void> g;

 g.add_node ();
 g.add_node ();
 g.add_node ();
 g.add_node ();
 g.add_node ();
 g.add_node ();

 g.add_edge (0, 1);
 g.add_edge (1, 2);
 g.add_edge (2, 3);
 g.add_edge (1, 3);
 g.add_edge (4, 5);
 g.add_edge (1, 4);

 g.coherence ();
 // g.print_debug ();

 std::vector<point2d> v;
 v.push_back (make::point2d (1,1));
 v.push_back (make::point2d (10,1));
 v.push_back (make::point2d (19,2));
 v.push_back (make::point2d (19,19));
 v.push_back (make::point2d (10,10));
 v.push_back (make::point2d (1,19));


 image2d_b<int> ima (20, 20, 1);

 mesh_p<point2d> m(g, v);

 draw::mesh (ima, m, 7, 1);

 std::vector<int> val;

 val.push_back (2);
 val.push_back (3);
 val.push_back (4);
 val.push_back (5);
 val.push_back (6);
 val.push_back (7);

 mesh_image<point2d, int> im (m, val);

 draw::mesh (ima, im);
 debug::println (ima);

 std::cout << im.domain() << std::endl;
}
