#include <mln/core/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/norm/l2.hh>

#include <sandbox/jardonnet/registration/icp.hh>
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

  float q = std::atof(argv[3]);
  int   e = std::atoi(argv[4]);

  if (q < 1 or e < 1)
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

  quat7<3> qk = registration::icp(c, map, q, e, x);

#ifndef NDEBUG
  std::cout << "closest_point(Ck[i]) = " << fun.i << std::endl;
  std::cout << "Pts processed        = " << registration::pts << std::endl;
#endif

  //FIXME: register img1
  
  //init output image
  //FIXME: Should be like
  //mln_concrete(I) output(res.bbox()) = convert::to_image<I>(res) ?
  
  qk.apply_on(c, c, c.npoints());
              
  const box_<point2d> box2d(400,700);
  image2d<value::rgb8> output(box2d, 1);


  float stddev, mean;
  registration::mean_stddev(c, map, mean, stddev);
  
#ifndef NDEBUG
  std::cout << "mean : " << mean << std::endl;
  std::cout << "stddev : " << stddev << std::endl;
#endif

  std::vector<float> length(c.npoints());
  for (size_t i = 0; i < c.npoints(); i++)
    length[i] = norm::l2(algebra::vec<3,int> (c[i] - map(c[i])));

  
  // final transform
  quat7<3> fqk = registration::final_qk(c, map, 2 * stddev);
  fqk.apply_on(c, c, c.npoints());

  //to 2d : projection (FIXME:if 3d)
  for (size_t i = 0; i < c.npoints(); i++)
    {
      //Ck points
      point2d p(c[i][0], c[i][1]);
      if (output.has(p))
        {
	  algebra::vec<3,float> xki = map(c[i]);
	  algebra::vec<3,float> ci = c[i];
	  
	  if (length[i] > 2 * stddev)
            output(p) = literal::red;
          else if (length[i] > stddev)
            output(p) = value::rgb8(255,200,0);
          else
            output(p) = literal::white;
        }
      //Xk points
      point2d x(map(c[i])[0], map(c[i])[1]);
      if (output.has(x))
        output(x) = value::rgb8(0,255,0);
    }
      
  io::ppm::save(output, "registred.ppm");
  
}

