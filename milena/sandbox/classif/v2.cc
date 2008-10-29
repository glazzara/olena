#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/histo/data.hh>

#include <mln/value/all.hh>

#include <mln/level/fill.hh>
#include <mln/geom/all.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/arith/revert.hh>
#include <mln/core/alias/neighb3d.hh>

#include "max_tree.hh"
#include "proj.hh"
#include "display.hh"

using namespace mln;

template <typename I>
mln::image3d<unsigned>
fill_histo(const I& ima, int f)
{
  const value::int_u8 v = 256 / f; // FIXME
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

template <typename I, typename J, typename N>
unsigned
compute_max_tree(const I& ima, const J& histo, const N& nbh, const unsigned f, int lambda)
{
  max_tree_<J,N> run(histo, nbh);
#if 0
  I out(ima.domain());
  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    algebra::vec<3, value::int_u8> v = make::vec(ima(p).red()   / f,
                                                 ima(p).green() / f,
                                                 ima(p).blue()  / f);
    point3d pn = run.parent(v);
    out(p) = value::rgb8(pn[0] * f, pn[1] * f, pn[2] * f);
  }
  io::ppm::save(out, "tmp.ppm");
#endif

  //run.number_of_nodes();
  run.volume();
  run.simple_filter_1(lambda);
  run.compute_mean_color();
  run.to_ppm(ima, "out.ppm", f);

  //std::cout << " Number of nodes : " << run.number_of_nodes() << std::endl;
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

int main(int argc, char* argv[])
{
  if (not usage(argc, argv))
    return 1;

  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);
  const int div_factor = atoi(argv[2]);
  const int lambda = atoi(argv[3]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima, div_factor);

  //proj
  accu::mean<unsigned, unsigned, unsigned> mean;
  image2d<unsigned> phisto = proj(histo, mean);

  //debug::println(phisto);

  // Compute max_tree
  compute_max_tree(ima, histo, c6(), div_factor, lambda);
}
