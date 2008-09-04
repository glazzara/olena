#include <mln/value/hsi.hh>
#include <mln/fun/v2v/rgb_to_hsi.hh>

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>

#include <iostream>

int main(void)
{
  std::cout << "GREEN" << std::endl;
  mln::value::rgb8 rgb1(0,255,0);
  mln::value::hsi_f hsi1 = mln::fun::v2v::f_rgb_to_hsi_f(rgb1);

  std::cout << "hue = " << hsi1.hue() << std::endl;
  std::cout << "sat = " << hsi1.sat() << std::endl;
  std::cout << "inty = " << hsi1.inty() << std::endl;

  std::cout << "RED" << std::endl;
  mln::value::rgb8 rgb2(255,0,0);
  mln::value::hsi_f hsi2 = mln::fun::v2v::f_rgb_to_hsi_f(rgb2);

  std::cout << "hue = " << hsi2.hue() << std::endl;
  std::cout << "sat = " << hsi2.sat() << std::endl;
  std::cout << "inty = " << hsi2.inty() << std::endl;
}

