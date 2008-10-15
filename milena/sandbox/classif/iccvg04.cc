#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/histo/data.hh>
#include <mln/value/all.hh>

#include <mln/level/fill.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/arith/revert.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/ppm/load.hh>

#include <mln/io/pgm/save.hh>

using namespace mln;

unsigned max = 0;

template <typename I>
mln::image3d<unsigned>
fill_histo(const I& ima)
{
  image3d< unsigned > histo(256,256,256);
  level::fill(histo, 0);
  unsigned i = 0;

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red(),ima(p).green(), ima(p).blue());
    histo(p3)++;
  }
  return histo;
}

template <typename I>
void gplot(const I& ima)
{
  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    if (ima(p) != 0)
      std::cout << p[0] << ' ' << p[1] << ' ' << p[2] << std::endl;
  }
}

template <typename I>
void display_proj(const I& histo)
{
  image2d< unsigned long long > proj_histo(256,256);
  level::fill(proj_histo, 0);

  mln_piter_(image2d< unsigned long long >) p(proj_histo.domain());
  double max = 0;
  for_all(p)
  {
    for (unsigned i = 0; i < 256; i++)
      proj_histo(p) += histo(point3d(p[0], p[1], i));

    if (proj_histo(p) > max)
      max = proj_histo(p);
  }

  image2d< value::int_u8 > out(256, 256);
  for_all(p)
  {
    out(p) = (proj_histo(p) / max) * 255;
  }

  io::pgm::save(out,"./chiche.pgm");
}

int main(int argc, char **argv)
{
  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);

  image3d<unsigned> histo = fill_histo(ima);

  histo = arith::revert(histo);
  image3d<value::int_u8> histo_closed(histo.domain());

  morpho::closing_area(histo, c4(), 510, histo_closed);

  //display_proj(arith::revert(histo));

  //display_proj(histo);
}
