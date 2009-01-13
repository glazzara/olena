#include "fllt.svg.7.hh"
#include "fllt_tree_to_image.hh"
#include "area_filter.hh"
#include "canonize_tree.hh"

namespace mln
{
  template <typename P, typename V>
  void flatten(fllt_node(P, V)* node,
	       unsigned min_contrast)
  {
    for (int i = 0; i < node->children().size(); i++)
    {
      if (abs(node->children()[i]->elt().value - node->elt().value) < min_contrast)
	node->children()[i]->elt().value = node->elt().value;
      flatten(node->children()[i], min_contrast);
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef fllt_tree(point2d, int_u8) tree_type;

  if (argc != 5)
  {
    std::cerr << "usage: " << argv[0] << " input min_area min_contrast output" << std::endl;
    return 1;
  }

  image2d<int_u8> ima;
  io::pgm::load(ima, argv[1]);

  tree_type tree = fllt(ima);
  compute_area(ima, tree);

  area_filter_min(tree.root(), atoi(argv[2]));
  canonize_tree(tree.root());
  flatten(tree.root(), atoi(argv[3]));
  io::pgm::save(fllt_tree_to_image(ima, tree), argv[4]);
}
