#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

#include "load.hh"

int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;

  //io::magick::load(lena, "/Users/HiSoKa/Work/LRDE/Olena/resources/CardiacCT/178562160.dcm");
  io::magick::load(lena, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm");
}
