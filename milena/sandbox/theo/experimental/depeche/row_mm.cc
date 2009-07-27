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
// #include <mln/draw/line.hh>
// #include <mln/literal/colors.hh>

#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/opening/volume.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/data/convert.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 4)
    usage(argv);

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);
  box2d b = input.domain();

  int lambda = std::atoi(argv[2]);

  typedef accu::stat::mean<int_u8, unsigned, int_u8> A;
  
  image1d<A> hmean(b.nrows());
  accu::image::init(hmean);

  fun::v2v::projection<point2d, 1> hproj;
  accu::image::take(unproject(hmean, b, hproj).rw(), input);

  image1d<int_u8> h = accu::image::to_result(hmean);
  io::plot::save(h, "temp_h0.txt");
  
  h = morpho::closing::volume(h, c2(), lambda);
  io::plot::save(h, "temp_h1.txt");

  h = morpho::opening::volume(h, c2(), lambda);
  io::plot::save(h, "temp_h2.txt");


  {
    int_u8 n;
    image1d<int_u8>
      lab_bg = labeling::regional_maxima(h, c2(), n),
      lab_fg = labeling::regional_minima(h, c2(), n);

    image2d<rgb8> cool = data::convert(rgb8(), input);
    mln_piter_(box2d) p(cool.domain());
    for_all(p)
    {
      if (lab_bg.at_(p.row()) != 0)
	cool(p).red() = 255; 
      if (lab_fg.at_(p.row()) != 0)
	cool(p).green() = 255; 
    }
    io::ppm::save(cool, "temp_cool.ppm");
  }

  // io::pgm::save(output, argv[2]);
}
