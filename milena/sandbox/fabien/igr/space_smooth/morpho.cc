#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb2d.hh>
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
#include <mln/convert/from_to.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/opening/area.hh>
#include <mln/util/array.hh>

/*#include <mln/draw/line.hh>
#include <mln/io/magick/save.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/all.hh>
using mln::value::rgb8;
using mln::value::int_u8;*/


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
  // Morpho //
  //	    //
  ////////////
  util::array<image2d<int_u12> > ima_morpho;
  for (unsigned i = 0; i < arr_ima.nelements(); ++i)
  {
    image2d<int_u12> tmp_ima = morpho::closing::area(arr_ima[i], c4(), 3);
    tmp_ima = morpho::opening::area(tmp_ima, c4(), 3);
    ima_morpho.append(tmp_ima);
  }

  /////////////
  //	     //
  // Outputs //
  //	     //
  /////////////
  /*image2d<rgb8> ima_color = data::convert(rgb8(), level::stretch(int_u8(), slice(input, 0)));
  algebra::vec<2, unsigned int> vmin;
  algebra::vec<2, unsigned int> vmax;
  vmin[0] = 160;
  vmin[1] = 120;
  vmax[0] = 122;
  vmax[1] = 115;
  mln_site_(image2d<bool>) pbeg(vmin);
  mln_site_(image2d<bool>) pend(vmax);
  draw::line(ima_color, pbeg, pend, literal::red);
  io::magick::save(ima_color, "test.png");*/
  image2d<util::array<float> > ima_result(input.nrows(), input.ncols());
  mln_piter_(image2d<util::array<float> >) p(ima_morpho[0].domain());
  for_all(p)
    for (int i = 0; i < ima_morpho.nelements(); ++i)
      ima_result(p).append(ima_morpho[i](p));

  io::plot::save(ima_result(point2d(156, 114)), "morpho_tumeur.plot");
  io::plot::save(ima_result(point2d(34, 94)), "morpho_air.plot");
  io::plot::save(ima_result(point2d(122, 115)), "morpho_poumon.plot");

  return 0;
}
