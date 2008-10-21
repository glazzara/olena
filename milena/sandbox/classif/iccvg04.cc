#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/histo/data.hh>
#include <mln/value/all.hh>

#include <mln/level/fill.hh>
#include <mln/morpho/closing_volume.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/arith/revert.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/core/alias/neighb3d.hh>

// FIXME: !?
#include <mln/geom/all.hh>

#include <mln/io/ppm/load.hh>

#include <mln/io/pgm/save.hh>

using namespace mln;

unsigned max = 0;

template <typename I>
mln::image3d<unsigned>
fill_histo(const I& ima)
{
  image3d< unsigned > histo(128,128,128);
  level::fill(histo, 0);
  unsigned i = 0;

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red()/2,ima(p).green()/2, ima(p).blue()/2);
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
void display_proj_revert(const I& histo, const char * s)
{
  image2d< unsigned long long > proj_histo(geom::nrows(histo),geom::ncols(histo));
  level::fill(proj_histo, 0);

  mln_piter_(image2d< unsigned long long >) p(proj_histo.domain());
  double max = 0;
  for_all(p)
  {
    for (unsigned i = 0; i < geom::nslis(histo); i++)
//      if (histo(point3d(p[0], p[1], i)) < 255)
	proj_histo(p) += histo(point3d(p[0], p[1], i));

    if (proj_histo(p) > max)
      max = proj_histo(p);
  }

  image2d< value::int_u8 > out(proj_histo.domain());
  for_all(p)
  {
    out(p) = (proj_histo(p) / max) * 255;

    if (out(p) < 255)
      out(p) -= (255 - out(p)) * 10;
  }

  io::pgm::save(out,s);
}

template <typename I>
void display_proj_min(const I& histo, const char * s)
{
  image2d< unsigned long long > proj_histo(geom::nrows(histo),geom::ncols(histo));
  level::fill(proj_histo, 0);

  mln_piter_(image2d< unsigned long long >) p(proj_histo.domain());
  double max = 0;
  for_all(p)
  {
    unsigned min = 9999999;
    for (unsigned i = 0; i < geom::nslis(histo); i++)
      if (histo(point3d(p[0], p[1], i)) < min)
        min = histo(point3d(p[0], p[1], i));
    proj_histo(p) = min;

    if (proj_histo(p) > max)
      max = proj_histo(p);
  }

  image2d< value::int_u8 > out(proj_histo.domain());
  for_all(p)
  {
    out(p) = (proj_histo(p) / max) * 255;
  }

  io::pgm::save(out, s);
}


template <typename I>
mln_ch_value(I, value::int_u8) normalizeu8(const I& ima)
{
  mln_ch_value(I, value::int_u8) res(ima.domain());
  level::fill(res, literal::zero);

  mln_piter(I) p(ima.domain());
  mln_value(I) max = 0;
  for_all(p)
  {
    if (ima(p) > max)
      max = ima(p);
  }

  for_all(p)
  {
    res(p) = (ima(p) / (double) max) * 255;
  }

  return res;
}

int main(int argc, char **argv)
{
  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima);

  image3d<value::int_u8> nhisto = normalizeu8(histo);

  //revert histo
  image3d<value::int_u8> rhisto = arith::revert(nhisto);
  //compute closing_area of histo

  image3d<unsigned> histo_closure(histo.domain());
  morpho::closing_area(rhisto, c6(), 420, histo_closure);

  //display_proj_revert(histo_closure, "chisto.ppm");

  //watershed over histo_closure
  value::int_u16 nbasins;
  image3d<value::int_u16> ws = morpho::meyer_wst(histo_closure, c6(), nbasins);

  display_proj_revert(ws, "whisto.ppm");
  //gplot(ws);

}
