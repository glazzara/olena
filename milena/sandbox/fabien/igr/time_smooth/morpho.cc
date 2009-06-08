#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>
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
  // Morpho //
  //	    //
  ////////////
  image2d<image1d<int_u12> > ima_morpho;
  initialize(ima_morpho, ima_arr);
  mln_piter_(image2d<int_u12>) p(ima_morpho.domain());
  accu::stat::median_h<int_u12> accu_med;
  for_all(p)
  {
    image1d<int_u12> tmp_ima;
    convert::from_to(ima_arr(p), tmp_ima);
    tmp_ima = morpho::closing::area(tmp_ima, c2(), 3);
    tmp_ima = morpho::opening::area(tmp_ima, c2(), 3);
    ima_morpho(p) = tmp_ima;
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

  io::plot::save(ima_morpho(point2d(160, 120)), "morpho_tumeur.plot");
  io::plot::save(ima_morpho(point2d(34, 94)), "morpho_air.plot");
  io::plot::save(ima_morpho(point2d(122, 115)), "morpho_poumon.plot");

  return 0;
}
