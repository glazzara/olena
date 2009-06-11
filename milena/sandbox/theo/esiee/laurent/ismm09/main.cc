
#include <mln/core/var.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/math/count.hh>

#include "pseudo_tree.hh"
#include "cplx2d.hh"
#include "extinct_attributes.hh"




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "Laurent ISMM 2009 scheme." << std::endl;
  abort();
}




int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_16;

  if (argc != 2)
    usage(argv);

  // f: regular image.

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);


  // g: weights on edges.

  mln_VAR(g, cplx2d::f_to_g(f) );
  debug::println("g:", g);

  typedef label_16 L;                        //  <---  Type of labels.
  L l_max;

  // w: watershed labeling on edges.

  mln_VAR( w, morpho::watershed::flooding(g, cplx2d::e2e(), l_max) );
  std::cout << "l_max = " << l_max << std::endl;
  debug::println("w:", w);


//   mln_VAR( is_w_line, pw::value(w) == pw::cst(0) );
//   mln_VAR( g_line, g | is_w_line );
//   debug::println("g | line:", g_line);


  mln_VAR(w_ext, cplx2d::extend_w_edges_to_all_faces(w));
//   debug::println("w_ext:", w_ext);


  // e -> (l1, l2)
  mln_VAR( e_to_l1_l2, function_e_to_l1_l2(w_ext, cplx2d::e2p()) );

//   {
//     // Test adjacency "e -> (l1, l2)".
//     L l1, l2;
//     mln_piter_(g_t) e(g.domain());
//     for_all(e)
//     if (w(e) == 0)
//     {
//     e_to_l1_l2(e, l1, l2);
//     std::cout << e << "=" << l1 << '|' << l2 << "  ";
//     }
//     std::cout << std::endl;
//   }



  // a: array "label -> attribute".

  typedef unsigned A;                        //  <---  Type of attributes.
  util::array<A> a(l_max.next(), 0);

  {
    typedef p_array<point2d> s_t;
    s_t s = data::sort_psites_decreasing(g); // min-tree

    typedef morpho::tree::data<g_t,s_t> tree_t;
    tree_t t(g, s, cplx2d::e2e());

    morpho::attribute::height<g_t> a_;
    mln_VAR(h, morpho::tree::compute_attribute_image(a_, t));
    debug::println("h | nodes:", h | t.nodes());

    extinct_attributes(t, h, g);
    debug::println("he:", h);
    debug::println("he | basins:", h | (pw::value(w) != pw::cst(0)));

    mln_invariant(t.leaves().nsites() == l_max);

    mln_leaf_piter_(tree_t) p(t);
    for_all(p)
      a[w(p)] = h(p);

    std::cout << "a = " << std::endl;
    for (unsigned i = 1; i <= l_max; ++i)
      std::cout << i << ':' << a[i] << "  ";
    std::cout << std::endl << std::endl;

  }


//   util::array<L> l_ = sort_by_increasing_attributes(a, l_max);

//   {
//     std::cout << "l_:" << std::endl;
//     for (unsigned i = 1; i <= l_max; ++i)
//       std::cout << l_[i] << "(" << a[l_[i]] << ") ";
//     std::cout << std::endl
// 	      << std::endl;
//   }



  // -> pseudo-tree

//   compute_pseudo_tree(w_ext, g, l_, a, e_to_l1_l2);

}
