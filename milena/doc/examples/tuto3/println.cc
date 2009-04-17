#include <mln/core/image/image2d.hh>
#include <mln/data/fill.hh>
#include <mln/border/fill.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println.hh>
#include <mln/debug/println_with_border.hh>

int main()
{
  using namespace mln;
  using namespace mln::value;

  {
    std::cout << "// \\{" << std::endl;
    // \{
    image2d<int_u8> ima(5,5);
    data::fill(ima, 2);
    debug::println(ima);
    // \}
    std::cout << "// \\}" << std::endl;
  }

  {
    std::cout << "// \\{" << std::endl;
    // \{
    image2d<int_u8> ima(5,5);
    data::fill(ima, 2);
    border::fill(ima, 7);
    debug::println_with_border(ima);
    // \}
    std::cout << "// \\}" << std::endl;
  }
}
