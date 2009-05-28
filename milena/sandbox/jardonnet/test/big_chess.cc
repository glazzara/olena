
#include <iostream>

# include <mln/core/image/dmorph/sub_image_if.hh>
# include <mln/core/image/dmorph/image_if.hh>

#include <mln/debug/println.hh>

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/data/paste.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/fun/p2b/big_chess.hh>

using namespace mln;
using namespace mln::value;

bool usage(int argc, char ** argv)
{
  if (argc != 4)
  {
    std::cout << argv[0] << " ima1.pgm ima2.pgm div" << std::endl;
    return false;
  }
  return true;
}


template <typename B>
struct is_ima2_t :  Function_p2b< is_ima2_t<B> >
{
  is_ima2_t(const B& box, const unsigned div)
    : box(box), div(div)
  {
  }

  typedef bool result;
  bool operator()(const point2d& p) const
  {
    return !(p.row() / ((box.pmax().row() - 1) / div) % 2) &&
      !(p.col() / ((box.pmax().row()-1) / div) % 2);
  }

  const B& box;
  const unsigned div;
};

int main(int argc, char ** argv)
{
  if (not usage(argc,argv))
    return 1;

  typedef image2d<rgb8> I;

  I ima1;
  I ima2;
  io::ppm::load(ima1, argv[1]);
  io::ppm::load(ima2, argv[2]);

  int div = atoi(argv[3]);

  fun::p2b::big_chess<box2d> fun(ima1.domain(), div);

  image_if< I, fun::p2b::big_chess<box2d> > visio = ima1 | fun;

  debug::println(visio);

  data::paste(visio, ima2);

  io::ppm::save(ima2, "out.ppm");
}
