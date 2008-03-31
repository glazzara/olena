#include <mln/core/image2d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/subsampling/subsampling.hh>


int main(int argc, char*argv[])
{
  if (argc != 3)
    {
      std::cout << "usage : " << argv[0] << " in.pbm out.pbm" << std::endl;
    }
  
  using namespace mln;

  image2d<bool> img;

  io::pbm::load(img, argv[1]);

  image2d<bool> output = subsampling::subsampling(img, make::dpoint2d(2,2), argc);
  
  io::pbm::save(output, argv[2]);
}

