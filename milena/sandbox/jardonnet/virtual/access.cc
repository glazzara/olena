#include <iostream>

#include "access.hh"
#include <mln/core/image/image2d.hh>
#include <mln/fun/x2x/all.hh>
#include <mln/debug/iota.hh>
#include <mln/algebra/vec.hh>

int main()
{
  using namespace mln;
  image2d<int> img(50,50);
  point2d p(5,5);


  //transformation
  fun::x2x::translation<2,float> t(make::vec(3,4));
  fun::x2x::rotation<2,float> r(90., make::vec(0,1));
  
  interpolation::nearest_neighbor< image2d<int> > nn(img);

  debug::iota(img);

  for (int i = 0; i < 50; i++)
    {
      for (int j = 0; j < 50; j++)
        std::cout << img(point2d(i,j));
      std::cout << std::endl;
    }
  
  std::cout << std::endl;
  
  for (int i = 3; i < 53; i++)
    {
      for (int j = 4; j < 54; j++)
        std::cout <<
          mln::access::access(img, point2d(i,j), t, nn);
      std::cout << std::endl;
    }
}
