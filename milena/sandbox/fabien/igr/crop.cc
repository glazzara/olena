#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/dump/save.hh>

#include <mln/value/int_u12.hh>




///////////////////
//               //
// Main Function //
//               //
///////////////////


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dcm s_min r_min c_min s_max r_max c_max output.dump" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u12;

  if (argc != 9)
    usage(argv);

  image3d<int_u12> vol;
  io::dicom::load(vol, argv[1]);

  int
    s_min = atoi(argv[2]),
    r_min = atoi(argv[3]),
    c_min = atoi(argv[4]),

    s_max = atoi(argv[5]),
    r_max = atoi(argv[6]),
    c_max = atoi(argv[7]);

  box3d b = make::box3d(s_min, r_min, c_min,  s_max, r_max, c_max);

  image3d<int_u12> ima(b);
  data::fill(ima, vol);
  io::dump::save(ima, argv[8]);
}
