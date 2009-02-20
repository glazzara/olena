#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include "load.hh"

#include <mln/io/dump/save.hh>
#include <mln/io/pgm/save.hh>

int main()
{
  using namespace mln;
  using value::int_u16;

  image2d<int_u16> lena2;
  image3d<int_u16> lena3;

  //io::dicom::load(lena3, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm");
  io::dicom::load(lena2, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm");
  //io::dump::save(lena3, "IM_0058.dump");
  io::pgm::save(lena2, "IM_0058.pgm");
}
