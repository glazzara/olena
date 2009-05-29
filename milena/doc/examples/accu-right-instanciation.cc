#include <mln/core/image/image2d.hh>

#include <mln/value/int_u8.hh>

#include <mln/level/compute.hh>

#include <mln/accu/max.hh>

int main()
{
  using namespace mln;

  image2d<value::int_u8> ima(2, 3);

  // \{
  level::compute(accu::meta::max(), ima);
  // \}
}
