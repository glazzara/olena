#include <mln/core/concept/image.hh>
#include <mln/core/concept/neighborhood.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/core/image/image2d.hh>

#include <mln/value/int_u12.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/dicom/load.hh>

#include <mln/value/label_16.hh>

#include "seg_vol_irm.hh"

using namespace mln;
using value::int_u12;
using value::label_16;



int usage()
{
  std::cout << "Usage: ./seg2d image.dcm" << std::endl;

  return 1;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    return usage();

  label_16 nlabels;
  image2d<int_u12> ima;
  io::dicom::load(ima, argv[1]);
  io::pbm::save(igr_seg(ima, c4(), nlabels), "result.pbm");

  return 0;
}
