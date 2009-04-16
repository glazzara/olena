#include <mln/core/image/image2d.hh>

#include <mln/estim/min_max.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/mean.hh>
#include <mln/level/transform.hh>

#include <mln/value/label_8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>





int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;
  using value::rgb8;

  image2d<int_u8> lab;
  io::pgm::load(lab, argv[1]);

  int_u8 min, nlabels;
  estim::min_max(lab, min, nlabels);
  
  std::cout << min << ' ' << nlabels << std::endl;

  image2d<rgb8> ima;
  io::ppm::load(ima, argv[2]);

  accu::mean<rgb8> m_;
  typedef algebra::vec<3,float> V;
  util::array<V> m = labeling::compute(m_, ima, lab, nlabels);

  util::array<rgb8> c(nlabels + 1);
  for (unsigned l = 0; l <= nlabels; ++l)
    {
      c[l].red() = m[l][0];
      c[l].green() = m[l][1];
      c[l].blue() = m[l][2];
    }

  io::ppm::save(level::transform(lab, c), argv[3]);
}
