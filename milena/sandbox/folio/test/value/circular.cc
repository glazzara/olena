
#include <mln/value/float01.hh>

#include "../../mln/value/circular.hh"


int main()
{
  using namespace mln;
  using namespace mln::value;

  circular<12, 0, 360> inter(21);
  std::cout << "21: " << inter << " " << float(inter) << std::endl;
  inter = 0;
  std::cout << "0: " << inter << std::endl;
  inter = 42;
  std::cout << "42: " << inter << std::endl;
  inter = 359;
  std::cout << "359: " << inter << std::endl;
  inter = 359.5;
  std::cout << "359.5: " << inter << std::endl;
  inter = 360;
  std::cout << "360: " << inter << std::endl;
  inter = 360.5;
  std::cout << "360.5: " << inter << std::endl;
  inter = 361;
  std::cout << "361: " << inter << std::endl;
  inter = 372;
  std::cout << "372: " <<  inter << std::endl;
  inter = 972;
  std::cout << "972: " <<  inter << std::endl;
  inter = -0.2;
  std::cout << "-0.2: " <<  inter << std::endl;
  inter = -1;
  std::cout << "-1: " <<  inter << std::endl;
  inter = -1;
  std::cout << "-1: " <<  inter << std::endl;
  inter = -359;
  std::cout << "-359: " << inter << std::endl;
  inter = -359.5;
  std::cout << "-359.5: " << inter << std::endl;
  inter = -360;
  std::cout << "-360: " << inter << std::endl;
  inter = -360.5;
  std::cout << "-360.5: " << inter << std::endl;
  inter = -361;
  std::cout << "-361: " << inter << std::endl;
}
