#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/all.hh>

#include <mln/data/fill.hh>
#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>
#include <mln/geom/nslis.hh>


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
  data::fill(histo, 0);

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f);
    histo(p3)++;
  }
  return histo;
}

template <typename I>
image3d< double >
compute_density(const I& weight, const I& volume)
{
  image3d< double > density(volume.domain());

  mln_piter(I) p(volume.domain());
  for_all(p)
    density(p) = weight(p) / volume(p);

  return density;
}

template <typename I, typename J, typename N>
void
process_max_tree(const I& ima, const J& histo, const N& nbh,
                 double density_lambda, double value_lambda,
                 unsigned volume_lambda, unsigned nb_represent_lambda,
                 unsigned color_lambda, unsigned div_factor)
{
  max_tree_<J,N> run(histo, nbh);


  // FIXME: write a compute() method with functor argument
  image3d<unsigned> nb_represent		  = run.compute_nb_represent();
  image3d<unsigned> volume			  = run.compute_volume(); // surface (area)
  image3d< algebra::vec<3, double> > mean_color	  = run.compute_mean_color();
  image3d<double> density			  = compute_density(nb_represent, volume);

  // Density fusion
  if (density_lambda > 0.00001)
  {
    std::cout << "Density fusion" << std::endl;
    run.nuclear_fusion(density, density_lambda);
  }

  // value fusion
  if (value_lambda > 0.00001)
  {
    std::cout << "Value fusion" << std::endl;
    run.nuclear_fusion(histo, value_lambda);
  }

  // Volume cutting
  if (volume_lambda != 0)
  {
    std::cout << "Volume cutting" << std::endl;
    run.lumberjack(volume, volume_lambda);
  }

  // Represent cutting
  if (nb_represent_lambda != 0)
  {
    std::cout << "nb_represent cutting" << std::endl;
    run.lumberjack(nb_represent, nb_represent_lambda);
  }

  // Color fusion
  if (color_lambda != 0)
  {
    std::cout << "Color fusion" << std::endl;
    run.color_fusion(color_lambda); // FIXME: factorize the code with a functor
  }

  // Compute mean color of active nodes
  std::cout << "Compute mean color" << std::endl;
  run.compute_mean_color();

  // Print informations on the filtered max tree
  {
    int nb_class = 0;

    mln_piter(image3d<unsigned>) p(nb_represent.domain());

    std::cerr.precision(2);

    std::cerr << "Color\t\tId\t\tdensity\t\tvolume\t\tnb_represent" << std::endl;

    for_all(p)
      if (run.is_active(p))
      {
	std::cerr << mean_color(p)  << "\t\t" << nb_class
                  << "\t\t" << density(p) << "\t\t" << volume(p)
                  << "\t\t" << nb_represent(p) << std::endl;
	++nb_class;
      }

    std::cout << "Number of classes : " << nb_class << std::endl;
  }


  // Write the image w.r.t. the max tree
  run.to_ppm(ima, "out.ppm", div_factor, mean_color);

  //proj
  save_class_v2(ima, histo, mean_color, run.new_parent,
                div_factor, "histo.ppm");
}

bool usage(int argc, char ** argv)
{
  if (argc != 8)
  {
    std::cout << "usage: " << argv[0] << " image div_factor "
              << "density_lambda value_lambda volume_lambda "
              << "nb_represent_lambda color_lambda" << std::endl;
    std::cout << "Help: A value of 0 for any lambda means "
              << "the related action won't be performed" << std::endl;
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

  int div_factor = atoi(argv[2]);
  float density_lambda = atof(argv[3]);
  float value_lambda = atof(argv[4]);
  unsigned volume_lambda = atoi(argv[5]);
  unsigned nb_represent_lambda = atoi(argv[6]);
  unsigned color_lambda = atoi(argv[7]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima, div_factor);

  // Process max_tree
  process_max_tree(ima, histo, c6(), density_lambda, value_lambda,
                   volume_lambda, nb_represent_lambda, color_lambda, div_factor);
}
