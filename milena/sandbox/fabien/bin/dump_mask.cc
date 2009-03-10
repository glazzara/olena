#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/var.hh>

#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/literal/colors.hh>
#include <mln/morpho/elementary/gradient_internal.hh>

#include <mln/level/transform.hh>
#include <mln/fun/v2b/threshold.hh>

#include <mln/data/fill.hh>
#include <mln/pw/all.hh>


int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump" << std::endl;
  return 1;
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 2)
    return usage(argv);

  image3d<int_u8> input;
  io::dump::load(input, argv[1]);

  image3d<bool> ima = level::transform(input, fun::v2b::threshold<int_u8>(1));
  mln_VAR(grad_int, morpho::elementary::gradient_internal(ima, c6()));

  util::array<unsigned> xproj(ima.nrows(), 0);
  util::array<unsigned> yproj(ima.ncols(), 0);
  util::array<unsigned> xgradproj(grad_int.nrows(), 0);
  util::array<unsigned> ygradproj(grad_int.ncols(), 0);

  mln_piter_(image3d<bool>) p(ima.domain());
  for_all(p)
  {
    if (ima(p))
    {
      ++xproj[p.row()];
      ++yproj[p.col()];
    }
    if (grad_int(p))
    {
      ++xgradproj[p.row()];
      ++ygradproj[p.col()];
    }
  }

  // Plot files
  std::ofstream fout_x("x.plot");
  std::ofstream fout_xgrad("xgrad.plot");
  for (unsigned int i = 0; i < xproj.nelements(); ++i)
  {
    fout_x << i << " " << xproj[i] << std::endl;
    fout_xgrad << i << " " << xgradproj[i] << std::endl;
  }

  std::ofstream fout_y("y.plot");
  std::ofstream fout_ygrad("ygrad.plot");
  for (unsigned int i = 0; i < yproj.nelements(); ++i)
  {
    fout_y << i << " " << yproj[i] << std::endl;
    fout_ygrad << i << " " << ygradproj[i] << std::endl;
  }

  return 0;
}
