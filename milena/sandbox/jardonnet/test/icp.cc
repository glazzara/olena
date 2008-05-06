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
  //FIXME: Should be
  //mln_concrete(I) output(res.bbox()) = convert::to_image<I>(res) ?
  
  qk.apply_on(c, c, c.npoints());
              
  const box_<point2d> box2d(400,700);
  image2d<value::rgb8> output(box2d, 1);

  std::vector<float> length(c.npoints());
  //mean + length
  float mean = 0;
  for (size_t i = 0; i < c.npoints(); i++)
    {
      float f = norm::l2(algebra::vec<3,int> (c[i] - map(c[i])));;
      length[i] = f;
      mean += f;
    }
  mean /= c.npoints();
#ifndef NDEBUG
  std::cout << "mean : " << mean << std::endl;
#endif

  
  //standar variation
  float stdev = 0;
  for (size_t i = 0; i < c.npoints(); i++)
    stdev += (length[i] - mean) * (length[i] - mean);
  stdev /= c.npoints();
  stdev = math::sqrt(stdev);
#ifndef NDEBUG
  std::cout << "stddev : " << stdev << std::endl;
#endif

  //final translate translate using point only separated less than 2*stdev
  //mu_Xk = center map(Ck)
  algebra::vec<3,float> mu_Xk(literal::zero);
  algebra::vec<3,float> mu_C(literal::zero);
  float nb_point = 0;
  for (size_t i = 0; i < c.npoints(); ++i)
    {       
      if (length[i] > 2 * stdev)
      {
        algebra::vec<3,float> xki = map(c[i]);
        algebra::vec<3,float> ci = c[i];
        mu_C += ci;
        
        mu_Xk += xki;
        nb_point++;
      }
    }
  mu_C  /= nb_point;
  mu_Xk /= nb_point;
  
  // qT
  const algebra::vec<3,float> qT = mu_Xk - mu_C;

  //translate
  for (size_t i = 0; i < c.npoints(); ++i)
    {
      algebra::vec<3,float> ci = c[i];
      ci  -= qT;
      c.hook_()[i] = algebra::to_point<point3d>(ci);
    }
  
  //to 2d : projection (FIXME:if 3d)
  for (size_t i = 0; i < c.npoints(); i++)
    {
      //Ck points
      point2d p(c[i][0], c[i][1]);
      if (output.has(p))
        {
          if (length[i] > 2 * stdev)
            output(p) = value::rgb8(255,0,0);
          else if (length[i] > stdev)
            output(p) = value::rgb8(255,200,0);
          else
            output(p) = value::rgb8(255,255,255);
        }
      //Xk points
      point2d x(map(c[i])[0], map(c[i])[1]);
      if (output.has(x))
        output(x) = value::rgb8(0,255,0);
    }
  
  io::ppm::save(output, "registred.ppm");
  
}

