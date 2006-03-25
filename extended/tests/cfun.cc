
#include <iostream> 
#include <xtd/cfun.hh>
#include <xtd/arg.hh>


namespace my
{

  float foo()
  {
    return 5.1f;
  }

  float bar(int i)
  {
    return float(i) * 3.f;
  }

}



int main()
{
  std::cout << xtd::mk_plain_fun(my::foo)() << std::endl;

  {
    using xtd::mk_fun;
    using xtd::_1;

    std::cout << mk_fun(my::bar)(_1)(17) << std::endl;
  }

}
