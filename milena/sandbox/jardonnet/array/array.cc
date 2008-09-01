#include <iostream>

#include "array.hh"

int main(int, char **)
{
  mln::metal::array1d<int, 5> a;
  mln::metal::array1d<int, 5> b;
  mln::metal::array1d<int, 5> c;

  c = a + b;

  a.get<0>() = 0;
  a.get<1>() = 1;
  a.get<2>() = 2;
  a.get<3>() = 3;
  
  for (int i = 0 ; i < a.length; i++)
    std::cout << a[i] << " ";

  std::cout << std::endl;
}
