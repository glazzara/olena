
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

#include <mln/debug/println.hh>


namespace mln
{

  template <typename T, typename A>
  void
  back_propagate(const T& t, A& a)
    // a value propagates from the representative point to every point
    // of the component at the same level (NOT to the branch points!)
  {
    mln_fwd_piter(T) p(t.domain());
    for_all(p)
      if (! t.is_a_node(p))
	{
	  mln_assertion(t.is_a_node(t.parent(p)));
	  a(p) = a(t.parent(p)); // ...here!
	}
  }


  template <typename T, typename A>
  void
  back_propagate_II(const T& t, A& a)
    // a value propagates from a representative point to every point
    // of the component and the children components (branch).
  {
    mln_bkd_piter(T) p(t.domain());
    for_all(p)
      if (t.is_a_node(p) && a(t.parent(p)) == true)
	a(p) = a(t.parent(p));
  }


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
      aa(n) = (a(n) == true && a(t.parent(n)) == false);

    if (echo) io::pbm::save(aa, "before.pbm");
    if (echo > 1) debug::println("aa (before)", aa);

    back_propagate_II(t, aa);
    back_propagate(t, aa);

    if (echo > 1) debug::println("aa (after)", aa);
    io::pbm::save(aa, "out.pbm");

  }

} // mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm echo lambda" << std::endl;
  std::cerr << "  echo: 0 (none) or 1 (verbose)" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  mln_VAR(nbh, c4());

  if (argc != 4)
    usage(argv);

  int echo = std::atoi(argv[2]);
  if (echo != 0 && echo != 1)
    {
      std::cerr << "bad 'echo' value!" << std::endl;
      usage(argv);
    }

  typedef image2d<int_u8> I;

  I input;
  io::pgm::load(input, argv[1]);
  if (echo > 1) debug::println("input", input);

   I f = input;
   //  I f = morpho::elementary::gradient(input, nbh);
  if (echo > 1) debug::println("f", f);


  typedef p_array<point2d> S;
  S s = level::sort_psites_decreasing(f);

  typedef morpho::tree::data<I,S> tree_t;
  tree_t t(f, s, nbh);

  morpho::attribute::occupation<I> a_;
  mln_VAR(a, morpho::tree::compute_attribute_image(a_, t));

  if (echo > 1)
    {
      debug::println("parent imagee", t.parent_image());
      debug::println("a", a);
      debug::println("a | nodes", a | t.nodes());
    }

  image2d<bool> b = duplicate((pw::value(a) < pw::cst(atoi(argv[3]))) | a.domain());
  if (echo > 1) debug::println("b", b | t.nodes());

  sample(t, b, echo);
}
