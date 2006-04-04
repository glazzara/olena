#include <iostream>
#include <xtd/cast.hh>
#include <xtd/arg.hh>
#include <xtd/math/cos.hh>


int main()
{

  {
    xtd::plain_cast_<int, float> fun;
    std::cout << fun(3.14) << std::endl;
  }

  {
    xtd::meta_cast_<int> fun;
    std::cout << fun(3.14) << std::endl;
  }

  {
    using xtd::cast_;
    using xtd::cos;
    using xtd::_1;
    std::cout << (cast_<int>(cos(_1)))(3.14) << std::endl;
    std::cout << cos(cast_<float>(_1))(3) << std::endl;
  }

//   {
//     xtd::cos_<int> c;
//   }

//   {
//     using xtd::cos;
//     cos(1);
//   }

}
