#include <iostream>
#include <xtd/math/abs.hh>

int main()
{
  long l = -3;
  long ll = xtd::abs(l);
  std::cout << ll << std::endl;

  long i = -3;
  long ii = xtd::abs(i);
  std::cout << ii << std::endl;
}
