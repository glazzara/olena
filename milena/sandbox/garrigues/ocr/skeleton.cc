
#include <mln/core/alias/point2d.hh>
#include "skeleton.hh"
#include <mln/level/paste.hh>
#include <mln/level/fill.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pbm/load.hh>


int main(int argc, char* argv[])
{
  if(argc!=5)
  {
    std::cout << "arguments: filename voisinage R alpha" << std::endl;
    exit(1);
  }
  image2d<bool> output;
  std::string filename = argv[1];
  int r = atoi(argv[3]);
  int alpha = atoi(argv[4]);
  
  image2d<bool> pic = io::pbm::load(filename);
  if(atoi(argv[2])==4)
    output = filteredSkeleton( pic, c4(), r, alpha, true);
  else
    output = filteredSkeleton( pic, c8(), r, alpha, true);
  mln::io::pbm::save(output, "FS-"+std::string(argv[2])+"_"+std::string(argv[3])+"_"+std::string(argv[4])+"_"+filename);
}
