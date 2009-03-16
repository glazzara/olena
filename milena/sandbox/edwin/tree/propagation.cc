#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>


#include <mln/level/sort_psites.hh>
#include <mln/morpho/tree/data.hh>


#include <../../theo/color/change_attributes.hh>
#include "propagate_node.hh"
#include "propagate_value.hh"
#include "run.hh"
#include "accumulator/max.hh"

void usage(char** argv)
{
  std::cerr << "usage: " << argv[0] << " input" << std::endl;
  abort();
}

void dsp(const char* str)
{
  std::cout << "*********************" << std::endl
	    << "** " << str << std::endl
	    << "*********************" << std::endl;
}

template <typename I>
void print(I& img, mln_psite(I) p)
{
  using namespace mln;

  std::cout << p << " -> ";
  mln_fwd_piter(p_array<mln_psite(I)>) it(img(p));
//   for_all(it)
//     std::cout << it << " ";
//   std::cout << std::endl;
  for_all(it)
    print(img, it);
}

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
    usage(argv);


  typedef image2d<int_u8> I;
  I input, dup;


  io::pgm::load(input, argv[1]);


  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;
  S sorted_sites = level::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());

  dsp("Input:");
  display_tree_attributes(tree, input);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_ancestors(point2d(1, 4), tree, dup, 0);
  dsp("Propagate node to ancestors  : (point2d(1, 4), tree, dup, 0)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_ancestors(point2d(1, 4), tree, dup);
  dsp("Propagate node to ancestors  : (point2d(1, 4), tree, dup)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_descendants(point2d(0, 2), tree, dup, 0);
  dsp("Propagate node to descendants  : (point2d(0, 2), tree, dup, 0)");
  display_tree_attributes(tree, dup);

  dup = duplicate(input);
  morpho::tree::propagate_node_to_descendants(point2d(0, 2), tree, dup);
  dsp("Propagate node to descendants  : (point2d(0, 2), tree, dup)");
  display_tree_attributes(tree, dup);

 //  dup = duplicate(input);
//   morpho::tree::propagate_value_to_ancestors(117, tree, dup, 0);
//   dsp("Propagate value to ancestors  : (117, tree, dup, 0)");
//   display_tree_attributes(tree, dup);

//   dup = duplicate(input);
//   morpho::tree::propagate_value_to_ancestors(117, tree, dup);
//   dsp("Propagate value to ancestors  : (117, tree, dup)");
//   display_tree_attributes(tree, dup);

//   dup = duplicate(input);
//   morpho::tree::propagate_value_to_descendants(117, tree, dup, 0);
//   dsp("Propagate value to descendants  : (117, tree, dup, 0)");
//   display_tree_attributes(tree, dup);

//   dup = duplicate(input);
//   morpho::tree::propagate_value_to_descendants(117, tree, dup);
//   dsp("Propagate value to descendants  : (117, tree, dup)");
//   display_tree_attributes(tree, dup);



  dup = duplicate(input);

  typedef morpho::tree::accumulator::max<tree_t, I> A;
  A accu(dup);
  morpho::tree::run_bkd(tree, accu);

  mln_bkd_piter_(tree_t::nodes_t) it_max = accu.to_result();
  morpho::tree::propagate_node_to_descendants(it_max, tree, dup, 69);
  dsp("Propagate value to descendants  : (it_max, tree, dup, 69)");
  display_tree_attributes(tree, dup);

//    mln_dn_node_piter_(tree_t) n(tree);
//    n.start();
//    print(tree.children_image(), n);

  std::cout << "\n";
  mln_preorder_piter_(tree_t) pit(tree);
  mln_psite_(I) parent;
  for_all(pit)
  {
    if (parent != tree.parent(pit))
      std::cout << std::endl;
    std::cout << pit << " -> ";
    parent = pit;
  }
}



