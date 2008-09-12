#ifndef MLN_FLLT_AREA_FILTER_HH
# define MLN_FLLT_AREA_FILTER_HH

#include "fllt.svg.7.hh"

namespace mln
{

  template <typename P, typename V, typename I>
  unsigned
  compute_area_rec(fllt_node(P, V)* node, I& ima)
  {

    if (!node)
      return 0;

    int area = 0;

    for (int i = 0; i < node->children().size();i++)
      area += compute_area_rec(node->children()[i], ima);

    mln_piter(p_array<P>) p(node->elt().points);
    for_all(p)
      if (!ima(P(p)))
      {
	++area;
	ima(p) = true;
      }

    node->elt().nsites = area;
    return area;
  }

  template <typename P, typename V, typename I>
  void
  compute_area(const Image<I>& input_, fllt_tree(P, V)& tree)
  {
    const I& input = exact(input_);

    image2d<bool> ima(input.domain());
    level::fill(ima, false);
    compute_area_rec(tree.root(), ima);
  }

  void area_filter_min_(fllt_node(point2d, value::int_u8)* node,
			unsigned min_area,
			value::int_u8 g)
  {
    unsigned area = (*node).elt().nsites;
    if (area > min_area)
      g = (*node).elt().value;

    node->elt().value = g;

    for (int i = 0; i < node->children().size(); i++)
      area_filter_min_(node->children()[i], min_area, g);
  }

  template <typename P, typename V>
  void area_filter_min(fllt_node(P, V)* node,
		       unsigned min_area)
  {
    area_filter_min_(node, min_area, (*node).elt().value);
  }

}


#endif // ! MLN_FLLT_AREA_FILTER_HH
