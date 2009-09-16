#include <mln/core/image/image2d.hh>
#include <mln/core/image/tiled2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>

#include <mln/debug/quiet.hh>


using namespace mln;
using value::rgb8;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input" << std::endl;
    return 1;
  }

  //image2d<rgb8> ima;
  tiled2d<rgb8> tiled_ima;

  //io::ppm::load(ima, argv[1]);
  io::ppm::load(tiled_ima, argv[1]);

  //std::cout << "bbox: " << tiled_ima.bbox() << std::endl;
  //std::cout << "file: " << tiled_ima.file_() << std::endl;

  /*point2d pt0(0, 0);
  mln_assertion(tiled_ima(pt0) == ima(pt0));

  point2d pt(0, 1);
  mln_assertion(tiled_ima(pt) == ima(pt));

  point2d pt2(1, 0);
  mln_assertion(tiled_ima(pt2) == ima(pt2));

  point2d pt3(1, 1);
  mln_assertion(tiled_ima(pt3) == ima(pt3));*/

  mln_piter_(tiled2d<rgb8>) p(tiled_ima.domain());
  for_all(p)
    if (p.row() % 7 == 0)
    {
      //std::cout << tiled_ima(p);
      tiled_ima(p) = literal::green;
      //std::cout << " -> " << tiled_ima(p) << std::endl;
      //mln_assertion(tiled_ima(p) == ima(p));
    }

  /*for_all(p)
    if (p.col() % 16 == 0)
      tiled_ima(p) = literal::purple;*/

  return 0;
}
