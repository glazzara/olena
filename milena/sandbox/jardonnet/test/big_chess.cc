
#include <iostream>

# include <mln/core/image/sub_image_if.hh>
# include <mln/core/image/image_if.hh>

#include <mln/debug/println.hh>

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/level/paste.hh>
#include <mln/io/pgm/save.hh>

#include <mln/fun/p2b/big_chess.hh>

using namespace mln;
using namespace mln::value;

bool usage(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cout << argv[0] << " ima1.pgm ima2.pgm" << std::endl;
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

  typedef image2d<int_u8> I;

  I ima1;
  I ima2;
  io::pgm::load(ima1, argv[1]);
  io::pgm::load(ima2, argv[2]);

  fun::p2b::big_chess<box2d> fun(ima1.domain(), 4);

  image_if< I, fun::p2b::big_chess<box2d> > visio = ima1 | fun;

  debug::println(visio);

  level::paste(visio, ima2);

  io::pgm::save(ima2, "out.ppm");
}
