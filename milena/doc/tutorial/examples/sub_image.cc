# include <mln/core/image2d.hh>
# include <mln/core/neighb2d.hh>
# include <mln/core/sub_image.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>

# include <mln/morpho/gradient_elementary.hh>


int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 5);
  debug::iota(ima);
  debug::println(ima);

  sub_image<I, box2d> sub(ima, box2d(2,3));
  debug::println(sub);
  debug::println(morpho::gradient_elementary(sub, c4()));

  trait::image::print(sub);
}
