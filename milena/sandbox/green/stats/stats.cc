// Faire ressortir des statistiques sur l'image
#include <iostream>
#include <mln/accu/stat/max.hh>
#include <mln/accu/stat/variance.hh>
#include <mln/histo/compute.hh>
#include <mln/io/pgm/load.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/compute.hh>
#include <mln/value/int_u8.hh>
#include <mln/histo/array.hh>

int main()
{
  using namespace mln;

  // (1) Charger une image
  image2d<value::int_u8> in_img;
  io::pgm::load(in_img, "/usr/local/share/olena/images/small.pgm");

  value::int_u8 max = data::compute(accu::meta::stat::max(), in_img);
  float         var = data::compute(accu::stat::variance<value::int_u8>(), in_img);

  histo::array<value::int_u8> histo = histo::compute(in_img);

 // value::int_u8 max2 = data::compute(accu::meta::stat::max(), histo);

  std::cout << histo << std::endl;

  std::cout << "La valeur max est : " << max << std::endl;
  std::cout << "La valeur var est : " << var << std::endl;
  //std::cout << "La valeur max est : " << max2 << std::endl;
}
