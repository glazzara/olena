#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/compare.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/najman_wst.hh>
#include <string>
#include <iostream>

int main ()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> image2dint;

  image2dint input, mverif, wverif;

  // #define TEST

  //   io::pgm::load(input, "./images/test_watershed.pgm");
  // io::pgm::load(input, "./images/little_test.pgm");
    io::pgm::load(input, "./images/test.pgm");

 //   io::pgm::load(input, "./images/lena_light.pgm");
 // io::pgm::load(mverif, "./images/result_m_watershed.pgm");
 // io::pgm::load(wverif, "./images/result_topo_watershed.pgm");

  morpho::najman_wst< image2d<int_u8>, neighb2d> n(input, c4());

  /*
  image2dint::fwd_piter it(input.domain());
  for_all(it)
  {
    input(it) = input(it)/17;
    mverif(it) = mverif(it)/17;
  }
  */

  //  io::tikz::save(input, "start.tex");

  n.go();

  io::pgm::save(n.pima, "out.pgm");

  return 0;
}
