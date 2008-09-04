#include <mln/core/image/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/norm/l2.hh>

#include <sandbox/jardonnet/registration/icp_ref.hh>
#include <sandbox/jardonnet/registration/tools.hh>

void usage(char *argv[])
{
  std::cout << "usage : " << argv[0]
            << " cloud.pbm surface.pbm q e" << std::endl
            << " q >= 1 and e >= 1" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 5)
    usage(argv);

  float q = 1;//std::atof(argv[3]);
  int   e = 1;//std::atoi(argv[4]);

  if (q < 1 or e < 1)
    usage(argv);
  
  using namespace mln;
  typedef image3d< bool > I3d;
  
  image2d<bool> img1;
  image2d<bool> img2;

  //load images
  io::pbm::load(img1, argv[1]);
  io::pbm::load(img2, argv[2]);

  //convert to image3d
  I3d cloud   = convert::to_image3d(img1);
  I3d surface = convert::to_image3d(img2);
  
  //build p_arrays.
  typedef point_<grid::cube,float> point3df;
  p_array<point3df> c;
  {
    mln_piter_(I3d) p(cloud.domain());
    for_all(p)
      if (cloud(p))
        c.append(algebra::to_point<point3df>(point3d(p)));
  }
  p_array<point3df> x;
  {
    mln_piter_(I3d) p(surface.domain());
    for_all(p)
      if (surface(p))
        x.append(algebra::to_point<point3df>(point3d(p)));
  }
  
  //working box
  const box_< point_<grid::cube, float> > working_box = enlarge(bigger(c.bbox(),x.bbox()),50);

  // FIXME : TODO : map : vec<3,float> -> point
  closest_point< point_<grid::cube, float> > map(x, working_box);
  
  quat7<3> qk = registration::icp(c, map, q, e, x);

#ifndef NDEBUG
  std::cout << "closest_point(Ck[i]) = " << map.i << std::endl;
  std::cout << "Pts processed        = " << registration::pts << std::endl;
#endif

  qk.apply_on(c, c, c.npoints());
  
  image2d<value::rgb8> output(convert::to_box2d(working_box), 1);
  level::fill(output, literal::white);

/* FIXME: remove or plot mu_Ck and mu_Xk
  //plot mu_Ck
  point3df mu_Ck = registration::center(c, c.npoints());
  draw::plot(output, point2d(mu_Ck[0], mu_Ck[1]), literal::green);

  //plot mu_X
  point3df mu_X = registration::center(x, x.npoints());
  draw::plot(output, point2d(mu_X[0], mu_X[1]), literal::black);
*/
  
  //to 2d : projection (FIXME:if 3d)
  for (unsigned i = 0; i < c.npoints(); i++)
    {
      point2d p(c[i][0], c[i][1]);
      if (output.has(p))
        output(p) = literal::green;
    }

  //ref image (black)
  for (unsigned i = 0; i < x.npoints(); i++)
    {
      point2d px(x[i][0], x[i][1]);
      if (output.has(px))
        output(px) = literal::black;
    }
  
  io::ppm::save(output, "registred.ppm");

}

