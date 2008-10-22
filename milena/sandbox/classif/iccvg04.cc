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
#include <mln/util/array.hh>
#include <mln/labeling/compute.hh>

// FIXME: !?
#include <mln/geom/all.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/estim/min_max.hh>

#include <sys/stat.h>
#include <sstream>


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
void display(const I& ima, const char * dir)
{
  mkdir(dir, 0777);
  chdir(dir);

  image2d< mln_value(I) > out(geom::nrows(ima), geom::ncols(ima));

  for (int s = 0; s < geom::nslis(ima); ++s)
  {
   // image2d< value::int_u8 > out(geom::nrows(ima), geom::ncols(ima));
    for (int r = 0; r < geom::nrows(ima); ++r)
    {
      for (int c = 0; c < geom::ncols(ima); ++c)
      {
	out(point2d(r, c)) = ima(point3d(s, r, c));
      }
    }

    std::ostringstream is;
    is << "out_" << s << ".pgm";

    io::pgm::save(out, is.str());
  }

  chdir("..");
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

template <typename I, typename J, typename K>
mln_ch_value(I, value::int_u8) donne_tout(const I& ima, const J& histo, const K& ws, int nbasins)
{
  image2d<value::rgb8> out(ima.domain());

  typedef mln::accu::mean<mln_value(J)> accu_t;

  accu_t acu;

  util::array<mln_result(accu_t)> tmp = labeling::compute(acu, histo, ws, nbasins);

  /*mln_piter(I) p(ima.domain());

  for_all(p)
  {
  }*/
}

int main(int argc, char **argv)
{
  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima);
  {
    unsigned m, M;
    estim::min_max(histo, m, M);
    std::cout << m << ' ' << M << std::endl;
  }

  image3d<value::int_u8> nhisto = normalizeu8(histo);
  {
    value::int_u8 m, M;
    estim::min_max(nhisto, m, M);
    std::cout << m << ' ' << M << std::endl;
  }

  //display(nhisto, "histo");

  //revert histo
  image3d<value::int_u8> rhisto = arith::revert(nhisto);
  {
    value::int_u8 m, M;
    estim::min_max(rhisto, m, M);
    std::cout << m << ' ' << M << std::endl;
  }
  //display(rhisto, "rhisto");
  //compute closing_area of histo

  image3d<value::int_u8> histo_closure(histo.domain());
  morpho::closing_area(rhisto, c6(), 20, histo_closure);
  {
    value::int_u8 m, M;
    estim::min_max(histo_closure, m, M);
    std::cout << m << ' ' << M << std::endl;
  }



  //display_proj_revert(histo_closure, "chisto.ppm");

  //watershed over histo_closure
  value::int_u8 nbasins;
  image3d<value::int_u8> ws = morpho::meyer_wst(histo_closure, c6(), nbasins);

  std::cout << nbasins << std::endl;

  display(ws, "ws");
  display_proj_revert(ws, "whisto.ppm");
  //gplot(ws);

  //donne_tout(ima, histo_closure, ws, nbasins);
}
