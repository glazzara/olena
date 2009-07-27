#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/fun/v2v/projection.hh>
#include <mln/core/image/dmorph/unproject_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

#include <mln/accu/stat/mean.hh>
#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>
#include <mln/accu/image/to_result.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save.hh>

#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>

#include <mln/linear/gaussian_1d.hh>
#include <mln/data/fill.hh>
#include <mln/data/convert.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/draw/line.hh>
#include <mln/literal/colors.hh>

#include <mln/accu/stat/median_h.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm output.ppm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 3)
    usage(argv);

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);
  box2d b = input.domain();

  typedef accu::stat::mean<int_u8, unsigned, double> A;
    
  image1d<A> hmean(b.nrows());
  accu::image::init(hmean);

  fun::v2v::projection<point2d, 1> hproj;
  accu::image::take(unproject(hmean, b, hproj).rw(), input);

  image1d<double> hmean_d = accu::image::to_result(hmean);
  io::plot::save(hmean_d, "temp_h.txt");

  hmean_d = linear::gaussian_1d(hmean_d, 5, 255);
  io::plot::save(hmean_d, "temp_hg.txt");

  io::plot::save(hmean, argv[2]);

  int_u8 n;
  image1d<int_u8> lab = labeling::regional_minima(hmean_d, c2(), n);
  // io::plot::save(lab, "temp_lab.txt");


  {
    image2d<rgb8> cool = data::convert(rgb8(), input);
    mln_piter_(box1d) p(lab.domain());
    for_all(p)
      if (lab(p) != 0)
	draw::line(cool,
		   point2d(p.ind(), 0),
		   point2d(p.ind(), b.ncols() -1),
		   literal::red);
    io::ppm::save(cool, argv[2]);
  }

//   accu::stat::median_h<int_u8> med;
//   {
//     double min = 255;
//     unsigned row_min;
//     int last = -1;
//     mln_piter_(box1d) p(lab.domain());
//     for_all(p)
//     {
//       if (lab(p) == 0)
// 	continue;
//       if (hmean_d(p) < min)
// 	{
// 	  min = hmean_d(p);
// 	  row_min = p.ind();
// 	}
//       if (last == -1)
// 	last = p.ind();
//       else
// 	{
// 	  med.take(p.ind() - last);
// 	  last = p.ind();
// 	}
//     }
//     std::cout << med << ' ' << row_min << std::endl;
//   }

}
