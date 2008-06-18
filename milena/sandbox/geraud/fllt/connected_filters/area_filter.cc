#include "fllt.svg.7.hh"
#include "fllt_tree_to_image.hh"
#include "area_filter.hh"

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef fllt_tree(point2d, int_u8) tree_type;

  if (argc != 4)
  {
    std::cerr << "usage: " << argv[0] << " input min_area output" << std::endl;
    return 1;
  }

  image2d<int_u8> ima;
  io::pgm::load(ima, argv[1]);

  tree_type tree = fllt(ima);
  compute_area(ima, tree);

  area_filter_min(tree.root(), atoi(argv[2]));
  io::pgm::save(fllt_tree_to_image(ima, tree), argv[3]);
}
