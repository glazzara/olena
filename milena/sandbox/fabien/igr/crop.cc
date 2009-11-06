#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/dump/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>




///////////////////
//               //
// Main Function //
//               //
///////////////////


void usage(char* argv[])
{
  std::cerr << "Usage: " << argv[0] << " input s_min r_min c_min s_max r_max c_max output" << std::endl
	    << "  input:  input image in DICOM format encoded in int_u12" << std::endl
	    << "  s_min:  slice index of pmin" << std::endl
	    << "  r_min:  row index of pmin" << std::endl
	    << "  c_min:  column index of pmin" << std::endl
	    << "  s_max:  slice index of pmax" << std::endl
	    << "  r_max:  row index of pmax" << std::endl
	    << "  c_max:  column index of pmax" << std::endl
	    << "  output: output image in dump format encoded in int_u12" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;

  typedef int_u12 input_type;

  if (argc != 9)
    usage(argv);

  image3d<input_type> vol;
  io::dicom::load(vol, argv[1]);

  std::cout << "Input bbox: " << vol.bbox() << std::endl;

  int
    s_min = atoi(argv[2]),
    r_min = atoi(argv[3]),
    c_min = atoi(argv[4]),

    s_max = atoi(argv[5]),
    r_max = atoi(argv[6]),
    c_max = atoi(argv[7]);

  box3d b = make::box3d(s_min, r_min, c_min,  s_max, r_max, c_max);
  std::cout << "Output bbox: " << b << std::endl;

  image3d<input_type> ima(b);
  data::fill(ima, vol);
  io::dump::save(ima, argv[8]);
}
