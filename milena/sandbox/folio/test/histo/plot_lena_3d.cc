#include <iostream>
#include <mln/debug/println.hh>
#include <mln/literal/all.hh>

#include <mln/io/ppm/load.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

//#include "../../mln/histo/compute_histo_rgb.hh"

namespace mln
{
  namespace histo
  {

    template <typename T>
    image3d<unsigned>
    compute_histo_3d(image2d<T> ima)
    {
      typedef mln_trait_value_comp(T, 0)::enc enc_0; // R -> int_u8
      typedef mln_trait_value_comp(T, 1)::enc enc_1; // G -> int_u8
      typedef mln_trait_value_comp(T, 2)::enc enc_2; // B -> int_u8

      // New histogram-image
      image3d<unsigned> out(box3d(point3d(mln_min(enc_0),    // -> 0
					  mln_min(enc_1),    // -> 0
					  mln_min(enc_2)),   // -> 0
				  point3d(mln_max(enc_0),    // -> 255
					  mln_max(enc_1),    // -> 255
					  mln_max(enc_2)))); // -> 255
      data::fill(out, 0);

      // Compute histogram
      mln_fwd_piter(image2d<T>) p(ima.domain());
      for_all(p)
	++out(point3d(ima(p).comp(0), ima(p).comp(1), ima(p).comp(2)));

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
  image2d<rgb8>ima;
  io::ppm::load(ima, argv[1]);

  std::cout << "  => computing histo..." << std::endl;
  image3d<unsigned> out = histo::compute_histo_3d(ima);

  typedef image3d<rgb8> I;
  mln_piter_(I) p(out.domain());
  unsigned i, j, k = 0;
  for_all(p)
    i += out(p);
  std::cout << i << std::endl;
}
