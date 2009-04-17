#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dump/all.hh>

#include <mln/value/int_u12.hh>

#include <mln/accu/image/all.hh>
#include <mln/accu/mean.hh>
#include <mln/arith/all.hh>
#include <mln/data/paste.hh>

// DEBUG
/*#include <mln/accu/min.hh>
#include <mln/accu/max.hh>
#include <mln/level/compute.hh>*/

using namespace mln;
using value::int_u12;



template <typename I>
inline
image2d<float> mean_slices(Image<I>& ima, int first, int last)
{
  image2d<accu::mean<float> > result;

  initialize(result, slice(ima, first));

  accu::image::init(result);
  for (int i = first; i <= last; ++i)
    accu::image::take(result, slice(ima, i));

  return accu::image::to_result(result);
}

template <typename I>
inline
image3d<float> normalize(Image<I>& ima, int first, int last)
{

  int min_sli = geom::bbox(ima).pmin().sli();
  int max_sli = geom::bbox(ima).pmax().sli();

  mln_precondition(first >= min_sli && first <= max_sli);
  mln_precondition(last >= min_sli && last <= max_sli);

  image3d<float> ima_f;
  initialize(ima_f, ima);
  data::fill(ima_f, ima);

  image2d<float> ima_ini = mean_slices(ima_f, first, last);

  image3d<float> ima_result;
  initialize(ima_result, ima_f);

  for (int i = min_sli; i <= max_sli; ++i)
    data::paste((slice(ima_f, i) - ima_ini), slice(ima_result, i).rw());

  // DEBUG
  /*accu::min<float> accu_min;
  accu::max<float> accu_max;
  float min = level::compute(accu_min, ima_ini);
  float max = level::compute(accu_max, ima_ini);
  std::cout << "min = " << min << std::endl;
  std::cout << "max = " << max << std::endl;*/

  return ima_result;
}


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " ima.dump output.dump"
	      << std::endl;
    return 1;
  }

  image3d<int_u12> ima;
  io::dump::load(ima, argv[1]);
  image3d<float> norm = normalize(ima, 1, 8);
  io::dump::save(norm, argv[2]);

  return 0;
}
