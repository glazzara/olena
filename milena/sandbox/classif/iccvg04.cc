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
#include <mln/io/ppm/save.hh>

#include <mln/estim/min_max.hh>
#include <mln/algebra/vec.hh>
#include <mln/algebra/vec.hh>

#include <mln/level/stretch.hh>

#include <sys/stat.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>


using namespace mln;

unsigned max = 0;

template <typename I>
mln::image3d<unsigned>
fill_histo(const I& ima, int f)
{
  const value::int_u8 v = 255 / f; // FIXME
  image3d<unsigned> histo(v,v,v);
  level::fill(histo, 0);
  unsigned i = 0;

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f);
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
void
classify_image(const I& ima, const J& histo, const K& ws, int nbasins, int f)
{

  unsigned count[nbasins + 1];
  memset(count, 0, nbasins + 1);
  algebra::vec<3, unsigned> sum[nbasins + 1];

  for (int i = 1; i < nbasins + 1; ++i)
  {
    sum[i] = literal::zero;
  }

  /*mln_piter(K) p(ws.domain());
  for_all(p)
  {
    count[ws(p)] += histo(p);
    sum[ws(p)] += histo(p) * convert::to<algebra::vec<3, value::int_u8> >(p) * f;
  }*/

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f);
    int w = ws(p3);

    // Check if we are not on a border of the WS
    if (w != 0)
    {
      count[w] += histo(p3);
      sum[w] += histo(p3) * convert::to< algebra::vec<3, value::int_u8> >(p3);
    }
    std::cerr << "p3 : " << p3 << " == " << convert::to<algebra::vec<3, value::int_u8> >(p3) << std::endl;
  }

  for (int i = 1; i < nbasins + 1; ++i)
  {
    sum[i] *= f;
    sum[i] /= count[i];
    //std::cout << "count[" << i << "] = " << count[i] << std::endl;
    std::cout << "sum[" << i << "] = " << sum[i] << std::endl;
  }


  mln_piter(I) pi(ima.domain());
  I out(ima.domain());
  I ws_out(ima.domain());

  for_all(pi)
  {
    value::rgb8 coul = ima(pi);

    int w = ws(point3d(coul.red() / f, coul.green() / f, coul.blue() / f));

    if (w != 0) // If w == 0 it means that the current point is part of a border of the watershed
    {
      std::cerr << "out(" << pi << ") = sum[" << w << "]; //" << sum[w] << std::endl;
      out(pi) = convert::to<value::rgb8>(sum[w]);
      //out(pi) = value::rgb8(sum[w][0], sum[w][1], sum[w][2]);
      ws_out(pi) = value::rgb8(0, 0, 0);
    }
    else
    {
      std::cerr << "Border : " << pi << std::endl;
      ws_out(pi) = value::rgb8(255, 255, 255);

      // FIXME
      // ima(pi) is a border in the histogram and therefore lacks of a mean color.
      // Choosing a good value for out(pi) is not so easy. One idea could be to randomly
      // choose a color from one of the surrounding classes, but it's not the best idea
      // because of large « border » zones.
      out(pi) = value::rgb8(255, 255, 255);
    }

    //std::cerr << "(" << coul << ") -> (" << out(pi) << "); sum[" << w << "] = " << sum[w] << " -> " << convert::to<value::rgb8>(sum[w]) << std::endl;
  }

  // 1- La valeur contenue dans sum n'est pas correctement écrite dans out.ppm. À vérifier !
  // 2- Le cast de sum[w] en rgb8 se passe très bizarrement. À vérifier !
  // 3- Rajoute le std::cout de count change le comportement du programme. À vérifier !
  // 4- Le WS semble faire des pâtés (beaucoup de points de classe 0)

  io::ppm::save(out, "out.ppm");
  io::ppm::save(ws_out, "out-ws.ppm");
}

int main(int argc, char **argv)
{
  const int div_factor = atoi(argv[2]);
  const int lambda = atoi(argv[3]);

  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima,div_factor);
  {
    unsigned m, M;
    estim::min_max(histo, m, M);
    std::cout << "histo : " << m << ' ' << M << std::endl;
  }

// Normalize histogram frequences
#if 0
  image3d<value::int_u8> nhisto(histo.domain());
  level::stretch(histo, nhisto);
  //image3d<value::int_u8> nhisto = normalizeu8(histo);
  {
    value::int_u8 m, M;
    estim::min_max(nhisto, m, M);
    std::cout << "nhisto : " << m << ' ' << M << std::endl;
   }
#endif

  //display(nhisto, "histo");

  //revert histo
  image3d<unsigned> rhisto = arith::revert(histo);
  {
    unsigned m, M;
    estim::min_max(rhisto, m, M);
    std::cout << "rhisto : " << m << ' ' << M << std::endl;
  }
  //display(rhisto, "rhisto");

  //compute closing_area of histo
  image3d<unsigned> histo_closure(histo.domain());
  morpho::closing_area(rhisto, c6(), lambda, histo_closure);
  {
    unsigned m, M;
    estim::min_max(histo_closure, m, M);
    std::cout << "histo_closure : " << m << ' ' << M << std::endl;
  }

  //display_proj_revert(histo_closure, "chisto.ppm");

  //watershed over histo_closure
  unsigned nbasins = 0;
  image3d<unsigned> ws = morpho::meyer_wst(histo_closure, c6(), nbasins);

  std::cout << "nbassins : " << nbasins << std::endl;

  //display(ws, "ws");
  //display_proj_revert(ws, "whisto.ppm");
  //gplot(ws);

  // Classify image !
  //classify_image(const I& ima, const J& histo, const K& ws, int nbasins, int f)
  classify_image(ima, histo, ws, nbasins, div_factor);
}
