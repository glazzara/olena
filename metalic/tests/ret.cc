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
  std::cout << mlc::ret_found_in_<yes>::value << std::endl;
  std::cout << mlc::ret_found_in_<no>::value << std::endl;
}
