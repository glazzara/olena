#include <mln/core/image/image2d.hh>
#include <mln/data/fill.hh>
#include <mln/border/fill.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println.hh>
#include <mln/debug/println_with_border.hh>

#include <doc/tools/sample_utils.hh>

int main()
{
  using namespace mln;
  using namespace mln::value;

  {
    doc::begin_output();
    // \{
    image2d<int_u8> ima(5,5);
    data::fill(ima, 2);
    debug::println(ima);
    // \}
    doc::end_output();
  }

  {
    doc::begin_output();
    // \{
    image2d<int_u8> ima(5,5);
    data::fill(ima, 2);
    border::fill(ima, 7);
    debug::println_with_border(ima);
    // \}
    doc::end_output();
  }
}
