#include <iostream>

#include <mln/metal/is_a.hh>
#include <mln/value/rgb8.hh>
#include "my_hsi.hh"
#include "rgb_to_hsi.hh"


int main()
{
  using namespace mln;
  using namespace mln::value;

  typedef hsi_d C;
  std::cout << mlc_is_a(C, HSI)::value << std::endl;
  std::cout << mlc_is_a(rgb8, HSI)::value << std::endl;

  rgb8 c(255, 10, 1);
  hsi_3x8 c_hsi = fun::v2v::f_rgb_to_hsi_3x8(c);
}
