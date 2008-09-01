#include <iostream>

#include <mln/metal/array.hh>

int main(int, char **)
{
  mln::metal::array1d<int, 5> a;
  mln::metal::array1d<int, 5> b;
  mln::metal::array1d<int, 5> c;

  c = a + b;
}
