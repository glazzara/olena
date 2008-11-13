#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  bool vals[2][2] = {
    {1, 0},
    {0, 1}
  };
  image2d<bool> ima = make::image(vals);
  std::cout << "ima:" << std::endl;
  debug::println(ima);

  std::cout << "----" << std::endl;

  image2d<bool> ima_neg = logical::not_(ima);

  std::cout << "ima_neg:" << std::endl;
  debug::println(ima_neg);
  std::cout << "ima:" << std::endl;
  debug::println(ima);

  std::cout << "----" << std::endl;

  logical::not_inplace(ima);

  std::cout << "not_inplace(ima)" << std::endl;
  debug::println(ima);
}
