#include <iostream>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>
#include <mln/core/image/image_if.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>
#include <mln/value/rgb8.hh>

#include <mln/labeling/regional_minima.hh>

#include <mln/level/transform.hh>
#include <mln/level/stretch.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/fun/l2l/wrap.hh>

#include <mln/labeling/colorize.hh>



///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::label_16;
  using value::label_32;
  using value::rgb8;

  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dcm> <closure_lambda>"
	      << std::endl;
    return 1;
  }

  unsigned closure_lambda = atoi(argv[2]);

  image3d<int_u12> dcm;
  io::dicom::load(dcm, argv[1]);

  // Gradient
  image3d<int_u12> grad = morpho::elementary::gradient(dcm, c6());

  // Closure
  image3d<int_u12> clo = morpho::closing::volume(grad, c6(), closure_lambda);

  // Watershed
  label_32 nbasins;
  image3d<label_32> wshed = morpho::watershed::flooding(clo, c6(), nbasins);
  //image3d<label_32> wshed = morpho::watershed::flooding(clo, c6(), nbasins);

  // Visualization
  std::cout << "    nbasins = " << nbasins << std::endl;
  //io::dump::save(level::transform(wshed, fun::l2l::wrap<int_u8>()), "result_nbasins.dump");
  //io::dump::save(level::stretch(int_u8(), wshed), "result_nbasins.dump");
  io::dump::save(labeling::colorize(rgb8(), wshed, nbasins), "result_nbasins.dump");

  return 0;
}
