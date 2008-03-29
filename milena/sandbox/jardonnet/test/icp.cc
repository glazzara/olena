#include <mln/core/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <sandbox/jardonnet/registration/icp.hh>

int main(int, char* argv[])
{
  using namespace mln;
  
  image2d< bool > img1;
  image2d< bool > img2;
  
  io::pbm::load(img1, argv[1]);
  io::pbm::load(img2, argv[2]);

  io::pbm::save(registration::icp(img1,img2), "./+registred.pbm");
}

