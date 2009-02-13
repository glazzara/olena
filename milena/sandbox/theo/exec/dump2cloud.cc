#include "filetype.hh"

#include <mln/io/cloud/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.txt" << std::endl
	    << "  Dump (binary, 3D) to cloud." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  std::string filename = argv[1];

  image3d<bool> vol;
  io::dump::load(vol, argv[1]);
  p_array<point3d> arr;
  convert::from_to(vol, arr);
  io::cloud::save(arr, argv[2]);

  trace::exiting("main");
}
