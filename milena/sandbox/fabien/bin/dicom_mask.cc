#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>

#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/literal/colors.hh>
#include <mln/morpho/elementary/gradient_internal.hh>

#include <mln/data/transform.hh>
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

  util::array<unsigned> xsrcproj(geom::nrows(input), 0);
  util::array<unsigned> ysrcproj(geom::ncols(input), 0);

  mln_piter_(image2d<int_u8>) psrc(input.domain());
  for_all(psrc)
  {
    xsrcproj[psrc.row()] += input(psrc);
    ysrcproj[psrc.col()] += input(psrc);
  }
  std::ofstream fout_xsrc("xsrc.plot");
  for (unsigned int i = 0; i < xsrcproj.nelements(); ++i)
  {
    fout_xsrc << i << " " << xsrcproj[i] << std::endl;
  }
  std::ofstream fout_ysrc("ysrc.plot");
  for (unsigned int i = 0; i < ysrcproj.nelements(); ++i)
  {
    fout_ysrc << i << " " << ysrcproj[i] << std::endl;
  }

  image2d<bool> ima = data::transform(input, fun::v2b::threshold<int_u8>(1));
  io::pbm::save(ima, argv[2]);
  mln_VAR(grad_int, morpho::elementary::gradient_internal(ima, c4()));

  util::array<unsigned> xproj(geom::nrows(ima), 0);
  util::array<unsigned> yproj(geom::ncols(ima), 0);
  util::array<unsigned> xgradproj(geom::nrows(grad_int), 0);
  util::array<unsigned> ygradproj(geom::ncols(grad_int), 0);

  mln_piter_(image2d<bool>) p(ima.domain());
  for_all(p)
  {
    if (ima(p))
    {
      ++xproj[p.row()];
      ++yproj[p.col()];
    }
  }
  mln_piter_(image2d<int_u8>) pgrad(grad_int.domain());
  for_all(pgrad)
  {
    xgradproj[pgrad.row()] += grad_int(pgrad);
    ygradproj[pgrad.col()] += grad_int(pgrad);
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

}
