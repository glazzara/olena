#include <mln/core/image3d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <sandbox/jardonnet/registration/icp.hh>

int main(int, char*)
{
  using namespace mln;

  image3d< value::rgb8 > img;
  
  registration::icp(img,img);
}

