/// TEST HISTO1D

#include <mln/accu/stat/histo1d.hh>
#include <mln/data/compute.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/img_path.hh>
#include <mln/io/pgm/load.hh>
#include <mln/value/int_u8.hh>

int main()
{
  typedef mln::value::int_u8 t_int_u8;
  mln::image2d<t_int_u8>     img;
  mln::image1d<unsigned>     histo;

  mln::io::pgm::load(img, OLENA_IMG_PATH"/lena.pgm");
  histo = mln::data::compute(mln::accu::meta::stat::histo1d(), img);

  return 0;
}
