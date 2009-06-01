#include <mln/core/var.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/unproject_image.hh>
#include <mln/fun/v2v/projection.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>
#include <mln/data/transform.hh>
#include <mln/data/stretch.hh>

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
#include <mln/accu/sum.hh>
#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>
#include <mln/accu/image/to_result.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
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

  struct take_log : Function_v2v< take_log >
  {
    typedef float result;
    float operator()(float f) const
    {
      mln_precondition(f > 0);
      return std::log(f + 1);
    }
  };

  template <typename A, unsigned direction, typename V>
  image2d<float>
  project_histo(const image3d<V>& h)
  {
    image2d<A> h_2d_a(h.nrows(), h.ncols());
    accu::image::init(h_2d_a);

    accu::image::take( unproject( h_2d_a,
				  h.domain(),
				  fun::v2v::projection<point3d, direction>() ).rw(),
		       h );

    return accu::image::to_result(h_2d_a);
  }

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
//  image2d<rgb6> ima6 = data::transform(ima, rgb8to6());

  std::cout << "  => computing histogram..." << std::endl;
  image3d<unsigned> histo = histo::compute_histo_rgb<unsigned>(ima);

  image2d<float> proj = project_histo<accu::sum<unsigned>, 2>(histo);
  image2d<value::int_u8> proj_int = data::stretch( value::int_u8(),
						   data::transform( proj,
								    take_log() ) );
  io::pgm::save(proj_int, argv[2]);

//   std::cout << "  => computing reverted histogram..." << std::endl;
//   image3d<unsigned> reverted = arith::revert(histo);

//   std::cout << "  => computing closure..." << std::endl;
//   image3d<unsigned> closed =
//     morpho::closing::volume(reverted, c6(), atoi(argv[2]));

//   std::cout << "  => computing watershed..." << std::endl;
//   value::label_8 nbasin;
//   image3d<value::label_8> labels =
//     morpho::watershed::flooding(closed, c6(), nbasin);
//   std::cout << "found " << nbasin << " labels" << std::endl;

//   labels = morpho::elementary::dilation(labels, c18());

//   std::cout << "  => computing output labelized image..." << std::endl;
//   image2d<value::label_8> lab = histo::classify_with_histo_rgb(ima, labels);

//   std::cout << "  => computing projection..." << std::endl;

//   typedef accu::mean<int_u8, unsigned, int_u8> A;
//   image2d<A> vmean(lab.nrows(), lab.ncols());
//   accu::image::init(vmean);
//   {
//     fun::v2v::projection<point3d, 0> vproj;
//     mln_VAR( vmean_, unproject(vmean, lab.domain(), vproj) );
//     accu::image::take(vmean_, lab);
//   }

//   std::cout << "  => saving " << argv[2] << "..." << std::endl;
//   io::ppm::save(vmean, argv[2]);

}
