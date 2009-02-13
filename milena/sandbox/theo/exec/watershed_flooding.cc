#include "filetype.hh"

#include <mln/morpho/watershed/flooding.hh>
#include <mln/value/label_16.hh>

#include <mln/level/transform.hh>
#include <mln/convert/to_fun.hh>


namespace mln
{

  mln::value::int_u8 L_to_int_u8(unsigned u)
  {
    return u == 0 ?
      0 : // wshed line
      1 + (u - 1) % 255; // basin
  }

} // mln


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx output.yyy" << std::endl
	    << "  Watershed flooding." << std::endl
	    << "  xxx in { pgm, dump }" << std::endl
	    << "  yyy in { pbm, pgm, dump }" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  typedef value::label_16 L;
  if (argc != 3)
    usage(argv);

  trace::entering("main");

  std::string filename = argv[1];
  L n_basins;

  switch (get_filetype(argv[1]))
    {
    case filetype::pgm:
      {
	image2d<int_u8> ima;
	io::pgm::load(ima, argv[1]);
	image2d<L> wst = morpho::watershed::flooding(ima, c4(), n_basins);
	if (get_filetype(argv[2]) == filetype::dump)
	  {
	    io::dump::save(wst, argv[2]);
	  }
	else if (get_filetype(argv[2]) == filetype::pgm)
	  {
	    io::pgm::save( level::transform(wst, convert::to_fun(L_to_int_u8)),
			   argv[2] );
	  }
	else if (get_filetype(argv[2]) == filetype::pbm)
	  {
	    io::pbm::save( (pw::value(wst) == 0u) | wst.domain(),
			   argv[2] );
	  }
	else
	  {
	    std::cerr << "Bad output file format!" << std::endl;
	    std::abort();
	  }
      }
      break;

    case filetype::dump:
      {
	image3d<int_u8> ima;
	io::dump::load(ima, argv[1]);
	image3d<L> wst = morpho::watershed::flooding(ima, c6(), n_basins);
	io::dump::save(wst, argv[2]);
      }
      break;

    case filetype::unknown:
      std::cerr << "unknown filename extension!" << std::endl;
      usage(argv);
      break;

    default:
      std::cerr << "file type not handled!" << std::endl;
      usage(argv);
    }

  trace::exiting("main");
}
