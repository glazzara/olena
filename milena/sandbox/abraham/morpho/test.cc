#include <mln/core/image2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/neighb2d.hh>

#include <mln/value/int_u8.hh>

//#include <mln/morpho/meyer_wst.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

# include "basic_najman.hh"
// # include "topo_wst.hh"

int main ()
{
  using namespace mln;
  using value::int_u8;

  using namespace mln;
  using value::int_u8;

  image2d<int_u8> input;
  io::pgm::load(input, "./images/test_component_tree.pgm");

  morpho::basic_najman< image2d<int_u8>, neighb2d> n(input, c4());
  n.image_output(input);
  n.go();
  n.image_output(n.Par_tree);
  n.print_tree(n.Root);
  n.image_output(n.Par_node);
  std::cout << " --------------------------- " << std::endl;
  n.image_output(n.Rnk_tree);
  std::cout << " --------------------------- " << std::endl;
  n.image_output(n.Rnk_node);
  std::cout << " --------------------------- " << std::endl;
  n.print_tree(n.Root);
  std::cout << " --------------------------- " << std::endl;

  n.m_watershed();

  // image2d<int_u8> output = morpho::topo_wst(input, c4());
  //  io::pgm::save(output, "out.pgm");

  return 0;
}
