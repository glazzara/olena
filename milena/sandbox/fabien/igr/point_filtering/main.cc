#include <mln/core/image/image1d.hh>
#include <mln/core/alias/window1d.hh>

#include <mln/io/plot/all.hh>

#include <mln/accu/mean.hh>
#include <mln/accu/image/all.hh>
#include <mln/convert/from_to.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/util/array.hh>


using namespace mln;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << "point.plot" << std::endl;
  }

  util::array<float> arr;
  io::plot::load(arr, argv[1]);
  image1d<float> ima;
  convert::from_to(arr, ima);

  // Raw point.
  io::plot::save(ima, "raw.plot");

  // Morpho filtering.
  window1d win_1;
  win_1
    .insert(-2)
    .insert(-1)
    .insert(0)
    .insert(1)
    .insert(2);

  image1d<float> opening_ima = morpho::opening::structural(ima, win_1);
  image1d<float> closing_ima = morpho::closing::structural(ima, win_1);

  image1d<accu::mean<float> > result;

  initialize(result, ima);

  accu::image::init(result);
  accu::image::take(result, closing_ima);
  accu::image::take(result, opening_ima);
  image1d<float> ima_morpho = accu::image::to_result(result);
  io::plot::save(ima_morpho, "morpho.plot");

  // Morpho (again).
  opening_ima = morpho::opening::structural(ima_morpho, win_1);
  closing_ima = morpho::closing::structural(ima_morpho, win_1);

  initialize(result, ima_morpho);

  accu::image::init(result);
  accu::image::take(result, closing_ima);
  accu::image::take(result, opening_ima);
  image1d<float> ima_morpho2 = accu::image::to_result(result);
  io::plot::save(ima_morpho2, "morpho2.plot");

  // Morpho (the return of the revenge).
  opening_ima = morpho::opening::structural(ima_morpho2, win_1);
  closing_ima = morpho::closing::structural(ima_morpho2, win_1);

  initialize(result, ima_morpho2);

  accu::image::init(result);
  accu::image::take(result, closing_ima);
  accu::image::take(result, opening_ima);
  image1d<float> ima_morpho3 = accu::image::to_result(result);
  io::plot::save(ima_morpho3, "morpho3.plot");


  return 0;
}
