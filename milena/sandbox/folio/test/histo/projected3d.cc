#include <mln/core/var.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/unproject_image.hh>
#include <mln/fun/v2v/projection.hh>

#include <mln/core/image/image_if.hh>
#include <mln/pw/value.hh>
#include <mln/level/transform.hh>

#include <mln/arith/revert.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/value/label_8.hh>

#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>

#include "../../mln/histo/compute_histo_rgb.hh"
#include "../../mln/histo/classify_with_histo_rgb.hh"

#include <mln/accu/count.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/debug/println.hh>

namespace mln
{
  struct rgb8to6 : Function_v2v< rgb8to6 >
  {
    typedef value::rgb<6> result;
    value::rgb<6> operator()(const value::rgb<8>& c) const
    {
      value::rgb<6> res(c.red() / 4, c.green() / 4, c.blue() / 4);
      return res;
    }
  };
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage:" << std::endl
	      << "./a.out <ima_in> <ima_out>" << std::endl;
  }

  using namespace mln;

  using value::int_u8;
  typedef value::rgb<6> rgb6;
  typedef value::int_u<6> int_u6;

  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<value::rgb8> ima;
  io::ppm::load(ima, argv[1]);
  image2d<rgb6> ima6 = level::transform(ima, rgb8to6());

  std::cout << "  => computing histogram..." << std::endl;
  image3d<unsigned> histo = histo::compute_histo_rgb<unsigned>(ima6);

  std::cout << "  => computing reverted histogram..." << std::endl;
  image3d<unsigned> reverted = arith::revert(histo);

  std::cout << "  => computing closure..." << std::endl;
  image3d<unsigned> closed =
    morpho::closing::volume(reverted, c6(), atoi(argv[2]));

  std::cout << "  => computing watershed..." << std::endl;
  value::label_8 nbasin;
  image3d<value::label_8> labels =
    morpho::watershed::flooding(closed, c6(), nbasin);
  std::cout << "found " << nbasin << " labels" << std::endl;

  labels = morpho::elementary::dilation(labels, c18());

  std::cout << "  => computing output labelized image..." << std::endl;
  image2d<value::label_8> lab = histo::classify_with_histo_rgb(ima, labels);

  std::cout << "  => computing projection..." << std::endl;
  typedef accu::mean<int_u8, unsigned, int_u8> A;

  image2d<A> vmean(lab.nrows(), lab.ncols());
  accu::image::init(vmean);
  {
    fun::v2v::projection<point3d, 0> vproj;
    mln_VAR( vmean_, unproject(vmean, lab.domain(), vproj) );
    accu::image::take(vmean_, lab);
  }

  std::cout << "  => saving " << argv[2] << "..." << std::endl;
  io::ppm::save(vmean, argv[2]);

}
