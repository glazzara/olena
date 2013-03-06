#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/pw/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/neighb.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/extension/adjust.hh>
#include <mln/border/mirror.hh>
#include <mln/extension/duplicate.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/debug/println.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2b/threshold.hh>
#include <mln/io/dump/save.hh>

mln::image2d<double> skewness;

#include <scribo/binarization/sauvola.hh>

namespace mln
{

  template <typename T, typename N>
  std::pair<image2d<double>, image2d<double> >
  init_(const image2d<T>& ima, const N& nbh)
    {
      image2d<double> output_2, output_3;
      initialize(output_2, ima);
      initialize(output_3, ima);
      extension::adjust(ima, nbh);
      border::mirror(ima);

      typedef image2d<T> I;
      mln_piter(I) p(ima.domain());
      mln_niter(N) n(nbh, p);

      extension::duplicate(ima);

      for_all(p)
      {
	double sum = ima(p);
	for_all(n)
	    sum += ima(n);

	double mean = sum / (double)nbh.size();
	double
	  res_2 = std::pow(ima(p) - mean, 2),
	  res_3 = std::pow(ima(p) - mean, 3);
	for_all(n)
	{
	  res_2 += std::pow(ima(n) - mean, 2);
	  res_3 += std::pow(ima(n) - mean, 3);
	}

	output_2(p) = res_2;
	output_3(p) = res_3;
      }

      return std::pair<image2d<double>, image2d<double> >(output_2, output_3);
    }


}



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <input.pgm> <prefix> <win_size>" << std::endl;
    return 1;
  }

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);
  std::string prefix = argv[2];

  int win_size = atoi(argv[3]);
  win::rectangle2d wr = win::rectangle2d(win_size,win_size);
  window2d win;
  win.insert(wr);

  std::pair<image2d<double>, image2d<double> > res_init = init_(input, neighb<window2d>(win));

  image2d<double> x_mean_2 = res_init.first;
  image2d<double> x_mean_3 = res_init.second;

  {
    io::pgm::save(data::stretch(value::int_u8(), x_mean_2), prefix + "x_mean_2_stretch.pgm");
    io::pgm::save(data::stretch(value::int_u8(), x_mean_3), prefix + "x_mean_3_stretch.pgm");
  }


  initialize(skewness, x_mean_2);
  data::fill(skewness, 0);

  int n;
  mln_piter_(image2d<double>) p(skewness.domain());
  for_all(p)
  {
    n = win.size();
    double frac = std::pow(std::sqrt((1.0/(double)n * x_mean_2(p))), 3);
    skewness(p) = (std::sqrt(n * (n - 1)) / (n - 2)) * ((1.0/(double)n * x_mean_3(p)) / frac);

    // if (p == point2d(5, 5))
    //   std::cout << p << " - " << std::setprecision(8) << (double)x_mean_3(p) << " - " << (double)x_mean_2(p) << " - " << skewness(p) << std::endl;
  }


  {
    io::dump::save(skewness, prefix + "skewness.dump");
    io::pgm::save(data::convert(value::int_u8(), skewness), prefix + "skewness_u8.pgm");
    io::pbm::save(data::transform(skewness, fun::v2b::threshold<double>(0)), prefix + "skewness.pbm");
    io::pgm::save(data::stretch(value::int_u8(), skewness), prefix + "skewness_stretch.pgm");
  }

//  image2d<bool> bin = scribo::binarization::sauvola(input, 51);
//  io::pbm::save(bin, "input.pbm");
}
