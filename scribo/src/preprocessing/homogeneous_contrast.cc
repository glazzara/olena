#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <scribo/preprocessing/homogeneous_contrast.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pgm", "A gray-level image." },
  { "h", "The height attribute value for the leveling closing." },
  {0, 0}
};



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm h output.pgm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Remove noise.",
				"input.pgm h output.pgm",
				args_desc);


  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  int h = std::atoi(argv[2]);

  io::pgm::save(scribo::preprocessing::homogeneous_contrast(input, h),
		argv[3]);
}
