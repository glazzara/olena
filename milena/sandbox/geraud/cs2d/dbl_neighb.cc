# include <cstdlib>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/neighb2d.hh>
# include <mln/core/window2d.hh>

# include <mln/convert/to_p_set.hh>
# include <mln/convert/to_image.hh>
# include <mln/debug/println.hh>

# include "dbl_neighb.hh"

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/ppm/save.hh>

# include <mln/level/transform.hh>
# include <mln/literal/black.hh>
# include <mln/pw/all.hh>

# include <mln/morpho/closing_area.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/meyer_wst.hh>




struct is_chess_t
{ 
  bool operator()(const mln::point2d& p) const
  {
    return p.col() % 2 == p.row() % 2;
  }
};


struct colorize : mln::Function_v2v< colorize >
{
  typedef mln::value::rgb8 result;
  colorize(unsigned max)
    : lut(max + 1)
  {
    lut[0] = mln::literal::black;
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


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.ppm" << std::endl;
  abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  image2d<int_u8> lena;
  io::pgm::load(lena, argv[1]);
  
  bool vert[] = { 1, 1, 0,
		  1, 0, 1,
		  0, 1, 1 };
    
  bool hori[] = { 0, 1, 1,
		  1, 0, 1,
		  1, 1, 0 };

  dbl_neighb_<dpoint2d, is_chess_t> nbh;
  nbh
    .when_true (make::neighb2d(vert))
    .when_false(make::neighb2d(hori));

  image2d<int_u8> clo, grad = morpho::gradient(lena, nbh);
  // io::pgm::save(grad, "grad.pgm");

  int lambda = atoi(argv[2]);
  if (lambda > 1)
    {
      clo = morpho::closing_area(grad, nbh, lambda);
      // io::pgm::save(clo, "clo.pgm");
    }
  else
    clo = grad;

  unsigned l;
  image2d<unsigned> wst = morpho::meyer_wst(clo, nbh, l);
  std::cout << "l = " << l << std::endl;

  io::ppm::save(level::transform(wst, colorize(l)), argv[3]);

//   io::pbm::save((pw::value(wst) == pw::cst(0)) | lena.domain(),
// 		argv[3]);
}
