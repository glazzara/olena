// # include <rle_pset.hh>
# include <iostream>

# include <point2d.hh>
//# include <p_run.hh>
//# include <rle_psite.hh>
# include <rle_image.hh>
# include <image2d.hh>


int main()
{
  using namespace mln;

  typedef point2d<int> point;

  point p1(5, 5), p3(12, 6), p2(7, 6);
  std::cout << "p1: " << p1 << std::endl;
  std::cout << "p2: " << p2 << std::endl;
  std::cout << "p3: " << p3 << std::endl;

  p_run<point> prun1(p1, 5);
  p_run<point> prun2(p2, 12);
  p_run<point> prun3(p3, 4);

  rle_image<point, int> ima;

  ima.insert(prun1, 1);
  ima.insert(prun2, 2);
  ima.insert(prun3, 3);

  rle_image<point, int>::piter p(ima.domain());
  for (p.start(); p.is_valid(); p.next())
  {
    std::cout << "coord: " << p[0] << ","
	      << p[1] << std::endl;
    std::cout << "ima: "<< ima(p) << std::endl;
  }


  /// Interoperability

  std::cout << "------------------------------------------------" << std::endl;
  int i = 0;
  point2d<int> p4(12, 17);
  box2d<int> bb(p1, p4);
  image2d<int> ima2d(bb);
  image2d<int>::fwd_piter piter2(ima2d.domain());
  for (piter2.start(); piter2.is_valid(); piter2.next())
  {
    ++i;
    ima2d(piter2) = i;
  }
  for (p.start(); p.is_valid(); p.next())
  {
    std::cout << "coord: " << p[0] << ","
	      << p[1] << std::endl;
    std::cout << "ima: "<< ima(p) << std::endl;
    std::cout << "ima2d: "<< ima2d(p) << std::endl;
  }
}

