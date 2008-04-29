#include <mln/core/image2d.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>

#include "io/tikz/save.hh"

int main () {
  using namespace mln;
  using value::rgb;

  typedef image2d< rgb<8> > image2drgb;

  image2drgb input;

  io::ppm::load(input, "../img/lena.ppm");

  io::tikz::save(input, "tex/lena.tex");

  typedef image2d< value::int_u<4> > image2dint4;
  typedef image2d< value::int_u<8> > image2dint8;

  image2dint8 input2;

  io::pgm::load(input2, "../img/test.pgm");

  image2dint4 input3 (input2.domain(), input2.border());
  image2dint8::fwd_piter p(input2.domain());

  for_all(p)
    input3(p) = input2(p);

  io::tikz::save(input3, "tex/test.tex");

  return 0;
} // int main ()
