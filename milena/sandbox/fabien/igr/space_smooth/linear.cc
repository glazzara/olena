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

  ///////////////////
  //		   //
  // Image loading //
  //		   //
  ///////////////////
  image3d<int_u12> input;
  io::dicom::load(input, argv[1]);
  util::array<image2d<int_u12> > arr_ima;
  for (int i = 0; i < input.nslices(); ++i)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(input, i));
    arr_ima.append(tmp_slice);
  }

  ////////////////////////
  //			//
  // Linear convolution //
  //			//
  ////////////////////////
  float ws[] = {   0, 1/8,   0,
		 1/8, 1/2, 1/8,
		   0, 1/8,   0 };

  util::array<image2d<float> > ima_linear;
  for (unsigned i = 1; i < arr_ima.nelements(); ++i)
    ima_linear.append(linear::convolve(arr_ima[i], make::w_window2d(ws)));

  /////////////
  //	     //
  // Outputs //
  //	     //
  /////////////
  image2d<util::array<float> > ima_result(input.nrows(), input.ncols());
  mln_piter_(image2d<util::array<float> >) p(ima_linear[0].domain());
  for_all(p)
    for (int i = 0; i < ima_linear.nelements(); ++i)
      ima_result(p).append(ima_linear[i](p));

  io::plot::save(ima_result(point2d(160, 120)), "linear_tumeur.plot");
  io::plot::save(ima_result(point2d(34, 94)), "linear_air.plot");
  io::plot::save(ima_result(point2d(122, 115)), "linear_poumon.plot");

  return 0;
}
