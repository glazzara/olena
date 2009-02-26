
#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/pw/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/level/sort_psites.hh>
#include <mln/morpho/elementary/gradient.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/card.hh>
#include "../attributes/occupation.hh"
#include "propagate.hh"

#include <mln/debug/println.hh>


namespace mln
{
  template <typename T, typename A>
  inline
  void
  sample(const T& t, const A& a, int echo)
  {
    A aa;
    initialize(aa, a);
    data::fill(aa, false); // 'aa' is false /a priori/.

    typedef typename T::nodes_t N;
    mln_fwd_piter(N) n(t.nodes());
    for_all(n)
      // We only keep "highest" nodes at 'true' => largest component.
      //aa(n) = (a(n) == true && a(t.parent(n)) == false);
      aa(n) = a(n);

    if (echo) io::pbm::save(aa, "before.pbm");
    if (echo > 1) debug::println("aa (before)", aa);

    //back_propagate_subbranch(t, aa, true);
    //if (echo > 1) debug::println("aa (After subbranch propagation)", aa);
    back_propagate_level(t, aa);

    if (echo > 1) debug::println("aa (Final)", aa);
    io::pbm::save(aa, "out.pbm");

  }



} // mln

int echo = 0;

template <typename I, typename A>
inline
void
create_tree_and_compute(I& f_, A a_, float lambda = mln_min(float), float lambda2 = mln_max(float))
{
  using namespace mln;
  using value::int_u8;

  I f = exact(f_);

  typedef p_array< mln_site(I) > S;
  S s = level::sort_psites_decreasing(f);

  typedef morpho::tree::data<I,S> tree_t;
  tree_t t(f, s, c4());

  mln_VAR(a, morpho::tree::compute_attribute_image(a_, t));

  if (echo > 1)
    {
      debug::println("parent imagee", t.parent_image());
      debug::println("a", a);
      debug::println("a | nodes", a | t.nodes());
    }

  image2d<bool> b = duplicate((pw::value(a) < pw::cst(lambda1) && pw::value(a) < pw::cst(lambda2)) | a.domain());
  sample(t, b, echo);
}

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm echo lambda" << std::endl;
  std::cerr << "\techo:\t0 (none)" << std::endl
	    << "\t\t1 (img output)" << std::endl
	    << "\t\t2 (debug)" << std::endl;
  abort();
}

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  mln_VAR(nbh, c4());

  if (argc != 4)
    usage(argv);

  echo = std::atoi(argv[2]);

  typedef image2d<int_u8> I;

  I input;
  io::pgm::load(input, argv[1]);
  if (echo > 1) debug::println("input", input);

  I f = input;
  //  I f = morpho::elementary::gradient(input, nbh);
  if (echo > 1) debug::println("f", f);

  // test de volume
  typedef image1d<int> IM;
  IM img(6);
  morpho::attribute::volume<IM> accu;
  img.element(0) = 50;
  img.element(1) = 40;
  img.element(2) = 20;
  img.element(3) = 20;
  img.element(4) = 40;
  img.element(5) = 50;

  mln_piter_(image1d<int>) p(img.domain());

  for_all(p)
    accu.take(img(p));

  std::cout << "Volume:" << accu.to_result() << std::endl;
  //create_tree_and_compute(img, morpho::attribute::volume<I2>());
  //

  create_tree_and_compute(f, morpho::attribute::card<I>(), std::atof(argv[3]));

}
