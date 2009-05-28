#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/accu/median_h.hh>
#include <mln/core/alias/window2d.hh>
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

  ////////////
  //	    //
  // Median //
  //	    //
  ////////////
  window2d win_c12p;
  win_c12p
    .insert(-1, -1)
    .insert( 0, -1)
    .insert(+1, -1)
    .insert(-1,  0)
    .insert( 0,  0)
    .insert(+1,  0)
    .insert(-1, +1)
    .insert( 0, +1)
    .insert(+1, +1)
    .insert(+2, +2)
    .insert(-2, -2)
    .insert(+2, -2)
    .insert(-2, +2);
  window2d win_c4 = win_c4p();
  util::array<image2d<int_u12> > ima_median3;
  util::array<image2d<int_u12> > ima_median5;
  accu::median_h<int_u12> accu_med3;
  accu::median_h<int_u12> accu_med5;
  image2d<int_u12> tmp;
  initialize(tmp, arr_ima[0]);
  mln_piter_(image2d<int_u12>) p(arr_ima[0].domain());
  mln_qiter_(window2d) q3(win_c4, p);
  mln_qiter_(window2d) q5(win_c12p, p);
  for (unsigned i = 0; i < arr_ima.nelements(); ++i)
  {
    for_all(p)
    {
      // Median 3
      accu_med3.init();
      for_all(q3) if (arr_ima[i].domain().has(q3))
	accu_med3.take(arr_ima[i](q3));
      tmp(p) = accu_med3.to_result();
    }
    ima_median3.append(duplicate(tmp));

    for_all(p)
    {
      // Median 5
      accu_med5.init();
      for_all(q5) if (arr_ima[i].domain().has(q5))
	accu_med5.take(arr_ima[i](q5));
      tmp(p) = accu_med5.to_result();
    }
    ima_median5.append(duplicate(tmp));
  }

  /////////////
  //	     //
  // Outputs //
  //	     //
  /////////////
  util::array<int_u12> arr_tumeur3;
  util::array<int_u12> arr_tumeur5;
  for (unsigned i = 0; i < ima_median3.nelements(); ++i)
  {
    arr_tumeur3.append(ima_median3[i](point2d(160, 120)));
    arr_tumeur5.append(ima_median5[i](point2d(160, 120)));
  }
  io::plot::save(arr_tumeur3, "median3_tumeur.plot");
  io::plot::save(arr_tumeur5, "median5_tumeur.plot");

  util::array<int_u12> arr_air3;
  util::array<int_u12> arr_air5;
  for (unsigned i = 0; i < ima_median3.nelements(); ++i)
  {
    arr_air3.append(ima_median3[i](point2d(34, 94)));
    arr_air5.append(ima_median5[i](point2d(34, 94)));
  }
  io::plot::save(arr_air3, "median3_air.plot");
  io::plot::save(arr_air5, "median5_air.plot");

  util::array<int_u12> arr_poumon3;
  util::array<int_u12> arr_poumon5;
  for (unsigned i = 0; i < ima_median3.nelements(); ++i)
  {
    arr_poumon3.append(ima_median3[i](point2d(122, 115)));
    arr_poumon5.append(ima_median5[i](point2d(122, 115)));
  }
  io::plot::save(arr_poumon3, "median3_poumon.plot");
  io::plot::save(arr_poumon5, "median5_poumon.plot");

  return 0;
}
