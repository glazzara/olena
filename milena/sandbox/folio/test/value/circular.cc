
#include <mln/value/float01.hh>

#include "../../mln/value/circular.hh"


template <typename T>
class DistFct
{
  int_u8 operator()(const value::rg8& a, const value::rg8& b)
  {
    return a;
  }
};


template <typename T, typename N, typename D>
image2d<value::int_u8>
dist_on_pixels(const image2d<T>& input, const N& nbh,
	       const D& dist)
{
  using value::int_u8;
  image2d<int_u8> output(input.domain());

  mln_piter(box2d) p(input.domain());
  mln_niter(N) n(nbh, p);
  for_all(p)
  {
    int_u8 d = 0u;
    for_all(n) if (input.domain().has(n))
    {
      int_u8 d_ = dist(input(p), input(n));
      if (d_ > d)
	d = d_;
    }
    output(p) = d;
  }

  io::pgm::save(output, "temp_dist.pgm");

  return output;
}


int main()
{
  using namespace mln;
  using namespace mln::value;

  // circular<12, 0, 360> inter(21);
  // std::cout << "21: " << inter << " " << float(inter) << std::endl;
  // inter = 0;
  // std::cout << "0: " << inter << std::endl;
  // inter = 42;
  // std::cout << "42: " << inter << std::endl;
  // inter = 359;
  // std::cout << "359: " << inter << std::endl;
  // inter = 359.5;
  // std::cout << "359.5: " << inter << std::endl;
  // inter = 360;
  // std::cout << "360: " << inter << std::endl;
  // inter = 360.5;
  // std::cout << "360.5: " << inter << std::endl;
  // inter = 361;
  // std::cout << "361: " << inter << std::endl;
  // inter = 372;
  // std::cout << "372: " <<  inter << std::endl;
  // inter = 972;
  // std::cout << "972: " <<  inter << std::endl;
  // inter = -0.2;
  // std::cout << "-0.2: " <<  inter << std::endl;
  // inter = -1;
  // std::cout << "-1: " <<  inter << std::endl;
  // inter = -1;
  // std::cout << "-1: " <<  inter << std::endl;
  // inter = -359;
  // std::cout << "-359: " << inter << std::endl;
  // inter = -359.5;
  // std::cout << "-359.5: " << inter << std::endl;
  // inter = -360;
  // std::cout << "-360: " << inter << std::endl;
  // inter = -360.5;
  // std::cout << "-360.5: " << inter << std::endl;
  // inter = -361;
  // std::cout << "-361: " << inter << std::endl;



}
