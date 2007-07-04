#include <oln/core/2d/image2d.hh>
#include <oln/core/3d/image3d.hh>
#include <oln/core/1d/image1d.hh>
#include <oln/level/fill.hh>


using namespace oln;

int main(void)
{
  image1d<char> ima1d1(5);
  image1d<int> ima1d2(5);
  image1d<float> ima1d3(5);
  image1d<double> ima1d4(5);

  level::fill(inplace(ima1d1), 5);
  level::fill(inplace(ima1d2), 5);
  level::fill(inplace(ima1d3), 5);
  level::fill(inplace(ima1d4), 5);

  image2d<char> ima2d1(5, 5);
  image2d<int> ima2d2(5, 5);
  image2d<float> ima2d3(5, 5);
  image2d<double> ima2d4(5, 5);

  level::fill(inplace(ima2d1), 5);
  level::fill(inplace(ima2d2), 5);
  level::fill(inplace(ima2d3), 5);
  level::fill(inplace(ima2d4), 5);

  image3d<char> ima3d1(5, 5, 5);
  image3d<int> ima3d2(5, 5, 5);
  image3d<float> ima3d3(5, 5, 5);
  image3d<double> ima3d4(5, 5, 5);

  level::fill(inplace(ima3d1), 5);
  level::fill(inplace(ima3d2), 5);
  level::fill(inplace(ima3d3), 5);
  level::fill(inplace(ima3d4), 5);

  return 0;
}
