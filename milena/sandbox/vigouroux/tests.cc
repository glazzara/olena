#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>

#include "rgb_to_yuv.hh"
#include "my_yuv.hh"

#include "rgb_to_xyz.hh"
#include "my_xyz.hh"

#include "rgb_to_cmy.hh"
#include "my_cmy.hh"

#include "rgb_to_hsi.hh"
#include "my_hsi.hh"

int main()
{
  using namespace mln;

  struct value::yuv<8>	yuv(4, 4, 4);
  std::cout << yuv.y() << std::endl;
  std::cout << yuv.u() << std::endl;
  std::cout << yuv.v() << std::endl; 
  
  return (0);
}
