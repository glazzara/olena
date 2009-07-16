#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/accu/stat/mean.hh>

#include <mln/labeling/compute_image.hh>
#include <mln/labeling/blobs.hh>

#include <mln/convert/to.hh>
#include <mln/data/convert.hh>

using namespace mln;
using value::int_u16;
using value::int_u8;
using value::rgb8;

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " in:mask.pgm in:source.ppm [out:colormean.ppm]"
	    << std::endl;
  abort();
}

template <typename M, typename I>
void
process(const Image<M>& mask, const Image<I>& ref, const char* foutput)
{
  int_u16 nlabels;

  image2d<int_u16> labels = labeling::blobs(mask, c4(), nlabels);

  typedef mln_ch_value(I, rgb8) O;
  O out = data::convert(rgb8(), labeling::compute_image(accu::meta::stat::mean (), ref, labels, nlabels));

  io::ppm::save(out, foutput);
}

int main(int argc, char** argv)
{
  if (argc < 3)
    usage(argv);

  const char* finput = argv[1];
  const char* freference = argv[2];
  const char* foutput = (argc >= 4) ? argv[3] : "colormean.ppm";

  image2d<bool> input;
  io::pbm::load(input, finput);

  image2d<rgb8> ref;
  io::ppm::load(ref, freference);
  process(input, ref, foutput);
}
