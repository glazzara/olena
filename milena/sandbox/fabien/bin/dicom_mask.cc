#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>

#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/literal/colors.hh>

#include <mln/level/transform.hh>
#include <mln/fun/v2b/threshold.hh>

#include <mln/data/fill.hh>
#include <mln/pw/all.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dcm output.pbm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  image2d<int_u8> input;
  io::dicom::load(input, argv[1]);

  util::array<unsigned> xproj(geom::nrows(input), 0);
  util::array<unsigned> yproj(geom::ncols(input), 0);

  mln_piter_(image2d<int_u8>) p(input.domain());
  for_all(p)
  {
    xproj[p.row()] += input(p);
    yproj[p.col()] += input(p);
  }

  // Plot files
  std::ofstream fout_x("x.plot");
  for (unsigned int i = 0; i < xproj.nelements(); ++i)
  {
    fout_x << i << " " << xproj[i] << std::endl;
  }

  std::ofstream fout_y("y.plot");
  for (unsigned int i = 0; i < yproj.nelements(); ++i)
  {
    fout_y << i << " " << yproj[i] << std::endl;
  }

  image2d<bool> ima = level::transform(input, fun::v2b::threshold<int_u8>(1));
  io::pbm::save(ima, argv[2]);
}
