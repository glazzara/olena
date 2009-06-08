#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/accu/stat/median_h.hh>
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

  ////////////
  //	    //
  // Median //
  //	    //
  ////////////
  image2d<util::array<int_u12> > ima_median3;
  image2d<util::array<int_u12> > ima_median5;
  initialize(ima_median3, ima_arr);
  initialize(ima_median5, ima_arr);
  mln_piter_(image2d<int_u12>) p(ima_median3.domain());
  accu::stat::median_h<int_u12> accu_med;
  for_all(p)
  {
    // Median 3
    ima_median3(p).append(ima_arr(p)[0]);
    for (unsigned i = 1; i < ima_arr(p).nelements() - 1; ++i)
    {
      accu_med.init();
      accu_med.take(ima_arr(p)[i - 1]);
      accu_med.take(ima_arr(p)[i]);
      accu_med.take(ima_arr(p)[i + 1]);
      ima_median3(p).append(accu_med.to_result());
    }
    ima_median3(p).append(ima_arr(p)[ima_arr(p).nelements() - 1]);

    // Median 5
    ima_median5(p).append(ima_arr(p)[0]);
    ima_median5(p).append(ima_arr(p)[1]);
    for (unsigned i = 2; i < ima_arr(p).nelements() - 2; ++i)
    {
      accu_med.init();
      accu_med.take(ima_arr(p)[i - 2]);
      accu_med.take(ima_arr(p)[i - 1]);
      accu_med.take(ima_arr(p)[i]);
      accu_med.take(ima_arr(p)[i + 1]);
      accu_med.take(ima_arr(p)[i + 2]);
      ima_median5(p).append(accu_med.to_result());
    }
    ima_median5(p).append(ima_arr(p)[ima_arr(p).nelements() - 2]);
    ima_median5(p).append(ima_arr(p)[ima_arr(p).nelements() - 1]);
  }

  /////////////
  //	     //
  // Outputs //
  //	     //
  /////////////
  io::plot::save(ima_median3(point2d(160, 120)), "median3_tumeur.plot");
  io::plot::save(ima_median3(point2d(34, 94)), "median3_air.plot");
  io::plot::save(ima_median3(point2d(122, 115)), "median3_poumon.plot");
  io::plot::save(ima_median5(point2d(160, 120)), "median5_tumeur.plot");
  io::plot::save(ima_median5(point2d(34, 94)), "median5_air.plot");
  io::plot::save(ima_median5(point2d(122, 115)), "median5_poumon.plot");

  return 0;
}
