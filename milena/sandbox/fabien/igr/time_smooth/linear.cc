#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/cast_image.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/plot/save.hh>

#include <mln/accu/median_h.hh>
#include <mln/util/array.hh>
#include <mln/linear/convolve.hh>
#include <mln/make/w_window3d.hh>


using namespace mln;
using value::int_u12;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << "input" << std::endl;
    return 1;
  }

  typedef float I;

  ///////////////////
  //		   //
  // Image loading //
  //		   //
  ///////////////////
  image3d<I> input;
  io::dump::load(input, argv[1]);
  image2d<util::array<I> > ima_arr;
  initialize(ima_arr, duplicate(slice(input, 0)));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<I> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<I>) p(tmp_slice.domain());
    for_all(p)
    {
      ima_arr(p).append(tmp_slice(p));
    }
  }
  /*image3d<I> output;
  initialize(output, input);*/

  ////////////////////////
  //			//
  // Linear convolution //
  //			//
  ////////////////////////
  /*float ws[][] = {{0, 0,    0,
		   0, 0.25, 0,
		   0, 0,    0},
		  {0, 0,    0,
		   0, 0.5,  0,
		   0, 0,    0},
		  {0, 0,    0,
		   0, 0.25, 0,
		   0, 0,    0}};

  output = linear::convolve(input, make::w_window3d(ws));*/
  image2d<util::array<I> > ima_linear;
  initialize(ima_linear, ima_arr);
  mln_piter_(image2d<I>) p(ima_linear.domain());
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
  //io::dump::save(output, "time_linear.dump");
  io::plot::save(ima_arr(point2d(156, 114)), "ref_tumeur.plot");
  io::plot::save(ima_linear(point2d(156, 114)), "linear_tumeur.plot");
  io::plot::save(ima_arr(point2d(54, 94)), "ref_air.plot");
  io::plot::save(ima_linear(point2d(54, 94)), "linear_air.plot");
  io::plot::save(ima_arr(point2d(122, 115)), "ref_poumon.plot");
  io::plot::save(ima_linear(point2d(122, 115)), "linear_poumon.plot");

  return 0;
}
