/* core */
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

/* io */
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

/* WST */
#include <mln/morpho/reconstruction/by_erosion/union_find.hh>
#include <mln/morpho/watershed/all.hh>
#include <mln/morpho/elementary/dilation.hh>


#include <cstring>

using namespace mln;
using value::int_u16;
using value::int_u8;
using value::rgb8;

bool check_extension(const char* filename, const char* extension)
{
  return strncmp(filename + strlen(filename) - strlen(extension),
		 extension, strlen(extension)) == 0;
}

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.pgm [reference] [superpose.pgm]"
	    << std::endl;
  abort();
}


template <typename I, typename J>
void process(const Image<I>& input,
	     const Image<J>& reference,
	     const char* foutput)
{
  typedef image2d<int_u16> WST;
  int_u16 n_basins = 0;
  WST wst = morpho::watershed::flooding(input, c4(), n_basins);

  image2d<rgb8> out = morpho::watershed::superpose(reference, wst);
  io::ppm::save(out, foutput);
  std::cout << "Number of basins: " << n_basins << std::endl;
}


int main(int argc, char** argv)
{
  if (argc < 2)
    usage(argv);

  const char* finput = argv[1];
  const char* freference = (argc >= 3) ? argv[2] : finput;
  const char* foutput = (argc == 4) ? argv[3] : "superpose.pgm";

  image2d<int_u8> input;
  io::pgm::load(input, finput);

  if (strcmp(finput, freference) == 0)
    process(input, input, foutput);
  else if (check_extension(freference, ".pgm"))
    {
      image2d<int_u8> ref;
      io::pgm::load(ref, freference);
      process(input, ref, foutput);
    }
  else if (check_extension(freference, ".ppm"))
    {
      image2d<rgb8> ref;
      io::ppm::load(ref, freference);
      process(input, ref, foutput);
    }
  else
    usage(argv);
}
