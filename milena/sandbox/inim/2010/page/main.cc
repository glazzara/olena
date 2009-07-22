#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>

#include <binarization.hh>
#include <grayscale.hh>
#include <white.hh>

void usage (const std::string& name)
{
  std::cout << "Usage: " << name
            << " [-w window_size] [-b] IN.ppm OUT.pbm" << std::endl;
  exit (1);
}

int
main (int argc,
      char** argv)
{
  if (argc < 2)
    usage(std::string(argv[0]));

  int i = 1;
  unsigned int w_val = 51;
  bool do_white = true;

  if (std::string(argv[i]) == "-w")
  {
    if (argc < i + 2)
      usage(std::string(argv[0]));

    std::string str_w_val(argv[++i]);
    std::istringstream ist(str_w_val);

    ist >> w_val;
    ++i;
  }

  if (i >= argc)
    usage(std::string(argv[0]));

  if (std::string(argv[i]) == "-b")
  {
    do_white = false;
    ++i;
  }

  if (i >= argc)
    usage(std::string(argv[0]));

  std::string infile(argv[i++]);

  if (i >= argc)
    usage(std::string(argv[0]));

  std::string outfile(argv[i]);

  // The original image
  mln::image2d<mln::value::rgb8> initial;
  mln::io::ppm::load(initial, infile);
  const mln::image2d<mln::value::rgb8>& loaded = initial;

  // Grayscale
  mln::image2d<mln::value::int_u8> grays(loaded.domain());
  binarization::grayscale(loaded, grays);

  // Binarization
  mln::image2d<bool> binarized(loaded.domain());
  binarization::sauvola_threshold(grays, binarized, w_val);


  if (do_white)
  {
    // Whitespace
    mln::image2d<bool> final(loaded.domain());
    whitespace::whitespace_compute(binarized, final);
    mln::io::pbm::save (final, outfile);
  }
  else
    mln::io::pbm::save (binarized, outfile);
}
