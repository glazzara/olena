# include <mln/core/image2d.hh>
# include <mln/core/image/flat_image.hh>
# include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  flat_image<unsigned, box2d> ima(3, box2d(3,3));
  debug::println(ima);
}
