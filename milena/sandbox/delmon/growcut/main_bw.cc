//
// main.cc for proto_growcut
// Made by Vivien Delmon
//



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Includes

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/value/all.hh>
#include <map>
#include <stack>

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Implementation

inline
float
g (mln::value::int_u8& p, mln::value::int_u8& n)
{
  return (1.f - std::abs(p - n) / 255.f);
}

int
main (int argc, char** argv)
{
  using namespace mln;

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " image.pgm image_seed.pgm labeled.pgm" << std::endl;
    return 1;
  }

  // file_in is the colored image we want to segment.
  std::string file_in   = argv[1];
  // file_seed is the same image as file_in with seeds for areas of interest colored.
  // For instance blue for background and red for the object we want to segment.
  // If we want to segment more than one object we can use up to 255 colors.
  std::string file_seed  = argv[2];
  // file_out is a colored image that represent the different objects with the color
  // of their seeds.
  std::string file_out  = argv[3];

  // Initialisations
  image2d<value::int_u8> in;
  io::pgm::load (in, file_in);

  image2d<value::int_u8> seed;
  io::pgm::load (seed, file_seed);

  image2d<value::int_u8> label;
  initialize(label, in);

  image2d<float> teta;
  initialize(teta, in);

  value::int_u8 nb_label = 1;
  mln_piter_(image2d<value::rgb8>) p(in.domain());
  for_all (p)
    if (in(p) == seed(p))
    {
      label(p) = 0;
      teta(p) = 0;
    }
    else
    {
      label(p) = seed(p);
      teta(p) = 1;
    }

  // Main Loop
  mln_niter_(neighb2d) n(c4(), p);
  // The to_do stack stores actions needed to go from rank n to n + 1.
  std::stack<std::pair<point2d, std::pair<value::int_u8, float> > > to_do;
  do
  {
    while (!to_do.empty())
    {
      label(to_do.top().first) = to_do.top().second.first;
      teta(to_do.top().first) = to_do.top().second.second;
      to_do.pop();
    }
    for_all (p)
      for_all (n)
	if (label(p) != label(n))
	{
	  float s = g(in(p), in(n));
	  if (s * teta(n) > teta(p))
	    to_do.push(std::make_pair(p, std::make_pair(label(n), s * teta(n))));
	}
  }
  while (!to_do.empty());

  // Save labels
  image2d<value::int_u8> out;
  initialize(out, in);

  for_all (p)
    if (label(p) != 0)
      out(p) = label(p);
    else
      out(p) = 0;
  mln::io::pgm::save (out, file_out);

  return 0;
}
