#include "filetype.hh"

#include <mln/core/image/image_if.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input_int_u8.dump input_bool.dump output.dump" << std::endl
	    << "  Superpose." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  image3d<int_u8> ima;
  io::dump::load(ima, argv[1]);

  image3d<bool> msk;
  io::dump::load(msk, argv[2]);

  data::fill( (ima | pw::value(msk)).rw(), 255 );

  io::dump::save(ima, argv[3]);

  trace::exiting("main");
}
