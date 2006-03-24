#include <iostream>
#include <xtd/literal.hh>


int main()
{

  {
    xtd::plain_literal_<double> l(5.1);
  }

  {
    typedef xtd::meta_literal_<double> F;
    F l(5.1);
    // xtd_res_0_(F) tmp;
    // int warning = tmp;
  }

  {
    xtd::literal_expr_<double> l(5.1);
  }


  std::cout << mlc_is_a_(xtd::meta_literal_<double>, xtd::abstract::meta_fun_)::eval::value << std::endl;
}
