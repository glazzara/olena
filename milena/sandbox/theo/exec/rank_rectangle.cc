#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/win/hline2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/morpho/rank_filter.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm height width k output.pbm" << std::endl
	    << "  Rank filter with a 2D rectangle." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 6)
    usage(argv);

  trace::entering("main");

  image2d<bool> ima, out;
  io::pbm::load(ima, argv[1]);

  int height = atoi(argv[2]);
  if (height < 0)
    usage(argv);

  int width  = atoi(argv[3]);
  if (width < 0)
    usage(argv);

  int k = atoi(argv[4]);
  if (k < 1 || k > height * width)
    usage(argv);

  out = morpho::rank_filter(ima,
			    win::rectangle2d(height, width),
			    k);
  io::pbm::save(out, argv[5]);

  trace::exiting("main");
}
