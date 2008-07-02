#include <mln/core/image2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/p_set.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/compare.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/basic_najman.hh>
#include <string>
#include <iostream>

int main ()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> image2dint;

  image2dint input, mverif, wverif;

  // #define TEST

     io::pgm::load(input, "./images/test_watershed.pgm");
  // io::pgm::load(input, "./images/little_test.pgm");
  //  io::pgm::load(input, "./images/test_2.pgm");

 //   io::pgm::load(input, "./images/lena_light.pgm");
 // io::pgm::load(mverif, "./images/result_m_watershed.pgm");
 // io::pgm::load(wverif, "./images/result_topo_watershed.pgm");

  morpho::basic_najman< image2d<int_u8>, neighb2d> n(input, c4());

  /*
  image2dint::fwd_piter it(input.domain());
  for_all(it)
  {
    input(it) = input(it)/17;
    mverif(it) = mverif(it)/17;
  }
  */

  //  io::tikz::save(input, "start.tex");

  std::cout << "Building Component tree..." << std::endl;

  n.go();

  std::cout << "M-Watershed" << std::endl;

  n.m_watershed();

  io::pgm::save(n.pima, "int.pgm");

 //   io::tikz::save(n.pima, "step.tex");

#ifdef TEST

  bool equal = true;

  equal = n.pima == mverif;

  if (!equal)
    {
      std::cout << "M-Watershed broken" << std::endl;
      return 1;
    }
  else
    std::cout << "M-Watershed good as chocolate ice cream" << std::endl;

#endif

  std::cout << "W-watershed" << std::endl;

  n.w_watershed();
  // io::pgm::save(n.pima, "int.pgm");
  //  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  //  n.w_watershed();

  // for_all(it)
  //   n.pima(it) = n.pima(it) * 17;

#ifdef TEST
  equal = n.pima == wverif;

  if (!equal)
    {
      std::cout << "W-Watershed broken" << std::endl;
      return 1;
    }
  else
    std::cout << "W-Watershed good as chocolate ice cream" << std::endl;
#endif

  // io::tikz::save(n.pima, "end.tex");

  //  n.image_output(n.pima);

  io::pgm::save(n.pima, "out.pgm");

  return 0;
}
