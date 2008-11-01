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
compute_max_tree(const I& ima, const J& histo, const N& nbh,
                 const unsigned f, float lambda, float ratio)
{
  max_tree_<J,N> run(histo, nbh);


  run.volume();

  std::cout << "step 1 - nb_represent fusion" << std::endl;
  //run.nb_represent_fusion(lambda);
  std::cout << "step 2 - volume fusion" << std::endl;
  //run.volume_fusion(lambda);
  std::cout << "step 3 - color fusion" << std::endl;
  //run.color_fusion(lambda);
  std::cout << "step 3 - density fusion" << std::endl;
  //run.density_fusion(ratio);

  std::cout << "step Update parents" << std::endl;
  run.update_parents();

  std::cout << "step Compute mean color" << std::endl;
  run.compute_mean_color();

  std::cout << "step Print class info" << std::endl;
  run.print_class_info();

  std::cout << "step Output image" << std::endl;
  run.to_ppm(ima, "out.ppm", f);
}

bool usage(int argc, char ** argv)
{
  if (argc != 5)
  {
    std::cout << "usage: " << argv[0] << " image div_factor lambda ratio" << std::endl;
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
  const float lambda = atof(argv[3]);
  const float ratio = atof(argv[4]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima, div_factor);

  //proj
  accu::mean<unsigned, unsigned, unsigned> mean;
  image2d<unsigned> phisto = proj(histo, mean);

  //debug::println(phisto);

  // Compute max_tree
  compute_max_tree(ima, histo, c6(), div_factor, lambda, ratio);
}
