#include <mln/core/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <sandbox/jardonnet/registration/icp.hh>
#include <sandbox/jardonnet/registration/tools.hh>

void usage(char *argv[])
{
  std::cout << "usage : " << argv[0]
            << " cloud.pbm surface.pbm" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);
  
  using namespace mln;
  typedef image3d< bool > I3d;
  
  image2d< bool > img1;
  image2d< bool > img2;

  //load images
  io::pbm::load(img1, argv[1]);
  io::pbm::load(img2, argv[2]);

  //convert to image3d
  I3d cloud   = convert::to_image3d(img1);
  I3d surface = convert::to_image3d(img2);
  
  //build p_arrays.
  p_array<mln_point_(I3d)> c = convert::to_p_array(cloud);
  p_array<mln_point_(I3d)> x = convert::to_p_array(surface);

  //Make a lazy_image map via function closest_point
  closest_point<mln_point_(I3d)> fun(x, box_<point3d>(1000,1000,1));
  lazy_image< closest_point<mln_point_(I3d)> > map(fun);

  registration::icp(c, map);

#ifndef NDEBUG
  std::cout << "closest_point(Ck[i]) = " << fun.i << std::endl;
  std::cout << "Pts processed        = " << registration::pts << std::endl;
#endif

  //   //init output image
  //   //FIXME: Should be 
  //   //mln_concrete(I) output(res.bbox()) = convert::to_image<I>(res)?
  //   const box_<point2d> box2d(1000,1000,0);
  //   image2d<bool> output(box2d, 1);
  
  //   //to 2d : projection (FIXME:if 3d)
  //   for (size_t i = 0; i < res.npoints(); i++)
  //     {
  //       point2d p(res[i][0], res[i][1]);
  //       if (output.has(p))
  //         output(p) = true;
  //     }
  
  //io::pbm::save("./+registred.pbm");
}

