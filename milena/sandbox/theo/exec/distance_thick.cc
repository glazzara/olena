#include "filetype.hh"

#include <mln/transform/distance_front.hh>
#include <mln/make/w_window2d_int.hh>
#include <mln/data/stretch.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm dmap.pgm" << std::endl
	    << "  Distance is stretch (from int_u12) to int_u8." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  typedef value::int_u<12> int_u12;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);

  int ws[] = { 00, 11,  0, 11,  0,
	       11,  7,  5,  7, 11,
	       00,  5,  0,  5,  0,
	       11,  7,  5,  7, 11,
	       00, 11,  0, 11,  0 };

  image2d<int_u12> dmap = transform::distance_front(ima,
						    c4(), make::w_window2d_int(ws),
						    mln_max(int_u12));

  io::pgm::save(data::stretch(int_u8(), dmap),
		argv[2]);

  trace::exiting("main");
}
