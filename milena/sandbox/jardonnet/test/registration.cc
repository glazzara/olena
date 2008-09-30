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

void usage(char *argv[])
{
  std::cout << "usage : " << argv[0]
            << " cloud.pbm surface.pbm q e" << std::endl
            << " q >= 1 and e >= 1" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  // usage
  float q = std::atof(argv[3]);
  int   e = std::atoi(argv[4]);
  if (argc != 5)
    usage(argv);
  if (q < 1 or e < 1)
    usage(argv);

  using namespace mln;
  typedef image2d< bool > image2db;

  image2db img1;
  image2db img2;

  //load images
  io::pbm::load(img1, argv[1]);
  io::pbm::load(img2, argv[2]);

  //build p_arrays.
  p_array<mln_psite_(image2db)> c = convert::to< p_array<point2d> >(img1);
  p_array<mln_psite_(image2db)> x = convert::to< p_array<point2d> >(img2);

  //working box
  const box<point2d> working_box =
    enlarge(bigger(geom::bbox(c), geom::bbox(x)), 100);

  //Make a lazy_image map via function closest_point
  closest_point<mln_psite_(image2db)> fun(x, working_box);

  // * Use real lazy image
  lazy_image<image2d<bool>, closest_point<mln_psite_(image2db)>, box2d > map(fun, fun.domain());
  quat7<2> qk ;//= registration::icp(c, map, q, e, x);

#ifndef NDEBUG
  std::cout << "closest_point(Ck[i]) = " << fun.i << std::endl;
  std::cout << "Pts processed        = " << registration::pts << std::endl;
#endif

  qk.apply_on(c, c, c.nsites());

  float stddev, mean;
  registration::mean_stddev(c, map, mean, stddev);

#ifndef NDEBUG
  std::cout << "mean : " << mean << std::endl;
  std::cout << "stddev : " << stddev << std::endl;
#endif

  std::vector<float> length(c.nsites());
  for (size_t i = 0; i < c.nsites(); i++)
    length[i] = norm::l2( convert::to< algebra::vec<2,float> >(c[i] - map(c[i])));


  // final transform
  quat7<2> fqk = registration::final_qk2(c, map, 2*stddev);
  fqk.apply_on(c, c, c.nsites());


  //init output image
  image2d<value::rgb8> output(convert::to_box2d(working_box), 0);
  level::fill(output, literal::white);


  //print x
  for (unsigned i = 0; i < x.nsites(); i++)
    {
      //Xk points
      point2d px(x[i][0], x[i][1]);
      if (output.has(px))
        output(px) = literal::black;
    }


  for (unsigned i = 0; i < c.nsites(); i++)
    {
      //Ck points
      point2d p(c[i][0], c[i][1]);
      if (output.has(p))
        output(p) = literal::green;
    }

  io::ppm::save(output, "registred.ppm");

}

