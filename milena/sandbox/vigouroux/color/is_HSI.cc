#include <iostream>

#include <mln/metal/is_a.hh>
#include <mln/value/rgb8.hh>
#include "my_hsi.hh"
#include "rgb_to_hsi.hh"


int main()
{
//   using namespace mln;
//   using namespace mln::value;

  typedef mln::value::hsi_d C;
  std::cout << mlc_is_a(C, mln::value::HSI)::value << std::endl;
  std::cout << mlc_is_a(mln::value::rgb8, mln::value::HSI)::value << std::endl;

  mln::value::rgb8 c(255, 10, 1);
  mln::value::rgb8 other(10, 20, 30);

  // Essai en 0 255
  
//   hsi_3x8 c_hsi = fun::v2v::f_rgb_to_hsi_3x8(c);
//   rgb8 c2 = fun::v2v::f_hsi_to_rgb_3x8(c_hsi);
//   std::cout << "c = " << c << std::endl;
//   std::cout << "c2 = " << c2 << std::endl;

//   hsi_3x8 c_cool = fun::v2v::f_rgb_to_hsi_3x8(other);
//   rgb8 coucou = fun::v2v::f_hsi_to_rgb_3x8(c_cool);

//   std::cout << "c = " << other << std::endl;
//   std::cout << "c2 = " << coucou << std::endl;
}
