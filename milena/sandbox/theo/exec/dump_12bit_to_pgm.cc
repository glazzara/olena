#include "filetype.hh"

#include <mln/value/int_u12.hh>
#include <mln/level/stretch.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.dump" << std::endl;
  std::cerr << "  The input dump file contains 12 bit unsigned 3D data." << std::endl;
  std::cerr << "  The output dump file reduces the dynamics to 8 bit." << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  image3d<value::int_u12> vol;
  io::dump::load(vol, argv[1]);

  using value::int_u8;
  image3d<int_u8> out = level::stretch(int_u8(), vol);
  io::dump::save(out, argv[2]);

  trace::exiting("main");
}
