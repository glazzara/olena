#ifndef MLN_FLLT_TREE_TO_IMAGE_HH
# define MLN_FLLT_TREE_TO_IMAGE_HH

#include "fllt.svg.7.hh"

namespace mln
{
  template <typename I>
  void fllt_tree_to_image_(I& output,
			   fllt_node(point2d, value::int_u8)* node)
  {
    level::fill(inplace(output | node->elt().points), node->elt().value);

    for (int i = 0; i < node->children().size(); i++)
      fllt_tree_to_image_(output, node->children()[i]);
  }

  template <typename I>
  I fllt_tree_to_image(const I& input,
		       fllt_tree(point2d, value::int_u8)& tree)
  {
    I output;
    initialize(output, input);
    fllt_tree_to_image_(output, tree.root());
    return output;
  }

}

#endif // ! MLN_FLLT_TREE_TO_IMAGE_HH
