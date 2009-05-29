#include <mln/core/image/image2d.hh>

#include <mln/estim/min_max.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/mean.hh>
#include <mln/data/transform.hh>

#include <mln/value/label_8.hh>
#include <mln/value/int_u16.hh>
#include <mln/data/convert.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.ppm label.pgm output.pgm" << std::endl
	    << "  Replace every label by its mean color value." << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;
  using value::label_8;
  using value::rgb8;

  if (argc != 4)
    usage(argv);


  image2d<rgb8> ima;
  io::ppm::load(ima, argv[1]);


  image2d<int_u8> lab;
  io::pgm::load(lab, argv[2]);

  image2d<int_u16> lab_ = data::convert(int_u16(), lab);

  int_u16 min, nlabels;
  estim::min_max(lab_, min, nlabels);
  
  std::cout << "n labels = " << nlabels << std::endl;

  accu::mean<rgb8> m_;
  typedef algebra::vec<3,float> V;
  util::array<V> m = labeling::compute(m_, ima, lab_, nlabels);

  util::array<rgb8> c(unsigned(nlabels) + 1);

  c[0] = rgb8(0,0,0);
  for (unsigned l = 1; l <= nlabels; ++l)
    {
      c[l].red() = m[l][0];
      c[l].green() = m[l][1];
      c[l].blue() = m[l][2];
    }

  io::ppm::save(data::transform(lab, c), argv[3]);
}
