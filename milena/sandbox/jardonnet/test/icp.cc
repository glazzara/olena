#include <mln/core/image2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <sandbox/jardonnet/registration/icp.hh>

int main(int, char*)
{
  using namespace mln;

  image2d< value::rgb8 > img;
  
  registration::icp(img,img);
}

