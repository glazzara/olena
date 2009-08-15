#include <mln/walue/rgb_3x8.hh>
#include <mln/core/var.hh>



int main()
{
  using namespace mln;
  using walue::rgb_3x8;
  using walue::set_encoding;

  {
    rgb_3x8 c(0, 0.5, 1);
    std::cout << c.to_vec() << std::endl;
  }
  {
    rgb_3x8 c = set_encoding(0, 128, 255);
    std::cout << c << std::endl;
  }

//   // mln_VAR(e, walue::set_encoding(0,1,2));
//   walue::internal::encoding<int> e = walue::set_encoding(0,1,2);
//   std::cout << e.t0 << ' ' << e.t1 << ' ' << e.t2 << std::endl;

  
}
