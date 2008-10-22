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
fill_histo(const I& ima, int f)
{
  const mln_value(I) v = mln_max(mln_value(I)) / f;
  image3d< unsigned > histo(v,v,v);
  level::fill(histo, 0);
  unsigned i = 0;

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red()/f,ima(p).green()/f, ima(p).blue()/f);
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


template <typename I, typename J, typename K>
mln_concrete(I)
classify_image(const I& ima, const J& histo, const K& ws, int nbasins)
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
  image3d<unsigned> histo = fill_histo(ima,4);
  {
    unsigned m, M;
    estim::min_max(histo, m, M);
    std::cout << m << ' ' << M << std::endl;
  }

  /*  image3d<value::int_u8> nhisto = normalizeu8(histo);
  {
    value::int_u8 m, M;
    estim::min_max(nhisto, m, M);
    std::cout << m << ' ' << M << std::endl;
    }*/

  //display(nhisto, "histo");

  //revert histo
  image3d<value::int_u8> rhisto = arith::revert(histo);
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
