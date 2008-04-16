#include <mln/core/image2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/p_set.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/compare.hh>

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

int main ()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> image2dint;

  image2dint input, mverif;
  io::pgm::load(input, "./images/test_watershed.pgm");
  // io::pgm::load(input, "./images/lena.pgm");
  io::pgm::load(mverif, "./images/result_m_watershed.pgm");

  morpho::basic_najman< image2d<int_u8>, neighb2d> n(input, c4());
  n.go();
  n.m_watershed();

  bool equal = true;

   equal = n.pima == mverif;

    if (!equal)
    {
      std::cout << "M-Watershed broken" << std::endl;
      return 1;
    }
  else
    std::cout << "M-Watershed good as chocolate ice cream" << std::endl;

    //    n.w_watershed();

  io::pgm::save(n.pima, "out.pgm");

  return 0;
}
