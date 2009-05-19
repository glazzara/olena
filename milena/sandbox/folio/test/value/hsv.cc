#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/debug/println.hh>

#include <mln/level/transform.hh>
#include <mln/core/image/image2d.hh>
#include "../../mln/fun/v2v/rgb_to_hsv.hh"
#include "../../mln/value/circular.hh"

int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::value;

  if (argc != 3)
  {
    std::cout << "Usage:" << std::endl
	      << "./a.out <ima_in> <ima_out>" << std::endl;
  }


  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<value::rgb8> ima;
  io::ppm::load(ima, argv[1]);

  image2d<hsv_16f> hsv = level::transform(ima,
					  fun::v2v::f_rgb_to_hsv_16f);
  ima = level::transform(hsv,
			 fun::v2v::f_hsv_to_rgb_8f);

  std::cout << "  => saving " << argv[2] << "..." << std::endl;
  io::ppm::save(ima, argv[2]);

}
