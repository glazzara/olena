#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/labeling/colorize.hh>
#include <mln/labeling/blobs.hh>

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
  std::cout << "Usage: " << argv[0] << " in:mask.pgm in:source.(pgm|ppm) [out:colorize.ppm]"
	    << std::endl;
  abort();
}

template <typename M, typename I>
void
process(const Image<M>& mask, const Image<I>& ref, const char* foutput)
{
  int_u16 nlabels;

  mln_ch_value(I, int_u16) labels = labeling::blobs(mask, c4(), nlabels);

  typedef mln_ch_value(I, rgb8) O;
  O out = labeling::colorize(rgb8(), labels, nlabels);

  io::ppm::save(out, foutput);
}


int main(int argc, char** argv)
{
  if (argc < 3)
    usage(argv);

  const char* finput = argv[1];
  const char* freference = argv[2];
  const char* foutput = (argc >= 4) ? argv[3] : "colorize.ppm";

  image2d<bool> input;
  io::pbm::load(input, finput);

  if (check_extension(freference, ".pgm"))
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



