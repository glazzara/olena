#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_set.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/compare.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/topo_wst.hh>
#include <string>
#include <iostream>

int main (int argc, const char * argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> image2dint;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
      image2dint ima;
      io::pgm::load(ima, argv[i]);

      morpho::topo_wst< image2d<int_u8>, neighb2d> n(ima, c4());

      n.go();

      std::string name(argv[i]);
      name.erase(name.length() - 4);

      io::pgm::save(morpho::topo_watershed(n), name.append("_wsd.pgm"));
    }
  return 0;
}
