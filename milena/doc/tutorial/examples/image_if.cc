# include <mln/core/image2d.hh>
# include <mln/core/image_if.hh>
# include <mln/core/alias.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>
# include <mln/fun/p2b/chess.hh>



int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 5);
  debug::iota(ima);

  debug::println(ima);
  debug::println(ima | fun::p2b::chess);

  trait::image::print(ima | fun::p2b::chess);
}
