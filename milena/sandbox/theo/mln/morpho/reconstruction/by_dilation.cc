#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
# include <mln/util/timer.hh>

#include "by_dilation.hh"


int main()
{
  using namespace mln;

  image2d<bool> f, g, o;
  neighb2d nbh = c4();

  io::pbm::load(f, "f_and_g.pbm");
  io::pbm::load(g, "g.pbm");

  float t_tufa, t_tufa_alt, t_tufa_fast, t_hybrid, t_hybrid_2, t_hybrid_fast;
  util::timer t;
 
  {
    t.start();
    o = morpho::reconstruction::impl::by_dilation_tufa_on_set(f, g, nbh);
    t_tufa = t.stop();
  }

  {
    t.start();
    o = morpho::reconstruction::impl::by_dilation_tufa_on_set_alt(f, g, nbh);
    t_tufa_alt = t.stop();
  }

  {
    t.start();
    o = morpho::reconstruction::impl::by_dilation_hybrid_on_set(f, g, nbh);
    t_hybrid = t.stop();
  }

  {
    t.start();
    o = morpho::reconstruction::impl::by_dilation_hybrid_on_set__two_loops(f, g, nbh);
    t_hybrid_2 = t.stop();
  }

  {
    t.start();
    o = morpho::reconstruction::impl::by_dilation_tufa_on_set_fastest(f, g, nbh);
    t_tufa_fast = t.stop();
  }

  {
    t.start();
    o = morpho::reconstruction::impl::by_dilation_hybrid_on_set_fastest(f, g, nbh);
    t_hybrid_fast = t.stop();
  }

  std::cout << "tufa " << t_tufa << std::endl
	    << "tufa_alt " << t_tufa_alt << std::endl
	    << "hybrid " << t_hybrid << std::endl
	    << "hybrid two loops " << t_hybrid_2 << std::endl
	    << "tufa_fast " << t_tufa_fast << std::endl
	    << "hybrid_fast " << t_hybrid_fast << std::endl;

  io::pbm::save(o, "o.pbm");
}
