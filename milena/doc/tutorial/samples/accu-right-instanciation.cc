#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  image2d<value::int_u8> ima(2, 3);

  // \{
  level::compute(accu::meta::max(), ima);
  // \}
}
