// DEMO ON KMEAN_RGB

#include <mln/clustering/kmean_rgb.hh>

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
//#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/ppm/load.hh>
//#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
//#include <mln/io/ppm/save.hh>
//#include <mln/io/plot/save_image_sh.hh>

//#include <mln/pw/value.hh>

//#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>

int main()
{
  const std::string& image     = OLENA_IMG_PATH"/house.ppm";
  const unsigned     k_center  = 3;
  const unsigned     n_times   = 10;
  const unsigned     watch_dog = 10;

  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<5>                          t_rgb5;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgb5>                        t_image2d_rgb5;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::fun::v2v::rgb8_to_rgbn<5>              t_rgb8_to_rgb5;

  t_image2d_rgb8                                      house_rgb8;
  t_image2d_rgb5                                      house_rgb5;
  t_image2d_lbl8                                      house_lbl8;

  mln::io::ppm::load(house_rgb8, image.c_str());
  house_rgb5 = mln::data::transform(house_rgb8, t_rgb8_to_rgb5());

  mln::trace::quiet = false;

  house_lbl8 = mln::clustering::kmean_rgb<double,5>(house_rgb5,
						    k_center,
						    watch_dog,
						    n_times);
  mln::trace::quiet = true;

  mln::io::pgm::save(house_lbl8, "label.pgm");
  /*



  t_kmean kmean(house_rgb5, k_center, watch_dog, n_times);



  //kmean.launch_one_time();
  kmean.launch_n_times();

  // Not safe because we don't test kmean.is_valid()

  t_kmean::t_color_dbg    color_img    = kmean.get_color_dbg();
  t_kmean::t_mean_dbg     mean_img     = kmean.get_mean_dbg();
  t_kmean::t_label_dbg    label_img    = kmean.get_label_dbg();
  t_kmean::t_variance_cnv variance_cnv = kmean.get_variance_cnv();
  t_kmean::t_mean_cnv     mean_cnv     = kmean.get_mean_cnv();

  mln::io::ppm::save(mean_img,  "mean.ppm");
  mln::io::ppm::save(color_img, "color.ppm");
  mln::io::pgm::save(label_img, "label.pgm");

  mln::io::plot::save_image_sh(mean_img,     "mean.sh");
  mln::io::plot::save_image_sh(mean_cnv,     "mean_cnv.sh");
  mln::io::plot::save_image_sh(variance_cnv, "variance_cnv.sh");

  */

  return 0;
}

