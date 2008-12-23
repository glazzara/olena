#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/data/fill.hh>
#include <mln/debug/println.hh>
#include <iomanip>


int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<bool> I;

  box2d b = make::box2d(-1,-1, 1,1);
  image2d<bool> ima(b, 0);
  point2d p(0, 0);

  std::vector<int_u8> x8(256), x4(256);


  for (unsigned i = 0; i < 256; i++)
  {
    data::fill(ima, false);

    int_u8 tmp = i;
    mln_fwd_niter_(neighb2d) n(c8(), p);
    for_all(n)
    {
      if (tmp % 2)
	ima(n) = true;
      tmp = tmp >> 1;
    }

    labeling::blobs(ima, c8(), x8[i]);

    {
      int_u8 nl;
      image2d<int_u8> lab = labeling::blobs(ima, c4(), nl);
      std::set<int_u8> s;
      mln_fwd_niter_(neighb2d) n(c4(), p);
      for_all(n)
	if (lab(n) != 0)
	  s.insert(lab(n));
      x4[i] = s.size();
    }
  }


  // Now printing!

  std::cout << "----- Object in C8 ------" << std::endl;

  for (unsigned i = 0; i < 256; i++)
  {
    std::cout << std::setw(2) << x8[i] << ", ";
    if (! ((i + 1) % 4)) std::cout << " ";
    if (! ((i + 1) % 16)) std::cout << std::endl;
    if (! ((i + 1) % 64)) std::cout <<  std::endl;
  }

  std::cout << "----- Object in C4 ------" << std::endl;

  for (unsigned i = 0; i < 256; i++)
  {
    std::cout <<  std::setw(2) << x4[i] << ", ";
    if (! ((i + 1) % 4)) std::cout << " ";
    if (! ((i + 1) % 16)) std::cout << std::endl;
    if (! ((i + 1) % 64)) std::cout <<  std::endl;
  }

}
