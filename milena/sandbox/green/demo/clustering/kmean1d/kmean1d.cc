// DEMO ON KMEAN1D

#include <mln/clustering/kmean1d.hh>

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>
#include <mln/pw/value.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

void demo(const unsigned watch_dog, const unsigned n_times)
{
  typedef mln::clustering::kmean1d<double,8,3>        t_kmean;
  typedef mln::value::label8                          t_label8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::image2d<int_u8>                        t_image2d_int_u8;
  typedef mln::image2d<rgb8>                          t_image2d_rgb8;

  t_image2d_int_u8                                    house;

  mln::io::pgm::load(house, OLENA_IMG_PATH"/house.pgm");

  t_kmean kmean(house, watch_dog, n_times);

  kmean.launch_n_times();
  
  t_kmean::t_color_dbg    color_img    = kmean.get_color_dbg();
  t_kmean::t_mean_dbg     mean_img     = kmean.get_mean_dbg();
  t_kmean::t_label_dbg    label_img    = kmean.get_label_dbg();
  t_kmean::t_variance_cnv variance_cnv = kmean.get_variance_cnv(); 
  t_kmean::t_mean_cnv     mean_cnv     = kmean.get_mean_cnv();

  mln::io::pgm::save(mean_img,  "mean.pgm");
  mln::io::ppm::save(color_img, "color.pgm");
  mln::io::pgm::save(label_img, "label.pgm");

  mln::io::plot::save_image_sh(mean_cnv,     "mean_cnv.sh");
  mln::io::plot::save_image_sh(variance_cnv, "variance_cnv.sh");
}

void usage(const int argc, const char *args[])
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "argc    : " << argc                       << std::endl;

  for (int i = 0; i < argc; ++i) 
    std::cout << "args[i] : " << args[i] << std::endl;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "usage: kmean1d watch_dog n_times k image" << std::endl;
  std::cout << "unsigned watch_dog (convergence loop)"    << std::endl;
  std::cout << "unsigned n_times   (number of launching)" << std::endl;
  //  std::cout << "unsigned k         (number of centers)"   << std::endl;
  //  std::cout << "pbm image          (points to work with)"  << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

int main(const int argc, const char *args[])
{
  if (3 == argc)
  {
    std::istringstream arg1(args[1]);
    std::istringstream arg2(args[2]);
    // std::istringstream arg3(args[3]);
    // std::istringstream arg3(args[4]);
    unsigned   watch_dog;
    unsigned   n_times;
    // const char *image;
    // unsigned   k;
    
    arg1 >> watch_dog;
    arg2 >> n_times;

    if (!arg1.fail() && !arg2.fail())
      demo(watch_dog, n_times);
    else
      usage(argc, args);
  }
  else
    usage(argc, args);

  return 0;
}
