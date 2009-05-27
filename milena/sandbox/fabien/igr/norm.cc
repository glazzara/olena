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
#include <mln/core/var.hh>
#include <mln/estim/min_max.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/stretch.hh>
#include <mln/value/int_u8.hh>
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

  // Debug.
  //io::pgm::save(level::stretch(value::int_u8(), ima_ini), "mean_slices.pgm");

  image3d<float> ima_result;
  initialize(ima_result, ima);

  for (int i = min_sli; i <= max_sli; ++i)
    data::paste((slice(ima_f, i) - ima_ini) / ima_ini, slice(ima_result, i).rw());

  return ima_result;
}


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    std::cout << "Usage: " << argv[0] << " ima.dump first_slice last_slice output.dump"
	      << std::endl;
    return 1;
  }

  image3d<float> ima;
  io::dump::load(ima, argv[1]);
  image3d<float> norm = normalize(ima, atoi(argv[2]), atoi(argv[3]));
  io::dump::save(norm, argv[4]);

  return 0;
}
