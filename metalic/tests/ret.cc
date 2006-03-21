# include <iostream>
# include <mlc/ret.hh>

struct yes
{
  typedef mlc::dummy ret;
};

struct no
{
};

int main()
{
  std::cout << mlc_bool( mlc::ret_found_in_<yes> ) << std::endl;
  std::cout << mlc_bool( mlc::ret_found_in_<no>  ) << std::endl;
}
