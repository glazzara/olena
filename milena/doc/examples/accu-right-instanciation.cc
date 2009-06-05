#include <mln/core/image/image2d.hh>

#include <mln/value/int_u8.hh>

#include <mln/data/compute.hh>

#include <mln/accu/stat/max.hh>

int main()
{
  using namespace mln;

  image2d<value::int_u8> ima(2, 3);

  // \{
  data::compute(accu::meta::stat::max(), ima);
  // \}
}
