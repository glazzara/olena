#include <mln/core/image2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/p_set.hh>

#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include "basic_najman.hh"
#include <string>
#include <iostream>

int print_and_exit (std::string s)
{
  std::cerr << s << std::endl;
  return 1;
}

template <typename T>
void swap (T& x, T& y)
{
  T memo = x;
  x = y;
  y = memo;
}

template <typename T>
class test_tree;

template <typename T>
class test_tree
{
public:

  test_tree(T pdata) : data(pdata), children(NULL), brother(NULL) { }

  T data;
  test_tree<T> *children;
  test_tree<T> *brother;

  test_tree<T>* insert_child(test_tree<T> *n)
  {
    test_tree<T> **p = &children;
    while (*p && (**p).data < n->data)
      p = &((**p).brother);
    n->brother = *p;
    *p = n;
    return n;
  }

  test_tree<T>* insert_child(T pdata)
  {
    test_tree<T> *n = new test_tree<T>(pdata);
    return insert_child(n);
  }
};

template <typename T>
bool compare_tree(test_tree<T>* t1, test_tree<T>* t2)
{
  if (!t1 && !t2)
    return true;

  if ((!t1 || !t2) || (t1->data != t2->data))
    return false;

  return (compare_tree(t1->brother, t2->brother)
	  && compare_tree(t1->children, t2->children));
}

template <typename T>
void show_tree (test_tree<T> *t)
{
  if (!t)
    return ;

  if (t->brother)
    {
      std::cout << t->data << " - ";
      show_tree(t->brother);
    }
  else
    std::cout << t->data << std::endl;

  if (t->children)
    {
      std::cout << "sons of " << t->data << " : ";
      show_tree(t->children);
    }
}


using namespace mln;
using value::int_u8;


test_tree<image2d<int_u8>::psite>* test_convert (morpho::basic_najman<image2d<int_u8>, neighb2d>& ima, image2d<int_u8>::psite root);

test_tree<image2d<int_u8>::psite>* test_convert (morpho::basic_najman<image2d<int_u8>, neighb2d>& ima, image2d<int_u8>::psite root)
{
  morpho::basic_najman<image2d<int_u8>, neighb2d>::node& n = ima.nodes(ima.Par_node(root));

  test_tree<image2d<int_u8>::psite> *res = new test_tree<image2d<int_u8>::psite>(ima.Par_node(root));

  p_array<image2d<int_u8>::psite>::fwd_piter it (n.children);
  for_all(it)
    res->insert_child(test_convert(ima, it.to_psite()));

  return res;
}

int main ()
{
  typedef image2d<int_u8> image2dint;

  image2dint input, verif;
  io::pgm::load(input, "./images/test_component_tree.pgm");
  io::pgm::load(verif, "./images/test_component_mapping.pgm");

  morpho::basic_najman< image2d<int_u8>, neighb2d> n(input, c4());
  n.go();

  /*
    Component mapping :

    3 3 3 3 9 2 2 2 2
    1 1 1 9 7 9 2 2 2
    1 1 9 7 7 7 9 2 2
    1 9 6 9 7 9 5 9 2
    1 1 9 1 7 2 7 2 2
    1 1 1 1 7 2 2 2 2


      Component tree

             9
            / \
           7   6
          /|\
         3 2 5
         |
         1
  */

  // Component mapping test

  image2dint::fwd_piter it(input.domain());
  image2dint::psite c[10];
  c[1] = n.Par_node(image2dint::psite(1, 0));
  c[2] = n.Par_node(image2dint::psite(0, 5));
  c[3] = n.Par_node(image2dint::psite(0, 0));
  c[5] = n.Par_node(image2dint::psite(3, 6));
  c[6] = n.Par_node(image2dint::psite(3, 2));
  c[7] = n.Par_node(image2dint::psite(1, 4));
  c[9] = n.Par_node(image2dint::psite(0, 4));

  bool id = true;

  for_all(it)
    id = (c[verif(it.to_point())] == n.Par_node(it.to_point()));

  if (!id)
    {
      std::cerr << "Component mapping is fucked up!" << std::endl;
      return 1;
    }

  // Component tree test

  test_tree<image2dint::psite> *gen, ref(c[9]), *l1, *l2;

  // Create the ref
  ref.insert_child(c[6]);
  l1 = ref.insert_child(c[7]);
  l1->insert_child(c[2]);
  l1->insert_child(c[5]);
  l2 = l1->insert_child(c[3]);
  l2->insert_child(c[1]);

  // Create the generated tree
  gen = test_convert(n, n.Par_node(n.Root));

  // Compare the trees
  if (!compare_tree(gen, &ref))
    {
      std::cout << "Your tree :" << std::endl;
      show_tree(gen);

      std::cout << "Reference tree :" << std::endl;
      show_tree(&ref);

      std::cerr << "Component tree is fucked up!" << std::endl;
      return 1;
    }

  // Test the LCA alorithm
  image2dint::psite p(0, 0), q(0, 6), r(2, 4);
  if (n.lca(p, q) != image2dint::psite(2,4))
    return 1;

  if (n.lca(r, q) != image2dint::psite(2,4))
    return 1;

  // Test the Highest Fork
  p_set<image2dint::psite> comp;
  comp.insert(image2dint::psite(0,0));
  comp.insert(image2dint::psite(2,4));
  comp.insert(image2dint::psite(0,6));
  comp.insert(image2dint::psite(1,0));

  if (n.highest_fork(comp) != image2dint::psite(2, 4))
    return 1;

  comp.insert(image2dint::psite(3,2));

  if (n.highest_fork(comp) != image2dint::psite(1, 3))
    return 1;

  std::cout << "Test is successfull" << std::endl;

  return 0;
}
