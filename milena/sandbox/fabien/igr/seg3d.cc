#include <mln/core/concept/image.hh>
#include <mln/core/concept/neighborhood.hh>
#include <mln/core/alias/neighb3d.hh>

#include <mln/core/image/image3d.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/dicom/load.hh>

#include <mln/value/label_16.hh>

#include "seg_vol_irm.hh"

using namespace mln;
using value::int_u12;
using value::label_16;

template <typename I, typename N, typename L>
mln_ch_value(I, bool)
igr_seg(const Image<I>& input_, const mln::Neighborhood<N>& nbh_, L& nlabels);

int usage()
{
  std::cout << "Usage: ./seg3d image.dcm" << std::endl;

  return 1;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    return usage();

  label_16 nlabels;
  image3d<int_u12> ima;
  io::dicom::load(ima, argv[1]);
  io::dump::save(igr_seg(ima, c6(), nlabels), "result.dump");

  return 0;
}
