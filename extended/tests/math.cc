#include <iostream>
#include <xtd/math.hh>


int main()
{
  using xtd::cos;
  using xtd::sin;
  using xtd::_1;
  using xtd::_2;

  std::cout << (cos(_1) * cos(_1) + sin(_2) * sin(_2))(_1, _1)(51.f) << std::endl;
  std::cout << (_1 * _1 + _2 * _2)(cos(_1), sin(_1))(51.f) << std::endl;
}
