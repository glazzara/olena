# include <mln/core/image2d.hh>
# include <mln/core/sub_image.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>



int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 5);
  debug::iota(ima);
  debug::println(ima);

  sub_image<I, box2d> sub(ima, box2d(2,3));
  debug::println(sub);
}
