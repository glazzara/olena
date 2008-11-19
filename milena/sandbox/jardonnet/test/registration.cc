#include <mln/core/image/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/convert/to_p_array.hh>
#include <mln/norm/l2.hh>
#include <mln/core/image/lazy_image.hh>

#include <sandbox/jardonnet/registration/registration.hh>
#include <sandbox/jardonnet/registration/tools.hh>
#include <sandbox/jardonnet/registration/final_qk.hh>
#include <mln/geom/bbox.hh>

#include <mln/fun/x2x/all.hh>

void usage(char *argv[])
{
  std::cout << "usage : " << argv[0]
            << " cloud.pbm surface.pbm q e" << std::endl
            << " q >= 1 and e >= 1" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  //usage
  float q = std::atof(argv[3]);
  int   e = std::atoi(argv[4]);
  if ((argc != 5) || (q < 1 or e < 1))  usage(argv);

  using namespace mln;
  typedef image2d< bool > image2db;


  //declare image
  image2db img1;
  image2db img2;


  //load images
  io::pbm::load(img1, argv[1]);
  io::pbm::load(img2, argv[2]);


  //build p_arrays
  p_array<mln_psite_(image2db)> c = convert::to< p_array<point2d> >(img1);
  p_array<mln_psite_(image2db)> x = convert::to< p_array<point2d> >(img2);


  //working box
  const box<point2d> working_box =
    enlarge(bigger(geom::bbox(c), geom::bbox(x)), 100);


  //make a lazy_image map via function closest_point
  closest_point<mln_psite_(image2db)> fun(x, working_box);
  lazy_image<image2d<bool>, closest_point<mln_psite_(image2db)>, box2d >
    map(fun, fun.domain());


  //do registration
  using namespace fun::x2x::geom;
  composed<rotation<2u, float>, translation<2u, float> > rigid_tr =
    registration::icp(c, map, q, e, x);

  io::ppm::save(output, "registred.ppm");

}

