#include <mln/core/image/image2d.hh>
#include <mln/core/sub_sampled_image.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  I input;
  sub_sampled_image<I> sima(input, point2d(1,1), 2);
  
}
