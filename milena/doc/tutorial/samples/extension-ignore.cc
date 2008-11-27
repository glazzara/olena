#include <mln/essential/2d.hh>

template <typename I>
void my_routine(const mln::Image<I>&)
{

}

int main()
{
  mln::image2d<mln::value::int_u8> ima(1, 1);

  // \{
  my_routine(ima | ima.domain());
  // \}

  (void) ima;
}
