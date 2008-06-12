#include <mln/core/image3d.hh>

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
  p_array< point_<grid::cube, float> > c;
  {
    mln_piter_(I3d) p(cloud.domain());
    for_all(p)
      if (cloud(p))
        {
          point_<grid::cube,float> p_;
          p_[0] = p[0]; p_[1] = p[1]; p_[2] = p[2];
          c.append(p_);
        }
  }
  p_array< point_<grid::cube, float> > x;
  {
    mln_piter_(I3d) p(surface.domain());
    for_all(p)
      if (surface(p))
        {
          point_<grid::cube,float> p_;
          p_[0] = p[0]; p_[1] = p[1]; p_[2] = p[2];
          x.append(p_);
        }
  }
  
  //working box
  const box_< point_<grid::cube, float> > working_box = enlarge(bigger(c.bbox(),x.bbox()),100);

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

  //to 2d : projection (FIXME:if 3d)
  for (unsigned i = 0; i < c.npoints(); i++)
    {
      point2d p(c[i][0], c[i][1]);
      if (output.has(p))
        output(p) = literal::black;
    }

  //ref image
  for (unsigned i = 0; i < x.npoints(); i++)
    {
      point2d px(x[i][0], x[i][1]);
      if (output.has(px))
        output(px) = literal::green;
    }
      
  io::ppm::save(output, "registred.ppm");
  
}

