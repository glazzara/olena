#include <iostream>
#include <mln/value/rgb8.hh>

int main()
{
  using namespace mln;
  value::rgb8 v(1, 2, 3);
  std::cout << v.comp(0); // Prints `1'
}
