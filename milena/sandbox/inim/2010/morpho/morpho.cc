#include <iostream>
#include <sstream>
#include <string>
#include <mln/debug/println.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>


#include <mln/core/alias/neighb2d.hh>
#include <mln/accu/max.hh>
#include <mln/value/label_8.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/data/fill.hh>
#include <mln/literal/all.hh>
#include <mln/arith/revert.hh>
#include <mln/level/convert.hh>
#include <mln/level/stretch.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/linear/gaussian.hh>

#include <mln/morpho/rank_filter.hh>

using namespace mln;
using namespace mln::value;

void
load_img(image2d<bool>& in, const char* filename)
{
  using namespace mln;

  // build test image
  std::cout << "  => loading " << filename << "...";
  std::cout.flush();
  io::pbm::load(in, filename);
  std::cout << "................[DONE]" << std::endl;
}


void
revert_img(image2d<int_u8>& out,
	   const image2d<bool>& in)
{
  using namespace mln;

  int_u8 k = 1;
  out = level::convert(k, in);
  out = level::stretch(k, out);

  std::cout << "  => revert image...";
  std::cout.flush();
  out = arith::revert(out);
  std::cout << "...................[DONE]" << std::endl;

}




void
gaussian_filter(image2d<int_u8>& out, int gaussian_force)
{
  using namespace mln;

  std::cout << "  => applying gaussian filter...";
  std::cout.flush();
  out = linear::gaussian(out, gaussian_force, 1);
  //  out =  linear::gaussian_1st_derivative(out, gaussian_force, 1);
  std::cout << ".......[DONE]" << std::endl;
}






image2d<int_u8>
rank_filter(const image2d<int_u8>& out,
	    window2d it, int k, unsigned int lambda)
{
  using namespace mln;

  std::cout << "  => applying rank k filter...";
  std::cout.flush();
  image2d<int_u8> rank_filtered = morpho::rank_filter(out, it, k);


  for (unsigned int y = 0; y <= rank_filtered.ncols () - 1; ++y)
    for (unsigned  int x = 0; x <= rank_filtered.nrows() - 1; ++x)
      {
	if (rank_filtered(point2d(x, y)) < lambda)
 	  rank_filtered(point2d(x, y)) = 0;
	else
 	  rank_filtered(point2d(x, y)) = 255;
      }

  std::cout << ".........[DONE]" << std::endl;

  return rank_filtered;
}





void
look_for_lines(const image2d<label_8>& labels, image2d<int_u8>& ima)
{
  mln_fwd_piter_(image2d<int_u8>) p(ima.domain());
  for_all(p)
  {
    if (ima(p) == mln_max(int_u8))
    {
      ima(p) = labels(p);
    }
  }
}


int
main(int argc, char** argv)
{
  // check arguments
  if (argc != 7)
    {
      std::cerr << "Usage:" << std::endl
		<< "  ./morpho <in.pbm> <gaussian_force> <k> "
		<< "<window_width> <lambda> "
		<< "<output_filename>" << std::endl;
      exit(1);
    }

  const char* input_filename = argv[1];
  float gaussian_force = atof(argv[2]);

  unsigned int k = atoi(argv[3]);
  unsigned int w_width = atoi(argv[4]);
  unsigned int lambda = atoi(argv[5]);
  const char* output_filename = argv[6];


  image2d<bool> in;
  load_img(in, input_filename);

  int_u8 i = 1;
  image2d<int_u8> in_u8 = level::convert(i, in);
  in_u8 = level::stretch(i, in_u8);

  image2d<int_u8> out = in_u8;
  gaussian_filter(out, gaussian_force);

  window2d window;
  window.insert(0, 0);

  for (unsigned i = 1; i <= w_width; ++i)
  {
    window.insert(0, -i);
    window.insert(0, i);
  }

  image2d<int_u8> ima =
    rank_filter(out, window, k, lambda);

  label_8 nlabels = 0;
  image2d<label_8> labels = labeling::regional_maxima(ima, c4(), nlabels);

  look_for_lines(labels, in_u8);
  io::pgm::save(in_u8, output_filename);
}
