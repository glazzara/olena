
# define MLN_FLOAT double

#include "filetype.hh"
#include "./gaussian_directional_2d.hh"

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/pw/all.hh>

#include <mln/data/fill.hh>
#include <mln/data/saturate.hh>





void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx bdr dir sigma output.pgm" << std::endl
	    << "  xxx is pbm or pgm" << std::endl
	    << "  bdr is the outer border value" << std::endl
	    << "  dir = 0 (vertical blur) or 1 (horizontal blur)" << std::endl
	    << "  sigma > 0" << std::endl
	    << "  Directional gaussian blur." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{

  using namespace mln;
  using value::int_u8;

  trace::entering("main");

  if (argc != 6)
    usage(argv);


  int dir = atoi(argv[3]);
  if (dir != 0 && dir != 1)
    usage(argv);

  MLN_FLOAT sigma = atof(argv[4]);
  if (sigma <= 0.f)
    usage(argv);


  switch (get_filetype(argv[1]))
    {
    case filetype::pbm:
      {
	int bdr = atoi(argv[2]);
	if (bdr != 0 && bdr != 1)
	  usage(argv);

	image2d<bool> ima;
	io::pbm::load(ima, argv[1]);

	image2d<MLN_FLOAT> temp(ima.domain()), out;
	data::fill(temp, ima);

	out = linear::gaussian_directional_2d(temp, dir, sigma, bdr);

	io::pgm::save(data::saturate(int_u8(),
				      (pw::value(out) * pw::cst(255.f)) | out.domain()),
		      argv[5]);
      }
      break;

    case filetype::pgm:
      {
	int bdr = atoi(argv[2]);
	if (bdr < 0 || bdr > 255)
	  usage(argv);

	image2d<int_u8> ima;
	io::pgm::load(ima, argv[1]);
    
	image2d<MLN_FLOAT> temp(ima.domain()), out;
	data::fill(temp, ima);

	out = linear::gaussian_directional_2d(temp, dir, sigma, bdr);

	io::pgm::save(data::saturate(int_u8(), out), argv[5]);
      }
      break;

    default:
      std::cerr << "file type not handled!" << std::endl;
      usage(argv);
    }

  trace::exiting("main");
}
