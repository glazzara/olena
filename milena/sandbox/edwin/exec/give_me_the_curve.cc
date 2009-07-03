#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/data/sort_psites.hh>

#include <mln/io/pgm/load.hh>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

void usage(char** argv)
{

  std::cout << "Usage: " << argv[0] << " in:source.pgm in:attribute.pgm [out:data.txt]"
	    << std::endl
	    << "Compute the tree from 'source' and display the values of"
	    << " 'attribute' along the branch."
	    << std::endl;
  abort();
}


int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc < 3)
    usage(argv);

  const char * finput = argv[1];
  const char * fattribute = argv[2];
  const char * fdata = (argc >= 4) ? argv[3] : "data.txt";

  // Image loadin'.
  typedef image2d<value::int_u8> I;
  I input, a;
  io::pgm::load(input, finput);
  io::pgm::load(a, fattribute);

  // Tree construction.
  typedef p_array<mln_psite_(I)> S;
  typedef morpho::tree::data<I, S> T;

  S s = data::sort_psites_decreasing(input);
  T tree(input, s, c4());

  // Display branches

  while (true)
    {
      unsigned x, y;

      std::cout << "x y: ";
      std::cout.flush();
      std::cin >> x;
      std::cin >> y;

      if (std::cin.fail())
	{
	  std::cout << "Parse error."
		    << std::endl;
	  continue;
	}

      mln_psite_(T) p(x, y);
      std::ofstream data(fdata);

      while (!tree.is_root(p))
	{
	  data << a(p) << std::endl;
	  p = tree.parent(p);
	}
      data.close();

      system("gnuplot plot.conf");
      if (fork() == 0)
	{
	  setsid();
	  //umask(0);
	  execlp("display", "display", "plot.png", 0);
	  exit(0);
	}
    }
}



