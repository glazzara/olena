
#include <mln/core/var.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/meyer_wst.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/count.hh>

#include "pseudo_tree.hh"
#include "cplx2d.hh"




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

  typedef mln_value_(g_t) T;                 //  <---  Type of edge values.
  typedef mln_psite_(g_t) E;                 //  <---  Type of edges.


  // w: watershed labeling on edges.

  typedef label_16 L;                        //  <---  Type of labels.
  L l_max;
  mln_VAR( w, morpho::meyer_wst(g, cplx2d::e2e(), l_max) );
  debug::println("w:", w);


  mln_VAR( is_w_line, pw::value(w) == pw::cst(0) );
  mln_VAR( g_line, g | is_w_line );
  debug::println("g | line:", g_line);

  mln_VAR(w_ext, cplx2d::extend_w_edges_to_all_faces(w));
  debug::println("w_ext:", w_ext);


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

  util::array<A> a = labeling::compute(accu::meta::count(),
				       g, // image of values
				       w, // image of labels
				       l_max);

  util::array<L> ls = sort_by_increasing_attributes(a, l_max);

//   {
//     std::cout << "ls:" << std::endl;
//     for (unsigned i = 1; i <= l_max; ++i)
//       std::cout << ls[i] << "(" << a[ls[i]] << ") ";
//     std::cout << std::endl
// 	      << std::endl;
//   }



  // -> pseudo-tree

  compute_pseudo_tree(w, g, ls, a, e_to_l1_l2);

}
