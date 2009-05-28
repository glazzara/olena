#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/accu/median_h.hh>
#include <mln/linear/convolve.hh>
#include <mln/make/w_window2d.hh>
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

  typedef float I;

  ///////////////////
  //		   //
  // Image loading //
  //		   //
  ///////////////////
  image3d<I> input;
  io::dump::load(input, argv[1]);
  util::array<image2d<I> > arr_ima;
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<I> tmp_slice = duplicate(slice(input, i));
    arr_ima.append(tmp_slice);
  }

  ////////////////////////
  //			//
  // Linear convolution //
  //			//
  ////////////////////////
  float ws[] = {   0.f, 0.125,   0.f,
		 0.125,	  0.5, 0.125,
		   0.f, 0.125,   0.f };

  util::array<image2d<float> > ima_linear;
  for (unsigned i = 0; i < arr_ima.nelements(); ++i)
    ima_linear.append(linear::convolve(arr_ima[i], make::w_window2d(ws)));

  /////////////
  //	     //
  // Outputs //
  //	     //
  /////////////
  image2d<util::array<float> > ima_result;
  initialize(ima_result, slice(input, 0));
  mln_piter_(image2d<util::array<float> >) p(ima_linear[0].domain());
  for_all(p)
    for (unsigned int i = 0; i < ima_linear.nelements(); ++i)
      ima_result(p).append(ima_linear[i](p));

  io::plot::save(ima_result(point2d(156, 114)), "linear_tumeur.plot");
  io::plot::save(ima_result(point2d(54, 94)), "linear_air.plot");
  io::plot::save(ima_result(point2d(122, 115)), "linear_poumon.plot");

  return 0;
}
