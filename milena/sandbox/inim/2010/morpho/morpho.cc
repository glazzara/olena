#include <iostream>
#include <sstream>
#include <string>

#include <mln/arith/revert.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/linear/gaussian.hh>
#include <mln/morpho/rank_filter.hh>
#include <mln/value/label_8.hh>

using namespace mln;
using namespace mln::value;

void
load_img(image2d<int_u8>& ima, const char* filename)
{
  std::cout << "  => loadimag " << filename << "..." << std::endl;

  // load.
  image2d<bool> in;
  io::pbm::load(in, filename);

  // convert and stretch.
  int_u8 k = 1;
  ima = data::convert(k, in);
  ima = data::stretch(k, ima);
}

void
revert_img(image2d<int_u8>& ima)
{
  std::cout << "  => revert image..." << std::endl;

  // revert.
  ima = arith::revert(ima);
}

void
gaussian_filter(image2d<int_u8>& ima, float gaussian_force)
{
  std::cout << "  => applying gaussian filter..." << std::endl;

  // gaussian filter.
  ima = linear::gaussian(ima, gaussian_force, 1);
}

void
rank_filter(image2d<int_u8>& ima,
	    const window2d& it,
	    const unsigned k,
	    const unsigned lambda)
{
  std::cout << "  => applying rank k filter..." << std::endl;

  // rank filter.
  ima = morpho::rank_filter(ima, it, k);

  mln_fwd_piter_(image2d<int_u8>) p(ima.domain());
  for_all(p)
    if (ima(p) < lambda)
      ima(p) = mln_min(int_u8);
    else
      ima(p) = mln_max(int_u8);
}

void
replace_with_labels(const image2d<label_8>& labels, image2d<int_u8>& ima)
{
  mln_fwd_piter_(image2d<int_u8>) p(ima.domain());
  for_all(p)
    if (ima(p) == mln_max(int_u8))
      ima(p) = labels(p); // replace with label at p.
}

int
main(int argc, char** argv)
{
  // check arguments
  if (argc != 7)
  {
    std::cerr << "Usage:" << std::endl
	      << "  ./morpho <ima.pbm> <gaussian_force> <k> "
	      << "<window_width> <lambda> "
	      << "<input_filename>" << std::endl;
    exit(1);
  }

  // get argv variables.
  const char* input_filename = argv[1];
  const float gaussian_force = atof(argv[2]);
  const unsigned k = atoi(argv[3]);
  const unsigned w_width = atoi(argv[4]);
  const unsigned lambda = atoi(argv[5]);
  const char* output_filename = argv[6];

  // load and revert image.
  image2d<int_u8> ima;
  load_img(ima, input_filename);
  image2d<int_u8> in(ima);

  // apply gaussian filter.
  gaussian_filter(ima, gaussian_force);

  // create window.
  window2d window;
  window.insert(0, 0);
  for (unsigned i = 1; i <= w_width; ++i)
  {
    window.insert(0, -i);
    window.insert(0, i);
  }

  // apply k-rank filter.
  rank_filter(ima, window, k, lambda);

  // labelize.
  label_8 nlabels = 0;
  image2d<label_8> labels = labeling::regional_maxima(ima, c4(), nlabels);

  // compute output image.
  replace_with_labels(labels, in);
  io::pgm::save(in, output_filename);
}
