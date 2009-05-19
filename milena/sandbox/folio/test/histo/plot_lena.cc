#include <iostream>
#include <mln/debug/println.hh>
#include <mln/literal/all.hh>

#include <mln/io/pgm/load.hh>
#include <mln/value/int_u8.hh>

#include "../../mln/histo/compute_histo_rgb.hh"

namespace mln
{
  namespace histo
  {

    template <typename T>
    image1d<unsigned> compute_histo_gs(image2d<T> ima)
    {
      typedef mln_trait_value_comp(T, 0)::enc enc; // -> int_u8

      // New histogram-image
      image1d<unsigned> out(box1d(point1d(mln_min(enc)),   // -> 0
				  point1d(mln_max(enc)))); // -> 255
      data::fill(out, 0);

      // Compute histogram
      mln_fwd_piter(image2d<T>) p(ima.domain());
      for_all(p)
	++out(point1d(ima(p)));

      return out;
    }

  }
}

int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::value;

  // build test image
  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<int_u8>ima;
  io::pgm::load(ima, argv[1]);

  std::cout << "  => computing histo..." << std::endl;
  image1d<unsigned> out = histo::compute_histo_gs(ima);

  typedef image1d<int_u8> I;

  mln_piter_(I) p(out.domain());
  unsigned i = 0;
  for_all(p)
    std::cout << i++ << " " << out(p) << std::endl;
}
