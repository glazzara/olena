#include <mln/core/image3d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/norm/l2.hh>

#include <sandbox/jardonnet/registration/icp.hh>
#include <sandbox/jardonnet/registration/tools.hh>
#include <sandbox/jardonnet/registration/final_qk.hh>

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

  //working box
  const box_<mln_point_(I3d)> working_box = enlarge(bigger(c.bbox(),x.bbox()),100);
  
  //Make a lazy_image map via function closest_point
  closest_point<mln_point_(I3d)> fun(x, working_box);
  lazy_image< closest_point<mln_point_(I3d)> > map(fun);

  quat7<3> qk = registration::icp(c, map, q, e, x);

#ifndef NDEBUG
  std::cout << "closest_point(Ck[i]) = " << fun.i << std::endl;
  std::cout << "Pts processed        = " << registration::pts << std::endl;
#endif

  qk.apply_on(c, c, c.npoints());
  
  //init output image
  image2d<value::rgb8> output(convert::to_box2d(working_box), 1);

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
  std::cout << fqk << std::endl; 
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
    }

  // print surface
  for (size_t i = 0; i < c.npoints(); i++)
    {
      //Xk points
      point2d x(map(c[i])[0], map(c[i])[1]);
      if (output.has(x))
        output(x) = literal::green;
    }
      
  io::ppm::save(output, "registred.ppm");
  
}

