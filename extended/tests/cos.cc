#include <iostream>

#include <xtd/math.hh>


int main()
{

//   {
//     using xtd::cos;
//     std::cout << cos(5.1f) << std::endl;
//   }

//   {
//     using xtd::cos;
//     using xtd::sin;
//     using xtd::_1;
//     using xtd::_2;

//     std::cout << cos(_1)(5.1f) << std::endl;
//     std::cout << (cos(_1) + sin(_2))(5.f, 1.f) << std::endl;
//     std::cout << (cos(_1) + sin(_2)).nargs() << std::endl;
//   }

//   {
//     using xtd::cos;
//     using xtd::_1;
//     using xtd::_2;
//     std::cout << (cos(_1)(_2)).nargs() << std::endl; // gives: 2
//   }

  {
    using xtd::cos;
    using xtd::sin;
    using xtd::_1;
    using xtd::_2;

//     std::cout << (cos(_1) * cos(_1) + sin(_2) * sin(_2))(5.f, 5.f) << std::endl;

//     std::cout << (_1)(_1)(51.f) << std::endl;
//     std::cout << (_1 + _1)(51.f) << std::endl;
//     std::cout << (_1 + _2)(5.f, 1.f) << std::endl;

//     (cos(_1) + sin(_1))(_1);
//     std::cout << (cos(_1) * cos(_1) + sin(_1) * sin(_1))(_1)(51.f) << std::endl;

    std::cout << (_1 * _1 + _2 * _2)(cos(_1), sin(_1))(51.f) << std::endl;

//     std::cout << ((cos(_1) + sin(_2))(_1, _1)).nargs() << std::endl;
  }

}
