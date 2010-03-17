#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

int main()
{
  using namespace mln;

  image2d<value::int_u8> input(2,2);

  box2d b(2,2);
  image2d<value::int_u8> input2(b);
}
