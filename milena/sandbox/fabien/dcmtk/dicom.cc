#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

#include "load.hh"

int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;

  io::dicom::load(lena, "/Users/HiSoKa/Work/LRDE/Olena/resources/data/IM_0001");
}
