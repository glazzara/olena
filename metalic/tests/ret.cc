# include <cassert>
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
  assert(mlc_bool( mlc::ret_found_in_<yes> ) == true);
  assert(mlc_bool( mlc::ret_found_in_<no>  ) == false);
}
