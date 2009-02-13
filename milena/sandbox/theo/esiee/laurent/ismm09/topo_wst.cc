
#include <mln/core/var.hh>

#include <mln/value/label.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/debug/iota.hh>

#include "pseudo_tree.hh"
#include "cplx2d.hh"




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm echo" << std::endl;
  std::cerr << "Laurent topological watershed transform thru ISMM 2009 scheme." << std::endl;
  std::cerr << "echo = 0 (none) or 1 (effective)." << std::endl;
  abort();
}




int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;


  if (argc != 3)
    usage(argv);

  int echo = std::atoi(argv[2]);
  if (echo != 0 && echo != 1)
    {
      std::cerr << "Bad 'echo' value!" << std::endl;
      usage(argv);
    }


  // f: regular image.

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);

  if (echo)
    debug::println("f:", f);


  // g: weights on edges.

  mln_VAR(g, cplx2d::f_to_g(f) );
  if (echo)
    debug::println("g:", g);


  // r: one pixel is one region.

  typedef value::label<32> L;
  L l_max = f.nsites();

  image2d<L> w_ext(2 * f.nrows() - 1, 2 * f.ncols() - 1);
  data::fill(w_ext, 0); // Useless but for display!
  mln_VAR( w_pixel, w_ext | cplx2d::is_pixel );
  {
    mln_fwd_piter_(w_pixel_t) p(w_pixel.domain());
    unsigned l = 0;
    for_all(p)
      w_pixel(p) = ++l;
  }
  if (echo)
    debug::println("w_ext:", w_ext);
  

  // e -> (l1, l2)
  mln_VAR( e_to_l1_l2, function_e_to_l1_l2(w_ext, cplx2d::e2p()) );



  // a: array "label -> attribute".

  typedef int_u8 A;                        //  <---  Type of attributes.

  util::array<A> a(l_max.next());
  {
    mln_fwd_piter_(box2d) p(f.domain());
    unsigned l = 0;
    for_all(p)
      a[++l] = f(p);
  }

  util::array<L> l_ = sort_by_increasing_attributes(a, l_max);

  if (echo)
    {
      std::cout << "l_:" << std::endl;
      for (unsigned i = 1; i <= l_max; ++i)
	std::cout << l_[i] << "(" << a[l_[i]] << ") ";
      std::cout << std::endl
		<< std::endl;
    }



  // -> pseudo-tree

  compute_pseudo_tree(w_ext, g, l_, a, e_to_l1_l2,
		      echo);
}
