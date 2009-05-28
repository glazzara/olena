#include <iostream>
#include <cmath>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/plot/save.hh>

#include <mln/value/int_u12.hh>

#include <mln/histo/array.hh>
#include <mln/histo/compute.hh>
#include <mln/level/transform.hh>


namespace mln
{
  struct int_u12_from_float : Function_v2v< int_u12_from_float >
  {
    int_u12_from_float(float min, float max)
    {
      min_ = min;
      max_ = max;
    }

    typedef value::int_u12 result;
    float min_;
    float max_;

    result operator()(float f) const
    {
      if (f > max_)
	f = max_;
      f -= min_;
      f /= (max_ - min_);
      f *= 4095; // int_u12 max

      return (int) floor(f);
    }
  };

} // end of namespace mln


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " ima.dump"
	      << std::endl;
    return 1;
  }

  float min = 1;
  float max = -1;

  image3d<float> input;
  io::dump::load(input, argv[1]);

  mln_piter_(image3d<float>) p(input.domain());
  for_all(p)
  {
    if (input(p) > max)
      max = input(p);
    if (input(p) < min)
      min = input(p);
  }

  std::cout << "min: " << min << std::endl;
  std::cout << "max: " << max << std::endl;

  /*image3d<value::int_u12> quantified = level::transform(input, int_u12_from_float(min, max));

  histo::array<value::int_u12> histogram = histo::compute(quantified);
  image1d<value::int_u12> hist_ima;
  convert::from_to(histogram, hist_ima);
  io::plot::save(hist_ima, "histogram.plot");*/

  return 0;
}
