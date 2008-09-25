# include <cstdlib>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

# include <mln/core/var.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/alias/window2d.hh>

# include <mln/convert/to_p_set.hh>
# include <mln/convert/to_image.hh>
# include <mln/debug/println.hh>

# include <mln/make/double_neighb2d.hh>

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


  bool is_chess(const mln::point2d& p)
  {
    return p.col() % 2 == p.row() % 2;
  }


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

   mln_VAR( nbh, make::double_neighb2d(is_chess, vert, hori) );
//   mln_VAR(nbh, c4());
//    mln_VAR(nbh, c8());

  image2d<int_u8> clo, grad = morpho::gradient(lena, nbh.win());
  // io::pgm::save(grad, "tmp_grad.pgm");

  int lambda = atoi(argv[2]);
  if (lambda > 1)
    {
      clo = morpho::closing_area(grad, nbh, lambda);
      io::pgm::save(clo, "tmp_clo.pgm");
    }
  else
    clo = grad;

  unsigned l;
  image2d<unsigned> wst = morpho::meyer_wst(clo, nbh, l);

  std::cout << "l = " << l << std::endl;
  debug::println(wst);

  io::ppm::save(level::transform(wst, colorize(l)), argv[3]);

//   io::pbm::save((pw::value(wst) == pw::cst(0)) | lena.domain(),
// 		argv[3]);
}
