#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include "load.hh"

#include <mln/io/dump/save.hh>

int main()
{
  using namespace mln;
  using value::int_u16;

  image3d<int_u16> lena;

  io::dicom::load(lena, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm");
  io::dump::save(lena, "IM_0052.dump");
}
