#include <iostream>

#include <xtd/math.hh>


int main()
{
  using xtd::_1;
  using xtd::_2;
  using xtd::lit;

  (_1 + _2)(_1, _1);

  (_1 + _2)(_1, lit(1.f));

  ;

  std::cout << (_1 + _2) .bind_2(2.6f) (2.5f) << std::endl;

//   std::cout << (_1 + _2)(_1, 1.)(5.) << std::endl;

  // autre problème :
  // std::cout << (_1 + 1)(5.f) << std::endl;

}
