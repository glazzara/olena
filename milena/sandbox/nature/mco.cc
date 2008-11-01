#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/cast_image.hh>
#include <mln/value/int_u16.hh>
#include "co_occurence.hh"
#include <mln/core/alias/dpoint2d.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/load.hh>
#include <mln/estim/min_max.hh>

int main(int argc, const char * argv[])
{
  using namespace mln;
  using namespace value;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  dpoint2d d(0, 1);

  for (int i = 1; i < argc; ++i)
    {
      image2d<int_u8> ima;
      io::pgm::load(ima, argv[i]);

      std::string name(argv[i]);
      name.erase(name.length() - 4);

      image2d<unsigned> co (co_occurence(ima, d));

      // Normalization
      unsigned m, M;
      estim::min_max(co, m, M);
      double norm = 255./M;
      mln_piter_ (image2d<unsigned>) p (co.domain());
      for_all(p)
	co(p) *= norm;

      io::pgm::save(cast_image<int_u<8> >(co), name.append("_mco.pgm"));
    }
}
