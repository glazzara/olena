#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/histo/data.hh>
#include <mln/value/all.hh>

#include <mln/level/fill.hh>

#include <mln/morpho/closing_volume.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/opening_volume.hh>
#include <mln/morpho/opening_area.hh>

#include <mln/arith/revert.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/util/array.hh>
#include <mln/labeling/compute.hh>

#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>
#include <mln/geom/nslis.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/estim/min_max.hh>
#include <mln/algebra/vec.hh>
#include <mln/algebra/vec.hh>

#include <mln/literal/all.hh>

#include <mln/level/stretch.hh>

#include <sys/stat.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>

#include "proj.hh"
#include "display.hh"

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

template <typename I, typename J, typename K>
void
classify_image(const I& ima, const J& histo, const K& ws, int nbasins, int f)
{
  unsigned border = 0;
  unsigned count[nbasins + 1];
  memset(count, 0, (nbasins + 1) * sizeof (unsigned));

  algebra::vec<3, double> sum[nbasins + 1];
  for (int i = 0; i < nbasins + 1; ++i)
    sum[i] = literal::zero;

  // Compute representatives of every class
  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f);
    int w = ws(p3);

    //check if we are not on a border of the WS
    if (w == 0)
      border++;
    count[w] += histo(p3);
    sum[w] += histo(p3) * convert::to< algebra::vec<3, value::int_u8> >(p3);

    std::cerr << "p3 : " << p3 << " == " <<
      convert::to<algebra::vec<3, value::int_u8> >(p3) << std::endl;
  }

  std::cout << border << std::endl;
  for (int i = 0; i < nbasins + 1; ++i)
  {
    std::cout << "sum[" << i << "] = " << sum[i] * f << " / " << count[i]  << " == ";
    sum[i] = (sum[i] * f) / count[i];
    std::cout << sum[i] << std::endl;
  }

  // Make an output image where colors are replaced by their representatives.
  mln_piter(I) pi(ima.domain());
  I out(ima.domain());
  for_all(pi)
  {
    //retrieve color class
    value::rgb8 coul = ima(pi);
    int w = ws(point3d(coul.red() / f, coul.green() / f, coul.blue() / f));

    //if w == 0, out(pi) = 0 ie is part of a border of the watershed
    if (w == 0)
      out(pi) = literal::red;
    else
      out(pi) = convert::to<value::rgb8>(sum[w]);

    std::cerr << "out(" << pi << ") = sum[" << w << "]; //"
              << sum[w] << " : rgb8(" << sum[w] << ")" << std::endl;
  }

  io::ppm::save(out, "out.ppm");
  save_class(histo, ws, sum, "palette.ppm");
}

bool usage(int argc, char ** argv)
{
  if (argc != 4)
  {
    std::cout << "usage: " << argv[0] << " image div_factor lambda" << std::endl;
    return false;
  }
  return true;
}


int main(int argc, char **argv)
{
  if (not usage(argc, argv))
    return 1;
  const int div_factor = atoi(argv[2]);
  const int lambda = atoi(argv[3]);

  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima,div_factor);

  //compute opening_volume of histo
  image3d<unsigned> histo_filtered(histo.domain());
  morpho::opening_volume(histo, c6(), lambda, histo_filtered);


  //watershed over histo_closure
  unsigned nbasins = 0;
  image3d<unsigned> ws = morpho::meyer_wst(arith::revert(histo_filtered),
                                           c6(), nbasins);
  std::cout << "nbassins : " << nbasins << std::endl;

  //classify image
  classify_image(ima, histo, ws, nbasins, div_factor);
}
