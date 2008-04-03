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

  // component_tree< image2d<int_u8> > c;

  using namespace mln;
  using value::int_u8;

  image2d<int_u8> input;
  io::pgm::load(input, "./images/test_component_tree.pgm");


  for(unsigned int i = 0; i < input.domain().len(0); ++i)
  for(unsigned int j = 0; j < input.domain().len(1); ++j)
    input.at(i, j) = 255 - input.at(i, j);


  morpho::basic_najman< image2d<int_u8>, neighb2d> n(input, c8());
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
  n.print_sup_tree();

  // image2d<int_u8> output = morpho::topo_wst(input, c4());
  //  io::pgm::save(output, "out.pgm");

  return 0;
}
