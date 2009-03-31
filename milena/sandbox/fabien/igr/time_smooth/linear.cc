#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/cast_image.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/accu/median_h.hh>
#include <mln/util/array.hh>


using namespace mln;
using value::int_u12;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << "input" << std::endl;
    return 1;
  }

  ///////////////////
  //		   //
  // Image loading //
  //		   //
  ///////////////////
  image3d<int_u12> input;
  io::dicom::load(input, argv[1]);
  image2d<util::array<int_u12> > ima_arr(input.nrows(), input.ncols());
  for (int i = 0; i < input.nslices(); ++i)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<int_u12>) p(tmp_slice.domain());
    for_all(p)
    {
      ima_arr(p).append(tmp_slice(p));
    }
  }

  ////////////////////////
  //			//
  // Linear convolution //
  //			//
  ////////////////////////
  image2d<util::array<int_u12> > ima_linear;
  initialize(ima_linear, ima_arr);
  mln_piter_(image2d<int_u12>) p(ima_linear.domain());
  accu::median_h<int_u12> accu_med;
  for_all(p)
  {
    ima_linear(p).append(ima_arr(p)[0]);
    for (unsigned i = 1; i < ima_arr(p).nelements() - 1; ++i)
      ima_linear(p).append(0.25 * ima_arr(p)[i - 1] + 0.5 * ima_arr(p)[i] + 0.25 * ima_arr(p)[i + 1]);
    ima_linear(p).append(ima_arr(p)[ima_arr(p).nelements() - 1]);
  }

  /////////////
  //	     //
  // Outputs //
  //	     //
  /////////////
  io::plot::save(ima_arr(point2d(160, 120)), "ref_tumeur.plot");
  io::plot::save(ima_linear(point2d(160, 120)), "linear_tumeur.plot");
  io::plot::save(ima_arr(point2d(34, 94)), "ref_air.plot");
  io::plot::save(ima_linear(point2d(34, 94)), "linear_air.plot");
  io::plot::save(ima_arr(point2d(122, 115)), "ref_poumon.plot");
  io::plot::save(ima_linear(point2d(122, 115)), "linear_poumon.plot");

  return 0;
}
