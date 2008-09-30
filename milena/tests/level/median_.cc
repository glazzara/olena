#include <mln/core/image/image2d.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/make/box2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/median.hh>

#include "tests/data.hh"

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  {
    border::thickness = 5;
    box2d bb = make::box2d(0, 0, 3, 3);
    image2d<int_u8> ima(bb);
    debug::iota(ima);
    debug::println(ima);

    win::rectangle2d rect(3, 3);
    image2d<int_u8> out(ima.domain());

    level::impl::median_(ima, rect, out);
  }


  {
    win::rectangle2d rect(20, 20);
    border::thickness = 6;

    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
    image2d<int_u8> out(lena.domain());

    level::impl::median_(lena, rect, out);
    io::pgm::save(out, "out.pgm");
  }

}
