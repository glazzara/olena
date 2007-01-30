
#include <iostream>
#include <iterator>

#include "vector.hh"
#include "algorithm.hh"



int main()
{
  abc::vector<int> v;
  v.push_back(3);
  v.push_back(1);
  v.push_back(4);

  abc::sort(v);
  abc::copy(v, std::ostream_iterator<int>(std::cout, "\n"));
}
