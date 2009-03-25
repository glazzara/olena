#include <mln/core/image/image2d.hh>

#include <mln/io/magick/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

using namespace mln;

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 1;

  image2d<value::rgb8> ima;
  io::magick::load(ima, argv[1]);
  io::magick::save(ima, "out_magick.png");
  io::ppm::save(ima, "out_pnm.ppm");
  return 0;
}
