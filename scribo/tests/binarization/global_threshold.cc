#include <mln/core/image/image2d.hh>
#include <mln/make/image.hh>
#include <mln/data/compare.hh>

#include <scribo/binarization/global_threshold.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  unsigned data[2][2] = { { 12, 24 },
			  {  2,  6 } };

  bool data_ref[2][2] = { { 1, 1 },
			  { 0, 0 } };

  image2d<unsigned> input = make::image(data);
  image2d<bool> ref = make::image(data_ref);

  image2d<bool> bin = scribo::binarization::global_threshold(input, 10);

  mln_assertion(bin == ref);
}
