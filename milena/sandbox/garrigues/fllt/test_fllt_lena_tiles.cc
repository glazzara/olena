# include "fllt_optimized.hh"
# include <mln/core/image2d.hh>
# include <mln/core/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/convert/to_image.hh>
# include <mln/level/fill.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/load.hh>
# include <sstream>


int main()
{

  using namespace mln;

  for (int i = 0; i < 16; ++i)
    for (int j = 0; j < 16; ++j)
    {
      std::stringstream path;
      path << "/lrde/tegucigalpa/theo/pub/mln_docs/lena_tiles/lena_" << i << "_" << j << ".pgm";
      std::cout << path.str () << std::endl;
      image2d<value::int_u8> ima = io::pgm::load(path.str());
      image2d<int> ima_int(ima.domain());
      level::fill(ima_int, ima);
      fllt::fllt(ima_int);
    }
}

