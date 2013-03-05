# include <cstdlib>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

# include <mln/io/pgm/load.hh>
# include <mln/io/ppm/save.hh>

# include <mln/core/var.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>

# include <mln/data/transform.hh>
# include <mln/literal/black.hh>
# include <mln/debug/println.hh>

# include <mln/morpho/closing_area.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/watershed/flooding.hh>


using namespace mln;
using value::int_u8;



struct colorize : Function_v2v< colorize >
{
  typedef value::rgb8 result;
  colorize(unsigned max)
    : lut(max + 1)
  {
    lut[0] = literal::black;
    for (unsigned i = 1; i <= max; ++i)
      lut[i] = result(100 + std::rand() % 150,
		      100 + std::rand() % 150,
		      100 + std::rand() % 150);
  }
  result operator()(unsigned i) const
  {
    return lut[i];
  }
  std::vector<result> lut;
};


bool is_chess(const point2d& p)
{
  return p.col() % 2 == p.row() % 2;
}


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm nbh lambda output.ppm" << std::endl;
  std::cerr << "  nbh in {4, 6, 8}; lambda >= 0" << std::endl;
  abort();
}


template <typename I, typename N>
void do_it(const I& ima,
	   const N& nbh,
	   int lambda,
	   const std::string& filename)
{
  I grad = morpho::gradient(ima, nbh.win());

  I clo;
  if (lambda > 1)
    clo = morpho::closing_area(grad, nbh, lambda);
  else
    clo = grad;

  unsigned l;
  mln_ch_value(I, unsigned) wst = morpho::watershed::flooding(clo, nbh, l);

  debug::println(labeling::regional_minima(clo, nbh, l));
  debug::println(wst);

  io::ppm::save(data::transform(wst, colorize(l)), filename);
}


int main(int argc, char* argv[])
{
  if (argc != 5)
    usage(argv);

  image2d<int_u8> ima;

  io::pgm::load(ima, argv[1]);

  int nbh_ = atoi(argv[2]);
  if (! (nbh_ == 4 || nbh_ == 6 || nbh_ == 8))
    usage(argv);

  int lambda = atoi(argv[3]);
  if (lambda < 0)
    usage(argv);

  std::string filename(argv[4]);

  if (nbh_ == 4)
    {
      mln_VAR(nbh, c4());
      do_it(ima, nbh, lambda, filename);
    }
  else if (nbh_ == 8)
    {
      mln_VAR(nbh, c8());
      do_it(ima, nbh, lambda, filename);
    }
  else if (nbh_ == 6)
    {
      bool vert[] = { 1, 1, 0,
		      1, 0, 1,
		      0, 1, 1 };

      bool hori[] = { 0, 1, 1,
		      1, 0, 1,
		      1, 1, 0 };
  
      mln_VAR(nbh, make::double_neighb2d(is_chess, vert, hori));
      do_it(ima, nbh, lambda, filename);
    }
}
